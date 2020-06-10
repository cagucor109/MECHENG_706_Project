# MECHENG_706_Project
Group 21


Instructions on how to run:
FIND PINOUTS IN THE SENSOR AND MOTOR LIBRARIES and 
confirm hardware is wired accordingly.
Open the main.ino file in the Arduino IDE.
Set the board to Arduino mega and then set the port.
Compile and upload the code to the robot.
With the robot powered off, place the robot in the starting position.
Turn the robot on.
Once complete, power off the robot.


The main loop updates sensor readings, then runs behavioural control using the “suppressor()” function
and then updates the control effort to the actuators. Behavioural control is implemented along with
fuzzy logic control for the obstacle avoidance and move to fire behaviours. 
