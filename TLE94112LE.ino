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

//! Max analog reading range with a 50K potentiometer
#define MAX_ANALOG_RANGE 255
//! Min analog reading range with a 50K potentiometer
#define MIN_ANALOG_RANGE 0

//! If defined every command is echoed on the serial terminal
#undef _SERIAL_ECHO

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

  analogDutyCycle = ANALOG_DCNONE;
  pinMode(LEDPIN, OUTPUT);   // LED reading signal
  pinMode(ANALOG_DCPIN, INPUT);\
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

  // -------------------------------------------------------------
  // BLOCK 1 : MOTORS RUNNING STATUS
  // -------------------------------------------------------------
  // Check if at least one motor is running to test the error status
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
  // If at least one motor is running check for diagnostic
  if(isRunStatus) {
    if(motor.tleCheckDiagnostic()) {
      motor.tleDiagnostic();
    }
  }
  // -------------------------------------------------------------
  // BLOCK 2 : SERIAL PARSING
  // -------------------------------------------------------------
  // Serial commands parser
  if(Serial.available() > 0){
    parseCommand(Serial.readString());
  } // serial available
  // -------------------------------------------------------------
  // BLOCK 3 : ANALOG READING
  // -------------------------------------------------------------
  // Check if a reading should be done
  if(analogDutyCycle != ANALOG_DCNONE) {
    // Read new value
    inputAnalogDC = readAnalogDutyCycle();
    // Check if value has changed. We should avoid multiple updates 
    // as consumes time (especially in the manual duty cycle case)
    if(inputAnalogDC != lastAnalogDC) {
      lastAnalogDC = inputAnalogDC;
      // Update the new analog value
      switch(analogDutyCycle) {
        case ANALOG_DCMIN:
          // Update the motor setting and display
          motor.setMotorMinDC(inputAnalogDC);
          lcdShowDutyCycleMin();
         break;
        case ANALOG_DCMAX:
          // Update the motor setting and display
          // Map the analog reading value to the high side of the motor dc
          // range as the analog reading max value is smaller.
          motor.setMotorMaxDC(inputAnalogDC);
          lcdShowDutyCycleMax();
         break;
        case ANALOG_DCMAN:
         break;
      } //  update switch
    } // new reading should be updated
  } // Analog reading is active
} // Main loop

//! Short loop flashing led for signal
void flashLED(void) {
  int j;
  
  for(j = 0; j < 5; j++) {
    digitalWrite(LEDPIN, 1);
    delay(100);
    digitalWrite(LEDPIN, 0);
    delay(100);
  }
}

//! Send a message to the serial
void serialMessage(String title, String description) {
#ifdef _SERIAL_ECHO
    Serial.print(title);
    Serial.print(" ");
    Serial.println(description);
#endif
}

/**
 * Read the analog value from potentiometer.
 * 
 * \note With a 50k potentiometer and the 5V reference reading range
 * is 0-180 so the value is not remapped to any scale
 * 
 * \return the reading value
 */
 uint8_t readAnalogDutyCycle(void) {
  int j;
  int readings;
  // Get the average on a series of readings
  readings = 0;
  for(j = 0; j < 5; j++) {
    readings += analogRead(ANALOG_DCPIN);
  }
  readings /= 5;
  delay(250);

  return map(readings, MIN_ANALOG_RANGE, MAX_ANALOG_RANGE, DUTYCYCLE_MIN, DUTYCYCLE_MAX);
 }

