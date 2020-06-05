// Project 2 (Firefighter project) for MechEng706 Mechatronics design
// Authors (Group 21):  Narada Hu, nhu769
//                   :  Rachel Li, rli142
//                   :  Michael Carswell, mcar546
//                   :  Carlos Aguilera, cagu554

// This is the main file for controlling the vex robot. It includes the
// main behaviour control system and calls upon supporting functions and
// files in the libraries. The purpose of the program is to control a vex
// robot to extinguish fires when placed in a walled area with unknown
// obstacles and fire locations. Behaviour and fuzzy logic control are used
// with a overarching FSM system.


//----------------------Libraries------------------------------------------------------------------------------------------------------------------------------------------
// include libraries
#include "libraries\Sensors\Sensors.h"
#include "libraries\Sensors\Sensors.cpp"
#include "libraries\Motors\Motors.h"
#include "libraries\Motors\Motors.cpp"
#include "libraries\Fuzzy\Fuzzy.h"
#include "libraries\Fuzzy\Fuzzy.cpp"

//Initializing global objects
Sensors sensor;
Motors motor;
Fuzzy avoidFuzzy(1);
Fuzzy moveToFireFuzzy(2);

//----------------------Battery check and Serial Comms---------------------------------------------------------------------------------------------------------------------
//Serial Pointer
HardwareSerial *SerialCom;
//----------------------Setup------------------------------------------------------------------------------------------------------------------------------------------

//Constants
#define FIREDISTANCE 0.12 // 20cm from middle of robot to middle of fire obstacle
#define FIRELOCATEVALUE 250 //photosensor value for fire to count as located might remove this if can be imported from sensors.h
#define DETECTION_THR 100 // threshold range for redetection of fire during relocation
#define FANRUNTIME 10000 // 10s
#define OBSTACLETHRESHOLD 0.25 // 15cm  
#define ARCTHRESHOLD 10 // if fire is +-10degrees threshold infront of robot
#define INTENSITYTHRESHOLD 0.8 // 80%

//Global Variables
int firesLeft = 2;
int firesRecorded = 0;
int maxPhotoDetected = 0;
int totalTurn = 0; //amount turned during scan phase of locate
int timeSearched; // time taken to move and serach of fire if nothing is seen for a full rotation
bool locateFinished = false;
bool first = false; //first time entering extinguish state
int updateFanMillis = 0;
float firstFire = 0;

// Enum for motion states
enum MOTION {
  FORWARD,
  BACKWARD,
  STRAFE_LEFT,
  STRAFE_RIGHT,
  LEFT_TURN,
  RIGHT_TURN,
  LEFT_ARC,
  RIGHT_ARC,
  BACKWARD_LEFT_TURN,
  STOP,
};

enum LOCATE {
  SCAN,
  SEARCH,
  RECORD,
  REPOSITION
};

LOCATE locate_state = SCAN;//Initialising locate FSM


//----------------------Main loop------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
}

void loop() {
  // the main loop updates sensors then selects the behaviour
  // based on the sensor inputs and sends them to the motors.
  sensor.updateArcAngle();  // this is for moveToFire
  sensor.updateGyro(); // locate FSM gyro update
  sensor.checkZones();      // this is for Avoid
  Suppressor();
  motor.powerMotors();
}


//----------------------Behaviour Selection--------------------------------------------------------------------------------------------------------------------------------
void Suppressor() {
  // This function calls sensing functions to evaluates which command to output to motors

  // highest priority
  if (halt_output_flag() == 1) {
    halt_command();
  } else if (extinguish_output_flag() == 1) {
    extinguish_command();
  } else  if (avoid_output_flag() == 1) {
    avoid_command();
  } else  if  (moveToFire_output_flag() == 1) {
    moveToFire_command();
  } else  { // default behaviour/lowest priority
    locate_command();
  }
}

//----------------------Suppressor flag functions--------------------------------------------------------------------------------------------------------------------------

bool halt_output_flag() {
  // checks battery or if all fires are extinguished
  if ((firesLeft == 0) || (checkBattery())) {
    return true;
  }
  return false;
}

bool extinguish_output_flag() {
  if (abs(sensor.getPhotoArcAngle()) < ARCTHRESHOLD && sensor.getMaxPhoto() > INTENSITYTHRESHOLD) {
    if (sensor.getZoneScore('front') < FIREDISTANCE) {
      return true;
    }
  }
  return false;
}

