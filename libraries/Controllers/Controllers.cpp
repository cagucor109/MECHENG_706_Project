#include "Controllers.h"

// constructors
Controllers::Controllers(){
    desiredDistFront_ = 150;
    desiredDistRight_ = 150;
    desiredAngle_ = 0;
}

// setters
void Controllers::setDesiredDistFront(double distance){
    desiredDistFront_ = distance;
}

void Controllers::setDesiredDistRight(double distance){
    desiredDistRight_ = distance;
}

void Controllers::setDesiredAngle(double angle){
    desiredAmgle_ = angle;
}

// getters
double Controllers::getDesiredDistFront(){
    return desiredDistFront_;
}

double Controllers::getDesiredDistRight(){
    return desiredDistRight_;
}

double Controllers::getDesiredAngle(){
    return desiredAngle_;
}

// utilities
// error positive if rotated clockwise
double Controllers::calculateError(char *field, double reading){
    if(field == "front"){
        return desiredDistFront_ - reading;
    }else if (field == "right"){
        return desiredDistRight_ - reading;
    }else if(field == "angle"){
        return desiredAngle_ - reading;
    }
}

double Controllers::controlP(char *field, double error){
    double P_GAIN;

    if(field == "front"){
        P_GAIN = FRONT_P_GAIN;
    }else if(field == "right"){
        P_GAIN = EDGE_P_GAIN;
    }else if(field == "angle"){
        P_GAIN = ANGLE_P_GAIN;
    }else{
        P_GAIN = 0;
    }

    errorPrev_ = error;
    errorTotal_ += error;
    return P_GAIN * error;

}

double Controllers::controlPI(char *field, double error){
    double P_GAIN;
    double I_GAIN;

    if(field == "front"){
        P_GAIN = FRONT_P_GAIN;
        I_GAIN = FRONT_I_GAIN;
    }else if(field == "right"){
        P_GAIN = EDGE_P_GAIN;
        I_GAIN = EDGE_I_GAIN;
    }else if(field == "angle"){
        P_GAIN = ANGLE_P_GAIN;
        I_GAIN = ANGLE_I_GAIN;
    }else{
        P_GAIN = 0;
        I_GAIN = 0;
    }

    errorPrev_ = error;
    errorTotal_ += error;
    return P_GAIN * error + I_GAIN * errorTotal_;

}

double Controllers::controlPID(char *field, double error){
    double P_GAIN;
    double I_GAIN;
    double D_GAIN;

    if(field == "front"){
        P_GAIN = FRONT_P_GAIN;
        I_GAIN = FRONT_I_GAIN;
        D_GAIN = FRONT_D_GAIN;
    }else if(field == "right"){
        P_GAIN = EDGE_P_GAIN;
        I_GAIN = EDGE_I_GAIN;
        D_GAIN = EDGE_D_GAIN;
    }else if(field == "angle"){
        P_GAIN = ANGLE_P_GAIN;
        I_GAIN = ANGLE_I_GAIN;
        D_GAIN = ANGLE_D_GAIN;
    }else{
        P_GAIN = 0;
        I_GAIN = 0;
        D_GAIN = 0;
    }

    double errorDif = error - errorPrev_;
    errorPrev_ = error;
    errorTotal_ += error;
    return P_GAIN * error + I_GAIN * errorTotal_ + D_GAIN * errorDif;

}
