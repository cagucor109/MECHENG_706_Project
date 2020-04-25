#ifndef MOTORS_H
#define MOTORS_H
#include "Arduino.h"
#include <Servo.h>  //Need for Servo pulse output

#define dim 1.3889 //For motor power calculation

class Motors {
  public:

    //Constructor
    Motors();
    //getters
	bool isEnabled();
    //setters
    
    //utilities
    void disable_motors();
    void enable_motors();
    void powerMotors();
    void calcMotorPower(double control_x, double control_y, double w);
    void motorsDebug();
	 

	//Reconfigure these to be private with getters and setters or make external variables that are passed via methods.
	float x_controlEffort;
    float y_controlEffort;
    float rotateControl;

  private:

    //Default motor control pins
    const byte left_front = 46;
    const byte left_rear = 47;
    const byte right_rear = 50;
    const byte right_front = 51;
	
	bool motorsEnabled;
    Servo *left_front_motor;
    Servo *left_rear_motor;
    Servo *right_rear_motor;
    Servo *right_front_motor;

    double motorPower[4];


};

#endif
