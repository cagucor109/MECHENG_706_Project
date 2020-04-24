//Add some comments and or descriptions at the top... ... ... 

//----------------------Libraries-------------------------------------------------------------------------------------------------------------------------------------------------------------------
// include libraries
#include "libraries\Controllers\Controllers.h"
#include "libraries\Controllers\Controllers.cpp"
#include "libraries\Sensors\Sensors.h"
#include "libraries\Sensors\Sensors.cpp"
#include "libraries\Motors\Motors.h"
#include "libraries\Motors\Motors.cpp"

  Controllers controlSystem;
  Sensors sensor;
  Motors motor;

//----------------------Battery check and Serial Comms-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#define NO_BATTERY_V_OK //Uncomment of BATTERY_V_OK if you do not care about battery damage.
//Serial Pointer
HardwareSerial *SerialCom;


//----------------------State machines-------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum STATE {
  INITIALISING,
  RUNNING,
  STOPPED
};
// modes for turning, following wall or setting up
enum MODE {
  SETTING_UP,
  EDGE_FOLLOW,
  TURNING,
  FINISHED
};


//----------------------Setup and main loop-------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial;
  SerialCom->begin(115200);
  SerialCom->println("Setup....");

  delay(500); //settling time but not really needed
}



void loop(void)
{
  static STATE machine_state = INITIALISING;

  //Finite-state machine Code
  switch (machine_state) {
    case INITIALISING:
      machine_state = initialising();
      break;
    case RUNNING: //Lipo Battery Volage OK
      machine_state =  running();
      break;
    case STOPPED: //Stop of Lipo Battery voltage is too low, to protect Battery
      machine_state =  stopped();
      break;
  };
}


//----------------------Initialising state-------------------------------------------------------------------------------------------------------------------------------------------------------------------

STATE initialising() {
  //initialising
  SerialCom->println("INITIALISING....");
  delay(1000); //One second delay to see the serial string "INITIALISING...."
  SerialCom->println("Enabling Motors...");
  motor.enable_motors();
  SerialCom->println("RUNNING STATE...");
  return RUNNING;
}


//----------------------Running state-------------------------------------------------------------------------------------------------------------------------------------------------------------------

