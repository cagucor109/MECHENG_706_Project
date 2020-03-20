//To change between implementation methods A and B change the name of the choosen function to "readIr".



//-------------------------------------------------------------------
//Method A this is the Calibration curve method.

//will make this into a struct and make the coefficent handling cleaner.






uint16_t readIr(byte readPin, float Coefficents[],uint16_t Constraints[]){
  float reading=analogRead(readPin);
  reading=constrain(reading,Constraints[0],Constraints[1]);
  //Serial.println(reading);
  return 1000/((Coefficents[0]*reading*reading)+(Coefficents[1]*reading)+(Coefficents[2]));  
  }



//---------------------------------------------------------------------
///Method B This is the linear interpolation method
/*
#define MAP_POINTS 10

struct MAP {
   int inputs[MAP_POINTS];
   int outFront[MAP_POINTS];
   int outBACK[MAP_POINTS];
   int outLeftFront[MAP_POINTS];
   int outLeftRear[MAP_POINTS];
  };


// note: the _in array should have increasing values
int multiMap(int val, int* _in, int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size-1]) return _out[size-1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while(val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos-1]) * (_out[pos] - _out[pos-1]) / (_in[pos] - _in[pos-1]) + _out[pos-1];
}

  uint16_t readIR(byte ReadPin, byte SensorID){ //Clearly some sensor ID stuff will have to be sorted to select between sensors and coefficents.
       int Read=analogRead(ReadPin);
       uint16_t Value=multiMap; //input some bullshit parameters       
       return Value;      
    }

    */
