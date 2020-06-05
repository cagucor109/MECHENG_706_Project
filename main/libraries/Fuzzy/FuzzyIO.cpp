#include "FuzzyIO.h"

// ------------------------ FUZZYIO ---------------------- //

// Constructors
FuzzyIO::FuzzyIO(){

}

FuzzyIO::FuzzyIO(char* name){
    strcpy(_name, name);
}

// Destructors
FuzzyIO::~FuzzyIO(){

    // delete all members
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        delete _fuzzyMembers.get(i);
    }
}

// Getters
char* FuzzyIO::listMembers(){

    strcpy(_names, "");

    for(int i = 0; i < _fuzzyMembers.size(); i++){
        strcat(_names, (*_fuzzyMembers.get(i)).getName());
        strcat(_names, "\n");
    }

    return _names;
}

char* FuzzyIO::getName(){
    return _name;
}

int FuzzyIO::findNamedMember(char* name){
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        if( !strcmp((*_fuzzyMembers.get(i)).getName(), name) ){
            return i;
        }
    }

    return -1;
}

FuzzyMember* FuzzyIO::getFuzzyMember(int index){
    return _fuzzyMembers.get(index);
}


// Public Methods
bool FuzzyIO::addMember(FuzzyMember *newMember){
    _fuzzyMembers.add(newMember);

    return true;
}


// ------------------------ FUZZYI ---------------------- //

// Constructors
FuzzyI::FuzzyI() : FuzzyIO(){

}

FuzzyI::FuzzyI(char* name) : FuzzyIO(name){

}

// Destructors
FuzzyI::~FuzzyI(){

}

// Getters
float FuzzyI::getInputVal(){
    return _inputValue;
}

float FuzzyI::getPertinence(int index){
    
    return (*_fuzzyMembers.get(index)).getPertinence();
}

// Setters
void FuzzyI::setInputVal(float inputValue){
    _inputValue = inputValue;
}

// Public Methods
bool FuzzyI::calculateAllPertinences(){
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        (*_fuzzyMembers.get(i)).calculatePertinence(_inputValue);
    }

    return true;
}

// ---------------------- FuzzyO --------------------- //

// Constructors
FuzzyO::FuzzyO(){

}

FuzzyO::FuzzyO(char* name) : FuzzyIO(name){

}

// Destructors
FuzzyO::~FuzzyO(){

}

// Getters
float FuzzyO::getOutputValue(){
    return _outputValue;
}

// Public Methods
bool FuzzyO::calculateOutput(){
    
    generatePoints();

    processPoints();

    float baseLength;
    float tempArea;
    float averageHeight;
    float centroidValue;

    float sumArea = 0.0;
    float sumWeightedArea = 0.0;

    // TODO: Calculate area under combined shape
    for(int i = 0; i < _xPointsFinal.size() - 1; i++){
        // if next point is a separator, skip 2 points
        if(_xPointsFinal.get(i+1) == SEPARATOR){
            i+=2;
        }
        // TODO: calculate area under the line connecting the current and next point_xPointsFinal.add(0);
        baseLength = _xPointsFinal.get(i+1) - _xPointsFinal.get(i);
        averageHeight = (_yPointsFinal.get(i+1) + _yPointsFinal.get(i)) / 2;
        tempArea = baseLength * averageHeight;

        // centroid of trapezium is h/3 * (b + 2a)/(b + a) where b is the side from which the centroid is measured
        centroidValue = _xPointsFinal.get(i) + (baseLength / 3) * (_yPointsFinal.get(i) + 2 * _yPointsFinal.get(i+1)) / (_yPointsFinal.get(i) + _yPointsFinal.get(i+1));

        // TODO: add current area to total area
        sumArea += tempArea;
        sumWeightedArea += tempArea * centroidValue;
    }
    
    _outputValue = sumWeightedArea / sumArea;

    return true;
}

