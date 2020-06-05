#include "FuzzyMember.h"

// Constructors

FuzzyMember::FuzzyMember(){

}

FuzzyMember::FuzzyMember(char name[8], int min, int max, float a, float b, float c, float d){
    strcpy(_name, name);
    _min_X = min;
    _max_X = max;
    _a = a;
    _b = b;
    _c = c;
    _d = d;
    _pertinence = 0.0;
    _truthLevel = 0.0;
}

// Destructors
FuzzyMember::~FuzzyMember(){

}

// Getters

int FuzzyMember::getMaxX(){
    return _max_X;
}

int FuzzyMember::getMinX(){
    return _min_X;
}

float FuzzyMember::getVertexA(){
    return _a;
}

float FuzzyMember::getVertexB(){
    return _b;
}

float FuzzyMember::getVertexC(){
    return _c;
}

float FuzzyMember::getVertexD(){
    return _d;
}

float FuzzyMember::getPertinence(){
    return _pertinence;
}

float FuzzyMember::getTruthLevel(){
    return _truthLevel;
}

char* FuzzyMember::getName(){
    return _name;
}

// Setters
bool FuzzyMember::setTruthLevel(float truthLevel){

    // only assign value if it is higher than current assigned value
    if(truthLevel > _truthLevel){
        _truthLevel = truthLevel;
        return true;
    }

    return false;
}


// Public Methods

// the points represent the location on the x axis. 
// if A, B and C or B, C and D are at the same location, that means that point is at one,
// otherwise the point is at 0.
// This allows trapezoids and triangles
bool FuzzyMember::calculatePertinence(float inputValue){

    // check if input is smaller than vertex A
    if(inputValue < _a){
        // check if this represents truth
        if(_a == _b && _b == _c){
            _pertinence = 1.0;
        }else{
            _pertinence = 0.0;
        }
    } 

    // check if input is between vertex A and B
    // note: slope always positive
    else if(inputValue >= _a && inputValue <= _b){
        float slope = 1.0/(_b - _a);
        _pertinence = slope * (inputValue - _a);
    }

    // check if input is between vertex B and C
    else if(inputValue >= _b && inputValue <= _c){
        _pertinence = 1.0;
    }

    // check if input is between vertex C and D
    // note: slope always negative
    else if(inputValue >= _c && inputValue <= _d){
        float slope = 1.0/(_d - _c);
        _pertinence = slope * (inputValue - _c) + 1.0;
    }

    // check if input is bigger than vertex D
    else if(inputValue > _d){
        // check if this represents truth
        if(_d == _c && _c == _b){
            _pertinence = 1.0;
        }else{
            _pertinence = 0.0;
        }
    } 

    return true;
}

void FuzzyMember::resetPertinence(){
    _pertinence = 0.0;
}