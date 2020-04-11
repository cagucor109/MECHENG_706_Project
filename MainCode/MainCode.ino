/*
  MechEng 706 Base Code

  This code provides basic movement and sensor reading for the MechEng 706 Mecanum Wheel Robot Project

  Hardware:
    Arduino Mega2560 https://www.arduino.cc/en/Guide/ArduinoMega2560
    MPU-9250 https://www.sparkfun.com/products/13762
    Ultrasonic Sensor - HC-SR04 https://www.sparkfun.com/products/13959
    Infrared Proximity Sensor - Sharp https://www.sparkfun.com/products/242
    Infrared Proximity Sensor Short Range - Sharp https://www.sparkfun.com/products/12728
    Servo - Generic (Sub-Micro Size) https://www.sparkfun.com/products/9065
    Vex Motor Controller 29 https://www.vexrobotics.com/276-2193.html
    Vex Motors https://www.vexrobotics.com/motors.html
    Turnigy nano-tech 2200mah 2S https://hobbyking.com/en_us/turnigy-nano-tech-2200mah-2s-25-50c-lipo-pack.html

  Date: 11/11/2016
  Author: Logan Stuart
  Modified: 15/02/2018
  Author: Logan Stuart
*/

#define wheelRadius 24
#define dim 1.3889
#include <Servo.h>  //Need for Servo pulse output
// includes from sensor code
#include <math.h>

// include libraries
#include <C:\Users\munih\Documents\GitHub\MECHENG_706_Project\MainCode\libraries\Controllers\Controllers.h>
#include <C:\Users\munih\Documents\GitHub\MECHENG_706_Project\MainCode\libraries\Controllers\Controllers.cpp>
#include <C:\Users\munih\Documents\GitHub\MECHENG_706_Project\MainCode\libraries\Sensors\Sensors.h>
#include <C:\Users\munih\Documents\GitHub\MECHENG_706_Project\MainCode\libraries\Sensors\Sensors.cpp>
#define IR_LEFT_FRONT_PIN 59 //A5
#define IR_LEFT_FRONT_POSITION_CORRECTION 68.65 //This is a correction factor in mm that is added to take distance from center of robot parallel with the left face.
#define IR_LEFT_FRONT_OFFSET 114  //This is a correction factor in mm that is added to take distance from center of robot parallel with the front face.
#define IR_LEFT_BACK_PIN  58//A4
#define IR_LEFT_BACK_POSITION_CORRECTION 68.65 //This is a correction factor in mm that is added to take distance from center of robot parallel with the left face.
#define IR_LEFT_BACK_OFFSET 128 //This is a correction factor in mm that is added to take distance from center of robot parallel with the front face.
#define IR_FORWARD_PIN 63 //A9
#define IR_FORWARD_POSITION_CORRECTION 44.4 //This is a correction factor in mm that is added to take distance from center of robot.
#define IR_REAR_PIN 62//A8
#define IR_REAR_POSITION_CORRECTION 52 //This is a correction factor in mm that is added to take distance from center of robot.


#define GYRO_PIN 69 //A15
#define GYRO_CORRECTION 1.1 //correction factor in degrees per second
#define GYRO_READ_TIME  0.01 //this is for a 10ms loop. Change to suit

#define SONAR_TRIG_PIN 48 //D48
#define SONAR_ECHO_PIN 49 //D49
#define SONAR_POSITION_CORRECTION 78.25 //This is a correction factor in mm that is added to take distance from center of robot.

#define P_GAIN_LEFT 10
#define P_GAIN_FRONT 10
#define P_GAIN_ALLIGN 10

//#define NO_READ_GYRO  //Uncomment of GYRO is not attached.
//#define NO_HC-SR04 //Uncomment of HC-SR04 ultrasonic ranging sensor is not attached.
//#define NO_BATTERY_V_OK //Uncomment of BATTERY_V_OK if you do not care about battery damage.

