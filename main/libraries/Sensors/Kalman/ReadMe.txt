Kalman Class 
==================

The Kalman class implements a Kalman filter that takes in two inputs; control actions made to the system, as well as sensor observations, to provide a mean position estimate that is more likely to be the true position.

The noise contribution of either the motion (R ) or sensor model (Q) can be adjusted according to physical tuning. 

Setters can be used to adjust the Sensor and Process noise, as well as the initial position to be used for the motion model

A getter is available to retrieve the new position estimate


