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
    _fuzzyAntecedents.push_back(antecedent);
    _fuzzyConsequents.push_back(consequent);
}

bool FuzzyRules::evaluateRules(){
    float truthLevel;

    for(int i = 0; i < _fuzzyAntecedents.size(); i++){
        truthLevel = (*_fuzzyAntecedents.at(i)).evaluateAntecedent();
        (*_fuzzyConsequents.at(i)).evaluateConsequent(truthLevel);
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
    _antecedentInputs.push_back(input);
    _antecedentMembers.push_back(member);
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

    _consequentMember = member;
    _consequentOutput = output;
}

bool Consequent::evaluateConsequent(float antecedentTruth){

    int index = (*_consequentOutput).findNamedMember( (*_consequentMember).getName() );

    //assign a truth level to the corresponding output member based on the truth of the consequent
    (*(*_consequentOutput).getFuzzyMember(index)).setTruthLevel(antecedentTruth);
}