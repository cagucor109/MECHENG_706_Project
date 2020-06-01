// Project 2 (Firefighter project) for MechEng706 Mechatronics design
// Authors (Group 21):  Narada Hu, nhu769
//                   :  Rachel Li, rli142
//                   :
//                   :

// This is the main file for controlling the vex robot. It includes the
// main behaviour control system and calls upon supporting functions and
// files in the libraries. The purpose of the program is to control a vex
// robot to extinguish fires when placed in a walled area with unknown
// obstacles and fire locations. Behaviour and fuzzy logic control are used
// with a overarching FSM system.


//----------------------Libraries------------------------------------------------------------------------------------------------------------------------------------------
// include libraries
/*#include "libraries\Controllers\Controllers.h"
#include "libraries\Controllers\Controllers.cpp"
*/#include "libraries\Sensors\Sensors.h"
#include "libraries\Sensors\Sensors.cpp"
#include "libraries\Motors\Motors.h"
#include "libraries\Motors\Motors.cpp"
#include "libraries\Fuzzy\Fuzzy.h"
#include "libraries\Fuzzy\Fuzzy.cpp"
//Initializing global objects
//Controllers controlSystem;
//Sensors sensor;
Motors motor;

Fuzzy avoidFuzzy(1);
Fuzzy moveToFireFuzzy(2);
//To be replaced by proper readings variables
int obstacleFrontDistance;
int photoReadings;
int firesLeft = 2;
int firesRecorded = 0;
int maxPhotoDetected = 0;
int totalTurn = 0; //amount turned during scan phase of locate
int timeSearched; // time taken to move and serach of fire if nothing is seen for a full rotation
// thigns to remove when combined with sensor
bool fireDetected, arcAngle, photoInten, checkFront, BatteryLow = false;


bool locateFinished = false;
bool first = false; //first time entering extinguish state
int updateFanMillis = 0;





//----------------------Battery check and Serial Comms---------------------------------------------------------------------------------------------------------------------
//Serial Pointer
HardwareSerial *SerialCom;
//----------------------Setup------------------------------------------------------------------------------------------------------------------------------------------
#define FIREDISTANCE 50
#define FIRELOCATEVALUE 0 //photosensor value for fire to count as located
#define DETECTION_THR 0 // threshold range for redetection of fire during relocation
#define FANRUNTIME 10000 // 10s
#define LIGHTOFF 0

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

static LOCATE locate_state = SCAN; //place in main loop?

// Declare commands and command flags
/*
//MOTION halt_command;
int halt_output_flag;
//MOTION extinguish_command;
int extinguish_output_flag;
//MOTION avoid_command;
int avoid_output_flag;
//MOTION moveToFire_command;
int moveToFire_output_flag;
//MOTION locate_command;
int locate_output_flag;

MOTION motor_input;*/

//----------------------Main loop------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
}

//void main() {
void loop() { 
  // the main loop updates sensors then selects the behaviour
  // based on the sensor inputs and sends them to the motors.
  // update functions need to be inplemented
  sensor.updateArcAngle();  // this is for moveToFire
  sensor.checkZones();      // this is for Avoid
  Suppressor();
  motor.powerMotors();
}

//----------------------Sensing functions------------------------------------------------------------------------------------------------------------------------------------------
bool extinguish_output_flag() {
  if (obstacleFrontDistance < FIREDISTANCE){
    return true;
  } else {
    return false;
  }
}

bool moveToFire_output_flag() {
  // some of this is for locate because it doesnt have a flag function
  // locatefinished is used to check that locate is finished before moving
  // to moveToFire behaviour. It is set when we have finishe repositioning to
  // the largest phototransistor reading and is also reset when moving to fire 
  // for the next time we enter that state/behaviour.
  if((locateFinished == true)&&(fireDetected == true)){ // add brackets and sensors.firedetected when ready
    return true;
   
  } else {
    if (fireDetected == false){
      locateFinished = false;
    }
    return false; 
  }
}
bool halt_output_flag() {
  if ((firesLeft == 0) || (BatteryLow)) { //add battery low
    return true;
  } else {
    return false;
  }
}
bool avoid_output_flag() {
  if (checkFront == true){ // add sensor.checkZone('front') < obstacleThreshold
    if ((arcAngle == true) && (photoInten == true)){     // abs(sensor.getPhotoArcAngle()) < arcThreshold && sensor.getIntensity > intensityThreshold
      if (obstacleFrontDistance < FIREDISTANCE) { // sensor.checkZone('front') <fireThreshold
        return true;
      }
      return false;
    }
    return true;
  }else {
    return false;
  }
}


//----------------------State output Functions------------------------------------------------------------------------------------------------------------------------------------------