/** ***********************************************************
 * Parse the command string and echo the executing message or 
 * command unknown error.
 * 
 * \param commandString the string coming from the serial
 *  ***********************************************************
 */
 void parseCommand(String commandString) {

  // First disable the analog pot reading. Should be active
  // only when the duty cycle is set (or when running in manual
  // dc mode)
  // Note that the analog reading is reset with ANY commmand, 
  // including wrong commands. As a matter of fact any character
  // sent by serial will disable the analog reading of the dc pot
  analogDutyCycle = ANALOG_DCNONE;

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
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_2)) {
    motor.currentMotor = 2;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_3)) {
    motor.currentMotor = 3;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_4)) {
    motor.currentMotor = 4;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_5)) {
    motor.currentMotor = 5;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_6)) {
    motor.currentMotor = 6;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  // =========================================================
  // Motor enable
  // =========================================================
  else if(commandString.equals(MOTOR_ALL)) {
    int j;
    motor.currentMotor = 0;
    for(j = 0; j < MAX_MOTORS; j++) {
      motor.internalStatus[j].isEnabled = true;
    }
    lcd.clear();
    lcdShowMotor();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(MOTOR_NONE)) {
    int j;
    motor.currentMotor = 0;
    for(j = 0; j < MAX_MOTORS; j++) {
      motor.internalStatus[j].isEnabled = false;
    }
    lcd.clear();
    lcdShowMotor();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(EN_MOTOR_1)) {
    motor.currentMotor = 1;
    motor.internalStatus[0].isEnabled = true;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(EN_MOTOR_2)) {
    motor.currentMotor = 2;
    motor.internalStatus[1].isEnabled = true;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(EN_MOTOR_3)) {
    motor.currentMotor = 3;
    motor.internalStatus[2].isEnabled = true;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(EN_MOTOR_4)) {
    motor.currentMotor = 4;
    motor.internalStatus[3].isEnabled = true;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(EN_MOTOR_5)) {
    motor.currentMotor = 5;
    motor.internalStatus[4].isEnabled = true;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  else if(commandString.equals(EN_MOTOR_6)) {
    motor.currentMotor = 6;
    motor.internalStatus[5].isEnabled = true;
    showMotorSetting();
    serialMessage(CMD_SET, commandString);
  }
  // =========================================================
  // PWM channel select
  // =========================================================
  else if(commandString.equals(PWM_0)) {
    motor.setPWM(tle94112.TLE_NOPWM);
    lcdShowPWM();
    serialMessage(CMD_PWM, commandString);
  }
  else if(commandString.equals(PWM_80)) {
    motor.setPWM(tle94112.TLE_PWM1);
    lcdShowPWM();
    serialMessage(CMD_PWM, commandString);
  }
  else if(commandString.equals(PWM_100)) {
    motor.setPWM(tle94112.TLE_PWM2);
    lcdShowPWM();
    serialMessage(CMD_PWM, commandString);
  }
  else if(commandString.equals(PWM_200)) {
    motor.setPWM(tle94112.TLE_PWM3);
    lcdShowPWM();
    serialMessage(CMD_PWM, commandString);
  }
  // =========================================================
  // Direction and acceleration setting
  // =========================================================
  else if(commandString.equals(DIRECTION_CW)) {
    motor.setMotorDirection(MOTOR_DIRECTION_CW);
    lcdShowDirection();
    serialMessage(CMD_DIRECTION, commandString);
  }
  else if(commandString.equals(DIRECTION_CCW)) {
    motor.setMotorDirection(MOTOR_DIRECTION_CCW);
    lcdShowDirection();
    serialMessage(CMD_DIRECTION, commandString);
  }
  else if(commandString.equals(MOTOR_RAMP)) {
    motor.setMotorRamp(RAMP_ON);
    lcdShowRamp();
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(MOTOR_NORAMP)) {
    motor.setMotorRamp(RAMP_OFF);
    lcdShowRamp();
    serialMessage(CMD_MODE, commandString);
  }
  // =========================================================
  // Freewheeling mode
  // =========================================================
  else if(commandString.equals(FW_ACTIVE)) {
    motor.setMotorFreeWheeling(MOTOR_FW_ACTIVE);
    lcdShowFreeWheeling();
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(FW_PASSIVE)) {
    motor.setMotorFreeWheeling(MOTOR_FW_PASSIVE);
    lcdShowFreeWheeling();
    serialMessage(CMD_MODE, commandString);
  }
  // =========================================================
  // Duty cycle settings
  // =========================================================
  else if(commandString.equals(MANUAL_DC)) {
    motor.setMotorManualDC(MOTOR_MANUAL_DC);
    showDutyCycle();
    lcdShowDutyCycleManual();
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(AUTO_DC)) {
    motor.setMotorManualDC(MOTOR_AUTO_DC);
    showDutyCycle();
    lcdShowDutyCycleAuto();
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(MIN_DC)) {
    analogDutyCycle = ANALOG_DCMIN;
    motor.setMotorMinDC(inputAnalogDC);
    showDutyCycle();
    lcdShowDutyCycleMin();
    serialMessage(CMD_MODE, commandString);
  }
  else if(commandString.equals(MAX_DC)) {
    analogDutyCycle = ANALOG_DCMAX;
    motor.setMotorMaxDC(inputAnalogDC);
    showDutyCycle();
    lcdShowDutyCycleMax();
    serialMessage(CMD_MODE, commandString);
  }
  // =========================================================
  // Motor running
  // =========================================================
  
  else
    Serial << CMD_WRONGCMD << " '" << commandString << "'" << endl;
 }

// ***********************************************************
// LCD Dispay manager methods
// ***********************************************************

/**
 * Show the current motor settings if one is selected else
 * only show the setting display mode
 */
void showMotorSetting() {
  lcd.clear();
  lcdShowMotor();
  lcdShowPWM();
  lcdShowFreeWheeling();
  lcdShowRamp();
  lcdShowDirection();
}

//! Show setting mode of current motor (or all)
void lcdShowMotor() {
  lcd.setCursor(0, 0);
  lcd << "Set M";
  // Show the current motor settings
  if(motor.currentMotor > 0) {
    lcd << motor.currentMotor;    // A motor is selected
    // Check if motor is enabled
    if(motor.internalStatus[motor.currentMotor - 1].isEnabled)
      lcd << "e"; // motor is enabled
    else
      lcd << "d"; // Motor is disabled
  }
  else {
    // Check if motor is enabled
    if(motor.internalStatus[0].isEnabled)
      lcd << "*e"; // motor is enabled
    else
      lcd << "*d"; // Motor is disabled
  }

  // Show duty cycle if a motor is selected
    lcd.setCursor(4, 1);
  if(motor.currentMotor > 0)
    lcd << "(" << motor.internalStatus[motor.currentMotor - 1].minDC << "-" << motor.internalStatus[motor.currentMotor - 1].maxDC << ")";
}

//! Show duty cycle settings header
void showDutyCycle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd << "M";
  // Show the current motor settings
  if(motor.currentMotor > 0) {
    lcd << motor.currentMotor;    // A motor is selected
    
    // Check if motor is enabled
    if(motor.internalStatus[motor.currentMotor - 1].isEnabled)
      lcd << "e"; // motor is enabled
    else
      lcd << "d"; // Motor is disabled
  }
  else {
    lcd << "*e";   // Undetermined
  }
  lcd << " Duty Cycle";
}