//State machine states
enum STATE {
  INITIALISING,
  RUNNING,
  STOPPED
};
// modes for turning, following wall or setting up
enum MODE {
  SETTING_UP,
  EDGE_FOLLOW,
  TURNING,
  FINISHED
};
// sensor structures
struct Position{
  uint16_t frontDistance;
  uint16_t rearDistance;
  uint16_t leftDistance;  
  float Angle;
  float Parallel;
  };

Position sensor;


struct SensorCalibration{
  float irLeftFront[3]= {0.0182,0.0371,0.4368}; //this is bullshit. Need to think about the short range IR
  float irLeftBack[3]= {-0.0002,0.1025,-7.9823};
  float irForward[3]= {0,0.0232,-0.7972};
  float irRear[3]= {0,0.0127,0.0566};
  uint16_t longConstraints[2] ={110,490}; //Need to confirm this in matlab
  uint16_t shortConstraints[2]={115,630};
  };

  
SensorCalibration Coefficents;

//Refer to Shield Pinouts.jpg for pin locations

//Default motor control pins
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;


//Default ultrasonic ranging sensor pins, these pins are defined my the Shield
const int TRIG_PIN = 48;
const int ECHO_PIN = 49;

// Anything over 400 cm (23200 us pulse) is "out of range". Hit:If you decrease to this the ranging sensor but the timeout is short, you may not need to read up to 4meters.
const unsigned int MAX_DIST = 23200;

Servo left_font_motor;  // create servo object to control Vex Motor Controller 29
Servo left_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_font_motor;  // create servo object to control Vex Motor Controller 29
Servo turret_motor;


int speed_val = 100;
int speed_change;

// setup stuff for motor control from open loop
  double motorPower[4] = {0,1,2,3};
  float x_controlEffort = 0.0;
  float y_controlEffort = 0.0;
  float rotateControl = 0.0;


//Serial Pointer
HardwareSerial *SerialCom;

int pos = 0;
//stuff for edge follow

int front_back_error = 0;
int correction_angle = 0;
int correction_left = 0;
int correction_front = 0;
//float Coefficents[3] = {0.4368, 0.0371, 0.0182};


void setup(void)
{
  turret_motor.attach(11);
  pinMode(LED_BUILTIN, OUTPUT);

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial;
  SerialCom->begin(115200);
  SerialCom->println("MECHENG706_Base_Code_25/01/2018");
  delay(1000);
  SerialCom->println("Setup....");


  delay(1000); //settling time but no really needed

}

void loop(void) //main loop
{
  static STATE machine_state = INITIALISING;

  //Finite-state machine Code
  switch (machine_state) {
    case INITIALISING:
      machine_state = initialising();
      break;
    case RUNNING: //Lipo Battery Volage OK
      machine_state =  running();
      break;
    case STOPPED: //Stop of Lipo Battery voltage is too low, to protect Battery
      machine_state =  stopped();
      break;
  };
}


STATE initialising() {
  //initialising
  SerialCom->println("INITIALISING....");
  delay(1000); //One second delay to see the serial string "INITIALISING...."
  SerialCom->println("Enabling Motors...");
  enable_motors();
  SerialCom->println("RUNNING STATE...");
  return RUNNING;
}

