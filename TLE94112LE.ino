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

//! Motor control class instance
MotorControl motor;

//! Status LED
#define LEDPIN 12
//! Analog duty cycle input pin
#define ANALOG_DCPIN A0

//! LCD library initialisation
ShiftLCD lcd(2, 3, 4);

//! Duty cycle value read from the analog in
uint8_t inputAnalogDC;
//! Last dutycycle value read from the analog in
uint8_t lastAnalogDC;

//! Duty cycle analog read should be ignore (bypass the analog reading)
#define ANALOG_DCNONE 0
//! Duty cycle analog read should be assgined to DC min
#define ANALOG_DCMIN 1
//! Duty cycle analog read should be assgined to DC mAX
#define ANALOG_DCMAX 2
//! Duty cycle analog read should be assgined to DC manual reading
#define ANALOG_DCMAN 3

//! Duty cycle analog value currently used during readings
int analogDutyCycle;

// ==============================================
// Initialisation
// ==============================================
void setup() {
  // Serial is initialised at high speed. If your Arduino boards
  // loose characters or show unwanted/unexpected behavior
  // try with a lower communication speed
  Serial.begin(38400);

  analogDutyCycle = ASNALOG_DCNONE;
  pinMode(LEDPIN, OUTPUT);   // LED reading signal
  pinMode(ANALOG_DCPIN, INPUT);
  // 1.1 V on AVR 328p and 1.8 - 3.3 on the XMC1100
  analogReference(INTERNAL);  
  inputAnalogDC = lastAnalogDC = readAnalogDutyCycle();
  
  flashLED();

  // Print the initialisation message
  Serial.println(APP_TITLE);

  // initialize the motor class
  motor.begin();  

  // initialize the LCD
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
 * Read the analog value from potentiometer and convert it in the range
 * 0 - max duty cycle value
 * 
 * \return the reading value
 */
 uint8_t readAnalogDutyCycle(void) {
   return map(analogRead(ANALOG_DCPIN), 0, 1024, 0, DUTYCYCLE_MAX)
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
  // =========================================================
  // Duty cycle settings
  // =========================================================
  else if(commandString.equals(MANUAL_DC)) {
    motor.setMotorManualDC(MOTOR_MANUAL_DC);
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(AUTO_DC)) {
    motor.setMotorManualDC(MOTOR_AUTO_DC);
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(MIN_DC)) {
    
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(MAX_DC)) {

    serialMessage(CMD_MODE, commandString);
  }
  
  
  
  
  else
    serialMessage(CMD_WRONGCMD, commandString);
 }

