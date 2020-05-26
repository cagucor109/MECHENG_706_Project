#ifndef FUZZYIO_H
#define FUZZYIO_H

#include "FuzzyMember.h"
#include <vector>

class FuzzyIO{
    public:
    // Constructors
    FuzzyIO();

    // Destructors
    ~FuzzyIO();

    // Public Methods
    bool addMember(FuzzyMember *newMember);

protected:
    std::vector<FuzzyMember*> fuzzyMembers;
}

class FuzzyI : public FuzzyIO{
public:
    // Constructors
    FuzzyI();

    // Destructors
    ~FuzzyI();

    // Public Methods

}

class FuzzyO{
public:
    // Constructors
    FuzzyO();

    // Destructors
    ~FuzzyO();

    // Public Methods

}

#endif