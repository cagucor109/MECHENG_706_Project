#ifndef KALMAN_H
#define KALMAN_H

#include "Arduino.h"

class Kalman {
  public:

    // constructor
    Kalman();

    // setters
    void setSensorNoise(double inputSensorNoise);
    void setProcessNoise(double inputProcessNoise);
    void setPrevEst(double previousEst);
    // getter
	double getFilteredValue();
	
    // utilities
    void filter(double rawdata, double distance);

  private:
    double processNoise;
    double sensorNoise;
    double prevEst;
};

#endif

