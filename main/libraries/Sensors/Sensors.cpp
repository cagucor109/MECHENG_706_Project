#include "Sensors.h"

// constructors
Sensors::Sensors(){
	//Setting pinModes 
pinMode(IR_LEFT_SIDE_PIN, INPUT);
pinMode(IR_RIGHT_SIDE_PIN, INPUT);
pinMode(IR_LEFT_FORWARD_PIN, INPUT);
pinMode(IR_RIGHT_FORWARD_PIN, INPUT);

pinMode(GYRO_PIN, INPUT);

pinMode(SONAR_ECHO_PIN, INPUT);
pinMode(SONAR_TRIG_PIN, OUTPUT);

pinMode(PHOTO_LEFT_SIDE,INPUT);
pinMode(PHOTO_LEFT_CENTER,INPUT);
pinMode(PHOTO_RIGHT_CENTER,INPUT);
pinMode(PHOTO_RIGHT_SIDE,INPUT);

//Filters
for (int i = 0; i<5; i++){
	DistanceFilters[i] = new Kalman();
}

for (int i = 0; i<4; i++){
	PhotoFilters[i] =new Kalman();
}

}


// getters

	uint16_t Sensors::getDistance(byte POSITION_ID){
		if(POSITION_ID>=5){return 65535;}
		else{return this->Distances[POSITION_ID];}
	}
	
	uint16_t Sensors::getPhoto(byte POSITION_ID){
		if(POSITION_ID>=4){return 65535;}
		else{return this->Photos[POSITION_ID];}
	}
	
	bool Sensors::getDetected(byte POSITION_ID){
	if(POSITION_ID>=4){return false;}
	else{return this->FireDetected[POSITION_ID];}
	}

	float Sensors::getAngle(){
		return Angle;
	}

	bool Sensors::getGyroState(){
		return gyroState;
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

//-----------------------Sonar reading-------------------------------  

uint16_t Sensors::readSonar(){
	//Sending read signal
	digitalWrite(SONAR_TRIG_PIN, LOW);
	delayMicroseconds(2);
	digitalWrite(SONAR_TRIG_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(SONAR_TRIG_PIN, LOW);
  
	//Read pin
	uint16_t reading=pulseIn(SONAR_ECHO_PIN,HIGH);

	//Apply speed of sound correction to calculate distance in mm.
    reading=reading/(2.94*2); //343m/s  give 2.941176ms/mm. This is then halved to account for rebounding
	
	return reading;
}


//-----------------------Infra red reading--------------------------- 

//This reads an IR sensors and converts to distance.
uint16_t Sensors::readIr(byte readPin, float Coefficents[],uint16_t Constraints[]){
	float reading=analogRead(readPin);
	reading=1000/((Coefficents[0]*reading*reading)+(Coefficents[1]*reading)+(Coefficents[2]));  //This equation is for a quadratic relationship between the analog value and the inverse of the distance. 
	reading=constrain(reading,Constraints[0],Constraints[1]); //This caps reading to the calibrated and verified range.
	return reading;
  }


//------------------------Distances---------------------------------


void Sensors::updateDistances(){
	uint16_t Reading=0;
	
	for(int i=0;i<5;i++){
		//Read sensors
		if(i==2){Reading=readSonar();} //accounts for sonar in position 2
		else{Reading=readIr(irPins[i],irCalibrations[i],irConstraints[i]);}
		//Filter sensors and store reading
		this->Distances[i]= DistanceFilters[i]->filter(Reading,0);
	}

}


//----------------------Phototransistors----------------------


uint16_t Sensors::readPhoto(byte readPin){
	uint16_t Reading=analogRead(readPin);
	//Could do some manipulation or conversion if required.

return  Reading;
}



void Sensors::updatePhotos(){  
	uint16_t Reading=0;
	NumFiresDetected = 0;

	for(int i=0;i<4;i++){
	//read sensors
	Reading=readPhoto(photoPins[i]);
	//Comparing to deotection threshold
	if(Reading>PHOTO_DETECT_THRESHOLD){
		FireDetected[i]=true;
		NumFiresDetected++;
	} else{FireDetected[i]=false;}
	//filter readings are store
	  this->Photos[i]= PhotoFilters[i]->filter(Reading,0);
	}
}

void Sensors::updateArcAngle() {
	//angle 0s at centre, and is negative CCW, and positive CW;
	int i;
	updatePhotos();
	
	if (NumFiresDetected == 2) {
		//if fire detected pattern is [1 X 1 X], [1 X X 1] or [X 1 X 1] , go towards max 
		//photo readings as more than one fire is likely detected
		if (FireDetected[0] == 1){
			if (FireDetected [2] == 1 || FireDetected[3] == 1) { 
				estArcAngle = (45*FindMaxPhotoIndex() + 22.5)-90;
		} else if (FireDetected[1] == 1 && FireDetected[3] == 1)
				estArcAngle = (45*FindMaxPhotoIndex() + 22.5)-90;
		} 
		//if more than 2 fires, more than 1 fire likely to exist, head towards max
	} else if (NumFiresDetected > 2) {
			estArcAngle = (45*FindMaxPhotoIndex() + 22.5)-90;
	} else {
		// normal calculation of weighted average to determine the estimated angle of fire away
		uint16_t total = 0;
		uint16_t position = 0;

		for (i=0; i<4; i++) {
			total = total+ Photos[i];
			position = position + (45*(i-1)+22.5)*Photos[i];
		}

		estArcAngle = position/total - 90; //centre 0 degree at centre
	}
		
}


// Finds the index of the max value in array Photos, which stores the 
// photo levels detected by each of the photo transistors
uint8_t Sensors::FindMaxPhotoIndex(){
	uint8_t location = 0;
	uint16_t val = 0;
	int i;
	
	 for (i=0; i<4; i++) {
        if (Photos[i] > val) {
            val = Photos[i];
            location = i;
		}
	}	
	
	return location;
}
