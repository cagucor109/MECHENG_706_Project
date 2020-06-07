This is a readme for the sensors library.

This library handles the reading and intepretation of sensor values environmental information. There are left, right and front zone scores, photo arc angle and photo intensity. 

Pin connections of the sensors are also defined at the top of the Sensors.h file

Values are updated with the update methods, which should be called before reading position using the getters
The exception to this is the gyro which should be updated in a 10ms loop. The gyro should be enabled before use and disabled after use,
with the readings it gives being relative to the initial position when enabled to remove error due to drift.

Calibration coefficents for the IR sensors, and calibrated range limits are stored in private members. 

