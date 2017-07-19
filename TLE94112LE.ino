/**

  3D Printer FIlament Dispense - USB to serial control

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 Thie is the motorised verion of the 3D printer filament monitor based on the Infineon brushed
 DC motor controller Arduino shield.
 The software has been developed on the Infineon XMC110 mciro controller board Arduino compatible

 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0 Release Candidate\n
 *  Licensed under GNU LGPL 3.0
*/

#include "filament.h"
#include "filamentweight.h"
#include "commands.h"
#ifdef _USE_MOTOR
#include "motorcontrol.h"
#endif

#ifdef _USE_MOTOR
MotorControl motor;
//! operating mode
boolean modeAuto;
#endif

//! The weight control class
FilamentWeight scale;

// ==============================================
// Initialisation
// ==============================================
void setup() {
  // Serial is initialised at high speed. If your Arduino boards
  // loose characters or show unwanted/unexpected behavior
  // try with a lower communication speed
  Serial.begin(38400);

  // Print the initialisation message
  Serial.println(APP_TITLE);

  // Initialize the weight class
  scale.begin();
#ifdef _USE_MOTOR
  // initialize the motor class
  motor.begin();  
  modeAuto = false;
#endif
}

// ==============================================
// Main loop
// ==============================================
/** 
 * The main loop role is execturing the service functions; display update, 
 * calculations, button checking
 * The scale reading is done at a specific frequence and is interrupt-driven
 */
void loop() {
  scale.readScale();

//  Serial.println(scale.lastRead);
  
#ifdef _USE_MOTOR
  // Check for the extruder request
  if( (scale.statID == STAT_RUN) && (scale.currentStatus.filamentNeededFromExtruder == true) ) {
    if(modeAuto) {
      motor.feedExtruder(FEED_EXTRUDER_DELAY);
      motor.tleDiagnostic();
    }
  }
#endif

  // Check if the motor is running to test the errors status
  if(motor.internalStatus.isRunning) {
    if(motor.tleCheckDiagnostic()) {
      motor.tleDiagnostic();
    }
  }

  if(Serial.available() > 0){
    parseCommand(Serial.readString());
  } // serial available
}

//! Send a single line message to the serial
void serialMessage(String title, String description) {
    Serial.print(title);
    Serial.print(" ");
    Serial.println(description);
}