STATE running() {
  static MODE runMode = SETTING_UP;
  static MODE nextMode = EDGE_FOLLOW;
  static Controllers controlSystem;  // declaring a control system object?
  static Sensors sensor;
  
  static unsigned long previous_millis;
  static int count_turns = 0;
  static int parallel = 0;
  static int left = 0;
  static int restartCtrl = 0;
  static int reached_wall = 0;
  read_serial_command();
  fast_flash_double_LED_builtin();

  //UPDATE SENSORS/INPUTS
  sensor.updateLeftDistance();
  sensor.updateParallel();
  sensor.updateFrontDistance();
  sensor.updateAngle();

  // NEXT STATE 
  switch(runMode){ 
    case SETTING_UP:
      if (sensor.getFrontDistance() < 150){
        nextMode = TURNING;
      }else if ((sensor.getLeftDistance() == 150) && (sensor.getParallel() == 0)){
        nextMode = EDGE_FOLLOW;
      }else { 
        nextMode = SETTING_UP;
      }
      break;
    case EDGE_FOLLOW:
        if ((count_turns < 4) && (sensor.getFrontDistance() < 150)){  // wall is too close muse turn
          nextMode = TURNING;
        }else if ((count_turns < 4) && (sensor.getFrontDistance() > 150)){  //  wall is not close enough
          nextMode = EDGE_FOLLOW;
        }else {
          nextMode = FINISHED;
        }
      break;
    case TURNING:   // turns with a flat rate based on gyro reading until about 75 deg then enters setup mode.
      if ((sensor.getGyroState() == 0) && (sensor.getLeftDistance() == 150) && (sensor.getParallel() == 0)){ // can change this so that it loops back into settin up mode before going back into edge follow for safety.
        nextMode = EDGE_FOLLOW; 
      }else {
        nextMode = TURNING;
      }
      break;
    case FINISHED:
      if (restartCtrl == 1){
        nextMode = SETTING_UP;
      }else{
        nextMode = FINISHED;
      }
        
      break;
  }
  rotateControl = 0;
  x_controlEffort = 0;
  y_controlEffort = 0;
  // STATE OUTPUTS
  switch(runMode){ 
    case SETTING_UP:
      if (nextMode == SETTING_UP){
        rotateControl = controlSystem.controlP("angle", controlSystem.calculateError("angle", sensor.getParallel()));
        x_controlEffort = controlSystem.controlP( "left",controlSystem.calculateError("left", sensor.getLeftDistance()));
      }else{
        // stop motor control to not overshoot
        rotateControl = 0;
        x_controlEffort = 0;
        y_controlEffort = 0;
        if (nextMode == TURNING){
          sensor.enableGyro(); // if turning start the gyro
          controlSystem.setDesiredAngle(90); // set turn target to 90 degrees
        }else { // edge follow mode 
          sensor.disableGyro();
          controlSystem.setDesiredAngle(0); // set turn target to 0 degrees
        }
      }
        
      /* update boolean 
      left = checkLeftDist();
      parallel = checkParallel();
      controlSystem.getxco = -100;
      rotateControl = -90;
      if ((parallel == 1) && (left == 1)){ // if the robot is parallel and at the correct distance(ie oriented properly at starting position)
        runMode = EDGE_FOLLOW;
      }*/
      break;
    case EDGE_FOLLOW:
      if (nextMode == EDGE_FOLLOW){
        if (sensor.getFrontDistance() > 150){
          y_controlEffort = controlSystem.controlP( "front",controlSystem.calculateError("front", sensor.getFrontDistance()));
        }
        if (sensor.getLeftDistance() != 150){
          x_controlEffort = controlSystem.controlP( "left",controlSystem.calculateError("left", sensor.getLeftDistance()));
        }
        if (sensor.getAngle() != 0){
          rotateControl = controlSystem.controlP("angle", controlSystem.calculateError("angle", sensor.getParallel()));
        }
      }else {
        //stop to turn or finish
        rotateControl = 0;
        x_controlEffort = 0;
        y_controlEffort = 0;
        if (nextMode == TURNING){ // if turning start the gyro
          sensor.enableGyro();
          controlSystem.setDesiredAngle(90); // set turn target to 90 degrees
        }else { // move into finished mode
          sensor.disableGyro();
          controlSystem.setDesiredAngle(0);
        }
      }
      break;
    case TURNING:  
      if (nextMode == TURNING){
        rotateControl = controlSystem.controlP( "angle",controlSystem.calculateError("angle", sensor.getAngle())); // directs rotation control to target previously set at 90 degrees.
      } else if (nextMode == EDGE_FOLLOW){
        // turn off motor controls
        rotateControl = 0;
        x_controlEffort = 0;
        y_controlEffort = 0;
        // disable gyro and reset angle to 0
        sensor.disableGyro();
        controlSystem.setDesiredAngle(0);
      }else {
        
      }
      break;
    case FINISHED:
      // stop motor controls
      x_controlEffort = 0;
      y_controlEffort = 0;
      rotateControl = 0;
      // ask user if they want to complete another lap
      Serial.println(" ");
      Serial.println("Another Lap?");   
      while(Serial.available()==0) {} //wait for user input
      restartCtrl=Serial.parseInt(); // read input
      
      break;
  }
  powerMotors(); // compiles the control efforts and sends power to motor drivers.

  if (millis() - previous_millis > 500) {  //Arduino style 500ms timed execution statement
    previous_millis = millis();

    SerialCom->println("RUNNING---------");
    speed_change_smooth();
    Analog_Range_A4();

#ifndef NO_READ_GYRO
    GYRO_reading();
#endif

#ifndef NO_HC-SR04
    HC_SR04_range();
#endif

#ifndef NO_BATTERY_V_OK
    if (!is_battery_voltage_OK()) return STOPPED;
#endif


    turret_motor.write(pos);

    if (pos == 0)
    {
      pos = 45;
    }
    else
    {
      pos = 0;
    }
  }

  return RUNNING;
}

