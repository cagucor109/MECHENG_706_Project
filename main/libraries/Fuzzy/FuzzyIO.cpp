#include "FuzzyIO.h"

// ------------------------ FUZZYIO ---------------------- //

// Constructors
FuzzyIO::FuzzyIO(){

}

FuzzyIO::FuzzyIO(char* name, int minX, int maxX){
    strcpy(_name, name);
    _max_X = maxX;
    _min_X = minX;
}

// Destructors
FuzzyIO::~FuzzyIO(){

    // delete all members
    for(int i = 0; i < _fuzzyMembers.size(); i++){
        delete _fuzzyMembers.get(i);
    }
}

// Getters
int FuzzyIO::getMaxX(){
    return _max_X;
}

int FuzzyIO::getMinX(){
    return _min_X;
}

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

FuzzyI::FuzzyI(char* name, int minX, int maxX) : FuzzyIO(name, minX, maxX){

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

FuzzyO::FuzzyO(char* name, int minX, int maxX) : FuzzyIO(name, minX, maxX){

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

    float a, b, c, d, truth;

    // map all points into cartesian coordinates starting from left side
    for(int i = 0; i < _fuzzyMembers.size(); i++){

        a = (*_fuzzyMembers.get(i)).getVertexA();
        b = (*_fuzzyMembers.get(i)).getVertexA();
        c = (*_fuzzyMembers.get(i)).getVertexA();
        d = (*_fuzzyMembers.get(i)).getVertexA();

        truth = (*_fuzzyMembers.get(i)).getTruthLevel();

        // if the function's left is all 1
        if( a == b && b == c ){
            _xPointsRaw.add( _min_X );
            _yPointsRaw.add( truth );

            _numLines++;

            linesIntersect(tempX, tempY, c, 1, d, 0, _min_X, truth, _max_X, truth);

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

            linesIntersect(tempX, tempY, a, 0, b, 1, _min_X, truth, _max_X, truth);

            _xPointsRaw.add(tempX);
            _yPointsRaw.add(tempY);

            _numLines++;

            _xPointsRaw.add(_max_X);
            _yPointsRaw.add(truth);

            _numLines++;
        }

        // any other shape will result in trapeze
        else{
            _xPointsRaw.add(a);
            _yPointsRaw.add(0);

            _numLines++;

            linesIntersect(tempX, tempY, a, 0, b, 1, _min_X, truth, _max_X, truth);

            _xPointsRaw.add(tempX);
            _yPointsRaw.add(tempY);

            _numLines++;

            linesIntersect(tempX, tempY, c, 1, d, 0, _min_X, truth, _max_X, truth);

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

    // variables to record start indecies of second and third membership functions
    int index2, index3;

    _xPointsFinal.clear();
    _yPointsFinal.clear();

    // first, add all non intersecting membership functions
    // start with the first membership function
    int current = 0;
    float largestPoint;
    
    // NOTE: replace initial list with -3 where a point has been processed
    while(_xPointsFinal.get(current) != SEPARATOR){
        _xPointsFinal.add(_xPointsRaw.get(current));
        _xPointsRaw.set(current, -3);
        _yPointsFinal.add(_yPointsRaw.get(current));
        _yPointsRaw.set(current, -3);
        current++;
    }

    index2 = current + 1;

    largestPoint = _xPointsRaw.get(current - 1);

    // check for non intersecting membership functions and add them

    // NOTE: MAX NUMBER OF MEMBERSHIP FUNCTIONS IS 3

    if(largestPoint < _xPointsRaw.get(++current)){
        while(_xPointsFinal.get(current) != SEPARATOR){
            _xPointsFinal.add(_xPointsRaw.get(current));
            _xPointsRaw.set(current, -3);
            _yPointsFinal.add(_yPointsRaw.get(current));
            _yPointsRaw.set(current, -3);
            current++;
        }

        index3 = current + 1;
    }else{
        while(_xPointsRaw.get(current) != SEPARATOR){
            current++;
        }

        index3 = current + 1;

        if(largestPoint < _xPointsRaw.get(++current)){
            while(_xPointsFinal.get(current) != SEPARATOR){
                _xPointsFinal.add(_xPointsRaw.get(current));
                _yPointsRaw.set(current, -3);
                _yPointsFinal.add(_yPointsRaw.get(current));
                _yPointsRaw.set(current, -3);
                current++;
            }
        }
    }

    // go through remaining membership functions one at a time and count intersections with current shape
    // also record points of intersection

    // temp varaibles to merge lines
    float xtemp, ytemp;
    LinkedList<float> xIntercepts, yIntercepts;
    LinkedList<float> xRawTemp, yRawTemp; 
    xIntercepts.add(_min_X);
    yIntercepts.add(0);

    // if membership function is unprocessed
    if(_xPointsRaw.get(index2) != -3){
        xIntercepts.clear();
        yIntercepts.clear();

        xRawTemp.clear();
        yRawTemp.clear();

        // while the membership function has not ended
        while(_xPointsRaw.get(index2+1) != -2){
            xRawTemp.add(_xPointsRaw.get(index2));
            yRawTemp.add(_yPointsRaw.get(index2));

            // check every line in the processed list
            for(int i = 0; i < _xPointsFinal.size(); i++){
                // check for an intersection
                if(linesIntersect(xtemp, ytemp, _xPointsRaw.get(index2), _yPointsRaw.get(index2), _xPointsRaw.get(index2+1), _yPointsRaw.get(index2+1), _xPointsFinal.get(i), _yPointsFinal.get(i), _xPointsFinal.get(i+1), _yPointsFinal.get(i+1))){
                    xIntercepts.add(xtemp);
                    yIntercepts.add(ytemp);

                    combineIntersect(xRawTemp, yRawTemp, xIntercepts, yIntercepts);
                }
            }   
            index2++;
        }
        xIntercepts.add(_max_X);
        yIntercepts.add(0);
        combineIntersect(xRawTemp, yRawTemp, xIntercepts, yIntercepts);
        
    }if(_xPointsRaw.get(index3) != -3){
        xIntercepts.clear();
        yIntercepts.clear();

        xRawTemp.clear();
        yRawTemp.clear();

        // while the membership function has not ended
        while(_xPointsRaw.get(index3+1) != -2){
            xRawTemp.add(_xPointsRaw.get(index2));
            yRawTemp.add(_yPointsRaw.get(index2));

            // check every line in the processed list
            for(int i = 0; i < _xPointsFinal.size(); i++){
                // check for an intersection
                if(linesIntersect(xtemp, ytemp, _xPointsRaw.get(index3), _yPointsRaw.get(index3), _xPointsRaw.get(index3+1), _yPointsRaw.get(index3+1), _xPointsFinal.get(i), _yPointsFinal.get(i), _xPointsFinal.get(i+1), _yPointsFinal.get(i+1))){
                    xIntercepts.add(xtemp);
                    yIntercepts.add(ytemp);
                   
                    combineIntersect(xRawTemp, yRawTemp, xIntercepts, yIntercepts);
                }
            }   
            index3++;
        }
        xIntercepts.add(_max_X);
        yIntercepts.add(0);
        combineIntersect(xRawTemp, yRawTemp, xIntercepts, yIntercepts);
    }
    
}

bool FuzzyO::combineIntersect(LinkedList<float> xRaw, LinkedList<float> yRaw, LinkedList<float> xIntercepts, LinkedList<float> yIntercepts){
    // get the last 2 intercepts 
    float xIntCurrent = xIntercepts.get(xIntercepts.size() - 1);
    float xIntPrev = xIntercepts.get(xIntercepts.size() - 2);

    // find the max value of the raw list and final list between these two points
    float maxRaw = 0.0;
    float maxFinal = 0.0;

    bool firstInterceptFoundFinal = false;
    bool firstInterceptFoundRaw = false;
    int startIndexRaw, startIndexFinal;

    // find max in raw list
    for(int i = 0; i < yRaw.size(); i++){
        // only consider values between last 2 intercepts
        if(xRaw.get(i) > xIntPrev && xRaw.get(i) < xIntCurrent){
            if(!firstInterceptFoundRaw){
                startIndexRaw = i;
            }
            if(yRaw.get(i) > maxRaw){
                maxRaw = yRaw.get(i);
            }
        }
    }

    // find max in final list
    for(int j = 0; j < _yPointsFinal.size(); j++){
        if(_xPointsFinal.get(j) > xIntPrev && _xPointsFinal.get(j) < xIntCurrent){
            // locate start of relevant points
            if(!firstInterceptFoundFinal){
                startIndexFinal = j;
            }

            if(_yPointsFinal.get(j) > maxFinal){
                maxFinal = _yPointsFinal.get(j);
            }
        }
    }

    // identify need to replace segment of line list
    if(maxRaw > maxFinal){
        // first delete all points in relevant section
        while(_xPointsFinal.get(startIndexFinal) < xIntCurrent){
            _xPointsFinal.remove(startIndexFinal);
        }

        // next, add all points in relevant section
        while(_xPointsRaw.get(startIndexRaw) < xIntCurrent){
            _xPointsFinal.add(startIndexFinal, _xPointsRaw.get(startIndexRaw));
            startIndexRaw++;
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


    