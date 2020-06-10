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

#ifndef FUZZYRULES_H
#define FUZZYRULES_H

#include "FuzzyIO.h"
#include "Arduino.h"

class Antecedent;
class Consequent;

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
    LinkedList<Antecedent*> _fuzzyAntecedents;
    LinkedList<Consequent*> _fuzzyConsequents;
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
    LinkedList<FuzzyMember*> _antecedentMembers; // fuzzy member
    LinkedList<FuzzyI*> _antecedentInputs; // fuzzy input
};

class Consequent{
public:
    // Constructors
    Consequent();

    // Destructors
    ~Consequent();

    // Public Methods
    bool addConsequent(FuzzyO* output, FuzzyMember* member); // add consequent to list
    bool evaluateConsequent(float antecedentTruth);
    
private:
    LinkedList<FuzzyMember*> _consequentMembers; // fuzzy member
    LinkedList<FuzzyO*> _consequentOutputs; // fuzzy output
};

#endif