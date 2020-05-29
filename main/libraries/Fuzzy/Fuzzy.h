#ifndef FUZZY_H
#define FUZZY_H

#include "FuzzyIO.h"
#include "FuzzyMember.h"
#include "FuzzyRules.h"

class Fuzzy{
public:
    // Constructors
    Fuzzy(); // default
    Fuzzy(int preDef); // specific to the rule set defined

    // Destructors
    ~Fuzzy();

    // Public Methods
    bool addRuleSet(FuzzyRules* ruleSet);  
    bool addFuzzyInput(FuzzyI* input);
    bool addFuzzyOutput(FuzzyO* output);

    bool setCrispInput(char* inputName, float crispInput); // set an input value to specific fuzzy input
    bool fuzzyify(); // perform fuzzification
    bool applyRules(); // evaluate all the rules
    bool defuzzify(); // defuzzify output (centroid)

private:
    FuzzyRules *_fuzzyRules; // Set of fuzzy rules
    std::vector<FuzzyI*> _fuzzyInputs;
    std::vector<FuzzyO*> _fuzzyOutputs;

    // Private Destructors
    void clearFuzzyRules();
};

#endif