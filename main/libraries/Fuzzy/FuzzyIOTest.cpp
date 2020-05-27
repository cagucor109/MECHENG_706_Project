#include <iostream>

#include "FuzzyIO.h"

int main(){

    FuzzyI* fuzzy = new FuzzyI();
    char name[8] = "a";
    FuzzyMember* fuzzyM1 = new FuzzyMember(name, 0.1, 0.2, 0.3, 0.4);
    strcpy(name,"b");
    FuzzyMember* fuzzyM2 = new FuzzyMember(name, 0.2, 0.3, 0.4, 0.5);
    strcpy(name, "c");
    FuzzyMember* fuzzyM3 = new FuzzyMember(name, 0.3, 0.4, 0.5, 0.6);

    // Test names

    (*fuzzy).addMember(fuzzyM1);
    (*fuzzy).addMember(fuzzyM2);
    (*fuzzy).addMember(fuzzyM3);

    std::cout << (*fuzzy).listMembers() << std::endl;

    (*fuzzy).setInputVal(0.3);
    (*fuzzy).calculateAllPertinences();
    
    std::cout << (*fuzzy).getInputVal() << std::endl;

    std::cout << (*fuzzy).getPertinence(0) << std::endl;
    std::cout << (*fuzzy).getPertinence(1) << std::endl;
    std::cout << (*fuzzy).getPertinence(2) << std::endl;

    return 0;
}