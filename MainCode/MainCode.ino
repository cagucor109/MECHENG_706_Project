//Mecheng 706 project 1 group 21

//----------------------Libraries------------------------------------------------------------------------------------------------------------------------------------------
// include libraries
#include "libraries\Controllers\Controllers.h"
#include "libraries\Controllers\Controllers.cpp"
#include "libraries\Sensors\Sensors.h"
#include "libraries\Sensors\Sensors.cpp"
#include "libraries\Motors\Motors.h"
#include "libraries\Motors\Motors.cpp"
#include "libraries\Kalman\Kalman.h"
#include "libraries\Kalman\Kalman.cpp"


//Initializing global objects
Controllers controlSystem;
Sensors sensor;
Motors motor;
Kalman kalmanX;
Kalman kalmanY;

//----------------------Battery check and Serial Comms---------------------------------------------------------------------------------------------------------------------
//Serial Pointer
HardwareSerial *SerialCom;

//----------------------Tolerances for state exit conditions---------------------------------------------------------------------------------------------------------------
#define ANGLETOLERANCE 5
#define LEFTTOLERANCE 10
#define FRONTTOLERANCE 150
#define UPDATETIME 100
#define MAXTURNS 4
#define FIXEDROTATE 10

//----------------------State machines-------------------------------------------------------------------------------------------------------------------------------------
enum STATE {
  INITIALISING,
  ALIGN,
  EDGE_FOLLOW,
  STOPPED,
  TURNING,
  FINISHED
};

//----------------------Setup and main loop--------------------------------------------------------------------------------------------------------------------------------

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial;
  SerialCom->begin(115200);
  SerialCom->println("Setup....");

  delay(500); //settling time but not really needed
}


void loop(void)
{
  static STATE machine_state = INITIALISING;
  static unsigned int turnCounter = 0;
  static unsigned long updateAngleMillis;
  static unsigned long updateFSMMillis;

  //Updating gryo at 10 ms interval
  if (millis() - updateAngleMillis > UPDATEGYROTIME) { //UPDATEGRYOTIME is defined in the sensors class
    sensor.updateAngle();
    
    updateAngleMillis = millis();
  }

  //Running FSM
  if (millis() - updateFSMMillis > UPDATETIME) {
    updateFSMMillis = millis();
    //Reading sensors
    sensor.updateLeftDistance();
    sensor.updateParallel();
    sensor.updateFrontDistance();
    filter();
    //Checking battery state
    if (!is_battery_voltage_OK()) machine_state = STOPPED;

    //Next state Finite-state machine Code
    // This logic decides when to exit a state and which state to transition to
    switch (machine_state) {
      case INITIALISING:
        if (isInit)
          machine_state = ALIGN;
        else
          machine_state = INITIALISING;
        break;
      case ALIGN:
        if (aligned)  {
          kalmanX.setPrevEst(150);
          machine_state = EDGE_FOLLOW;
        }
        else
          machine_state = ALIGN;
        break;
      case TURNING:
        if (aligned) machine_state = ALIGN;
        break;
      case EDGE_FOLLOW: 
        // checks distance infront and whether it should exit
        // if robot has made    
        if (front) {
          machine_state = TURNING;
          turnCounter++;
          if (turnCounter == MAXTURNS)  {
            turnCounter = 0;
            machine_state = FINISHED;
          }
        }
        break;
      case STOPPED:
        if (checkBattery()) machine_state = INITIALISING;
        break;
      case FINISHED:
        if (reset) machine_state = ALIGN;
        break;
    };


    //Output State Machine
    // This logic determines the output based on the current state
    switch (machine_state) {
      case INITIALISING:
        initialising();
        break;
      case ALIGN:
        // set sensor noise to 0 to only rely on sensor input
        kalmanX.setSensorNoise(0);
        kalmanX.setProcessNoise(10);
        align();
        break;
      case TURNING:
        turn();
        break;
      case EDGE_FOLLOW:
        // reset sensor and process noise to default
        kalmanX.setSensorNoise(1);
        kalmanX.setProcessNoise(8);
        follow();
        break;
      case STOPPED:
        stopped();
        break;
      case FINISHED:
        finished();
        break;
    };
  }
}

//----------------------Next State Functions-------------------------------------------------------------------------------------------------------------------------------
// Boolean functions that return true when the exit condition is met
bool isInit() {
  // check for initialisation errors
  if (motor.isEnabled() == true) return 1;
  else return 0;
}

