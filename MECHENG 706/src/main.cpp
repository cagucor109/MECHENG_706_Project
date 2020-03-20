#include <Arduino.h>
#include "Controllers.h"

#define wheelRadius 24
#define dim 180
#include <Servo.h>  //Need for Servo pulse output

//Default motor control pins
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;

Servo left_front_motor;  // create servo object to control Vex Motor Controller 29
Servo left_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_front_motor;  // create servo object to control Vex Motor Controller 29

Controllers controller();

enum STATE {
  INITIALISING,
  RUNNING,
  STOPPED
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("MECHENG 706 Main code (19/3/2020)...");
  delay(500);
  Serial.println("Setup...");
}

void loop() {
  // put your main code here, to run repeatedly:
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
  Serial.println("INITIALISING...");
  delay(1000); //One second delay to see the serial string "INITIALISING...."
  Serial.println("Enabling Motors...");
  enable_motors();
  Serial.println("RUNNING STATE...");
  return RUNNING;
}

STATE running(){

  Serial.println("RUNNING...");

  // add safety for battery percentage to change state to STOPPED

  return RUNNING;
}

STATE stopped() {
  
  return STOPPED;
}

void disable_motors(){
  left_front_motor.detach();  // detach the servo on pin left_front to turn Vex Motor Controller 29 Off
  left_rear_motor.detach();  // detach the servo on pin left_rear to turn Vex Motor Controller 29 Off
  right_rear_motor.detach();  // detach the servo on pin right_rear to turn Vex Motor Controller 29 Off
  right_front_motor.detach();  // detach the servo on pin right_front to turn Vex Motor Controller 29 Off

  pinMode(left_front, INPUT);
  pinMode(left_rear, INPUT);
  pinMode(right_rear, INPUT);
  pinMode(right_front, INPUT);
}

void enable_motors(){
  left_front_motor.attach(left_front);  // attaches the servo on pin left_front to turn Vex Motor Controller 29 On
  left_rear_motor.attach(left_rear);  // attaches the servo on pin left_rear to turn Vex Motor Controller 29 On
  right_rear_motor.attach(right_rear);  // attaches the servo on pin right_rear to turn Vex Motor Controller 29 On
  right_front_motor.attach(right_front);  // attaches the servo on pin right_front to turn Vex Motor Controller 29 On
}
void stop(){
  left_front_motor.writeMicroseconds(1500);
  left_rear_motor.writeMicroseconds(1500);
  right_rear_motor.writeMicroseconds(1500);
  right_front_motor.writeMicroseconds(1500);
}

//new stuff
void directionControl (double control_x, double control_y, double w, double *motorPower) {
     
  *motorPower = (control_x + control_y - dim*w);
  *(motorPower+1) = (control_x - control_y + dim*w);
  *(motorPower+2) = (-control_x + control_y + dim*w);
  *(motorPower+3) = (-control_x - control_y - dim*w);
}