/**

 Advanced testing application for the Infineon TLE94112LE Arduino Sield
 
 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0 Release Candidate\n
 *  Licensed under GNU LGPL 3.0
*/

#include "commands.h"
#include "motorcontrol.h"

MotorControl motor;
//! operating mode
boolean modeAuto;
//! Status LED
int ledPin;

// ==============================================
// Initialisation
// ==============================================
void setup() {
  // Serial is initialised at high speed. If your Arduino boards
  // loose characters or show unwanted/unexpected behavior
  // try with a lower communication speed
  Serial.begin(38400);

  // Assign the LED pint number and initialize the output  
  ledPin = 13;
  pinMode(ledPin, OUTPUT);   // LED reading signal

  flashLED();

  // Print the initialisation message
  Serial.println(APP_TITLE);

  // initialize the motor class
  motor.begin();  
  modeAuto = false;

  flashLED();

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

  // Check if the motor is running to test the error status
//  if(motor.internalStatus.isRunning) {
//    if(motor.tleCheckDiagnostic()) {
//      motor.tleDiagnostic();
//    }
//  }

  if(Serial.available() > 0){
    parseCommand(Serial.readString());
    flashLED();
  } // serial available
}

//! Short loop flashing led for signal
void flashLED(void) {
  int j;
  
  for(j = 0; j < 5; j++) {
    digitalWrite(ledPin, 1);
    delay(100);
    digitalWrite(ledPin, 0);
    delay(100);
  }
}

//! Send a message to the serial
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
  // Informative commands
  // =========================================================

  if(commandString.equals(SHOW_CONF)) {
    motor.showInfo();
  }

  // =========================================================
  // Motor control
  // =========================================================

  else if(commandString.equals(MOTOR_FEED)) {
    serialMessage(CMD_EXEC, commandString);
    motor.feedExtruder(1500);
    motor.tleDiagnostic();
  }
  else if(commandString.equals(MOTOR_PULL)) {
    serialMessage(CMD_EXEC, commandString);
    motor.filamentLoad(1500);
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

