# MECHENG_706_Project
## Structure
Every arduino file (type extension .ino) must be in a folder of the same name to be recognised by the arduino IDE. With this in mind, each file should come a small and descriptive readme in a seperate folder.

Instructions on how to run:
Open the MainCode.ino file in the Arduino IDE.
Set the board to Arduino mega and then set the port.
Compile and upload the code to the robot.
With the robot powered off, place the robot in the start position of the course.
Turn the robot on.
Once complete, power off the robot and reset to the starting position.


The main loop contains two switch case statements.
One for deciding exit conditions and next state and one for state outputs.
Included are also boolean functions used to evaluate sensor readings
and output motor control functions.