/**
 * Parse the command string and echo the executing message or command unknown error.
 * 
 * \param commandString the string coming from the serial
 */
 void parseCommand(String commandString) {

  // =========================================================
  // Parameters settings
  // =========================================================

  // Set PLA material and recalculate the material characteristics
  // Flag is set to display an update nesxt loop cycle
  if(commandString.equals(SET_PLA)) {
    scale.materialID = PLA;
    scale.calcMaterialCharacteristics();
    scale.showInfo();
  }
  // Set ABS material and recalculate the material characteristics
  // Flag is set to display an update nesxt loop cycle
  else if(commandString.equals(SET_ABS)) {
    scale.materialID = ABS;
    scale.calcMaterialCharacteristics();
    scale.showInfo();
  }
  // Set 1.75 mm filament diameter and recalculate the material characteristics
  // Flag is set to display an update nesxt loop cycle
  else if(commandString.equals(SET_175)) {
    scale.diameterID = DIAM_175;
    scale.calcMaterialCharacteristics();
    scale.showInfo();
  }
  // Set 3.00 mm filament diameter and recalculate the material characteristics
  // Flag is set to display an update nesxt loop cycle
  else if(commandString.equals(SET_300)) {
    scale.diameterID = DIAM_300;
    scale.calcMaterialCharacteristics();
    scale.showInfo();
  }
  // Set 1kg filament spool and recalculate the material characteristics
  // Flag is set to display an update next loop cycle
  else if(commandString.equals(SET_1KG)) {
    scale.wID = ROLL1KG;
    scale.calcMaterialCharacteristics();
    scale.showInfo();
  }
  // Set 2kg filament spool and recalculate the material characteristics
  // Flag is set to display an update nesxt loop cycle
  else if(commandString.equals(SET_2KG)) {
    scale.wID = ROLL2KG;
    scale.calcMaterialCharacteristics();
    scale.showInfo();
  }
  // Set units in grams
  else if(commandString.equals(SET_WEIGHT)) {
    serialMessage(CMD_UNITS, commandString);
    scale.filamentUnits = _GR;
  }
  // Set units in cm
  else if(commandString.equals(SET_CENTIMETERS)) {
    serialMessage(CMD_UNITS, commandString);
    scale.filamentUnits = _CM;
  }

  // =========================================================
  // Change current functional status
  // =========================================================

  // Send a reset command and restore the parameters to the defaults
  // The tare is not recalculated to avoid wrong measure (if the spool
  // is already on the scale platform
  // This command had mandatory executi9on and ignore the previous state
  // The flag is set to show an update nextg loop cycle
  else if(commandString.equals(S_RESET)) {
    scale.reset();
    scale.stat = SYS_READY;
    scale.statID = STAT_READY;
//    scale.flashLED();
    scale.showInfo();
  }
  // Send a load command status setting
  // Should be executed after the filament roll has been set 
  // and placed on the scale base or after a reset command
  // The flag is set to show an update nextg loop cycle
  else if(commandString.equals(S_LOAD)) {
    scale.stat = SYS_LOAD;
    scale.statID = STAT_LOAD;
    scale.initialWeight = 0;
    scale.readScale();
    scale.showLoad();
  }
  // Send a run command status setting
  // Should be sent when a print job is started
  else if(commandString.equals(S_RUN)) {
    scale.stat = SYS_RUN;
    scale.statID = STAT_RUN;
    scale.initialWeight = scale.lastRead - scale.rollTare;
    scale.prevRead = scale.lastRead;
    scale.lastConsumedGrams = 0;
    scale.showStat();
  }
  // Send a default command status setting
  // Should be used to reset the system to the default values 
  // of the material without changing any setting in the weight
  // tare and calculations but the current status is not changed.
  // Use this commmand to reset the material to the internal conditions
  else if(commandString.equals(S_DEFAULT)) {
    scale.setDefaults();
    scale.showInfo();
  }  

  // =========================================================
  // Informative commands
  // =========================================================

  else if(commandString.equals(SHOW_INFO)) {
    scale.showInfo();
  }
  else if(commandString.equals(SHOW_STATUS)) {
    scale.showLoad();
    scale.showStat();
  }
  else if(commandString.equals(SHOW_DUMP)) {
    scale.showConfig();
  }
  else if(commandString.equals(SHOW_WEIGHT)) {
    Serial.print(CMD_WEIGHT);
    Serial.print(scale.getWeight());
    Serial.println(UNITS_GR);
  }

  // =========================================================
  // Motor control
  // =========================================================

#ifdef _USE_MOTOR
  else if(commandString.equals(MOTOR_FEED)) {
    serialMessage(CMD_EXEC, commandString);
    motor.feedExtruder(FEED_EXTRUDER_DELAY);
    motor.tleDiagnostic();
  }
  else if(commandString.equals(MOTOR_PULL)) {
    serialMessage(CMD_EXEC, commandString);
    motor.filamentLoad(FEED_EXTRUDER_DELAY);
    motor.tleDiagnostic();
  }
  else if(commandString.equals(MOTOR_STOP)) {
    serialMessage(CMD_EXEC, commandString);
    motor.motorBrake();
    motor.tleDiagnostic();
  }
  else if(commandString.equals(MOTOR_FEED_CONT)) {
    serialMessage(CMD_EXEC, commandString);
    motor.filamentContFeed();
  }
  else if(commandString.equals(MOTOR_PULL_CONT)) {
    serialMessage(CMD_EXEC, commandString);
    motor.filamentContLoad();
  }
#endif

  // =========================================================
  // Change behaviour mode
  // =========================================================

  else if(commandString.equals(MODE_AUTO)) {
    serialMessage(CMD_MODE, commandString);
    modeAuto = true;
  }
  else if(commandString.equals(MODE_MANUAL)) {
    serialMessage(CMD_MODE, commandString);
    modeAuto = false;
  }
  else
    serialMessage(CMD_WRONGCMD, commandString);
 }

