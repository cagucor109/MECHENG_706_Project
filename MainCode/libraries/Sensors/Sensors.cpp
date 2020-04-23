#include "Sensors.h"

// constructors
Sensors::Sensors(){
//Need to set some variables that the user should have access to.

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

// utilities
// error positive if rotated clockwise



//This reads an IR sensors and converts to distance.
uint16_t Sensors::readIr(byte readPin, float Coefficents[],uint16_t Constraints[]){
  float reading=analogRead(readPin);
  reading=constrain(reading,Constraints[0],Constraints[1]);
  //Serial.println(reading);
  return 1000/((Coefficents[0]*reading*reading)+(Coefficents[1]*reading)+(Coefficents[2]));  
  }
  
  
void Sensors::enableGyro(){
	this->Angle=0;
	this->gyroState=true;
}

void Sensors::disableGyro(){
	this->gyroState=false;
}

void Sensors::updateAngle(){
	if(gyroState){//Now the gyro will only update if enabled and the checking for enabled is handled internally within the sensors class.
	
	float gyroRate = (analogRead(GYRO_PIN) * 5.0) / 1023;
   gyroRate -= 2.48;//Center readings on 0V.
  gyroRate/=0.007; //finds acceleration by dividing by sensitivity of gyro



   if (gyroRate >= 1 || gyroRate <= -1) {//this is a minimum read threshold in degrees/second
  gyroRate/=100; //Double integration of acceleration to angle *10ms*10ms (multiply by sample rate twice).
  gyroRate-=GYRO_CORRECTION*GYRO_READ_TIME; //this corrects for sensor drift that is inherrent in approximation of double integration.
      this->Angle+=gyroRate;

    if ( this->Angle < -360)// This is probably not required for the use case.
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
    reading=reading/(2.94*2); //340m/s  give 2.941176ms/mm. This is then halved to account for rebounding

  reading+=SONAR_POSITION_CORRECTION;
  
  this->leftDistance=reading;
}

void Sensors::updateFrontDistance(){
	  //read sensor
  uint16_t frontIr=readIr(IR_FORWARD_PIN,irForward,longConstraints);// the array passing is subject to change
//Need to check for out of range (Currently handled by the readIR)
  frontIr+=IR_FORWARD_POSITION_CORRECTION;
  this->frontDistance= frontIr;
}

void Sensors::updateRearDistance(){
		//read sensor
  uint16_t rearIr=readIr(IR_REAR_PIN,irRear,longConstraints);// the array passing is subject to change  
//Need to check for out of range (Currently handled by the readIR)  
  rearIr+=IR_REAR_POSITION_CORRECTION;
  this->rearDistance=rearIr;	
}

void Sensors::updateParallel(){
	  //read sensors (2xIR)
 // uint16_t frontIr=readIr(IR_LEFT_FRONT_PIN,irLeftFront,shortConstraints);// the array passing is subject to change
  uint16_t backIr=readIr(IR_LEFT_BACK_PIN,irLeftBack,shortConstraints);
//Should do some constraining

 // Serial.print(",");
 // Serial.println(frontIr);
 
  Serial.println(backIr);
  //Take Difference  
//  double difference=frontIr-backIr;

  //Add some offset to the readings
  //Calculate angle
   //double rads=atan2(difference,(IR_LEFT_FRONT_OFFSET+IR_LEFT_BACK_OFFSET));
    
  //Store in sensors Struct
  //this->Parallel=rads*180/PI;
}


