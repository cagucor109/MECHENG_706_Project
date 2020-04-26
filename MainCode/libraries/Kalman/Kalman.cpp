#include "Kalman.h"  
 
// constructor
Kalman::Kalman() {
  processNoise_ = 8;
  sensorNoise_ = 1;
}

// setters
void Kalman::setProcessNoise(double inputProcessNoise) {
  processNoise_ = inputProcessNoise;
}

void Kalman::setSensorNoise(double inputSensorNoise) {
  sensorNoise_ = inputSensorNoise;
}

// utilites
double Kalman::filter(double rawData, double displacement){   // Kalman Filter
  double priorEst, postEst, priorCovar, postCovar, kalmanGain;

  //Prediction Step
  priorEst = this->prevEst_ + displacement;  
  priorCovar = this->processNoise_; 

  // Correction Step
  kalmanGain = priorCovar/(priorCovar + this->sensorNoise_);
  postEst = priorEst + kalmanGain*(rawData - priorEst);
  postCovar = (1 - kalmanGain)*priorCovar;

  // for next time step
  this->prevEst_ = postEst;

  return postEst;
}

