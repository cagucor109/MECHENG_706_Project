#include "Fuzzy.h"    
    
// Constructors
Fuzzy::Fuzzy(){

}

Fuzzy::Fuzzy(int preDef){
    if (preDef == 1){ //moveToFire fuzzy controller

        // ------------------- Inputs ----------------------- //

        FuzzyI *arcPosition = new FuzzyI("arcPosition", -1, 1);
        FuzzyMember *left_ap = new FuzzyMember("left", -0.8, -0.8, -0.8, 0.0);
        (*arcPosition).addMember(left_ap);
        FuzzyMember *front_ap = new FuzzyMember("front", -1.0, -0.2, 0.2, 1.0);
        (*arcPosition).addMember(front_ap);
        FuzzyMember * right_ap = new FuzzyMember("right", 0.0, 0.8, 0.8, 0.8);
        (*arcPosition).addMember(right_ap);
        addFuzzyInput(arcPosition);

        FuzzyI *intensity = new FuzzyI("intensity", 0, 1);
        FuzzyMember *far_i = new FuzzyMember("far", 0.0, 0.0, 0.0, 0.6);
        (*intensity).addMember(far_i);
        FuzzyMember *close_i = new FuzzyMember("close", 0.3, 1.0, 1.0, 1.0);
        (*intensity).addMember(close_i);
        FuzzyMember *medium_i = new FuzzyMember("medium", 0.1, 0.28, 0.37, 1.0);
        (*intensity).addMember(medium_i);
        addFuzzyInput(intensity);

        // ---------------------- Outputs -------------------- //

        FuzzyO *Y = new FuzzyO("Y", 0, 1);
        FuzzyMember *slow_y = new FuzzyMember("slow", 0.2, 0.2, 0.2, 0.7);
        (*Y).addMember(slow_y);
        FuzzyMember *medium_y = new FuzzyMember("medium", 0.0, 0.5, 0.69, 0.7);
        (*Y).addMember(medium_y);
        FuzzyMember *fast_y = new FuzzyMember("fast", 0.3, 0.7, 0.7, 0.7);
        (*Y).addMember(fast_y);
        addFuzzyOutput(Y);

        FuzzyO *Z = new FuzzyO("Z", -1, 1);
        FuzzyMember *ccw_z = new FuzzyMember("ccw", -0.8, -0.8, -0.8, 0.0);
        (*Z).addMember(ccw_z);
        FuzzyMember *cw_z = new FuzzyMember("cw", 0.0, 0.7, 0.7, 0.7);
        (*Z).addMember(cw_z);
        addFuzzyOutput(Z);

        // ----------------------- Rules ---------------------- //

    } else if (preDef == 2){ // Avoidance fuzzy controller

        // ------------------- Inputs ----------------------- //

        FuzzyI *front = new FuzzyI("front", 0, 1);
        FuzzyMember *close_f = new FuzzyMember("close", 0.0, 0.1, 0.3, 0.4);
        (*front).addMember(close_f);
        FuzzyMember *far_f = new FuzzyMember("far", 0.2, 0.6, 0.6, 0.6);
        (*front).addMember(far_f);
        FuzzyMember *critical_f = new FuzzyMember("critical", 0.1, 0.1, 0.1, 0.2);
        (*front).addMember(critical_f);
        addFuzzyInput(front);

        FuzzyI *left = new FuzzyI("left", 0, 1);
        FuzzyMember *critical_l = new FuzzyMember("critical", 0.1, 0.1, 0.1, 0.2);
        (*left).addMember(critical_l);
        FuzzyMember *close_l = new FuzzyMember("close", 0.0, 0.1, 0.2, 0.5);
        (*left).addMember(close_l);
        FuzzyMember *far_l = new FuzzyMember("far", 0.2, 0.3, 0.3, 0.3);
        (*left).addMember(far_l);
        addFuzzyInput(left);

        FuzzyI *right = new FuzzyI("right", 0, 1);
        FuzzyMember *critical_r = new FuzzyMember("critical", 0.1, 0.1, 0.1, 0.2);
        (*right).addMember(critical_r);
        FuzzyMember *close_r = new FuzzyMember("close", 0.0, 0.1, 0.2, 0.5);
        (*right).addMember(close_r);
        FuzzyMember *far_r = new FuzzyMember("far", 0.2, 0.3, 0.3, 0.3);
        (*right).addMember(far_r);
        addFuzzyInput(right);

        // ---------------------- Outputs -------------------- //

        FuzzyO *X = new FuzzyO("X", -1, 1);
        FuzzyMember *left_x = new FuzzyMember("left", -0.8, -0.8, -0.8, 0.0);
        (*X).addMember(left_x);
        FuzzyMember *centre_x = new FuzzyMember("centre", -0.8, 0.0, 0.0, 0.8);
        (*X).addMember(centre_x);
        FuzzyMember *right_x = new FuzzyMember("right", 0.0, 0.8, 0.8, 0.8);
        (*X).addMember(right_x);
        addFuzzyOutput(X);
        
        FuzzyO *Y = new FuzzyO("Y", -1, 1);
        FuzzyMember *back_y = new FuzzyMember("back", -0.8, -0.8, -0.8, 0.4);
        (*Y).addMember(back_y);
        FuzzyMember *slow_y = new FuzzyMember("slow", -0.4, 0.6, 0.6, 0.6);
        (*Y).addMember(slow_y);
        FuzzyMember *stop_y = new FuzzyMember("stop", -0.2, -0.1, 0.1, 0.2);
        (*Y).addMember(stop_y);
        addFuzzyOutput(Y);

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
