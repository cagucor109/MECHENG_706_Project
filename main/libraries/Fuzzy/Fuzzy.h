#ifndef FUZZY_H
#define FUZZY_H

#include "FuzzyIO.h"
#include "Arduino.h"
#include "FuzzyMember.h"
#include "FuzzyRules.h"

class Fuzzy{
public:
    // Constructors
    Fuzzy(); // default
    Fuzzy(int preDef); // specific to the rule set defined

    // Destructors
    ~Fuzzy();
    
    // Getters
    float getOutputValue(char* outputName);

    // Public Methods
    bool addRuleSet(FuzzyRules* ruleSet);  
    bool addFuzzyInput(FuzzyI* input);
    bool addFuzzyOutput(FuzzyO* output);

    bool setCrispInput(char* inputName, float crispInput); // set an input value to specific fuzzy input
    bool updateFuzzy();
    

private:
    FuzzyRules *_fuzzyRules; // Set of fuzzy rules
    LinkedList<FuzzyI*> _fuzzyInputs;
    LinkedList<FuzzyO*> _fuzzyOutputs;
    
    // Private methods
    bool fuzzyify(); // perform fuzzification
    bool applyRules(); // evaluate all the rules
    bool defuzzify(); // defuzzify output (centroid)

    // Private Destructors
    void clearFuzzyRules();
};

#endif
