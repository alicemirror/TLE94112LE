/** 
 *  \file motorcontrol.cpp
 *  \brief This file defines functions and predefined instances from motorcontrol.h
 *  
 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0 Release Candidate
 *  Licensed under GNU LGPL 3.0
 */

#include "motorcontrol.h"

void MotorControl::begin(void) {
  // enable tle94112
  tle94112.begin();
  
  reset();
}

void MotorControl::end(void) {
  tle94112.end();
}

void MotorControl::reset() {
  int j;

  // Loop on all the available motors
  for(j = 0; j < MAX_MOTORS; j++) {
    internalStatus[j].useRamp = false;      // No acceleration
    internalStatus[j].channelPWM = tle94112.TLE_NOPWM;        // PWM disabled on start
    internalStatus[j].isEnabled = false;    // Motors initially disabled
    internalStatus[j].isRunning = false;    // Not running (should be enabled)
    internalStatus[j].freeWheeling = true;  // Free wheeling active
    internalStatus[j].minDC = DUTYCYCLE_MIN;  // Min duty cycle
    internalStatus[j].maxDC = DUTYCYCLE_MAX;  // Max duty cycle
    internalStatus[j].manDC = false;          // Duty cycle in auto mode
    internalStatus[j].motorDirection = MOTOR_DIRECTION_CW;
    // Configure the half bridges in no active state
  } // loop on the motors array

  // Preset all the half bridges to disabled
  tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB5, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB9, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_FLOATING, tle94112.TLE_NOPWM);

  currentMotor = 0; // No motors selected
}

void MotorControl::setPWM(uint8_t pwmCh) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].channelPWM = pwmCh;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].channelPWM = pwmCh;
    }
  }
}

void MotorControl::setMotorDirection(int dir) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].motorDirection = dir;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].motorDirection = dir;
    }
  }
}

void MotorControl::setMotorRamp(boolean acc) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].useRamp = acc;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].useRamp = acc;
    }
  }
}

void MotorControl::setMotorFreeWheeling(boolean fw) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].freeWheeling = fw;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].freeWheeling = fw;
    }
  }
}

void MotorControl::setMotorManualDC(boolean dc) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].manDC = dc;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].manDC = dc;
    }
  }
}

void MotorControl::setMotorMinDC(uint8_t dc) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].minDC = dc;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].minDC = dc;
    }
  }
}

void MotorControl::setMotorMaxDC(uint8_t dc) {
  if(currentMotor != 0) {
    // We are setting a specific mmotor
    internalStatus[currentMotor - 1].maxDC = dc;
  }
  else {
    int j;
    for (j = 0; j < MAX_MOTORS; j++) {
      internalStatus[j].maxDC = dc;
    }
  }
}

void MotorControl::rotateCW(long duration) {
//  motorRun(DC_MIN_EXTRUDER, DC_MAX_EXTRUDER, ACCELERATION_DELAY, duration, DIRECTION_FEED);
  motorBrake();
}

void MotorControl::filamentFeed(long duration) {
//  motorRun(DC_MIN_MANUAL_FFED, DC_MAX_MANUAL_FFED, ACCELERATION_DELAY, duration, DIRECTION_FEED);
  motorBrake();
}

void MotorControl::filamentContFeed(void) {
//  motorStart(DC_MIN_MANUAL_FFED, DC_MAX_MANUAL_FFED, ACCELERATION_DELAY, DIRECTION_FEED);
}

void MotorControl::filamentLoad(long duration) {
//  motorRun(DC_MIN_MANUAL_LOAD, DC_MAX_MANUAL_LOAD, ACCELERATION_DELAY, duration, DIRECTION_LOAD);
  motorBrake();
}

void MotorControl::filamentContLoad(void) {
//  motorStart(DC_MIN_MANUAL_FFED, DC_MAX_MANUAL_FFED, ACCELERATION_DELAY, DIRECTION_LOAD);
}

void MotorControl::motorRun(int minDC, int maxDC, int accdelay, long duration, int motorDirection) {
  int j;

  // If the motor is already running it is stopped before starting again
//  if(internalStatus.isRunning)
//    motorBrake();
  // Check for the direction
//  if(motorDirection == DIRECTION_FEED) {
//#ifdef _HIGHCURRENT
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//#else
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//#endif
//  }
//  else {
//#ifdef _HIGHCURRENT
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//#else
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//#endif
//  }

  // Acceleration loop  
  for(j = minDC; j <= maxDC; j++) {
    // Update the speed
    tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
  //Check for error
  if(tleCheckDiagnostic()) {
    tleDiagnostic();
  }
    delay(accdelay);
  }

  // Wait for the requeste number of ms at the regime speed
  tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, maxDC);
  //Check for error
  if(tleCheckDiagnostic()) {
    tleDiagnostic();
  }
  delay(duration);

  // Deceleration loop  
  for(j = maxDC; j > minDC; j--) {
    // Update the speed
    tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
  //Check for error
  if(tleCheckDiagnostic()) {
    tleDiagnostic();
  }
    delay(accdelay);
  }
}

