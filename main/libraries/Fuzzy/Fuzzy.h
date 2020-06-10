/*
 *  Implementation of Fuzzy logic control
 *  
 *  This class provides an interface to fuzzy logic control.
 *  To add an input, call the setCrispInput() method.
 *  To get the output, call the getCrispOutput() method.
 *  To run the fuuzy control process, call the updateFuzzy() method.
 * 
 *  The constructor which takes an integer has a predefined set of inputs, outputs and rules.
 *  The creation of a custom set of inputs, outputs and rules can be achieved by invoking the
 *  addRuleSet(), addInput() and addOutput() methods. For more detail on how to create these,
 *  inspect the FuzzyIO and FuzzyRules files.
 * 
 *  Author: Carlos Aguilera
 *  Contact: cagu554@aucklanduni.ac.nz
 * 
 */



#ifndef FUZZY_H
#define FUZZY_H

#include "Arduino.h"
#include "FuzzyIO.h"
#include "FuzzyIO.cpp"
#include "FuzzyRules.h"
#include "FuzzyRules.cpp"

class Fuzzy{
public:
    // Constructors
    Fuzzy(); // default
    Fuzzy(int preDef); // specific to the rule set defined

    // Destructors
    ~Fuzzy();
    
    // Getters
    float getCrispOutput(char* outputName);

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
