This is a readme for the sensors library.

This library handles the reading and intepretation of sensor values into position of the robot. All positional data is relative to
the center of the robot. Centering is acheived with numerical offsets measured for each sensor that are defined at the top of Sensors.h

Pin connections of the sensors are also defined at the top of the Sensors.h file

Position values are updated with the update methods, which should be called before reading position using the getters.
The exception to this is the gyro which should be updated in a 10ms loop. The gyro should be enabled before use and disabled after use,
with the readings it gives being relative to the initial position when enabled to remove error due to drift.

Calibration coefficents for the IR sensors, and calibrated range limits are stored in private members. 

The ParallelError function checks that the two short range IR sensors are detecting the wall. This function should be called to check 
the validity of readings before values from the getParallel() function.
