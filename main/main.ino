// Project 2 (Firefighter project) for MechEng706 Mechatronics design
// Authors (Group 21):  Narada Hu, nhu769
//                   :
//                   :
//                   :

// This is the main file for controlling the vex robot. It includes the
// main behaviour control system and calls upon supporting functions and
// files in the libraries. The purpose of the program is to control a vex
// robot to extinguish fires when placed in a walled area with unknown
// obstacles and fire locations. Behaviour and fuzzy logic control are used
// to implement this system.


//----------------------Libraries------------------------------------------------------------------------------------------------------------------------------------------
// include libraries
#include "libraries\Controllers\Controllers.h"
#include "libraries\Controllers\Controllers.cpp"
#include "libraries\Sensors\Sensors.h"
#include "libraries\Sensors\Sensors.cpp"
#include "libraries\Motors\Motors.h"
#include "libraries\Motors\Motors.cpp"

//Initializing global objects
Controllers controlSystem;
Sensors sensor;
Motors motor;

//----------------------Battery check and Serial Comms---------------------------------------------------------------------------------------------------------------------
//Serial Pointer
HardwareSerial *SerialCom;
//----------------------Setup------------------------------------------------------------------------------------------------------------------------------------------
#define FIRES_LEFT 2

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


// Declare commands and command flags
MOTION halt_command;
int halt_output_flag;
MOTION extinguish_command;
int extinguish_output_flag;
MOTION avoid_command;
int avoid_output_flag;
MOTION moveToFire_command;
int moveToFire_output_flag;
MOTION locate_command;
int locate_output_flag;

MOTION motor_input;

//----------------------Main loop------------------------------------------------------------------------------------------------------------------------------------------

void main () {
  // the main loop updates sensors then selects the behaviour
  // based on the sensor inputs and sends them to the motors.

  // update functions need to be inplemented
  sensor.updateDistances();
  sensor.updatePhotos();
  Suppressor();
  motor.updateMotors();
}

//----------------------Sensing functions------------------------------------------------------------------------------------------------------------------------------------------
bool locate_output_flag() {

}
bool extinguish_output_flag() {

}
bool moveToFire_output_flag() {

}
bool halt_output_flag() {

}
bool avoid_output_flag() {

}
//----------------------State output Functions------------------------------------------------------------------------------------------------------------------------------------------

void halt() {
  halt_command = STOP;
  halt_output_flag = 1;
}


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
  sleep(tick);
}
