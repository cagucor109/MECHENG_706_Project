#ifndef FUZZYRULES_H
#define FUZZYRULES_H

#include "FuzzyIO.h"
#include "FuzzyMember.h"


class FuzzyRules{
public:
    // Constructors
    FuzzyRules(); // default constructor
    FuzzyRules(Antecedent *fuzzyAntecedent, Consequent *fuzzyConsequent); // antecendent and consequent constructor

    // Destructors
    ~FuzzyRules();

    // Getters
    bool isRuleMet(); // return if the rule applies

    // Public Methods
    bool evaluateRule(); // check if the rule applies

private:
    Antecedent *_fuzzyAntecedent;
    Consequent *_fuzzyConsequent;
    bool _ruleMet; // true if the antecendents match the input

};

class Antecedent{
public:
    // Constructors
    Antecedent();

    // Destructors
    ~Antecedent();

    // Public Methods
    bool makeAntecedent(); // make antecendent based on array of conditions and operations
    bool evaluateAntecedent(); // check if the antecedent returns true

private:
    std::vector<FuzzyMember*> antecedentMember; // fuzzy member
    std::vector<FuzzyI*> antecedentInput; // fuzzy input
    std::vector<int> relations; // is or is not
    std::vector<int> operators; // and/ or 
};

class Consequent{
public:
    // Constructors
    Consequent();

    // Destructors
    ~Consequent();

    // Public Methods
    bool makeConsequent(); // make consequent
    
private:
    FuzzyMember *consequentMember; // fuzzy member
    FuzzyO *consequentOutput; 
};

#endif