// Private Methods
void FuzzyO::generatePoints(){

    // NOTE: consequtive points indicate there is a line between them, when this is not true,
    // the points will be separated by a separator defined in the header

    // clear the current points vectors
    _yPointsRaw.clear();
    _xPointsRaw.clear();

    _numLines = 0;

    float tempX;
    float tempY;

    float a, b, c, d, minX, maxX, truth;

    // map all points into cartesian coordinates starting from left side
    for(int i = 0; i < _fuzzyMembers.size(); i++){

        a = (*_fuzzyMembers.get(i)).getVertexA();
        b = (*_fuzzyMembers.get(i)).getVertexA();
        c = (*_fuzzyMembers.get(i)).getVertexA();
        d = (*_fuzzyMembers.get(i)).getVertexA();

        minX = (*_fuzzyMembers.get(i)).getMinX();
        maxX = (*_fuzzyMembers.get(i)).getMaxX();

        truth = (*_fuzzyMembers.get(i)).getTruthLevel();

        // if the function's left is all 1
        if( a == b && b == c ){
            _xPointsRaw.add( minX );
            _yPointsRaw.add( truth );

            _numLines++;

            linesIntersect(tempX, tempY, c, 1, d, 0, minX, truth, maxX, truth);

            _xPointsRaw.add(tempX);
            _yPointsRaw.add(tempY);

            _numLines++;

            _xPointsRaw.add(d);
            _yPointsRaw.add(0);

            _numLines++;
        }

        // if the function's right is all 1
        else if(b == c && c ==d){
            _xPointsRaw.add(a);
            _yPointsRaw.add(0);

            _numLines++;

            linesIntersect(tempX, tempY, a, 0, b, 1, minX, truth, maxX, truth);

            _xPointsRaw.add(tempX);
            _yPointsRaw.add(tempY);

            _numLines++;

            _xPointsRaw.add(maxX);
            _yPointsRaw.add(truth);

            _numLines++;
        }

        // any other shape will result in trapeze
        else{
            _xPointsRaw.add(a);
            _yPointsRaw.add(0);

            _numLines++;

            linesIntersect(tempX, tempY, a, 0, b, 1, minX, truth, maxX, truth);

            _xPointsRaw.add(tempX);
            _yPointsRaw.add(tempY);

            _numLines++;

            linesIntersect(tempX, tempY, c, 1, d, 0, minX, truth, maxX, truth);

            _xPointsRaw.add(tempX);
            _yPointsRaw.add(tempY);

            _numLines++;

            _xPointsRaw.add(d);
            _yPointsRaw.add(0);

            _numLines++;
        }

        _xPointsRaw.add( SEPARATOR );
        _yPointsRaw.add( SEPARATOR );
    }
}

void FuzzyO::processPoints(){
    int linesProcessed = 0;

    // WARNING: COMMENTED OUT FOR PLACEHOLDER
    // start with the first line on the processed list and build from there
    //_xPointsFinal.get(0) = _xPointsRaw.get(0); 
    //_yPointsFinal.get(0) = _yPointsRaw.get(0); 

    // temp varaibles to merge lines
    float xtemp, ytemp;

    // temp variables to reassign points when merging lines
    float firstPointX, secondPointX, firstPointY, secondPointY;

    // variables to keep track of line gradients
    float grad1, grad2; 


    // WARNING: LOGIC NEEDS TO BE REDONE
    // NOTE: as a place holder, output a trapeze with vertices at 0.0, 0.4, 0.6, 1.0
    _xPointsFinal.add(0.0);
    _xPointsFinal.add(0.4);
    _xPointsFinal.add(0.6);
    _xPointsFinal.add(1.0);

    _yPointsFinal.add(0.0);
    _yPointsFinal.add(0.5);
    _yPointsFinal.add(0.5);
    _yPointsFinal.add(0.0);

    

}


bool FuzzyO::linesIntersect(float &xout, float &yout, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){

    // first generate the equation of each line in the format:
    // ax + by = c
    float a1 = y1 - y2;
    float b1 = x2 - x1;
    float c1 = y1*x2 - x1*y2;

    float a2 = y3 - y4;
    float b2 = x4 - x3;
    float c2 = y3*x4 - x3*y4;

    float determinant = a1*b2 - b1*a2;

    // parallel line check
    if(determinant == 0){
        xout = 0;
        yout = -1;
        return false;
    }else{
        float x = (b2*c1 - b1*c2)/determinant;
        float y = (a1*c2 - a2*c1)/determinant;

        // line segment range check
        if(x >= min(x1, x2) && x <= max(x1,x2) && y >= min(y1, y2) && y <= max(y1,y2) ){
            xout = x;
            yout = y;
            return true;
        }else{
            xout = 0;
            yout = -1;
            return false;
        }
    }
}


    