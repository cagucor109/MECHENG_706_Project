#include "Fuzzy.h"    
    
// Constructors
Fuzzy::Fuzzy(){

}

Fuzzy::Fuzzy(int preDef){

}

// Destructors
Fuzzy::~Fuzzy(){

}

// Public Methods
bool Fuzzy::addRuleSet(FuzzyRules* ruleSet){
    _fuzzyRules = ruleSet;

    return true;
}

bool Fuzzy::addFuzzyInput(FuzzyI* input){
    _fuzzyInputs.push_back(input);

    return true;
}

bool Fuzzy::addFuzzyOutput(FuzzyO* output){
    _fuzzyOutputs.push_back(output);

    return true;
}


bool Fuzzy::setCrispInput(char* inputName, float crispInput){
    for(int i = 0; i < _fuzzyInputs.size(); i++){
        if( !strcmp(inputName, ( *_fuzzyInputs.at(i)).getName() )){
            (*_fuzzyInputs.at(i)).setInputVal(crispInput);
        }
    }

    return true;
}

bool Fuzzy::fuzzyify(){
    for(int i = 0; i < _fuzzyInputs.size(); i++){
        (*_fuzzyInputs.at(i)).calculateAllPertinences();
    }

    return true;
}

bool Fuzzy::applyRules(){
    (*_fuzzyRules).evaluateRules();

    return true;
}

bool Fuzzy::defuzzify(){
    
}