#include "Motors.h"

Motors::Motors() {
  this->left_front_motor= new Servo();
  this->left_rear_motor= new Servo();
  this->right_rear_motor= new Servo();
  this->right_front_motor= new Servo();
  this->motorsEnabled = false;
}

//setters
void Motors::desiredControl(double x_controlEffort, double y_controlEffort, double rotateControl) {
	this->x_controlEffort = x_controlEffort;
	this->y_controlEffort = y_controlEffort;
	this->rotateControl = rotateControl;
}

void Motors::enable_motors()
{
  left_front_motor->attach(LEFT_FRONT);  
  left_rear_motor->attach(LEFT_REAR); 
  right_rear_motor->attach(RIGHT_REAR); 
  right_front_motor->attach(RIGHT_FRONT);
  
  pinMode(FAN_PIN, OUTPUT);

  motorsEnabled = true;
  
}

void Motors::disable_motors()
{
  left_front_motor->detach();  
  left_rear_motor->detach(); 
  right_rear_motor->detach(); 
  right_front_motor->detach();

  pinMode(LEFT_FRONT, INPUT);
  pinMode(LEFT_REAR, INPUT);
  pinMode(RIGHT_REAR, INPUT);
  pinMode(RIGHT_FRONT, INPUT);
  pinMode(FAN_PIN, INPUT);
  
  motorsEnabled = false;
}

void Motors::powerMotors() { // this function sends power to motors all at once
  calcMotorPower(x_controlEffort, y_controlEffort, rotateControl);

  left_front_motor->writeMicroseconds(ZERO_OFFSET + motorPower[0]);
  right_front_motor->writeMicroseconds(ZERO_OFFSET + motorPower[1]);
  left_rear_motor->writeMicroseconds(ZERO_OFFSET + motorPower[2]);
  right_rear_motor->writeMicroseconds(ZERO_OFFSET + motorPower[3]);
}


void Motors::calcMotorPower(double control_x, double control_y, double w) {

  motorPower[0] = (control_x + control_y + dim * w);
  motorPower[1] = (control_x - control_y + dim * w);
  motorPower[2] = (-control_x + control_y + dim * w);
  motorPower[3] = (-control_x - control_y + dim * w);
  
  double maxValue=abs(motorPower[0]);
  for (int i=1;i<4;i++){
	  if(maxValue<abs(motorPower[i])) maxValue=abs(motorPower[i]);
  }
  
  //Scaling motorPowers proportionally to a valid range if currently out of range.
  if(maxValue>MAXPOWER){
  for(int i=0;i<4;i++){
	  motorPower[i]= (motorPower[i]/maxValue)*MAXPOWER;
  }
  }
}

void Motors::controlFan(bool state) {
	if (state == true) {
		digitalWrite(FAN_PIN, HIGH);
	} else {
		digitalWrite(FAN_PIN, LOW);
	}
}