//! Show the manual duty cycle setting
void lcdShowDutyCycleManual() {
  lcd.setCursor(0, 1);
  lcd << "set to manual";
}

//! Set duty cycle min value from analog input
void lcdShowDutyCycleMin() {
  lcd.setCursor(0, 1);
  lcd << "min value: ";
  if(inputAnalogDC < 100)
    lcd.print(" ");
  lcd << inputAnalogDC;
}

//! Set duty cycle Max value from analog input
void lcdShowDutyCycleMax() {
  lcd.setCursor(0, 1);
  lcd << "Max value: ";
  if(inputAnalogDC < 100)
    lcd.print(" ");
  lcd << inputAnalogDC;
}

//! Show the default duty cycle automatic range
void lcdShowDutyCycleAuto() {
  lcd.setCursor(0, 1);
  lcd << "min " << DUTYCYCLE_MIN << " Max " << DUTYCYCLE_MAX;
}

//! Show the set PwM channel
void lcdShowPWM() {
  String pwmNames[] = { " No", " 80", "100", "200" };
  lcd.setCursor(8, 0);
  if(motor.currentMotor > 0)
    lcd << "PWM" << pwmNames[motor.internalStatus[motor.currentMotor - 1].channelPWM];
  else
    lcd << "PWM" << pwmNames[motor.internalStatus[0].channelPWM];
}

//! Show the freewheeling mode
void lcdShowFreeWheeling() {
  boolean fw;
  
  if(motor.currentMotor > 0)
    fw = motor.internalStatus[motor.currentMotor - 1].freeWheeling;
  else
    fw = motor.internalStatus[0].freeWheeling;

  lcd.setCursor(0, 1);
  lcd << "Fw"; 
  if(fw)
    lcd << "A";
  else
    lcd << " ";
}

//! Show the acceleration setting
void lcdShowRamp() {
  boolean acc;
  
  if(motor.currentMotor > 0)
    acc = motor.internalStatus[motor.currentMotor - 1].useRamp;
  else
    acc = motor.internalStatus[0].useRamp;

  lcd.setCursor(15, 0);
  if(acc)
    lcd << ">";
  else
    lcd << ".";
}

//! Show the rotatin direction of the motor
void lcdShowDirection() {
  int dir;
  
  if(motor.currentMotor > 0)
    dir = motor.internalStatus[motor.currentMotor - 1].motorDirection;
  else
    dir = motor.internalStatus[0].motorDirection;

  lcd.setCursor(13, 1);
  if(dir == MOTOR_DIRECTION_CW)
    lcd << " CW";
  else
    lcd << "CCW";
}

