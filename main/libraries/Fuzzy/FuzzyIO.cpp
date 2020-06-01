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
        delete _fuzzyMembers.at(i);
    }
}

// Getters
char* FuzzyIO::listMembers(){

    strcpy(_names, "");

    for(int i = 0; i < _fuzzyMembers.size(); i++){
        strcat(_names, (*_fuzzyMembers.at(i)).getName());
        strcat(_names, "\n");
    }

    return _names;
}

char* FuzzyIO::getName(){
    return _name;
}

int FuzzyIO::findNamedMember(char* name){
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        if( !strcmp((*_fuzzyMembers.at(i)).getName(), name) ){
            return i;
        }
    }

    return -1;
}

FuzzyMember* FuzzyIO::getFuzzyMember(int index){
    return _fuzzyMembers.at(index);
}


// Public Methods
bool FuzzyIO::addMember(FuzzyMember *newMember){
    _fuzzyMembers.push_back(newMember);

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
    
    return (*_fuzzyMembers.at(index)).getPertinence();
}

// Setters
void FuzzyI::setInputVal(float inputValue){
    _inputValue = inputValue;
}

// Public Methods
bool FuzzyI::calculateAllPertinences(){
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        (*_fuzzyMembers.at(i)).calculatePertinence(_inputValue);
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

// Public Methods
bool FuzzyO::calculateOutput(){
    
    generatePoints();

    processPoints();

    // TODO: Calculate area under combined shape
    for(int i = 0; i < _xPointsFinal.size() - 1; i++){
        // TODO: calculate area under the line connecting the current and next point

        // TODO: add current area to total area
    }
    

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

        a = (*_fuzzyMembers.at(i)).getVertexA();
        b = (*_fuzzyMembers.at(i)).getVertexA();
        c = (*_fuzzyMembers.at(i)).getVertexA();
        d = (*_fuzzyMembers.at(i)).getVertexA();

        minX = (*_fuzzyMembers.at(i)).getMinX();
        maxX = (*_fuzzyMembers.at(i)).getMaxX();

        truth = (*_fuzzyMembers.at(i)).getTruthLevel();

        // if the function's left is all 1
        if( a == b && b == c ){
            _xPointsRaw.push_back( minX );
            _yPointsRaw.push_back( truth );

            _numLines++;

            linesIntersect(tempX, tempY, c, 1, d, 0, minX, truth, maxX, truth);

            _xPointsRaw.push_back(tempX);
            _yPointsRaw.push_back(tempY);

            _numLines++;

            _xPointsRaw.push_back(d);
            _yPointsRaw.push_back(0);

            _numLines++;
        }

        // if the function's right is all 1
        else if(b == c && c ==d){
            _xPointsRaw.push_back(a);
            _yPointsRaw.push_back(0);

            _numLines++;

            linesIntersect(tempX, tempY, a, 0, b, 1, minX, truth, maxX, truth);

            _xPointsRaw.push_back(tempX);
            _yPointsRaw.push_back(tempY);

            _numLines++;

            _xPointsRaw.push_back(maxX);
            _yPointsRaw.push_back(truth);

            _numLines++;
        }

        // any other shape will result in trapeze
        else{
            _xPointsRaw.push_back(a);
            _yPointsRaw.push_back(0);

            _numLines++;

            linesIntersect(tempX, tempY, a, 0, b, 1, minX, truth, maxX, truth);

            _xPointsRaw.push_back(tempX);
            _yPointsRaw.push_back(tempY);

            _numLines++;

            linesIntersect(tempX, tempY, c, 1, d, 0, minX, truth, maxX, truth);

            _xPointsRaw.push_back(tempX);
            _yPointsRaw.push_back(tempY);

            _numLines++;

            _xPointsRaw.push_back(d);
            _yPointsRaw.push_back(0);

            _numLines++;
        }

        _xPointsRaw.push_back( SEPARATOR );
        _yPointsRaw.push_back( SEPARATOR );
    }
}

void FuzzyO::processPoints(){
    int linesProcessed = 0;

    // start with the first line on the processed list and build from there
    _xPointsFinal.at(0) = _xPointsRaw.at(0); 
    _yPointsFinal.at(0) = _yPointsRaw.at(0); 

    // temp varaibles to merge lines
    float xtemp, ytemp;

    // temp variables to reassign points when merging lines
    float firstPointX, secondPointX, firstPointY, secondPointY;

    // variables to keep track of line gradients
    float grad1, grad2; 

    while(linesProcessed < _numLines){ 

        for(int i = 2; i < _xPointsRaw.size(); i++){

            // skip separators at current and next index
            if(_xPointsRaw.at(i) == -2 || _xPointsRaw.at(i+1) == -2){
                continue;
            }else{
                for(int j = 0; j < _xPointsFinal.size(); j++){
                    // check if line intersects with any existing line
                    if(linesIntersect(xtemp, ytemp, _xPointsRaw.at(i),_yPointsRaw.at(i), _xPointsRaw.at(i+1), _yPointsRaw.at(i+1), _xPointsFinal.at(j), _yPointsFinal.at(j), _xPointsFinal.at(j+1), _yPointsFinal.at(j+1) ) ){
                        
                        // calculate gradients
                        grad1 = (_yPointsRaw.at(i+1) - _yPointsRaw.at(i)) / (_xPointsRaw.at(i+1) - _xPointsRaw.at(i));
                        grad2 = (_yPointsFinal.at(j+1) - _yPointsFinal.at(j)) / (_xPointsFinal.at(j+1) - _xPointsFinal.at(j));
                        
                        if(grad1 > 0 && grad2 > 0){
                            if(grad1 > grad2){
                                firstPointX = _xPointsFinal.at(j);
                                firstPointY = _yPointsFinal.at(j);

                                secondPointX = _xPointsRaw.at(i+1);
                                secondPointY = _yPointsRaw.at(i+1);
                            }else{
                                firstPointX = _xPointsRaw.at(i);
                                firstPointY = _yPointsRaw.at(i);

                                secondPointX = _xPointsFinal.at(j+1);
                                secondPointY = _yPointsFinal.at(j+1);
                            }
                        }else if(grad1 < 0 && grad2 < 0){
                            if(grad1 > grad2){
                                firstPointX = _xPointsRaw.at(i);
                                firstPointY = _yPointsRaw.at(i);

                                secondPointX = _xPointsFinal.at(j+1);
                                secondPointY = _yPointsFinal.at(j+1);
                            }else{
                                firstPointX = _xPointsFinal.at(j);
                                firstPointY = _yPointsFinal.at(j);

                                secondPointX = _xPointsRaw.at(i+1);
                                secondPointY = _yPointsRaw.at(i+1);
                            }
                        }else{
                            firstPointX = (grad1 > grad2) ? _xPointsFinal.at(j) : _xPointsRaw.at(i);
                            firstPointY = (grad1 > grad2) ? _yPointsFinal.at(j) : _yPointsRaw.at(i);

                            secondPointX = (grad1 > grad2) ? _xPointsFinal.at(j+1) : _xPointsRaw.at(i+1);
                            secondPointY = (grad1 > grad2) ? _yPointsFinal.at(j+1) : _yPointsRaw.at(i+1);
                        }
                        // TODO: replace points j and j+1 with first and second points
                    }
                }

            }
        }

    }
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
        if(x >= std::min(x1, x2) && x <= std::max(x1,x2) && y >= std::min(y1, y2) && y <= std::max(y1,y2) ){
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
    