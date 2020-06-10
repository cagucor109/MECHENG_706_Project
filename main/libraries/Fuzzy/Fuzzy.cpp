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

        FuzzyRules *R1 = new FuzzyRules();
        Antecedent *A1 = new Antecedent();
        Consequent *C1 = new Consequent();
        (*A1).addAntecedent(arcPosition, front_ap);
        (*A1).addAntecedent(intensity, far_i);
        (*C1).addConsequent(Y, fast_y);
        (*R1).addRule(A1, C1);
        addRuleSet(R1);

        FuzzyRules *R2 = new FuzzyRules();
        Antecedent *A2 = new Antecedent();
        Consequent *C2 = new Consequent();
        (*A2).addAntecedent(arcPosition, left_ap);
        (*A2).addAntecedent(intensity, far_i);
        (*C2).addConsequent(Y, medium_y);
        (*C2).addConsequent(Z, ccw_z);
        (*R2).addRule(A2, C2);
        addRuleSet(R2);

        FuzzyRules *R3 = new FuzzyRules();
        Antecedent *A3 = new Antecedent();
        Consequent *C3 = new Consequent();
        (*A3).addAntecedent(arcPosition, right_ap);
        (*A3).addAntecedent(intensity, far_i);
        (*C3).addConsequent(Y, medium_y);
        (*C3).addConsequent(Z, cw_z);
        (*R3).addRule(A3, C3);
        addRuleSet(R3);

        FuzzyRules *R4 = new FuzzyRules();
        Antecedent *A4 = new Antecedent();
        Consequent *C4 = new Consequent();
        (*A4).addAntecedent(arcPosition, front_ap);
        (*A4).addAntecedent(intensity, close_i);
        (*C4).addConsequent(Y, medium_y);
        (*R4).addRule(A4, C4);
        addRuleSet(R4);

        FuzzyRules *R5 = new FuzzyRules();
        Antecedent *A5 = new Antecedent();
        Consequent *C5 = new Consequent();
        (*A5).addAntecedent(arcPosition, left_ap);
        (*A5).addAntecedent(intensity, close_i);
        (*C5).addConsequent(Y, slow_y);
        (*C5).addConsequent(Z, ccw_z);
        (*R5).addRule(A5, C5);
        addRuleSet(R5);

        FuzzyRules *R6 = new FuzzyRules();
        Antecedent *A6 = new Antecedent();
        Consequent *C6 = new Consequent();
        (*A6).addAntecedent(arcPosition, right_ap);
        (*A6).addAntecedent(intensity, close_i);
        (*C6).addConsequent(Y, slow_y);
        (*C6).addConsequent(Z, cw_z);
        (*R6).addRule(A6, C6);
        addRuleSet(R6);

        FuzzyRules *R7 = new FuzzyRules();
        Antecedent *A7 = new Antecedent();
        Consequent *C7 = new Consequent();
        (*A7).addAntecedent(arcPosition, front_ap);
        (*A7).addAntecedent(intensity, medium_i);
        (*C7).addConsequent(Y, medium_y);
        (*R7).addRule(A7, C7);
        addRuleSet(R7);

        FuzzyRules *R8 = new FuzzyRules();
        Antecedent *A8 = new Antecedent();
        Consequent *C8 = new Consequent();
        (*A8).addAntecedent(arcPosition, left_ap);
        (*A8).addAntecedent(intensity, medium_i);
        (*C8).addConsequent(Y, slow_y);
        (*C8).addConsequent(Y, ccw_z);
        (*R8).addRule(A8, C8);
        addRuleSet(R8);

        FuzzyRules *R9 = new FuzzyRules();
        Antecedent *A9 = new Antecedent();
        Consequent *C9 = new Consequent();
        (*A9).addAntecedent(arcPosition, right_ap);
        (*A9).addAntecedent(intensity, medium_i);
        (*C9).addConsequent(Y, slow_y);
        (*C9).addConsequent(Y, cw_z);
        (*R9).addRule(A9, C9);
        addRuleSet(R9);

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

        FuzzyRules *R1 = new FuzzyRules();
        Antecedent *A1 = new Antecedent();
        Consequent *C1 = new Consequent();
        (*A1).addAntecedent(front, critical_f);
        (*C1).addConsequent(X, right_x);
        (*C1).addConsequent(Y, stop_y);
        (*R1).addRule(A1, C1);
        addRuleSet(R1);

        FuzzyRules *R2 = new FuzzyRules();
        Antecedent *A2 = new Antecedent();
        Consequent *C2 = new Consequent();
        (*A2).addAntecedent(left, critical_l);
        (*C2).addConsequent(X, left_x);
        (*C2).addConsequent(Y, stop_y);
        (*R2).addRule(A2, C2);
        addRuleSet(R2);

        FuzzyRules *R3 = new FuzzyRules();
        Antecedent *A3 = new Antecedent();
        Consequent *C3 = new Consequent();
        (*A3).addAntecedent(right, critical_r);
        (*C3).addConsequent(X, left_x);
        (*C3).addConsequent(Y, stop_y);
        (*R3).addRule(A3, C3);
        addRuleSet(R3);

        FuzzyRules *R4 = new FuzzyRules();
        Antecedent *A4 = new Antecedent();
        Consequent *C4 = new Consequent();
        (*A4).addAntecedent(front, critical_f);
        (*A4).addAntecedent(left, close_l);
        (*C4).addConsequent(X, right_x);
        (*C4).addConsequent(Y, stop_y);
        (*R4).addRule(A4, C4);
        addRuleSet(R4);

        FuzzyRules *R5 = new FuzzyRules();
        Antecedent *A5 = new Antecedent();
        Consequent *C5 = new Consequent();
        (*A5).addAntecedent(front, critical_f);
        (*A5).addAntecedent(right, close_r);
        (*C5).addConsequent(X, left_x);
        (*C5).addConsequent(Y, stop_y);
        (*R5).addRule(A5, C5);
        addRuleSet(R5);

        FuzzyRules *R6 = new FuzzyRules();
        Antecedent *A6 = new Antecedent();
        Consequent *C6 = new Consequent();
        (*A6).addAntecedent(left, close_l);
        (*A6).addAntecedent(right, close_r);
        (*C6).addConsequent(X, centre_x);
        (*C6).addConsequent(Y, slow_y);
        (*R6).addRule(A6, C6);
        addRuleSet(R6);

        FuzzyRules *R7 = new FuzzyRules();
        Antecedent *A7 = new Antecedent();
        Consequent *C7 = new Consequent();
        (*A7).addAntecedent(left, critical_l);
        (*A7).addAntecedent(right, close_r);
        (*C7).addConsequent(X, centre_x);
        (*C7).addConsequent(Y, slow_y);
        (*R7).addRule(A7, C7);
        addRuleSet(R7);

        FuzzyRules *R8 = new FuzzyRules();
        Antecedent *A8 = new Antecedent();
        Consequent *C8 = new Consequent();
        (*A8).addAntecedent(front, far_f);
        (*C8).addConsequent(X, centre_x);
        (*C8).addConsequent(Y, slow_y);
        (*R8).addRule(A8, C8);
        addRuleSet(R8);

        FuzzyRules *R9 = new FuzzyRules();
        Antecedent *A9 = new Antecedent();
        Consequent *C9 = new Consequent();
        (*A9).addAntecedent(front, critical_f);
        (*C9).addConsequent(X, right_x);
        (*C9).addConsequent(Y, stop_y);
        (*R9).addRule(A9, C9);
        addRuleSet(R9);

        FuzzyRules *R10 = new FuzzyRules();
        Antecedent *A10 = new Antecedent();
        Consequent *C10 = new Consequent();
        (*A10).addAntecedent(front, critical_f);
        (*A10).addAntecedent(left, critical_l);
        (*A10).addAntecedent(right, critical_r);
        (*C10).addConsequent(X, centre_x);
        (*C10).addConsequent(Y, stop_y);
        (*R10).addRule(A10, C10);
        addRuleSet(R10);

        FuzzyRules *R11 = new FuzzyRules();
        Antecedent *A11 = new Antecedent();
        Consequent *C11 = new Consequent();
        (*A11).addAntecedent(left, close_l);
        (*C11).addConsequent(X, right_x);
        (*C11).addConsequent(Y, stop_y);
        (*R11).addRule(A11, C11);
        addRuleSet(R11);
        
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
