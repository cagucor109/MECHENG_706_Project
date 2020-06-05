#include "Fuzzy.h"    
    
// Constructors
Fuzzy::Fuzzy(){
    FuzzyI *testI = new FuzzyI("test");
    FuzzyMember *testIM = new FuzzyMember();
    (*testI).addMember(testIM);

    addFuzzyInput(testI);

    FuzzyO *testO = new FuzzyO("test");
    FuzzyMember *testOM = new FuzzyMember();
    (*testO).addMember(testOM);

    addFuzzyOutput(testO);
}

Fuzzy::Fuzzy(int preDef){
    if (preDef == 1){ //moveToFire fuzzy controller

        // ------------------- Inputs ----------------------- //

        FuzzyI *arcPosition = new FuzzyI("arcPosition");
        FuzzyMember *left_ap = new FuzzyMember("left", -1, 1, -0.8, -0.8, -0.8, 0.0);
        (*arcPosition).addMember(left_ap);
        FuzzyMember *front_ap = new FuzzyMember("front", -1, 1, -1.0, -0.2, 0.2, 1.0);
        (*arcPosition).addMember(front_ap);
        FuzzyMember * right_ap = new FuzzyMember("right", -1, 1, 0.0, 0.8, 0.8, 0.8);
        (*arcPosition).addMember(right_ap);

        FuzzyI *intensity = new FuzzyI("intensity");
        FuzzyMember *far_i = new FuzzyMember("far", 0, 1, 0.0, 0.0, 0.0, 0.6);
        (*intensity).addMember(far_i);
        FuzzyMember *close_i = new FuzzyMember("close", 0, 1, 0.3, 1.0, 1.0, 1.0);
        (*intensity).addMember(close_i);
        FuzzyMember *medium_i = new FuzzyMember("medium", 0, 1, 0.1, 0.28, 0.37, 1.0);
        (*intensity).addMember(medium_i);

        // ---------------------- Outputs -------------------- //

        FuzzyO *Y = new FuzzyO("Y");
        FuzzyMember *slow_y = new FuzzyMember("slow", 0, 1, 0.2, 0.2, 0.2, 0.7);
        (*Y).addMember(slow_y);
        FuzzyMember *medium_y = new FuzzyMember("medium", 0, 1, 0.0, 0.5, 0.69, 0.7);
        (*Y).addMember(medium_y);
        FuzzyMember *fast_y = new FuzzyMember("fast", 0, 1, 0.3, 0.7, 0.7, 0.7);
        (*Y).addMember(fast_y);

        FuzzyO *Z = new FuzzyO("Z");
        FuzzyMember *ccw_z = new FuzzyMember("ccw", -1, 1, -0.8, -0.8, -0.8, 0.0);
        (*Z).addMember(ccw_z);
        FuzzyMember *cw_z = new FuzzyMember("cw", -1, 1, 0.0, 0.7, 0.7, 0.7);
        (*Z).addMember(cw_z);

        // ----------------------- Rules ---------------------- //

    } else if (preDef == 2){ // Avoidance fuzzy controller

        // ------------------- Inputs ----------------------- //

        FuzzyI *front = new FuzzyI("front");
        FuzzyMember *close_f = new FuzzyMember("close", 0, 1, 0.0, 0.1, 0.3, 0.4);
        (*front).addMember(close_f);
        FuzzyMember *far_f = new FuzzyMember("far", 0, 1, 0.2, 0.6, 0.6, 0.6);
        (*front).addMember(far_f);
        FuzzyMember *critical_f = new FuzzyMember("critical", 0, 1, 0.1, 0.1, 0.1, 0.2);
        (*front).addMember(critical_f);

        FuzzyI *left = new FuzzyI("left");
        FuzzyMember *critical_l = new FuzzyMember("critical", 0, 1, 0.1, 0.1, 0.1, 0.2);
        (*left).addMember(critical_l);
        FuzzyMember *close_l = new FuzzyMember("close", 0, 1, 0.0, 0.1, 0.2, 0.5);
        (*left).addMember(close_l);
        FuzzyMember *far_l = new FuzzyMember("far", 0, 1, 0.2, 0.3, 0.3, 0.3);
        (*left).addMember(far_l);

        FuzzyI *right = new FuzzyI("right");
        FuzzyMember *critical_r = new FuzzyMember("critical", 0, 1, 0.1, 0.1, 0.1, 0.2);
        (*right).addMember(critical_r);
        FuzzyMember *close_r = new FuzzyMember("close", 0, 1, 0.0, 0.1, 0.2, 0.5);
        (*right).addMember(close_r);
        FuzzyMember *far_r = new FuzzyMember("far", 0, 1, 0.2, 0.3, 0.3, 0.3);
        (*right).addMember(far_r);

        // ---------------------- Outputs -------------------- //

        FuzzyO *X = new FuzzyO("X");
        FuzzyMember *left_x = new FuzzyMember("left", -1, 1, -0.8, -0.8, -0.8, 0.0);
        (*X).addMember(left_x);
        FuzzyMember *centre_x = new FuzzyMember("centre", -1, 1, -0.8, 0.0, 0.0, 0.8);
        (*X).addMember(centre_x);
        FuzzyMember *right_x = new FuzzyMember("right", -1, 1, 0.0, 0.8, 0.8, 0.8);
        (*X).addMember(right_x);
        
        FuzzyO *Y = new FuzzyO("Y");
        FuzzyMember *back_y = new FuzzyMember("back", -1, 1, -0.8, -0.8, -0.8, 0.4);
        (*Y).addMember(back_y);
        FuzzyMember *slow_y = new FuzzyMember("slow", -1, 1, -0.4, 0.6, 0.6, 0.6);
        (*Y).addMember(slow_y);
        FuzzyMember *stop_y = new FuzzyMember("stop", -1, 1, -0.2, -0.1, 0.1, 0.2);
        (*Y).addMember(stop_y);

        // ----------------------- Rules ---------------------- //
        
    } else{

    }
}

