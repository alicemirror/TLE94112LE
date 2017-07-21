/**

 Advanced testing application for the Infineon TLE94112LE Arduino Sield
 
 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0 Release Candidate\n
 *  Licensed under GNU LGPL 3.0
*/

#include <ShiftLCD.h>
#include <Streaming.h>
#include "commands.h"
#include "motorcontrol.h"

MotorControl motor;
//! operating mode
boolean modeAuto;
//! Status LED
int ledPin;
//! LCD library initialisation
ShiftLCD lcd(2, 3, 4);

// ==============================================
// Initialisation
// ==============================================
void setup() {
  // Serial is initialised at high speed. If your Arduino boards
  // loose characters or show unwanted/unexpected behavior
  // try with a lower communication speed
  Serial.begin(38400);

  // Assign the LED pint number and initialize the output  
  ledPin = 12;
  pinMode(ledPin, OUTPUT);   // LED reading signal

  flashLED();

  // Print the initialisation message
  Serial.println(APP_TITLE);

  // initialize the motor class
  motor.begin();  
  modeAuto = false;

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print(L_APP_NAME1);
  lcd.setCursor(0, 1);
  lcd.print(L_APP_NAME2);
  delay(3000);
  lcd.clear();
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
  int j;
  boolean isRunStatus;

  // Check if the motor is running to test the error status
  // Note: It is possible to isolate the error status, if any,
  // for any motor. Here we make a simplification and check over
  // the motor without filtering
  isRunStatus = false;
  for(j = 0; j < MAX_MOTORS; j++) {
    if(motor.internalStatus[j].isRunning) {
      isRunStatus = true;
      j = MAX_MOTORS; // Force exit from loop
    } // check for running motors
  } // motors loop

  if(isRunStatus) {
    if(motor.tleCheckDiagnostic()) {
      motor.tleDiagnostic();
    }
  }

  if(Serial.available() > 0){
    parseCommand(Serial.readString());
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
  // Motor select
  // =========================================================
  else if(commandString.equals(MOTOR_1)) {
    motor.currentMotor = 1;
    motor.internalStatus[0].isEnabled = true;
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_2)) {
    motor.currentMotor = 2;
    motor.internalStatus[1].isEnabled = true;
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_3)) {
    motor.currentMotor = 3;
    motor.internalStatus[2].isEnabled = true;
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_4)) {
    motor.currentMotor = 4;
    motor.internalStatus[3].isEnabled = true;
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_5)) {
    motor.currentMotor = 5;
    motor.internalStatus[4].isEnabled = true;
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_6)) {
    motor.currentMotor = 6;
    motor.internalStatus[5].isEnabled = true;
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_ALL)) {
    int j;
    motor.currentMotor = 0;
    for(j = 0; j < MAX_MOTORS; j++) {
      motor.internalStatus[j].isEnabled = true;
    }
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_NONE)) {
    int j;
    motor.currentMotor = 0;
    for(j = 0; j < MAX_MOTORS; j++) {
      motor.internalStatus[j].isEnabled = false;
    }
    serialMessage(CMD_SET, commandString);
  }
  // =========================================================
  // PWM channel select
  // =========================================================
  else if(commandString.equals(PWM_0)) {
    motor.setPWM(tle94112.TLE_NOPWM);
    serialMessage(CMD_PWM, commandString);
  }
  else if(commandString.equals(PWM_80)) {
    motor.setPWM(tle94112.TLE_PWM1);
    serialMessage(CMD_PWM, commandString);
  }
  else if(commandString.equals(PWM_100)) {
    motor.setPWM(tle94112.TLE_PWM2);
    serialMessage(CMD_PWM, commandString);
  }
  else if(commandString.equals(PWM_200)) {
    motor.setPWM(tle94112.TLE_PWM3);
    serialMessage(CMD_PWM, commandString);
  }
  // =========================================================
  // Direction and acceleration setting
  // =========================================================
  else if(commandString.equals(DIRECTION_CW)) {
    motor.setMotorDirection(MOTOR_DIRECTION_CW);
    serialMessage(CMD_DIRECTION, commandString);
  }
  else if(commandString.equals(DIRECTION_CCW)) {
    motor.setMotorDirection(MOTOR_DIRECTION_CCW);
    serialMessage(CMD_DIRECTION, commandString);
  }
  else if(commandString.equals(MOTOR_RAMP)) {
    motor.setMotorRamp(RAMP_ON);
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(MOTOR_NORAMP)) {
    motor.setMotorRamp(RAMP_OFF);
    serialMessage(CMD_MODE, commandString);
  }
  // =========================================================
  // Freewheeling mode
  // =========================================================
  else if(commandString.equals(FW_ACTIVE)) {
    motor.setMotorFreeWheeling(MOTOR_FW_ACTIVE);
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(FW_PASSIVE)) {
    motor.setMotorFreeWheeling(MOTOR_FW_PASSIVE);
    serialMessage(CMD_MODE, commandString);
  }
  
  
  
  
  else
    serialMessage(CMD_WRONGCMD, commandString);
 }