//Stop of Lipo Battery voltage is too low, to protect Battery
STATE stopped() {
  static byte counter_lipo_voltage_ok;
  static unsigned long previous_millis;
  int Lipo_level_cal;
  disable_motors();
  slow_flash_LED_builtin();

  if (millis() - previous_millis > 500) { //print massage every 500ms
    previous_millis = millis();
    SerialCom->println("STOPPED---------");


#ifndef NO_BATTERY_V_OK
    //500ms timed if statement to check lipo and output speed settings
    if (is_battery_voltage_OK()) {
      SerialCom->print("Lipo OK waiting of voltage Counter 10 < ");
      SerialCom->println(counter_lipo_voltage_ok);
      counter_lipo_voltage_ok++;
      if (counter_lipo_voltage_ok > 10) { //Making sure lipo voltage is stable
        counter_lipo_voltage_ok = 0;
        enable_motors();
        SerialCom->println("Lipo OK returning to RUN STATE");
        return RUNNING;
      }
    } else
    {
      counter_lipo_voltage_ok = 0;
    }
#endif
  }
  return STOPPED;
}


/*void read_front_back_error(){        stuff from edge follow we dont need anymore with sensor codes?
  float front_reading = sensor.frontDistance;
  Serial.print("The front distance value is: ");
  Serial.println(front_reading);
  float back_reading = sensor.rearDistance;
  Serial.print("The back distance value is: ");
  Serial.println(back_reading);
  front_back_error = front_reading - back_reading;
}

float RearIR() {
  return analogRead(A4);
}

float FrontIR() {
  return analogRead(A5);
}

float CalcVoltage(float readValue) {
  return readValue * 5 / 1023;
}

float CalcInvDist( float Voltage) {
  return (Coefficents[2] * Voltage * Voltage) + (Coefficents[1] * Voltage) + Coefficents[0];//currently uses a second order polynomial fitting.
}

float CalcDist ( float InvDist) {
  return 1 / (InvDist - .42);
}

int checkFrontDist (){ // this function checks front distance and adjusts control input 
  
  y_controlEffort = controlSystem.controlP( "front",controlSystem.calculateError("front", sensor.frontDistance));
  if (sensor.frontDistance <  400){
    y_controlEffort = correction_front;
    if (sensor.frontDistance <180){ // could be tuned depending on update rate of sensors.
      return 1;
    }
    return 0;
  }else {
    y_controlEffort = 300; // base speed NEED TO TUNE THIS TO TIMING CONSTRAINTS
    return 0;
  }
}
int checkParallel(){ // this function finds the amount of rotation w the robot needs to do.
  /*positive correction will turn anticlockwise
  Serial.print("Correction: ");
  Serial.println(correction);
  left_font_motor.writeMicroseconds(1500 + speed_val - correction);
  left_rear_motor.writeMicroseconds(1500 + speed_val - correction);
  right_rear_motor.writeMicroseconds(1500 - speed_val - correction);
  right_font_motor.writeMicroseconds(1500 - speed_val - correction); 

  // if its parallel do nothing or if the magnitude is greater than 0 adjust the rotation
  correction_angle = P_GAIN_ALLIGN * sensor.Parallel;
  if (sensor.Parallel == 0){
    rotateControl = 0;
    return 1;
  }else if (abs(sensor.Parallel) > 0){
    rotateControl = correction_angle;
    return 0;
  }
  return 0;
}

int checkLeftDist(){
  // there is a negative one at the start due to the flipping of coordinate systems with openloop control.
  float left_error = (150 - sensor.leftDistance);
  correction_front = (-1)*P_GAIN_LEFT*left_error;
  if (sensor.leftDistance ==  150){
    x_controlEffort = 0;
    return 1;
  }else {
    x_controlEffort = correction_front;
    return 0;
  }
}*/

