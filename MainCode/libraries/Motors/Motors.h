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

class Motors {
  public:

    //Constructor
    Motors();
    //getters
	bool isEnabled();
    
    //utilities
    void disable_motors();
    void enable_motors();
    void powerMotors();
    void calcMotorPower(double control_x, double control_y, double w);
    void motorsDebug();
	 

			
	float x_controlEffort;	//Reconfigure these to be private with getters and setters or make external variables that are passed via methods.
    float y_controlEffort;	//Reconfigure these to be private with getters and setters or make external variables that are passed via methods.
    float rotateControl;	//Reconfigure these to be private with getters and setters or make external variables that are passed via methods.

  private:
	
	bool motorsEnabled;
	
    Servo *left_front_motor;
    Servo *left_rear_motor;
    Servo *right_rear_motor;
    Servo *right_front_motor;

    double motorPower[4];
};

#endif
