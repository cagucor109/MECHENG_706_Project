#include <iostream>

#include "FuzzyMember.h"

int main(){

    std::cout << "trapezoid shape" << std::endl;
    char name[8] = "fast";
    FuzzyMember* fuzzy = new FuzzyMember(name, 0.0, 0.3, 0.6, 0.8);

    std::cout<< "the name is: " << (*fuzzy).getName() << std::endl;

    // test vertex placement

    float a = (*fuzzy).getVertexA();
    std::cout << a << std::endl;

    float b = (*fuzzy).getVertexB();
    std::cout << b << std::endl;

    float c = (*fuzzy).getVertexC();
    std::cout << c << std::endl;

    float d = (*fuzzy).getVertexD();
    std::cout << d << std::endl;

    // test pertinence

    (*fuzzy).calculatePertinence(0.4);
    std::cout << (*fuzzy).getPertinence() << std::endl;

    (*fuzzy).calculatePertinence(0.15);
    std::cout << (*fuzzy).getPertinence() << std::endl;

    // test shapes
    std::cout << "Z shape" << std::endl;

    strcpy(name, "slow");
    FuzzyMember* fuzzy2 = new FuzzyMember(name, 0.2, 0.2, 0.2, 0.6);
    std::cout<< "the name is: " << (*fuzzy2).getName() << std::endl;

    // test pertinence

    (*fuzzy2).calculatePertinence(0.1);
    std::cout << (*fuzzy2).getPertinence() << std::endl;

    (*fuzzy2).calculatePertinence(0.7);
    std::cout << (*fuzzy2).getPertinence() << std::endl;

    return 0;
}