void powerMotors(){ // this function sends power to motors all at once
  directionControl(x_controlEffort,y_controlEffort,rotateControl, motorPower);  

  left_font_motor.writeMicroseconds(1500 + motorPower[0]);
  right_font_motor.writeMicroseconds(1500 + motorPower[1]);
  left_rear_motor.writeMicroseconds(1500 + motorPower[2]);
  right_rear_motor.writeMicroseconds(1500 + motorPower[3]);
}


// end of edge follow code
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

void speed_change_smooth()
{
  speed_val += speed_change;
  if (speed_val > 1000)
    speed_val = 1000;
  speed_change = 0;
}

#ifndef NO_BATTERY_V_OK
boolean is_battery_voltage_OK()
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
#endif

#ifndef NO_HC-SR04
void HC_SR04_range()
{
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 0 ) {
    t2 = micros();
    pulse_width = t2 - t1;
    if ( pulse_width > (MAX_DIST + 1000)) {
      SerialCom->println("HC-SR04: NOT found");
      return;
    }
  }

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min

  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1)
  {
    t2 = micros();
    pulse_width = t2 - t1;
    if ( pulse_width > (MAX_DIST + 1000) ) {
      SerialCom->println("HC-SR04: Out of range");
      return;
    }
  }

  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    SerialCom->println("HC-SR04: Out of range");
  } else {
    SerialCom->print("HC-SR04:");
    SerialCom->print(cm);
    SerialCom->println("cm");
  }
}
#endif

void Analog_Range_A4()
{
  SerialCom->print("Analog Range A4:");
  SerialCom->println(analogRead(A4));
}

#ifndef NO_READ_GYRO
void GYRO_reading()
{
  SerialCom->print("GYRO A3:");
  SerialCom->println(analogRead(A3));
}
#endif

//Serial command pasing
void read_serial_command()
{
  if (SerialCom->available()) {
    char val = SerialCom->read();
    SerialCom->print("Speed:");
    SerialCom->print(speed_val);
    SerialCom->print(" ms ");

    //Perform an action depending on the command
    switch (val) {
      case 'w'://Move Forward
      case 'W':
        forward ();
        SerialCom->println("Forward");
        break;
      case 's'://Move Backwards
      case 'S':
        reverse ();
        SerialCom->println("Backwards");
        break;
      case 'q'://Turn Left
      case 'Q':
        strafe_left();
        SerialCom->println("Strafe Left");
        break;
      case 'e'://Turn Right
      case 'E':
        strafe_right();
        SerialCom->println("Strafe Right");
        break;
      case 'a'://Turn Right
      case 'A':
        ccw();
        SerialCom->println("ccw");
        break;
      case 'd'://Turn Right
      case 'D':
        cw();
        SerialCom->println("cw");
        break;
      case '-'://Turn Right
      case '_':
        speed_change = -100;
        SerialCom->println("-100");
        break;
      case '=':
      case '+':
        speed_change = 100;
        SerialCom->println("+");
        break;
      default:
        stop();
        SerialCom->println("stop");
        break;
    }

  }

}