bool avoid_output_flag() {
  // if there is an obstacle infront
  if (sensor.getZoneScore('front') < OBSTACLETHRESHOLD) {
    // if fire brightly infront and centreish
    if (abs(sensor.getPhotoArcAngle()) < ARCTHRESHOLD && sensor.getMaxPhoto() > INTENSITYTHRESHOLD) {
      return false;
    }
    return true;
  }
  return false;
}

bool moveToFire_output_flag() {
  // some of this is for locate because it doesnt have a flag function
  // locatefinished is used to check that locate is finished before moving
  // to moveToFire behaviour. It is set when we have finished repositioning to
  // the largest phototransistor reading and is also reset when moving to fire
  // for the next time we enter that state/behaviour.
  if ((locateFinished == true) && (sensor.isDetected())) { //Only move to fire when locate is complete
    return true;
  }
  if (sensor.isDetected()) { // this resets locateFinished when we supress move to fire
    locateFinished = false;
  }
  return false;
}
//----------------------State output Functions------------------------------------------------------------------------------------------------------------------------------------------

//-------Locate---------
void locate_command() {
  //next state FSM
  //when to exit a state and which state to transition to
  switch (locate_state)
  {
    case SCAN:
      if (sensor.getGyroState()) { // if gyro off turn on
        sensor.enableGyro();
      }
      if (sensor.getPhoto(2) > PHOTO_DETECT_THRESHOLD) { // this define is from sensors.h
        locate_state = SCAN;
        firstFire = sensor.getAngle();
        locate_state = RECORD;
      }
      else if (sensor.getAngle() > 360) {
        locate_state = SEARCH;
        timeSearched = millis();
        sensor.disableGyro();
      }
      break;
    case SEARCH:
      if (millis() - timeSearched > 1000) {
        timeSearched = millis();
        locate_state = SCAN;
        sensor.enableGyro();
      } else {
        locate_state = SEARCH;
      }
      break;
    case RECORD:
      if (sensor.getAngle() > 360 || firesLeft == firesRecorded)
      {
        locate_state = REPOSITION;
  } else {
    locate_state = SCAN;
  }
  break;
case REPOSITION:
  if (locateFinished == true) {
    locate_state = SCAN;
    sensor.disableGyro();
  }
  break;
}

// State output logic
switch (locate_state) {
case SCAN:
  scan();
  break;
case SEARCH:
  search();
  break;
case RECORD:
  record();
  break;
case REPOSITION:
  reposition();
  break;
}
}

void scan() {
  //rotate CW
  motor.desiredControl(0, 0, 90);
}

void search() {
  //move forward
  motor.desiredControl(500, 0, 0);
}

void record() {
  // continues to turn until fire is no longer seen on the photoresistor
  firesRecorded++;
  if (sensor.getPhoto(2) > maxPhotoDetected) {
    maxPhotoDetected = sensor.getPhoto(2);//record photoresistor value
  }
}

void reposition() {
  // rotate CW until find max photoresistor position
  if (sensor.getPhoto(2) > (maxPhotoDetected - DETECTION_THR)) {
    motor.desiredControl(0, 0, 0);
    locateFinished = true;
  } else {
    motor.desiredControl(0, 0, 90);
  }
}

//-------Halt---------
void halt_command() {
  motor.desiredControl(0, 0, 0);
  //flash a LED to be cool
  slow_flash_LED_builtin();
  fast_flash_double_LED_builtin();
  slow_flash_LED_builtin();
}

//-------Extinguish---------
void extinguish_command() {
  //entering: timestamp, turn fan on, and stop moving
  if (first = false) {
    motor.desiredControl(0, 0, 0);
    motor.controlFan(true);

    updateFanMillis = millis;
    first = true;
  }

  if ((millis() - updateFanMillis > FANRUNTIME) && ((!sensor.getDetected(2) || !sensor.getDetected(3)))) { //Extingush for at least 10 seconds and until fire is no longer detected.
    motor.controlFan(false);
    firesLeft--;
    updateFanMillis = millis();
    first = false;

  }
}

//-------Avoid---------
void avoid_command() {
  bool front, left, right;
  double Xnorm, Ynorm = 0;
  front = moveToFireFuzzy.setCrispInput('front', sensor.getZoneScore('front')); // connects sensors to fuzzy input
  left = moveToFireFuzzy.setCrispInput('left', sensor.getZoneScore('left'));    // connects sensors to fuzzy input
  right = moveToFireFuzzy.setCrispInput('right', sensor.getZoneScore('right')); // connects sensors to fuzzy input
  if (front && left && right) { // boolean return check if inputs are successfully connected
    avoidFuzzy.updateFuzzy();
    Xnorm = moveToFireFuzzy.getCrispOutput('X');  // num between -1 - 1
    Ynorm = moveToFireFuzzy.getCrispOutput('Y');  // num between -1 - 1
    // scale to motor range of +- 500 and input to motor
    motor.desiredControl(Xnorm * 500, Ynorm * 500, 0);
  }
}

