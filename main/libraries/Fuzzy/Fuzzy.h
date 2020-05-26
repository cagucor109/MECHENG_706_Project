#ifndef FUZZY_H
#define FUZZY_H

#include "FuzzyIO.h"
#include "FuzzyMember.h"
#include "FuzzyRules.h"

class Fuzzy{
public:
    // Constructors
    Fuzzy();

    // Destructors
    ~Fuzzy();

    // Public Methods
    bool addInput(); // adds a type of input 
    bool addOutput(); // adds a type of output 
    bool addRule(); // adds a rule 

    bool setInput(); // set a value for each type of input
    bool fuzzyify(); // perform fuzzification
    bool checkRules(); // see which rules are triggered
    bool defuzzify(float &x, float &y, float &w); // defuzzify output (weighted average)

private:
    FuzzyI *fuzzyInputs;    // Set of fuzzy inputs
    FuzzyO *fuzzyOutputs;   // Set of fuzzy outputs
    FuzzyRules *fuzzyRules; // Set of fuzzy rules

    // Private Destructors
    void clearFuzzyInputs();
    void clearFuzzyOutputs();s
    void clearFuzzyRules();
}

#endif