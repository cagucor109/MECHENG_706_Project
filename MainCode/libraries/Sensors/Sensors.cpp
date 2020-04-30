#include "Sensors.h"

// constructors
Sensors::Sensors(){

}


// getters

	uint16_t Sensors::getFrontDistance(){
		return frontDistance;
	}
	uint16_t Sensors::getRearDistance(){
		return rearDistance;
	}
	uint16_t Sensors::getLeftDistance(){
		return leftDistance;
	}
	float Sensors::getAngle(){
		return Angle;
	}
	float Sensors::getParallel(){
		return Parallel;
	}
	bool Sensors::getGyroState(){
		return gyroState;
	}
	
	bool Sensors::getParallelError(){
	return parallelError;
	}

// utilities

//-----------------------GRYO--------------------------------  
void Sensors::enableGyro(){
	this->Angle=0;
	this->gyroState=true;
}

void Sensors::disableGyro(){
	this->gyroState=false;
}


void Sensors::updateAngle(){
	if(gyroState){//The gyro will only update if enabled.
	
		float gyroRate = (analogRead(GYRO_PIN) * 5.0) / 1023;
		gyroRate -= 2.48;//Center readings on 0V. calibrated to the provided gryo
		gyroRate/=0.007; //finds acceleration by dividing by sensitivity of gyro. With rotation in degrees

		if (abs(gyroRate) >= 1) {//this is a minimum read threshold in degrees/second
			gyroRate/=100; //Double integration of acceleration to angle *10ms*10ms (multiply by sample rate twice).
			gyroRate-=GYRO_CORRECTION*GYRO_READ_TIME; //this corrects for sensor drift that is inherrent in approximation of double integration.
			this->Angle+=gyroRate;

			if ( this->Angle < -360)// Shifting of angle when a revolution is complete.
			{
				this->Angle += 360;
			}
			else if ( this->Angle > 359)
			{
				this->Angle -= 360;
			}
		}		
	}
}

//-----------------------Sonar-------------------------------  

void Sensors::updateLeftDistance(){
	//Sending read signal
	digitalWrite(SONAR_TRIG_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(SONAR_TRIG_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(SONAR_TRIG_PIN, LOW);
  
	//Read pin
	float reading=pulseIn(SONAR_ECHO_PIN,HIGH);

	//Apply speed of sound correction to calculate distance in mm.
    reading=reading/(2.94*2); //343m/s  give 2.941176ms/mm. This is then halved to account for rebounding
	reading+=SONAR_POSITION_CORRECTION; //applying offset to center reading on the center of the robot.
	this->leftDistance=reading;
}


//-----------------------Infra red--------------------------- 

//This reads an IR sensors and converts to distance.
uint16_t Sensors::readIr(byte readPin, float Coefficents[],uint16_t Constraints[]){
	float reading=analogRead(readPin);
	reading=1000/((Coefficents[0]*reading*reading)+(Coefficents[1]*reading)+(Coefficents[2]));  //This equation is for a quadratic relationship between the analog value and the inverse of the distance. 
	reading=constrain(reading,Constraints[0],Constraints[1]); //This caps reading to the calibrated and verified range.
	return reading;
  }

void Sensors::updateFrontDistance(){
	//read sensor
	uint16_t frontIr=readIr(IR_FORWARD_PIN,irForward,longConstraints);
	frontIr+=IR_FORWARD_POSITION_CORRECTION;//applying offset to center reading on the center of the robot.
	this->frontDistance= frontIr;
}



void Sensors::updateRearDistance(){
	//read sensor
	uint16_t rearIr=readIr(IR_REAR_PIN,irRear,longConstraints);
	rearIr+=IR_REAR_POSITION_CORRECTION;//applying offset to center reading on the center of the robot.
	this->rearDistance=rearIr;	
}



void Sensors::updateParallel(){
	//read sensors (2xIR)
	uint16_t left_FrontIr=readIr(IR_LEFT_FRONT_PIN,irLeftFront,shortConstraints);
	uint16_t left_BackIr=readIr(IR_LEFT_BACK_PIN,irLeftBack,shortConstraints);

	//Max value check
	if(left_FrontIr==shortConstraints[1]&& left_BackIr== shortConstraints[1]){ //If IRSensors are out of range parallel cannot be validated.
		parallelError=true;
	}else{
		parallelError=false;
	}

	double difference=left_FrontIr-left_BackIr;  //Take Difference  
	double rads=atan2(difference,(IR_LEFT_FRONT_OFFSET+IR_LEFT_BACK_OFFSET));    //Calculate angle
	this->Parallel=rads*180/PI; //Conversion to degrees
}


