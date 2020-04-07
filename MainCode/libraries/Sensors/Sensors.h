#ifndef SENSORS_H
#define SENSORS_H


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

#include "Arduino.h"
#include <math.h>

class Sensors {
public: //These are the methods
    // constructors
    Sensors();

    // getters
	uint16_t getFrontDistance();
	uint16_t getRearDistance();
	uint16_t getLeftDistance();
	float getAngle();
	float getParallel();
	bool getGyroState();

    // utilities
uint16_t readIr(byte readPin, float Coefficents[],uint16_t Constraints[]);
void enableGyro();
void disableGyro();
void updateAngle();
void updateLeftDistance();
void updateFrontDistance();
void updateRearDistance();
void updateParallel();


private:  //These are the variables
 //These need getters
  uint16_t frontDistance; 
  uint16_t rearDistance;
  uint16_t leftDistance;  
  float Angle;
  float Parallel;
  bool gyroState;
  
  //These are not accessed by external code. //Move the assigning of values to the constructor.
    float irLeftFront[3]; //this is bullshit. Need to think about the short range IR
  float irLeftBack[3];
  float irForward[3];
  float irRear[3];
  uint16_t longConstraints[2]; //Need to confirm this in matlab
  uint16_t shortConstraints[2];

};

#endif