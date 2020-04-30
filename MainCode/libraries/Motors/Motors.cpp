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

double Motors::getDistanceChange_x() {
	return distanceChange_x;
}

double Motors::getDistanceChange_y() {
	return distanceChange_y;
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
  
  motorsEnabled = false;
}

void Motors::enable_motors()
{
  left_front_motor->attach(LEFT_FRONT);  
  left_rear_motor->attach(LEFT_REAR); 
  right_rear_motor->attach(RIGHT_REAR); 
  right_front_motor->attach(RIGHT_FRONT);

  motorsEnabled = true;
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
  for (int i=1;i<sizeof(motorPower);i++){
	  if(maxValue<abs(motorPower[i])) maxValue=abs(motorPower[i]);
  }
  
  //Scaling motorPowers proportionally to a valid range if currently out of range.
  if(maxValue>MAXPOWER){
  for(int i=0;i<sizeof(motorPower);i++){
	  motorPower[i]= (motorPower[i]/maxValue)*MAXPOWER;
  }
  }
}

double Motors::calcChangeDistance() {
	
	newTime=millis();
	timeStep=newTime-previousTime;
	previousTime=newTime;
		
	int i;
	
	for (i = 0; i<4; i++) {
		this->theta_dot[i] = POWER2ROTATION_FACTOR * motorPower[i];
	}
	
	//forward kinematics
	this->velocity_x = this->theta_dot[0] + this->theta_dot[1] + this->theta_dot[2] + this->theta_dot[3];
	this->velocity_y = this->theta_dot[0]*-1 + this->theta_dot[1] + this->theta_dot[2] + this->theta_dot[3]*-1;
	
	//update prevVelocity for next iteration
	this->prevVelocity_x = this->velocity_x;
	this->prevVelocity_y = this->velocity_y;
	
	this->distanceChange_x = ((this->prevVelocity_x + this->velocity_x)/2)*timeStep;
	this->distanceChange_y = ((this->prevVelocity_y + this->velocity_y)/2)*timeStep;
 
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

  left_front_motor->writeMicroseconds(ZERO_OFFSET + motorPower[0]);
  right_front_motor->writeMicroseconds(ZERO_OFFSET + motorPower[1]);
  left_rear_motor->writeMicroseconds(ZERO_OFFSET + motorPower[2]);
  right_rear_motor->writeMicroseconds(ZERO_OFFSET + motorPower[3]);
}


