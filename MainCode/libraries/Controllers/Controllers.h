#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#define EDGE_P_GAIN 5
#define EDGE_I_GAIN 1
#define EDGE_D_GAIN 0.5

#define FRONT_P_GAIN 5
#define FRONT_I_GAIN 1
#define FRONT_D_GAIN 0.5

#define ANGLE_P_GAIN 5
#define ANGLE_I_GAIN 1
#define ANGLE_D_GAIN 0.5

#include "Arduino.h"

class Controllers {
public:
    // constructors
    Controllers();

    // setters
    void setDesiredDistFront(double distance);
    void setDesiredDistRight(double distance);
    void setDesiredAngle(double angle);

    // getters
    double getDesiredDistFront();
    double getDesiredDistRight();
    double getDesiredAngle();

    // utilities
    double calculateError(const char *field, double reading);
    double controlP(const char *field, double error);
    double controlPI(const char *field, double error);
    double controlPID(const char *field, double error);

private:
    double desiredDistFront_;
    double desiredDistRight_;
    double desiredAngle_;
    double errorTotal_;
    double errorPrev_;
};

#endif
