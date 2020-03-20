
/*
 * This is the position data that is used for the control of the robot.
 * This data is interpretted from sensor readings
 * 
 * frontDistance is the distance to the center of the robot to the obstacle in front of the robot in mm. 
 * A reading of max value means there is no observable obstacle. A reading less than ____ means the robot is touching the obstacle in front. 
 *  
 * rearDistance is the distance to the center of the robot to the obstacle behind of the robot in mm. 
 * A reading of max value means there is no observable obstacle. A reading less than ____ means the robot is touching the obstacle behind.
 * 
 * leftDistance is the distance to the center of the robot to the obstacle to the left of the robot in mm. 
 * A reading of max value means there is no observable obstacle. A reading less than ____ means the robot is touching the obstacle to the left.
 * 
 * Angle is the relative position of the robot, as measured by the gyro. 
 * This measurement is only accurate short term (<1 minute (requires testing)) and should be reset before use. 
 * Here clockwise is positive.
 * 
 * Parallel is a measure of the left face of the robot relative to an obstacle such as a wall. 
 * This is an angle from -120 to 120 degrees where 0 is parallel and an anticlockwise is positive convention is used. 
 * Such that a positive reading means the front of the robot is closer to the wall than the rear of the robot. 
 * The angle is then mapped to a char (-127,127) using the map function. 
 */



void sensorInit(){
 //Sonar
  pinMode(SONAR_ECHO_PIN, INPUT);
  pinMode(SONAR_TRIG_PIN, OUTPUT);
  
  
  }



//---------------------------------------------------------------------------------
//Gyro Code

//Could look at interpolation instead of rectangular approximation for better accuracy. 
//This function has to run at a constant interval defined by GYRO_READ_TIME.

void UpdateAngle(byte gyroPin){
  float gyroRate = (analogRead(gyroPin) * 5.0) / 1023;
   gyroRate -= 2.48;//Center readings on 0.
  gyroRate/=0.007; //finds acceleration by dividing by sensitivity of gyro



   if (gyroRate >= 1 || gyroRate <= -1) {//this is a minimum read threshold in degrees/second
  gyroRate/=100; //Double integration of acceleration to angle *10ms*10ms (multiply by sample rate twice).
  gyroRate-=GYRO_CORRECTION*GYRO_READ_TIME; //this corrects for sensor drift that is inherrent in approximation of double integration.
      robotPosition.Angle+=gyroRate;

    if ( robotPosition.Angle < -360)// This is probably not required for the use case.
     {
      robotPosition.Angle += 360;
     }
  else if ( robotPosition.Angle > 359)
     {
      robotPosition.Angle -= 360;
     }
  }
 //Serial.println(millis()); //Debug : this will help you calculate the GYRO_READ_TIME. which is critical for accuracy 
 }

void ResetAngle(){   //Could merge with an enable flag and create two functions called GYRO START and GYRO STOP.
  //Set Struct value to zero
  robotPosition.Angle=0;
  }




  //---------------------------------------------------------------------------------------
  //Sonar code

  //Might add multiple sampling and smoothing if required.
void updateLeftDistance(){

  //Sending read signal
  digitalWrite(SONAR_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(SONAR_TRIG_PIN, LOW);
  
  //Read pin
  float reading=pulseIn(SONAR_ECHO_PIN,HIGH);

  //Apply speed of sound correction to calculate distance in mm.
    reading=reading/(2.94*2); //340m/s  give 2.941176ms/mm. This is then halved to account for rebounding

  reading+=SONAR_POSITION_CORRECTION;
  
  robotPosition.leftDistance=reading;
  }

  //----------------------------------------------------------------------------------------
  //This is the IR Code

//PATCH NOTE: might add multiple sampling or split out into front and back functions if required.
//This is for front and back distance.
void  UpdateDistances(){
  //read sensor
  uint16_t frontIr=readIr(IR_FORWARD_PIN,Coefficents.irForward,Coefficents.longConstraints);// the array passing is subject to change
  uint16_t rearIr=readIr(IR_REAR_PIN,Coefficents.irRear,Coefficents.longConstraints);
  
//Need to check for out of range (Currently handled by the readIR)

  frontIr+=IR_FORWARD_POSITION_CORRECTION;
  robotPosition.frontDistance= frontIr;
  
  rearIr+=IR_REAR_POSITION_CORRECTION;
  robotPosition.rearDistance=rearIr;

  }


///This is broken. The short range IR's are giving me shit.
  void UpdateParallel(){
  //read sensors (2xIR)
 // uint16_t frontIr=readIr(IR_LEFT_FRONT_PIN,Coefficents.irLeftFront,Coefficents.shortConstraints);// the array passing is subject to change
  uint16_t backIr=readIr(IR_LEFT_BACK_PIN,Coefficents.irLeftBack,Coefficents.shortConstraints);
//Should do some constraining

 // Serial.print(",");
 // Serial.println(frontIr);
 
  Serial.println(backIr);
  //Take Difference  
//  double difference=frontIr-backIr;

  //Add some offset to the readings
  //Calculate angle
   //double rads=atan2(difference,(IR_LEFT_FRONT_OFFSET+IR_LEFT_BACK_OFFSET));
    
  //Store in sensors Struct
  //robotPosition.Parallel=rads*180/PI;
  }
