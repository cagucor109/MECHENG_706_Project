#include "FuzzyRules.h"

// -------------------------- Fuzzy Rules ------------------------- //

// Constructors
FuzzyRules::FuzzyRules(){

}
    
// Destructors
FuzzyRules::~FuzzyRules(){

}

// Public Methods
bool FuzzyRules::addRule(Antecedent* antecedent, Consequent* consequent){
    _fuzzyAntecedents.add(antecedent);
    _fuzzyConsequents.add(consequent);
}

bool FuzzyRules::evaluateRules(){
    float truthLevel;

    for(int i = 0; i < _fuzzyAntecedents.size(); i++){
        truthLevel = (*_fuzzyAntecedents.get(i)).evaluateAntecedent();
        (*_fuzzyConsequents.get(i)).evaluateConsequent(truthLevel);
    }

}

// ----------------------------- Antecedent --------------------------- //

// Constructors
Antecedent::Antecedent(){

}

// Destructors
Antecedent::~Antecedent(){

}

// Public Methods
bool Antecedent::addAntecedent(FuzzyI* input, FuzzyMember* member){
    _antecedentInputs.add(input);
    _antecedentMembers.add(member);
}

float Antecedent::evaluateAntecedent(){
    float truth = 2.0;
    float tempTruth;
    int index;

    for(int i = 0; i < _antecedentInputs.size(); i++){
        index = (*_antecedentInputs.at(i)).findNamedMember( (*_antecedentMembers.at(i)).getName() );
        tempTruth = (*(*_antecedentInputs.at(i)).getFuzzyMember(index)).getPertinence();

        // apply min function
        if(tempTruth < truth){
            truth = tempTruth;
        }
    }

    return truth;
}

// ------------------------------ Consequent -------------------------- //

// Constructors
Consequent::Consequent(){

}

// Destructors
Consequent::~Consequent(){

}

// Public Methods
bool Consequent::makeConsequent(FuzzyO* output, FuzzyMember* member){

    _consequentMembers.add(member); 
    _consequentOutput.add(output);
}

bool Consequent::evaluateConsequent(float antecedentTruth){

    for(int i = 0; i < _consequentMembers.size(); i++){
        int index = (*_consequentOutputs.get(i)).findNamedMember( (*_consequentMembers.get(i)).getName() );

        //assign a truth level to the corresponding output member based on the truth of the consequent
        (*(*_consequentOutputs.get(i)).getFuzzyMember(index)).setTruthLevel(antecedentTruth);
    }
}