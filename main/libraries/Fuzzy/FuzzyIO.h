#ifndef FUZZYIO_H
#define FUZZYIO_H

#include "FuzzyMember.h"
#include <vector>
#include <stdlib.h>

class FuzzyIO{
    public:
    // Constructors
    FuzzyIO();
    FuzzyIO(char* name);

    // Destructors
    ~FuzzyIO();

    // Getters
    char* listMembers();
    char* getName();
    int findNamedMember(char* name); // return index of named fuzzy member, -1 if not found
    FuzzyMember* getFuzzyMember(int index); // return member at specified index, use with findNamedMember

    // Public Methods
    bool addMember(FuzzyMember *newMember);
    

protected:
    std::vector<FuzzyMember*> _fuzzyMembers;
    char _name[8];
    char _names[32];
};

class FuzzyI : public FuzzyIO{
public:
    // Constructors
    FuzzyI();
    FuzzyI(char* name);

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
    FuzzyO(char* name);

    // Destructors
    ~FuzzyO();

    // Public Methods
    bool calculateOutput(); // centroid method

private:
    float _outputValue; // final output
};

#endif