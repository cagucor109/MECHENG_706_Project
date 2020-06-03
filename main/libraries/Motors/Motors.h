#ifndef MOTORS_H
#define MOTORS_H
#include "Arduino.h"
#include <Servo.h>  //Need for Servo pulse output

#define dim 1.3889 //For motor power calculation
#define MAXPOWER 500 //change this to make the robot go faster, valid range is 0 to 800
#define ZERO_OFFSET 1500 //Zero value of servo motors

#define LEFT_FRONT 46
#define LEFT_REAR 47
#define RIGHT_FRONT 51 
#define RIGHT_REAR 50
#define FAN_PIN 53 //update



class Motors {
  public:

    //constructor
    Motors();
    //getters
	void desiredControl(double x_controlEffort, double y_controlEffort, double rotateControl);

    //utilities
    void disable_motors();
    void enable_motors();
    void powerMotors();
    void calcMotorPower(double control_x, double control_y, double w);
	void controlFan(bool state);
	

  private:
	
	bool motorsEnabled;
	
    Servo *left_front_motor;
    Servo *left_rear_motor;
    Servo *right_rear_motor;
    Servo *right_front_motor;

	double motorPower[4];
	double x_controlEffort;	
    double y_controlEffort;	
    double rotateControl;
	
};

#endif