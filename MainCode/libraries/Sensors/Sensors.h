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

#define UPDATEGYROTIME 10 //this is for a 10ms read period
#define GYRO_READ_TIME  0.01 //this is for a 10ms read period
#define GYRO_PIN 69 //A15
#define GYRO_CORRECTION 1.1 //correction factor in degrees per second

#define SONAR_TRIG_PIN 48 //D48
#define SONAR_ECHO_PIN 49 //D49
#define SONAR_POSITION_CORRECTION 78.25 //This is a correction factor in mm that is added to take distance from center of robot.

#include "Arduino.h"
#include <math.h>

class Sensors {
public:
    // constructors
    Sensors();

    // getters
	uint16_t getFrontDistance();
	uint16_t getRearDistance();
	uint16_t getLeftDistance();
	float getAngle();
	float getParallel();
	bool getGyroState();
	bool getParallelError();

    // utilities
	uint16_t readIr(byte readPin, float Coefficents[],uint16_t Constraints[]);
	void enableGyro();
	void disableGyro();
	void updateAngle();
	void updateLeftDistance();
	void updateFrontDistance();
	void updateRearDistance();
	void updateParallel();


private: 
	//Robot Position variables
	uint16_t frontDistance; 
	uint16_t rearDistance;
	uint16_t leftDistance;  
	float Angle; // angle positive if rotated clockwise
	float Parallel; // angle positive if rotated clockwise
	
	//Sensor state variables
	bool parallelError;
	bool gyroState;
	
	//IR calibration coefficent and calibration range limits
	const float irLeftFront[3]= {-0.0002,0.1007,-7.5392};
	const float irLeftBack[3]= {-0.0002,0.1025,-7.9823};
	const float irForward[3]= {0,0.0232,-0.7972};
	const float irRear[3]= {0,0.0127,0.0566};
	const uint16_t longConstraints[2] ={110,500}; //Need to confirm this in matlab
	const uint16_t shortConstraints[2]={40,400}; //constraints are relative to analog reading where read range is 0-1024 //Should change these to mm and apply after the calibration equation.
	
};

#endif