//----------------------Motor moments------------------------
//The Vex Motor Controller 29 use Servo Control signals to determine speed and direction, with 0 degrees meaning neutral https://en.wikipedia.org/wiki/Servo_control

void disable_motors()
{
  left_font_motor.detach();  // detach the servo on pin left_front to turn Vex Motor Controller 29 Off
  left_rear_motor.detach();  // detach the servo on pin left_rear to turn Vex Motor Controller 29 Off
  right_rear_motor.detach();  // detach the servo on pin right_rear to turn Vex Motor Controller 29 Off
  right_font_motor.detach();  // detach the servo on pin right_front to turn Vex Motor Controller 29 Off

  pinMode(left_front, INPUT);
  pinMode(left_rear, INPUT);
  pinMode(right_rear, INPUT);
  pinMode(right_front, INPUT);
}

void enable_motors()
{
  left_font_motor.attach(left_front);  // attaches the servo on pin left_front to turn Vex Motor Controller 29 On
  left_rear_motor.attach(left_rear);  // attaches the servo on pin left_rear to turn Vex Motor Controller 29 On
  right_rear_motor.attach(right_rear);  // attaches the servo on pin right_rear to turn Vex Motor Controller 29 On
  right_font_motor.attach(right_front);  // attaches the servo on pin right_front to turn Vex Motor Controller 29 On
}
void stop() //Stop
{
  left_font_motor.writeMicroseconds(1500);
  left_rear_motor.writeMicroseconds(1500);
  right_rear_motor.writeMicroseconds(1500);
  right_font_motor.writeMicroseconds(1500);
}

void forward()
{
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

//new stuff for openloop control
void directionControl (double control_x, double control_y, double w, double *motorPower) {
     
  *motorPower = (control_x + control_y + dim*w);
  *(motorPower+1) = (control_x - control_y + dim*w);
  *(motorPower+2) = (-control_x + control_y + dim*w);
  *(motorPower+3) = (-control_x - control_y + dim*w);

   
   Serial.println("after the math");
   Serial.println(motorPower[0]);
    Serial.println(motorPower[1]);
     Serial.println(motorPower[2]);
     Serial.println(motorPower[3]);
}
//

void reverse ()
{
  double motorPower[4] = {0,1,2,3};
  float x_controlEffort = 0.0;
  float y_controlEffort = 0.0;
  float rotateControl = 0.0;
  Serial.println(" ");
  Serial.println("Enter x component");   
  while(Serial.available()==0) {} //wait for user input
  x_controlEffort=Serial.parseFloat(); // read input
      
  Serial.println("Enter y component");   
  while(Serial.available()==0) {}
  y_controlEffort=Serial.parseFloat();  

   Serial.println("Enter rotation component");   
  while(Serial.available()==0) {}
  rotateControl=Serial.parseFloat();
      
  directionControl(x_controlEffort,y_controlEffort,rotateControl, motorPower);  
  Serial.println("when it comes out");
  Serial.println(motorPower[0],4);
  Serial.println(motorPower[1],4);
  Serial.println(motorPower[2],4);
  Serial.println(motorPower[3],4 );

  left_font_motor.writeMicroseconds(1500 + motorPower[0]);
  right_font_motor.writeMicroseconds(1500 + motorPower[1]);
  left_rear_motor.writeMicroseconds(1500 + motorPower[2]);
  right_rear_motor.writeMicroseconds(1500 + motorPower[3]);
  /*
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);*/
}

void ccw ()
{
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void cw ()
{
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}

void strafe_left ()
{
  left_font_motor.writeMicroseconds(1500 - speed_val);
  left_rear_motor.writeMicroseconds(1500 + speed_val);
  right_rear_motor.writeMicroseconds(1500 + speed_val);
  right_font_motor.writeMicroseconds(1500 - speed_val);
}

void strafe_right ()
{
  left_font_motor.writeMicroseconds(1500 + speed_val);
  left_rear_motor.writeMicroseconds(1500 - speed_val);
  right_rear_motor.writeMicroseconds(1500 - speed_val);
  right_font_motor.writeMicroseconds(1500 + speed_val);
}
