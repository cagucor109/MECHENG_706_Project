Controllers class 
=================

The controllers class implements 3 PID controllers (front motion, side motion and yaw angle) with seperate define macros specifying their corresponding gains. 

Methods
-------

There are methods to set and get the desired set point for each respective controller. These are set by default in the constructors as 150 mm to the wall on the front, 150 mm to the wall on the left and 0 degrees yaw angle relative to the left wall.

The user now has the option of which controller to use: P, PI or PID. Each of these has a seperate method to call, but they all must be called after the method to calculate error.

The use should be as follows:

.. code-block:: c

   Controllers controller();

   double error;
   double controlEffort;

   error = controller.calculateError("front", 180);
   controlEffort = controller.controlPID("front", error);
   
This control effort can then be input into the motors described in the Motors class to move the robot to a position specified in by the desired set points.