//-------Move to Fire---------
void moveToFire_command() {
  bool arcPosition, intensity;
  double Ynorm, Znorm = 0;
  arcPosition = moveToFireFuzzy.setCrispInput('arcPosition', sensor.getNormPhotoArc()); // connects sensors to fuzzy input
  intensity = moveToFireFuzzy.setCrispInput('intensity', sensor.getMaxPhoto());         // connects sensors to fuzzy input
  if (arcPosition && intensity) { // boolean return check if inputs are successfully connected
    moveToFireFuzzy.updateFuzzy();
    Ynorm = moveToFireFuzzy.getCrispOutput('Y');  // num between  0 - 1
    Znorm = moveToFireFuzzy.getCrispOutput('Z');  // num between -1 - 1
    // scale to motor range of +- 500 and input to motor
    motor.desiredControl(0, Ynorm * 500, Znorm * 500);
  }
}

//---------------------- Battery Check and LED Flash -----------------------------------------

void fast_flash_double_LED_builtin()
{
  static byte indexer = 0;
  static unsigned long fast_flash_millis;
  if (millis() > fast_flash_millis) {
    indexer++;
    if (indexer > 4) {
      fast_flash_millis = millis() + 700;
      digitalWrite(LED_BUILTIN, LOW);
      indexer = 0;
    } else {
      fast_flash_millis = millis() + 100;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}

void slow_flash_LED_builtin()
{
  static unsigned long slow_flash_millis;
  if (millis() - slow_flash_millis > 2000) {
    slow_flash_millis = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

bool checkBattery() {
  static byte counter_lipo_voltage_ok;
  static unsigned long previous_millis;
  if (millis() - previous_millis > 500) { //print massage every 500ms
    previous_millis = millis();
    SerialCom->println("STOPPED---------");
    //500ms timed if statement to check lipo and output speed settings
    if (is_battery_voltage_OK()) {
      SerialCom->print("Lipo OK waiting of voltage Counter 10 < ");
      SerialCom->println(counter_lipo_voltage_ok);
      counter_lipo_voltage_ok++;
      if (counter_lipo_voltage_ok > 10) { //Making sure lipo voltage is stable
        counter_lipo_voltage_ok = 0;
        motor.enable_motors();
        SerialCom->println("Lipo OK returning to RUN STATE");
        return 1;
      }
    } else
    {
      counter_lipo_voltage_ok = 0;
      return 0;
    }
  }
}


bool is_battery_voltage_OK()
{
  static byte Low_voltage_counter;
  static unsigned long previous_millis;

  int Lipo_level_cal;
  int raw_lipo;
  //the voltage of a LiPo cell depends on its chemistry and varies from about 3.5V (discharged) = 717(3.5V Min) https://oscarliang.com/lipo-battery-guide/
  //to about 4.20-4.25V (fully charged) = 860(4.2V Max)
  //Lipo Cell voltage should never go below 3V, So 3.5V is a safety factor.
  raw_lipo = analogRead(A0);
  Lipo_level_cal = (raw_lipo - 717);
  Lipo_level_cal = Lipo_level_cal * 100;
  Lipo_level_cal = Lipo_level_cal / 143;

  if (Lipo_level_cal > 0 && Lipo_level_cal < 160) {
    previous_millis = millis();
    SerialCom->print("Lipo level:");
    SerialCom->print(Lipo_level_cal);
    SerialCom->print("%");
    // SerialCom->print(" : Raw Lipo:");
    // SerialCom->println(raw_lipo);
    SerialCom->println("");
    Low_voltage_counter = 0;
    return true;
  } else {
    if (Lipo_level_cal < 0)
      SerialCom->println("Lipo is Disconnected or Power Switch is turned OFF!!!");
    else if (Lipo_level_cal > 160)
      SerialCom->println("!Lipo is Overchanged!!!");
    else {
      SerialCom->println("Lipo voltage too LOW, any lower and the lipo with be damaged");
      SerialCom->print("Please Re-charge Lipo:");
      SerialCom->print(Lipo_level_cal);
      SerialCom->println("%");
    }

    Low_voltage_counter++;
    if (Low_voltage_counter > 5)
      return false;
    else
      return true;
  }
}
