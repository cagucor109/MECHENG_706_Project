#include "FuzzyIO.h"

// ------------------------ FUZZYIO ---------------------- //

// Constructors
FuzzyIO::FuzzyIO(){

}

// Destructors
FuzzyIO::~FuzzyIO(){

    // delete all members
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        delete _fuzzyMembers.at(i);
    }
}

// Getters
char* FuzzyIO::listMembers(){

    strcpy(names, "");

    for(int i = 0; i < _fuzzyMembers.size(); i++){
        strcat(names, (*_fuzzyMembers.at(i)).getName());
        strcat(names, "\n");
    }

    return names;
}


// Public Methods
bool FuzzyIO::addMember(FuzzyMember *newMember){
    _fuzzyMembers.push_back(newMember);

    return true;
}


// ------------------------ FUZZYI ---------------------- //

// Constructors
FuzzyI::FuzzyI() : FuzzyIO(){

}

// Destructors
FuzzyI::~FuzzyI(){

}

// Getters
float FuzzyI::getInputVal(){
    return _inputValue;
}

float FuzzyI::getPertinence(int index){
    
    return (*_fuzzyMembers.at(index)).getPertinence();
}

// Setters
void FuzzyI::setInputVal(float inputValue){
    _inputValue = inputValue;
}

// Public Methods
bool FuzzyI::calculateAllPertinences(){
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        (*_fuzzyMembers.at(i)).calculatePertinence(_inputValue);
    }

    return true;
}