void MotorControl::motorStart(int minDC, int maxDC, int accdelay, int motorDirection) {
  int j;
  // If the motor is already running it is stopped before starting again
//  if(internalStatus.isRunning)
//    motorBrake();
//  // Set the motor status
//  internalStatus.isRunning = true;
//  internalStatus.minDC = minDC;
//  internalStatus.maxDC = maxDC;
//  internalStatus.accdelay = accdelay;
//  internalStatus.motorDirection = motorDirection;

  // Check for the direction
//  if(motorDirection == DIRECTION_FEED) {
//#ifdef _HIGHCURRENT
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//#else
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//#endif
//  }
//  else {
//#ifdef _HIGHCURRENT
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//    tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//#else
//    tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_LOW, tle94112.TLE_NOPWM);
//    tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM1);
//#endif
//  }

  // Acceleration loop  
  for(j = minDC; j <= maxDC; j++) {
    // Update the speed
    tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
    //Check for error
    if(tleCheckDiagnostic()) {
      tleDiagnostic();
    }
    delay(accdelay);
  }
  tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, maxDC);
}

void MotorControl::motorBrake(void) {
  // If motor is running then it decelerates before stop
//  if(internalStatus.isRunning) {
//    int j;
//    // Deceleration loop  
//    for(j = internalStatus.maxDC; j > internalStatus.minDC; j--) {
//      // Update the speed
//      tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
//    //Check for error
//    if(tleCheckDiagnostic()) {
//      tleDiagnostic();
//    }
//      delay(internalStatus.accdelay);
//    }
//    // Update the motor status
////    internalStatus.isRunning = false;
//  }
#ifdef _HIGHCURRENT
  // High current configuration, uses HB1&2 + 3&4
  tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
#else
  // No high current mode, use only HB1 & 2
  tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
  tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
#endif
//Check for error
  if(tleCheckDiagnostic()) {
    tleDiagnostic();
  }
}

boolean MotorControl:: tleCheckDiagnostic(void) {
  if(tle94112.getSysDiagnosis() == tle94112.TLE_STATUS_OK)
    return false;
  else
    return true;
}

void MotorControl::showInfo(void) {
  Serial.println("Info");  
}

void MotorControl::tleDiagnostic() {
  int diagnosis = tle94112.getSysDiagnosis();

  if(diagnosis == tle94112.TLE_STATUS_OK) {
    Serial.println(TLE_NOERROR);
  } // No errors
  else {
    // Open load error can be ignored
    if(tle94112.getSysDiagnosis(tle94112.TLE_LOAD_ERROR)) {
#ifndef _IGNORE_OPENLOAD
      Serial.println(TLE_ERROR_MSG);
      Serial.println(TLE_LOADERROR);
      Serial.println("");
#endif
    } // Open load error
    else {
      Serial.println(TLE_ERROR_MSG);
      if(tle94112.getSysDiagnosis(tle94112.TLE_SPI_ERROR)) {
        Serial.println(TLE_SPIERROR);
      }
      if(tle94112.getSysDiagnosis(tle94112.TLE_UNDER_VOLTAGE)) {
        Serial.println(TLE_UNDERVOLTAGE);
      }
      if(tle94112.getSysDiagnosis(tle94112.TLE_OVER_VOLTAGE)) {
        Serial.println(TLE_OVERVOLTAGE);
      }
      if(tle94112.getSysDiagnosis(tle94112.TLE_POWER_ON_RESET)) {
        Serial.println(TLE_POWERONRESET);
      }
      if(tle94112.getSysDiagnosis(tle94112.TLE_TEMP_SHUTDOWN)) {
        Serial.println(TLE_TEMPSHUTDOWN);
      }
      if(tle94112.getSysDiagnosis(tle94112.TLE_TEMP_WARNING)) {
        Serial.println(TLE_TEMPWARNING);
      }
      Serial.println("");
    } // Any other error
    // Clear all possible error conditions        
    tle94112.clearErrors();
  } // Error condition
}