bool aligned() {
  // exit condition if robot is aligned with wall and correct distance from wall.
  if ((abs(150 - sensor.getLeftDistance()) <= LEFTTOLERANCE) && (abs(sensor.getParallel()) <= ANGLETOLERANCE)) {
    return 1;
  } else {
    return 0;
  }
}
bool reset() {
  if (userInput()) return 1;
  else return 0;
}
bool front() {
  // if robot is within tolerance to wall infront
  if (sensor.getFrontDistance() <= FRONTTOLERANCE)  return 1;
  else  return 0;
}

//-----------------------State Output Functions----------------------------------------------------------------------------------------------------------------------------
// These void functions output control to the robot based on current state.
void initialising() {
  SerialCom->println("Enabling Motors");
  motor.enable_motors();
  // Sets up kalman filter for Y direction control
  // ignores motion model and smoothes motion model data
  kalmanY.setProcessNoise(100);
  kalmanY.setSensorNoise(2);
  return;
}

void align() {
  // Outputs control efforts to motor to align robot parallel with wall
  // and 15cm perpendicular distance.
  sensor.disableGyro();
  if (controlSystem.getDesiredAngle() != 0) controlSystem.setDesiredAngle(0);
  if (!sensor.getParallelError()) { // if there is no error ie wall detected on left side
    // control Y with PID controller to be tuned in real life
    motor.rotateControl = controlSystem.controlPID("angle", controlSystem.calculateError("angle", sensor.getParallel()));
  }else { // if error detected (no wall is detected on left side) rotate CCW until wall is found.
    motor.rotateControl = controlSystem.controlPID("angle", controlSystem.calculateError("angle", FIXEDROTATE));
  }
  //By setting this value to the define ANGLETOLERANCE the robot will be parallel before changing left distance. Increase to greater than the defined tolerance to make both actions at the same time
  if (abs(sensor.getParallel()) < ANGLETOLERANCE) { 
    // control Y with PID controller to be tuned in real life
    motor.x_controlEffort = controlSystem.controlPID( "left", controlSystem.calculateError("left", kalmanX.getFilteredValue()));
  }
  motor.powerMotors();// compiles the control efforts and sends power to motor drivers.
}

void turn() {
  // uses gyro to turn 90 degrees
  if (sensor.getGyroState() == 0) sensor.enableGyro();
  if (controlSystem.getDesiredAngle() != 90)  controlSystem.setDesiredAngle(90);// directs rotation control to target previously set at 90 degrees.
  // control Y with PID controller to be tuned in real life
  motor.rotateControl = controlSystem.controlPID( "angle", controlSystem.calculateError("angle", sensor.getAngle()));
  motor.powerMotors(); // compiles the control efforts and sends power to motor drivers.
}

void follow() {
  // utilises align function to align with wall whilst also driving forward based on 
  // distance infront. Control efforts are summed to increase timing efficiency
  align(); //Align handles left distance and parallel with the wall
  // control Y with PID controller to be tuned in real life
  motor.y_controlEffort = controlSystem.controlPID( "front", controlSystem.calculateError("front", kalmanY.getFilteredValue()));
  motor.powerMotors(); // compiles the control efforts and sends power to motor drivers.
}
void finished() {
  // stop motor controls when path is completed
  motor.x_controlEffort = 0;
  motor.y_controlEffort = 0;
  motor.rotateControl = 0;
  motor.powerMotors();
}
void stopped() {
  //Stop if Lipo Battery voltage is too low, to protect Battery
  static byte counter_lipo_voltage_ok;
  static unsigned long previous_millis;
  int Lipo_level_cal;
  if (motor.isEnabled())motor.disable_motors();
  slow_flash_LED_builtin();
}
//----------------------Helper functions-------------------------------------------------------------------------------------------------------------------------------

void filter(){
  // Filter sensor inputs for left distance and front distance
  motor.calcChangeDistance();
  kalmanX.filter(sensor.getLeftDistance(), motor.getDistanceChange_x());
  kalmanY.filter(sensor.getFrontDistance(), 0);
}

bool userInput() {
  // Determines if robot should take another lap
  Serial.println(" ");
  Serial.println("Another Lap? Y/N");
  char restartCtrl;
  while (Serial.available() == 0) {} //wait for user input
  restartCtrl = Serial.read(); // read input
  if (restartCtrl.equals("Y")) return 1;
  else return 0;
}

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
