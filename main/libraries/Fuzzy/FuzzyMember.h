#ifndef FUZZYMEMBER_H
#define FUZZYMEMBER_H

#include "Arduino.h"

class FuzzyMember{
public:
    // Constructors
    FuzzyMember();
    FuzzyMember(char* name, float a, float b, float c, float d); // set the points for the set

    // Destructors
    ~FuzzyMember();

    // Getters
    float getVertexA();
    float getVertexB();
    float getVertexC();
    float getVertexD();
    float getPertinence();
    float getTruthLevel();
    char* getName();

    // Setters
    bool setTruthLevel(float truthLevel);

    // Public Methods
    bool calculatePertinence(float inputValue);
    void resetPertinence();

private:
    float _a, _b, _c, _d; // Vertices of membership function
    float _pertinence; // for input fuzzy members
    float _truthLevel; // for output fuzzy members
    char _name[8];
};

#endif