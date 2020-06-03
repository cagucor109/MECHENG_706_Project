#ifndef SENSORS_H
#define SENSORS_H

#define IR_LEFT_SIDE_PIN 59 //A5
#define IR_RIGHT_SIDE_PIN  58//A4
#define IR_LEFT_FORWARD_PIN 63 //A9
#define IR_RIGHT_FORWARD_PIN 62//A8

#define GYRO_PIN 69 //A15
#define UPDATEGYROTIME 10 //this is for a 10ms read period
#define GYRO_READ_TIME  0.01 //this is for a 10ms read period
#define GYRO_CORRECTION 1.1 //correction factor in degrees per second

#define SONAR_TRIG_PIN 48 //D48
#define SONAR_ECHO_PIN 49 //D49
#define SONAR_POSITION_CORRECTION 78.25 //This is a correction factor in mm that is added to take distance from center of robot.

#define PHOTO_LEFT_SIDE 64 //A10
#define PHOTO_LEFT_CENTER 65 //A11
#define PHOTO_RIGHT_CENTER 66 //A12
#define PHOTO_RIGHT_SIDE 67 //A13
#define PHOTO_AMBIENT_READING 60
#define PHOTO_DETECT_THRESHOLD 250
#define PHOTO_VIEW_ANGLE 50
#define PHOTO_CENTER_OFFSET 100
#define PHOTO_HALF_VIEW_ANGLE PHOTO_VIEW_ANGLE/2 //This might be incorrect

#include "Kalman\Kalman.h"
#include "Kalman\Kalman.cpp"
#include "Arduino.h"
#include <math.h>

class Sensors {
public:
// constructors
    Sensors();

// getters
	//Distance and photo posistion IDs are left most facing to right most facing. 0-4 for distance and 0-3 for position.
	//Distance
	uint16_t getDistance(byte POSITION_ID);
	float getZoneScore(const char *zone); //'left','front','center'
	//Photos
	uint16_t getPhoto(byte POSITION_ID);
	uint16_t getMaxPhoto();
	int getPhotoArcAngle();
	float getNormPhotoArc();
	bool getDetected(byte POSITION_ID);
	bool isDetected();
	//Gyro
	float getAngle();
	bool getGyroState();

// utilities
    // Gryo
	void enableGyro();
	void disableGyro();	
	void updateAngle();
	
	//Distance and light
	void updateArcAngle();
	void checkZones();

private: 
// utilities
	//Sensor reading
	uint16_t readIr(byte readPin, float Coefficents[],uint16_t Constraints[]); //
	uint16_t readSonar();
	uint16_t readPhoto(byte readPin);
	//
	void updateDistances();
	void updatePhotos();


//Sensors variables
	
	//Gryo
	bool gyroState;
	float Angle; // angle positive if rotated clockwise

	//Distance
	uint16_t Distances[5]; 
	float LeftScore;
	float RightScore;
	float FrontScore;
	//IR calibration coefficent and calibration range limits
	//calibration curves and constraints ordered based on position ID, position 2 is left blank because it is the sonar
	const byte irPins[5]={IR_LEFT_SIDE_PIN,IR_LEFT_FORWARD_PIN,0,IR_RIGHT_FORWARD_PIN,IR_RIGHT_SIDE_PIN};
	const float irCalibrations[5][3]={{-0.0002,0.1007,-7.5392},{0,0.0232,-0.7972},{0,0,0},{0,0.0127,0.0566},{-0.0002,0.1025,-7.9823}};
	const uint16_t irConstraints[5][3]={{40,400},{110,500},{0,0},{110,500},{40,400}};
	
	//Phototransistors
	const byte photoPins[4]={PHOTO_LEFT_SIDE,PHOTO_LEFT_CENTER,PHOTO_RIGHT_CENTER,PHOTO_RIGHT_SIDE};
	uint16_t Photos[4]; 
	bool FireDetected[4];
	int NumFiresDetected;
	int estArcAngle;
	float estArcNorm;
	uint8_t FindMaxPhotoIndex();
	
	//Filters
	Kalman *DistanceFilters[5];
	Kalman *PhotoFilters[4];
	
};

#endif
