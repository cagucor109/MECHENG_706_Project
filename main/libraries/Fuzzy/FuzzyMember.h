#ifndef FUZZYMEMBER_H
#define FUZZYMEMBER_H

class FuzzyMember{
public:
    // Constructors
    FuzzyMember();
    FuzzyMember(float a, float b, float c, float d); // set the points for the set

    // Destructors
    ~FuzzyMember();

    // Getters
    float getVertexA();
    float getVertexB();
    float getVertexC();
    float getVertexD();
    float getPertinence();

    // Public Methods
    bool calculatePertinence(float inputValue);
    void setBestPertinence(float pertinence);
    void resetPertinence();

private:
    float _a, _b, _c, _d; // Vertices of membership function
    float _pertinence;
    char name[8];
};

#endif