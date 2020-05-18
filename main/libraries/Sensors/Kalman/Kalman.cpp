#include "Kalman.h"  
 
// constructor
Kalman::Kalman() {
  processNoise = 100; //Preset to ignore motion model and act as a smoothing filter for sensors
  sensorNoise = 2;    //Will be tuned for the sensors with testing. and can use setters to change for each sensor.
  prevEst = 0;
}

// setters
void Kalman::setProcessNoise(double inputProcessNoise) {
  processNoise = inputProcessNoise;
}
	
void Kalman::setSensorNoise(double inputSensorNoise) {
  sensorNoise = inputSensorNoise;
}

void Kalman::setPrevEst(double previousEst){
	this->prevEst = previousEst;
}


// utilites
double Kalman::filter(double rawData, double displacement){   // Kalman Filter
  double priorEst, postEst, priorCovar, postCovar, kalmanGain;

  //Prediction Step
  priorEst = this->prevEst + displacement;  
  priorCovar = this->processNoise; 

  // Correction Step
  kalmanGain = priorCovar/(priorCovar + this->sensorNoise);
  postEst = priorEst + kalmanGain*(rawData - priorEst);
  postCovar = (1 - kalmanGain)*priorCovar;

  // for next time step
  this->prevEst = postEst;
 return this->prevEst;
}

