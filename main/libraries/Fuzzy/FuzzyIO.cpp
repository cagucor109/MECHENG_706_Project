#include "FuzzyIO.h"

// ------------------------ FUZZYIO ---------------------- //

// Constructors
FuzzyIO::FuzzyIO(){

}

FuzzyIO::FuzzyIO(char* name){
    strcpy(_name, name);
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

    strcpy(_names, "");

    for(int i = 0; i < _fuzzyMembers.size(); i++){
        strcat(_names, (*_fuzzyMembers.at(i)).getName());
        strcat(_names, "\n");
    }

    return _names;
}

char* FuzzyIO::getName(){
    return _name;
}

int FuzzyIO::findNamedMember(char* name){
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        if( !strcmp((*_fuzzyMembers.at(i)).getName(), name) ){
            return i;
        }
    }

    return -1;
}

FuzzyMember* FuzzyIO::getFuzzyMember(int index){
    return _fuzzyMembers.at(index);
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

FuzzyI::FuzzyI(char* name) : FuzzyIO(name){

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

// ---------------------- FuzzyO --------------------- //

// Constructors
FuzzyO::FuzzyO(){

}

FuzzyO::FuzzyO(char* name) : FuzzyIO(name){

}

// Destructors
FuzzyO::~FuzzyO(){

}

// Public Methods
bool FuzzyO::calculateOutput(){
    
}