// Destructors
Fuzzy::~Fuzzy(){

}

// Getters
float Fuzzy::getCrispOutput(char* outputName){
    for(int i = 0; i < _fuzzyOutputs.size(); i++){
        if( !strcmp(outputName, ( *_fuzzyOutputs.get(i)).getName() )){
            return (*_fuzzyOutputs.get(i)).getOutputValue();
        }
    }
    return 0.0;
}

// Public Methods
bool Fuzzy::addRuleSet(FuzzyRules* ruleSet){
    _fuzzyRules = ruleSet;

    return true;
}

bool Fuzzy::addFuzzyInput(FuzzyI* input){
    _fuzzyInputs.add(input);

    return true;
}

bool Fuzzy::addFuzzyOutput(FuzzyO* output){
    _fuzzyOutputs.add(output);

    return true;
}

bool Fuzzy::setCrispInput(char* inputName, float crispInput){
    for(int i = 0; i < _fuzzyInputs.size(); i++){
        if( !strcmp(inputName, ( *_fuzzyInputs.get(i)).getName() )){
            (*_fuzzyInputs.get(i)).setInputVal(crispInput);
            break;
        }
    }

    return true;
}

bool Fuzzy::updateFuzzy(){
    fuzzyify();
    applyRules();
    defuzzify();
}

// Private Methods
bool Fuzzy::fuzzyify(){
    for(int i = 0; i < _fuzzyInputs.size(); i++){
        (*_fuzzyInputs.get(i)).calculateAllPertinences();
    }

    return true;
}

bool Fuzzy::applyRules(){
    (*_fuzzyRules).evaluateRules();

    return true;
}

bool Fuzzy::defuzzify(){
    for(int i = 0; i < _fuzzyOutputs.size(); i++){
        (*_fuzzyOutputs.get(i)).calculateOutput();
    }

    return true;
}
