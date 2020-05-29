#ifndef FUZZYRULES_H
#define FUZZYRULES_H

#include "FuzzyIO.h"
#include "FuzzyMember.h"


class FuzzyRules{
public:
    // Constructors
    FuzzyRules(); // default constructor
    
    // Destructors
    ~FuzzyRules();

    // Public Methods
    bool addRule(Antecedent* antecedent, Consequent* consequent);
    bool evaluateRules(); // assign outputs based on inputs

private:
    std::vector<Antecedent*> _fuzzyAntecedents;
    std::vector<Consequent*> _fuzzyConsequents;
};

class Antecedent{
public:
    // Constructors
    Antecedent();

    // Destructors
    ~Antecedent();

    // Public Methods
    bool addAntecedent(FuzzyI* input, FuzzyMember* member); // add an antecedent to the list
    float evaluateAntecedent(); // check the truth level of the antecendent, a 2.0 represents no antecendents set

private:
    std::vector<FuzzyMember*> _antecedentMembers; // fuzzy member
    std::vector<FuzzyI*> _antecedentInputs; // fuzzy input
};

class Consequent{
public:
    // Constructors
    Consequent();

    // Destructors
    ~Consequent();

    // Public Methods
    bool makeConsequent(FuzzyO* output, FuzzyMember* member); // make consequent
    bool evaluateConsequent(float antecedentTruth);
    
private:
    FuzzyMember *_consequentMember; // fuzzy member
    FuzzyO *_consequentOutput; 
};

#endif