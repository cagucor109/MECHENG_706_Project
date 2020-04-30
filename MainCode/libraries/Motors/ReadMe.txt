Motor Class
===============

The motors class include functions that communicate with the servos controlling them

Motor powers are calculated using inverse kinematics, taking in the desired x, y and w input and outputting the appropraite angular rotation, the inputs can be received from the Control Library.

The servo motors to the wheels respond to PWM microseconds signals, with the centre assumed at 1500 micro seconds, with the range 1000 microseconds to 2000 microseconds. 

The class is also about to calcualte the distance change of the robot that has occured since the las time the function was run. This serves as the input to the Kalman filter in the Kalman library. 

The motor class is also about to indicate if the wheels are enabled or disabled. 

A debug function is included.

