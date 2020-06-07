#ifndef FUZZYIO_H
#define FUZZYIO_H

#include "FuzzyMember.h"
#include "FuzzyMember.cpp"
#include "Arduino.h"
#include "LinkedList.h"

#define SEPARATOR -2 // separates points not joined by lines

class FuzzyIO{
public:
    // Constructors
    FuzzyIO();
    FuzzyIO(char* name, int minX, int maxX);

    // Destructors
    ~FuzzyIO();

    // Getters
    int getMaxX();
    int getMinX();
    char* listMembers();
    char* getName();
    int findNamedMember(char* name); // return index of named fuzzy member, -1 if not found
    FuzzyMember* getFuzzyMember(int index); // return member at specified index, use with findNamedMember

    // Public Methods
    bool addMember(FuzzyMember *newMember);
    

protected:
    LinkedList<FuzzyMember*> _fuzzyMembers;
    int _max_X; // maximum value for x axis in member function
    int _min_X; // minimum value for x axis in member function
    int _numLines;
    char _name[8];
    char _names[32];
};

class FuzzyI : public FuzzyIO{
public:
    // Constructors
    FuzzyI();
    FuzzyI(char* name, int minX, int maxX);

    // Destructors
    ~FuzzyI();

    // Getters
    float getInputVal();
    float getPertinence(int index);

    // Setters
    void setInputVal(float inputValue);

    // Public Methods
    bool calculateAllPertinences();

private:
    float _inputValue; // value to calculate 

};

class FuzzyO : public FuzzyIO{
public:
    // Constructors
    FuzzyO();
    FuzzyO(char* name, int minX, int maxX);

    // Destructors
    ~FuzzyO();

    // Getters
    float getOutputValue();

    // Public Methods
    bool calculateOutput(); // centroid method

private:
    float _outputValue; // final output
    // all points
    LinkedList<float> _xPointsRaw;
    LinkedList<float> _yPointsRaw;
    // final points
    LinkedList<float> _xPointsFinal;
    LinkedList<float> _yPointsFinal;

    // Private Methods
    void generatePoints(); // create raw points based on member function and truth value
    void processPoints(); // populates the final points based on intersections
    bool combineIntersect(LinkedList<float> xRaw, LinkedList<float> yRaw, LinkedList<float> xIntercepts, LinkedList<float> yIntercepts); // checks if the first shape contains the second
    bool linesIntersect(float &xout, float &yout, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4); // find point of intersection of two lines (0,-1) means no intersection

};

#endif