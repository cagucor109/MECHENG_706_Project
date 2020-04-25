#include "Motors.h"

Motors::Motors() {
  this->left_front_motor= new Servo();
  this->left_rear_motor= new Servo();
  this->right_rear_motor= new Servo();
  this->right_front_motor= new Servo();
	this->motorsEnabled = false;

}

bool Motors::isEnabled(){
	return motorsEnabled;
}

void Motors::disable_motors()
{
  left_front_motor->detach();  // detach the servo on pin left_front to turn Vex Motor Controller 29 Off
  left_rear_motor->detach();  // detach the servo on pin left_rear to turn Vex Motor Controller 29 Off
  right_rear_motor->detach();  // detach the servo on pin right_rear to turn Vex Motor Controller 29 Off
  right_front_motor->detach();  // detach the servo on pin right_front to turn Vex Motor Controller 29 Off
	motorsEnabled = false;
  pinMode(left_front, INPUT);
  pinMode(left_rear, INPUT);
  pinMode(right_rear, INPUT);
  pinMode(right_front, INPUT);
}

void Motors::enable_motors()
{
		motorsEnabled = true;
  left_front_motor->attach(left_front);  // attaches the servo on pin left_front to turn Vex Motor Controller 29 On
  left_rear_motor->attach(left_rear);  // attaches the servo on pin left_rear to turn Vex Motor Controller 29 On
  right_rear_motor->attach(right_rear);  // attaches the servo on pin right_rear to turn Vex Motor Controller 29 On
  right_front_motor->attach(right_front);  // attaches the servo on pin right_front to turn Vex Motor Controller 29 On
}


void Motors::powerMotors() { // this function sends power to motors all at once
  calcMotorPower(x_controlEffort, y_controlEffort, rotateControl);

  left_front_motor->writeMicroseconds(1500 + motorPower[0]);
  right_front_motor->writeMicroseconds(1500 + motorPower[1]);
  left_rear_motor->writeMicroseconds(1500 + motorPower[2]);
  right_rear_motor->writeMicroseconds(1500 + motorPower[3]);
}

//new stuff for openloop control
void Motors::calcMotorPower(double control_x, double control_y, double w) {

  motorPower[0] = (control_x + control_y + dim * w);
  motorPower[1] = (control_x - control_y + dim * w);
  motorPower[2] = (-control_x + control_y + dim * w);
  motorPower[3] = (-control_x - control_y + dim * w);

}

void Motors::motorsDebug()  //This is for debugging and testing DirectionalControl
{
  Serial.println(" ");
  Serial.println("Enter x component");
  while (Serial.available() == 0) {} //wait for user input
  x_controlEffort = Serial.parseFloat(); // read input

  Serial.println("Enter y component");
  while (Serial.available() == 0) {}
  y_controlEffort = Serial.parseFloat();

  Serial.println("Enter rotation component");
  while (Serial.available() == 0) {}
  rotateControl = Serial.parseFloat();

  calcMotorPower(x_controlEffort, y_controlEffort, rotateControl);
  Serial.println("when it comes out");
  Serial.println(motorPower[0], 4);
  Serial.println(motorPower[1], 4);
  Serial.println(motorPower[2], 4);
  Serial.println(motorPower[3], 4 );

  left_front_motor->writeMicroseconds(1500 + motorPower[0]);
  right_front_motor->writeMicroseconds(1500 + motorPower[1]);
  left_rear_motor->writeMicroseconds(1500 + motorPower[2]);
  right_rear_motor->writeMicroseconds(1500 + motorPower[3]);
}
