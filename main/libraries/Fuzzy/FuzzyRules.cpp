/*
 *  Implementation of Fuzzy rules
 *  
 *  This class provides a framework to establish a relationship between antecedents and consequents.
 * 
 *  This class works by creating a matching set of antecendents and consequents, specified by
 *  the Antecendent and Consequent classes. This implements a rule set as well as an inference engine.
 * 
 *  Author: Carlos Aguilera
 *  Contact: cagu554@aucklanduni.ac.nz
 * 
 */

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
        index = (*_antecedentInputs.get(i)).findNamedMember( (*_antecedentMembers.get(i)).getName() );
        tempTruth = (*(*_antecedentInputs.get(i)).getFuzzyMember(index)).getPertinence();

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
bool Consequent::addConsequent(FuzzyO* output, FuzzyMember* member){

    _consequentMembers.add(member); 
    _consequentOutputs.add(output);
}

bool Consequent::evaluateConsequent(float antecedentTruth){

    for(int i = 0; i < _consequentMembers.size(); i++){
        int index = (*_consequentOutputs.get(i)).findNamedMember( (*_consequentMembers.get(i)).getName() );

        //assign a truth level to the corresponding output member based on the truth of the consequent
        (*(*_consequentOutputs.get(i)).getFuzzyMember(index)).setTruthLevel(antecedentTruth);
    }
}