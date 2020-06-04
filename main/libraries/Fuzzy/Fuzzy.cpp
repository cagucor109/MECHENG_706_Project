#include "Fuzzy.h"    
    
// Constructors
Fuzzy::Fuzzy(){

}

Fuzzy::Fuzzy(int preDef){
if (preDef == 1){ //moveToFire fuzzy controller
	
} else if (preDef == 2){ // Avoidance fuzzy controller
	
} else{
}
}

// Destructors
Fuzzy::~Fuzzy(){

}

// Getters
float Fuzzy::getOuputValue(char* outputName){

}

// Public Methods
bool Fuzzy::addRuleSet(FuzzyRules* ruleSet){
    _fuzzyRules = ruleSet;

    return true;
}

bool Fuzzy::addFuzzyInput(FuzzyI* input){
    _fuzzyInputs.add(input);

    return true;
}

bool Fuzzy::addFuzzyOutput(FuzzyO* output){
    _fuzzyOutputs.add(output);

    return true;
}

bool Fuzzy::setCrispInput(char* inputName, float crispInput){
    for(int i = 0; i < _fuzzyInputs.size(); i++){
        if( !strcmp(inputName, ( *_fuzzyInputs.get(i)).getName() )){
            (*_fuzzyInputs.get(i)).setInputVal(crispInput);
        }
    }

    return true;
}

bool Fuzzy::updateFuzzy(){
    fuzzyify();
    applyRules();
    defuzzify();
}

// Private Methods
bool Fuzzy::fuzzyify(){
    for(int i = 0; i < _fuzzyInputs.size(); i++){
        (*_fuzzyInputs.get(i)).calculateAllPertinences();
    }

    return true;
}

bool Fuzzy::applyRules(){
    (*_fuzzyRules).evaluateRules();

    return true;
}

bool Fuzzy::defuzzify(){
    for(int i = 0; i < _fuzzyOutputs.size(); i++){);

    return true;
}

bool Fuzzy::addFuzzyOutput(FuzzyO* output){
    _fuzzyOutputs.push_back
        (*_fuzzyOutputs.get(i)).calculateOutput();
    }

    return true;
}