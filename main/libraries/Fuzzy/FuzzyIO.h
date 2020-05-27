#ifndef FUZZYIO_H
#define FUZZYIO_H

#include "FuzzyMember.h"
#include <vector>
#include <stdlib.h>

class FuzzyIO{
    public:
    // Constructors
    FuzzyIO();

    // Destructors
    ~FuzzyIO();

    // Getters
    char* listMembers();

    // Public Methods
    bool addMember(FuzzyMember *newMember);
    

protected:
    std::vector<FuzzyMember*> _fuzzyMembers;
    char names[32];
};

class FuzzyI : public FuzzyIO{
public:
    // Constructors
    FuzzyI();

    // Destructors
    ~FuzzyI();

    // Getters
    float getInputVal();
    float getPertinence(int index);

    // Setters
    void setInputVal(float inputValue);

    // Public Methods
    bool calculateAllPertinences();

private:
    float _inputValue; // value to calculate 

};

class FuzzyO : public FuzzyIO{
public:
    // Constructors
    FuzzyO();

    // Destructors
    ~FuzzyO();

    // Public Methods

};

#endif