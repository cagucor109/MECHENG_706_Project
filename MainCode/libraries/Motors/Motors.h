#ifndef MOTORS_H
#define MOTORS_H
#include "Arduino.h"
#include <Servo.h>  //Need for Servo pulse output

#define dim 1.3889 //For motor power calculation
#define MAXPOWER 500 //change this to make the robot go faster, valid range is 0 to 800
#define ZERO_OFFSET 1500 //Zero value of servo motors
#define POWER2ROTATION_FACTOR 0.01 //PWM power value sent to motors * factor = angular velocity of shaft
//#define TIMESTEP 0.01 //timestep update for integrating velocity to distance


#define LEFT_FRONT 46
#define LEFT_REAR 47
#define RIGHT_FRONT 51 
#define RIGHT_REAR 50



class Motors {
  public:

    //constructor
    Motors();
    //getters
	bool isEnabled();
	double getDistanceChange_x();
	double getDistanceChange_y();
    
    //utilities
    void disable_motors();
    void enable_motors();
    void powerMotors();
    void calcMotorPower(double control_x, double control_y, double w);
	double calcChangeDistance();
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
	double theta_dot[4];
	double prevVelocity_x;
	double velocity_x;
	double prevVelocity_y;
	double velocity_y;
	double distanceChange_x;
	double distanceChange_y;
	double previousTime
	double newTime
	double timeStep
	
};

#endif