STATE running() {
  static MODE runMode = SETTING_UP;
  static MODE nextMode = EDGE_FOLLOW;


  static unsigned long previous_millis;
  static int count_turns = 0;
  static int parallel = 0;
  static int left = 0;
  static int restartCtrl = 0;
  static int reached_wall = 0;
  fast_flash_double_LED_builtin();

  //UPDATE SENSORS/INPUTS
  sensor.updateLeftDistance();
  sensor.updateParallel();
  sensor.updateFrontDistance();
  sensor.updateAngle();

  // NEXT STATE
  switch (runMode) {
    case SETTING_UP:
      if (sensor.getFrontDistance() < 150) { //I think that this is invalid should be != //But also not because It would be better to have a tolerance band.
        nextMode = TURNING;
      } else if ((sensor.getLeftDistance() == 150) && (sensor.getParallel() == 0)) { //Should add some tolerance to this Otherwise it will never exit. any error will be accounted for in edge_follow anyway.
        nextMode = EDGE_FOLLOW;
      } else {
        nextMode = SETTING_UP;
      }
      break;
    case EDGE_FOLLOW:
      if ((count_turns < 4) && (sensor.getFrontDistance() < 150)) { // wall is too close muse turn
        nextMode = TURNING;
      } else if ((count_turns < 4) && (sensor.getFrontDistance() > 150)) { //  wall is not close enough
        nextMode = EDGE_FOLLOW;
      } else {
        nextMode = FINISHED;
      }
      break;
    case TURNING:   // turns with a flat rate based on gyro reading until about 75 deg then enters setup mode.
      if ((sensor.getGyroState() == 0) && (sensor.getLeftDistance() == 150) && (sensor.getParallel() == 0)) { // can change this so that it loops back into settin up mode before going back into edge follow for safety.
        nextMode = EDGE_FOLLOW;
      } else {
        nextMode = TURNING;
      }
      break;
    case FINISHED:
      if (restartCtrl == 1) {
        nextMode = SETTING_UP;
      } else {
        nextMode = FINISHED;
      }

      break;
  }



  
  motor.rotateControl = 0;
  motor.x_controlEffort = 0;
  motor.y_controlEffort = 0;

  
  // STATE OUTPUTS
  switch (runMode) {
    case SETTING_UP:
      if (nextMode == SETTING_UP) {
        motor.rotateControl = controlSystem.controlP("angle", controlSystem.calculateError("angle", sensor.getParallel()));
        motor.x_controlEffort = controlSystem.controlP( "left", controlSystem.calculateError("left", sensor.getLeftDistance()));
      } else {
        // stop motor control to not overshoot
        motor.rotateControl = 0;
        motor.x_controlEffort = 0;
        motor.y_controlEffort = 0;
        if (nextMode == TURNING) {
          sensor.enableGyro(); // if turning start the gyro
          controlSystem.setDesiredAngle(90); // set turn target to 90 degrees
        } else { // edge follow mode
          sensor.disableGyro();
          controlSystem.setDesiredAngle(0); // set turn target to 0 degrees
        }
      }

      
      break;
    case EDGE_FOLLOW:
      if (nextMode == EDGE_FOLLOW) {
        if (sensor.getFrontDistance() > 150) {
          motor.y_controlEffort = controlSystem.controlP( "front", controlSystem.calculateError("front", sensor.getFrontDistance()));
        }
        if (sensor.getLeftDistance() != 150) {
          motor.x_controlEffort = controlSystem.controlP( "left", controlSystem.calculateError("left", sensor.getLeftDistance()));
        }
        if (sensor.getAngle() != 0) {
          motor.rotateControl = controlSystem.controlP("angle", controlSystem.calculateError("angle", sensor.getParallel()));
        }
      } else {
        //stop to turn or finish
        motor.rotateControl = 0;
        motor.x_controlEffort = 0;
        motor.y_controlEffort = 0;
        if (nextMode == TURNING) { // if turning start the gyro
          sensor.enableGyro();
          controlSystem.setDesiredAngle(90); // set turn target to 90 degrees
        } else { // move into finished mode
          sensor.disableGyro();
          controlSystem.setDesiredAngle(0);
        }
      }
      break;
    case TURNING:
      if (nextMode == TURNING) {
        motor.rotateControl = controlSystem.controlP( "angle", controlSystem.calculateError("angle", sensor.getAngle())); // directs rotation control to target previously set at 90 degrees.
      } else if (nextMode == EDGE_FOLLOW) {
        // turn off motor controls
        motor.rotateControl = 0;
        motor.x_controlEffort = 0;
        motor.y_controlEffort = 0;
        // disable gyro and reset angle to 0
        sensor.disableGyro();
        controlSystem.setDesiredAngle(0);
      } else {

      }
      break;
    case FINISHED:
      // stop motor controls
      motor.x_controlEffort = 0;
      motor.y_controlEffort = 0;
      motor.rotateControl = 0;
      // ask user if they want to complete another lap
      Serial.println(" ");
      Serial.println("Another Lap?");
      while (Serial.available() == 0) {} //wait for user input
      restartCtrl = Serial.parseInt(); // read input

      break;
  }
  motor.powerMotors(); // compiles the control efforts and sends power to motor drivers.

  if (millis() - previous_millis > 500) {  //Arduino style 500ms timed execution statement
    previous_millis = millis();

    SerialCom->println("RUNNING---------");

#ifndef NO_BATTERY_V_OK
    if (!is_battery_voltage_OK()) return STOPPED;
#endif

    return RUNNING;
  }
}
  //----------------------Stopped state-------------------------------------------------------------------------------------------------------------------------------------------------------------------

  //Stop of Lipo Battery voltage is too low, to protect Battery
  STATE stopped() {
    static byte counter_lipo_voltage_ok;
    static unsigned long previous_millis;
    int Lipo_level_cal;
    motor.disable_motors();
    slow_flash_LED_builtin();

    if (millis() - previous_millis > 500) { //print massage every 500ms
      previous_millis = millis();
      SerialCom->println("STOPPED---------");


#ifndef NO_BATTERY_V_OK
      //500ms timed if statement to check lipo and output speed settings
      if (is_battery_voltage_OK()) {
        SerialCom->print("Lipo OK waiting of voltage Counter 10 < ");
        SerialCom->println(counter_lipo_voltage_ok);
        counter_lipo_voltage_ok++;
        if (counter_lipo_voltage_ok > 10) { //Making sure lipo voltage is stable
          counter_lipo_voltage_ok = 0;
          motor.enable_motors();
          SerialCom->println("Lipo OK returning to RUN STATE");
          return RUNNING;
        }
      } else
      {
        counter_lipo_voltage_ok = 0;
      }
#endif
    }
    return STOPPED;
  }

  //----------------------Satus and battery Check---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // end of edge follow code
  void fast_flash_double_LED_builtin()
  {
    static byte indexer = 0;
    static unsigned long fast_flash_millis;
    if (millis() > fast_flash_millis) {
      indexer++;
      if (indexer > 4) {
        fast_flash_millis = millis() + 700;
        digitalWrite(LED_BUILTIN, LOW);
        indexer = 0;
      } else {
        fast_flash_millis = millis() + 100;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      }
    }
  }

  void slow_flash_LED_builtin()
  {
    static unsigned long slow_flash_millis;
    if (millis() - slow_flash_millis > 2000) {
      slow_flash_millis = millis();
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }


#ifndef NO_BATTERY_V_OK
  boolean is_battery_voltage_OK()
  {
    static byte Low_voltage_counter;
    static unsigned long previous_millis;

    int Lipo_level_cal;
    int raw_lipo;
    //the voltage of a LiPo cell depends on its chemistry and varies from about 3.5V (discharged) = 717(3.5V Min) https://oscarliang.com/lipo-battery-guide/
    //to about 4.20-4.25V (fully charged) = 860(4.2V Max)
    //Lipo Cell voltage should never go below 3V, So 3.5V is a safety factor.
    raw_lipo = analogRead(A0);
    Lipo_level_cal = (raw_lipo - 717);
    Lipo_level_cal = Lipo_level_cal * 100;
    Lipo_level_cal = Lipo_level_cal / 143;

    if (Lipo_level_cal > 0 && Lipo_level_cal < 160) {
      previous_millis = millis();
      SerialCom->print("Lipo level:");
      SerialCom->print(Lipo_level_cal);
      SerialCom->print("%");
      // SerialCom->print(" : Raw Lipo:");
      // SerialCom->println(raw_lipo);
      SerialCom->println("");
      Low_voltage_counter = 0;
      return true;
    } else {
      if (Lipo_level_cal < 0)
        SerialCom->println("Lipo is Disconnected or Power Switch is turned OFF!!!");
      else if (Lipo_level_cal > 160)
        SerialCom->println("!Lipo is Overchanged!!!");
      else {
        SerialCom->println("Lipo voltage too LOW, any lower and the lipo with be damaged");
        SerialCom->print("Please Re-charge Lipo:");
        SerialCom->print(Lipo_level_cal);
        SerialCom->println("%");
      }

      Low_voltage_counter++;
      if (Low_voltage_counter > 5)
        return false;
      else
        return true;
    }

  }
#endif
