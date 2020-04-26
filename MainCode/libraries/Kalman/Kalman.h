#ifndef KALMAN_H
#define KALMAN_H

#include "Arduino.h"

class Kalman {
  public:

    //constructors?
    Kalman();

    // setters
    void setSensorNoise(double inputSensorNoise);
    void setProcessNoise(double inputProcessNoise);
   
    
    // utilities
    double filter(double rawdata, double distance);

  private:
    double processNoise_;
    double sensorNoise_;
    double prevEst_;
};

#endif