void locate_command() {
  int firesRecorded = 0;
  float firstFire = 0;
  //next state FSM
  //when to exit a state and which state to transition to
  switch (locate_state)
  {
    case SCAN:
      if (sensor.getGyroState()){ // if gyro off turn on
        sensor.enableGyro();
      }
      if (sensor.getPhoto(2) > FIRELOCATEVALUE){
        firstFire = sensor.getAngle();
        locate_state = RECORD;
      }
      else if (sensor.getAngle() > 360) {
        locate_state = SEARCH;
        timeSearched = millis();
      }
      break;
    case SEARCH:
      if (millis() - timeSearched > 3000) {
        timeSearched = millis();
        locate_state = SCAN;
      } else locate_state = SEARCH;
    case RECORD:
      if (firesLeft == firesRecorded) {
        locate_state = REPOSITION;
      } else if (firesRecorded < firesLeft && sensor.getPhoto(2) < FIRELOCATEVALUE) {
        locate_state = SCAN;
      }
      break;
    case REPOSITION:
      if (locateFinished == true) locate_state = SCAN; 
      break; //may need to reevaluate
  }

  //doing stuff
  switch(locate_state) {
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
  motor.desiredControl(0,0,90);
  //read photo sensors here?
  
}

void search() {
  //move forward
  motor.desiredControl(500,0,0);
}

void record() {
  // continues to turn until fire is no longer seen on the 
  firesRecorded++;
  maxPhotoDetected = sensor.getPhoto(2);//to replace with sensor function
}

void reposition() { 
    if (photoReadings > (maxPhotoDetected - DETECTION_THR)){
       motor.desiredControl(0,0,0);
       locateFinished = true;
       
    } else {
      motor.desiredControl(0,0,90); 
    }
}

void halt_command() {
  motor.desiredControl(0,0,0);
  //flash a LED to be cool
}

void extinguish_command(){

  //entering timestamp turn fan on and stop moving
  if (first == false) {
    motor.desiredControl(0,0,0);
    motor.controlFan(true);
    
    updateFanMillis = millis;
    first = true;
  }

  if ((millis() - updateFanMillis > FANRUNTIME) && (photoReadings > LIGHTOFF)){
    motor.controlFan(false);
    firesLeft--;
    updateFanMillis = millis();
    first=false;
    
  } 
}

void avoid_command() {
  bool front, left, right;
  double Xnorm, Ynorm = 0;
  front = moveToFireFuzzy.setCrispInput('front', sensor.getZoneScore('front'));
  left = moveToFireFuzzy.setCrispInput('left', sensor.getZoneScore('left'));
  right = moveToFireFuzzy.setCrispInput('right', sensor.getZoneScore('right'));
  if (front && left && right){
    avoidFuzzy.updateFuzzy();
    Xnorm = moveToFireFuzzy.getOutputValue('X');  // num between -1 - 1
    Ynorm = moveToFireFuzzy.getOutputValue('Y');  // num between -1 - 1
    // scale to motor range of +- 500 and input to motor
    motor.desiredControl(Xnorm*500, Ynorm*500); 
  }
}

void moveToFire_command() {
  bool arcPosition, intensity;
  double Ynorm, Znorm = 0;
  arcPosition = moveToFireFuzzy.setCrispInput('arcPosition', sensor.getPhotoArcAngle());
  intensity = moveToFireFuzzy.setCrispInput('intensity', sensor.getMaxPhoto());
  if (arcPosition && intensity){
    moveToFireFuzzy.updateFuzzy();
    Ynorm = moveToFireFuzzy.getOutputValue('Y');  // num between  0 - 1
    Znorm = moveToFireFuzzy.getOutputValue('Z');  // num between -1 - 1
    // scale to motor range of +- 500 and input to motor
    motor.desiredControl(0, Ynorm*500, Znorm*500); 
  }
}

void Suppressor() {
  // This function calls sensing functions to evaluates
  // which command to output to motors

  // second lowest priority`
  if (moveToFire_output_flag() == 1) {
    moveToFire_command();
  } else  if (avoid_output_flag() == 1) {
    avoid_command();
  } else  if (extinguish_output_flag() == 1) {
    extinguish_command();
  } else if (halt_output_flag() == 1) {
    // highest priority
    halt_command();
  } else { // default behaviour/lowest priority
    locate_command();
  }

  motor.powerMotors();
  //sleep(tick);
}

//----------------------- case study code ---------------------------------------------------------
/*
void avoid() {
  intval;
  val = ir_detect();
  if (val == 0b11) {
    avoid_output_flag = 1;
    avoid_command = BACKWARD;
  }
  else if (val == 0b10) {
    avoid_output_flag = 1;
    avoid_command = RIGHT_ARC;
  } else if (val == 0b01) {
    avoid_output_flag = 1;
    avoid_command = LEFT_ARC;
  } else {
    avoid_output_flag = 0;
  }
}

void extinguish() {
  int left_photo, right_photo, delta;
  left_photo = analog(1);
  right_photo = analog(0);
  delta = right_photo - left_photo;
  if (abs(delta) > photo_dead_zone) {
    if (delta > 0)extinguish_command = LEFT_TURN;
    else extinguish_command = RIGHT_TURN;
    extinguish_output_flag = 1;
  } else  extinguish_output_flag = 0;
}

void locate() {
  int left_photo, right_photo, delta;
  left_photo = analog(1);
  right_photo = analog(0);
  delta = right_photo - left_photo;
  if (abs(delta) > photo_dead_zone) {
    if (delta > 0)locate_command = LEFT_TURN;
    else locate_command = RIGHT_TURN;
    locate_output_flag = 1;
  } else  locate_output_flag = 0;
}

void moveToFire() {
  bumper_check();
  if (bumper_left && bump_right) {
    moveToFire_output_flag = 1;
    moveToFire_command = BACKWARD;
    sleep (0.2);
    moveToFire_command = LEFT_TURN;
    sleep (0.4);
  } else if (bumper_left) {
    moveToFire_output_flag = 1;
    moveToFire_command = RIGHT_TURN;
    sleep(0.4);
  } else if (bump_right) {
    moveToFire_output_flag = 1;
    moveToFire_command = LEFT_TURN;
    sleep (0.4);
  } else if (bump_back) {
    moveToFire_output_flag = 1;
    moveToFire_command = LEFT_TURN;
    sleep(0.2);
  } else  moveToFire_output_flag = 0
  }
  */
