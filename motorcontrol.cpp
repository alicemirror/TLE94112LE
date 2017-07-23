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

  for(j = 0; j < MAX_MOTORS; j++) {
    internalStatus[j].useRamp = false;      // No acceleration
    internalStatus[j].channelPWM = tle94112.TLE_NOPWM; // PWM disabled on start
    internalStatus[j].isEnabled = false;    // Motors initially disabled
    internalStatus[j].isRunning = false;    // Not running (should be enabled)
    internalStatus[j].freeWheeling = true;  // Free wheeling active
    internalStatus[j].motorDirection = MOTOR_DIRECTION_CW;
  } // loop on the motors array

  for(j = 0; j < AVAIL_PWM_CHANNELS; j++) {
    dutyCyclePWM[j].minDC = DUTYCYCLE_MIN;  // Min duty cycle
    dutyCyclePWM[j].maxDC = DUTYCYCLE_MAX;  // Max duty cycle
    dutyCyclePWM[j].manDC = false;          // Duty cycle in auto mode
  } // loop on the PWM channels array

  resetHB();
  currentPWM = 0; // No PWM channels selected
  currentMotor = 0; // No motors selected
}

void MotorControl::resetHB(void) {
    // Set all the half bridges to disabled (motors stopped)
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

void MotorControl::setPWMManualDC(boolean dc) {
  if(currentPWM != 0) {
    // We are setting a specific PWM channel
    dutyCyclePWM[currentPWM - 1].manDC = dc;
  }
  else {
    int j;
    for (j = 0; j < AVAIL_PWM_CHANNELS; j++) {
      dutyCyclePWM[j].manDC = dc;
    }
  }
}

void MotorControl::setPWMMinDC(uint8_t dc) {
  if(currentPWM != 0) {
    // We are setting a specific PWM channel
    dutyCyclePWM[currentPWM - 1].minDC = dc;
  }
  else {
    int j;
    for (j = 0; j < AVAIL_PWM_CHANNELS; j++) {
      dutyCyclePWM[j].minDC = dc;
    }
  }
}

void MotorControl::setPWMMaxDC(uint8_t dc) {
  if(currentPWM != 0) {
    // We are setting a specific PWM channel
    dutyCyclePWM[currentPWM - 1].maxDC = dc;
  }
  else {
    int j;
    for (j = 0; j < AVAIL_PWM_CHANNELS; j++) {
      dutyCyclePWM[j].maxDC = dc;
    }
  }
}

//void MotorControl::motorRun(int minDC, int maxDC, int accdelay, long duration, int motorDirection) {
//  int j;
//
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
//  for(j = minDC; j <= maxDC; j++) {
//    // Update the speed
//    tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
//  //Check for error
//  if(tleCheckDiagnostic()) {
//    tleDiagnostic();
//  }
//    delay(accdelay);
//  }
//
//  // Wait for the requeste number of ms at the regime speed
//  tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, maxDC);
//  //Check for error
//  if(tleCheckDiagnostic()) {
//    tleDiagnostic();
//  }
//  delay(duration);
//
//  // Deceleration loop  
//  for(j = maxDC; j > minDC; j--) {
//    // Update the speed
//    tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
//  //Check for error
//  if(tleCheckDiagnostic()) {
//    tleDiagnostic();
//  }
//    delay(accdelay);
//  }
//}

//void MotorControl::motorStart(int minDC, int maxDC, int accdelay, int motorDirection) {
//  int j;
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
//
//  // Acceleration loop  
//  for(j = minDC; j <= maxDC; j++) {
//    // Update the speed
//    tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, j);
//    //Check for error
//    if(tleCheckDiagnostic()) {
//      tleDiagnostic();
//    }
//    delay(accdelay);
//  }
//  tle94112.configPWM(tle94112.TLE_PWM1, tle94112.TLE_FREQ200HZ, maxDC);
//}

//void MotorControl::motorBrake(void) {
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
//#ifdef _HIGHCURRENT
//  // High current configuration, uses HB1&2 + 3&4
//  tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
//  tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
//  tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
//  tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
//#else
//  // No high current mode, use only HB1 & 2
//  tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
//  tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_NOPWM);
//#endif
////Check for error
//  if(tleCheckDiagnostic()) {
//    tleDiagnostic();
//  }
//}

void MotorControl::motorConfigHB(void) {
  int j;

    for(j == 0; j < MAX_MOTORS; j++) {
      motorConfigHB(j);
    }
}

void MotorControl::motorConfigHB(int motor) {

  if(internalStatus[motor].isEnabled) {
    if(internalStatus[motor].motorDirection == MOTOR_DIRECTION_CW)
      motorConfigHBCW(motor);
    else
      motorConfigHBCCW(motor);
  }
}

void MotorControl::motorConfigHBCW(int motor) {
  int hb1;

  // Calculate the first half bridge depending on the motor ID
  // Note that HB are numbered base 1 while motor is index array base 0
  // and HB usage is different if high current mode is selected in the
  // preprocessor directive
  hb1 = calcHB1(motor + 1);

  // Select the half bridges to configure the motor.
  // Depending on the mode (normal = 1 + 1 HB per motor, high current = 2+2)
  // not all the half bridges can be the first (=hb1)
  switch(hb1) {
    // -------------------------------------------------
    // Motor 1 (in both modes)
    // -------------------------------------------------
    case 1: 
      #ifdef _HIGHCURRENT
      tle94112.configHB(tle94112.TLE_HB3,  tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #else
      tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #endif
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 2 (no high current mode)
    // -------------------------------------------------
    case 3: 
      tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 3 (Motor 2 in high current mode)
    // -------------------------------------------------
    case 5: 
      #ifdef _HIGHCURRENT
      tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #else
      tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #endif
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB5, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB5, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB5, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB5, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 4 (no high current mode)
    // -------------------------------------------------
    case 7:  
      tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 5 ((Motor 3 in high current mode)
    // -------------------------------------------------
    case 9: 
      #ifdef _HIGHCURRENT
      tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #else
      tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #endif
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB9, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB9, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB9, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB9, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 6 (no high current mode)
    // -------------------------------------------------
    case 11:
      tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
        break;
      }
    break;
  }
}

void MotorControl::motorConfigHBCCW(int motor) {
  int hb1;

  // Calculate the first half bridge depending on the motor ID
  // Note that HB are numbered base 1 while motor is index array base 0
  // and HB usage is different if high current mode is selected in the
  // preprocessor directive
  hb1 = calcHB1(motor + 1);

  // Select the half bridges to configure the motor.
  // Depending on the mode (normal = 1 + 1 HB per motor, high current = 2+2)
  // not all the half bridges can be the first (=hb1)
  switch(hb1) {
    // -------------------------------------------------
    // Motor 1 (in both modes)
    // -------------------------------------------------
    case 1:
      tle94112.configHB(tle94112.TLE_HB1, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #ifdef _HIGHCURRENT
      tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #endif
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB3,  tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM1:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB3,  tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM2:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB3,  tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM3:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB3,  tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB2, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 2 (no high current mode)
    // -------------------------------------------------
    case 3: 
      tle94112.configHB(tle94112.TLE_HB3, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB4, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 3 (Motor 2 in high current mode)
    // -------------------------------------------------
    case 5: 
      tle94112.configHB(tle94112.TLE_HB5, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #ifdef _HIGHCURRENT
      tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #endif
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM1:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM2:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
        case tle94112.TLE_PWM3:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #else
          tle94112.configHB(tle94112.TLE_HB6, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          #endif
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 4 (no high current mode)
    // -------------------------------------------------
    case 7:  
      tle94112.configHB(tle94112.TLE_HB7, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB8, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
        break;
      }
    break;
    // -------------------------------------------------
    // // Motor 5 ((Motor 3 in high current mode)
    // -------------------------------------------------
    case 9: 
      tle94112.configHB(tle94112.TLE_HB9, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #ifdef _HIGHCURRENT
      tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      #endif
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling );
          #else
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
        case tle94112.TLE_PWM1:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling );
          #else
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
        case tle94112.TLE_PWM2:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling );
          #else
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
        case tle94112.TLE_PWM3:
          #ifdef _HIGHCURRENT
          tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling );
          #else
          tle94112.configHB(tle94112.TLE_HB10, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling );
          #endif
        break;
      }
    break;
    // -------------------------------------------------
    // Motor 6 (no high current mode)
    // -------------------------------------------------
    case 11: 
      tle94112.configHB(tle94112.TLE_HB11, tle94112.TLE_LOW, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
      switch(internalStatus[motor].channelPWM){
        case tle94112.TLE_NOPWM:
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_NOPWM, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM1:
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_PWM1, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM2:
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_PWM2, (uint8_t)internalStatus[motor].freeWheeling);
        break;
        case tle94112.TLE_PWM3:
          tle94112.configHB(tle94112.TLE_HB12, tle94112.TLE_HIGH, tle94112.TLE_PWM3, (uint8_t)internalStatus[motor].freeWheeling);
        break;
      }
    break;
  }
}

boolean MotorControl:: tleCheckDiagnostic(void) {
  if(tle94112.getSysDiagnosis() == tle94112.TLE_STATUS_OK)
    return false;
  else
    return true;
}

void MotorControl::showInfo(void) {
  int j;
  // Motor table header
  Serial << INFO_MAIN_HEADER1 << endl << INFO_MOTORS_TITLE << endl << INFO_MAIN_HEADER1 << endl;
  Serial << INfO_TAB_HEADER2 << endl << INfO_TAB_HEADER1 << endl << INfO_TAB_HEADER2 << endl;
  // Build the motors settings table data
  for (j = 0; j < MAX_MOTORS; j++) {
    // #1 - Motor
    Serial << INFO_FIELD1A << (j + 1) << INFO_FIELD1B;
    // #2 - Enabled
    if(internalStatus[j].isEnabled)
      Serial << INFO_FIELD2Y;
    else
      Serial << INFO_FIELD2N;
    // #3 - Acceleration
    if(internalStatus[j].useRamp)
      Serial << INFO_FIELD3Y;
    else
      Serial << INFO_FIELD3N;
    // #4 - Active freewheeling
    if(internalStatus[j].freeWheeling)
      Serial << INFO_FIELD4Y;
    else
      Serial << INFO_FIELD4N;
    // #8 - Direction
    if(internalStatus[j].motorDirection == MOTOR_DIRECTION_CW)
      Serial << INFO_FIELD8A;
    else
      Serial << INFO_FIELD8B;
    // #9 - PWM
    switch(internalStatus[j].channelPWM) {
      case tle94112.TLE_NOPWM:
        Serial << INFO_FIELD9_NO;
      break;
      case tle94112.TLE_PWM1:
        Serial << INFO_FIELD9_80;
      break;
      case tle94112.TLE_PWM2:
        Serial << INFO_FIELD9_100;
      break;
      case tle94112.TLE_PWM3:
        Serial << INFO_FIELD9_200;
      break;
    }
    Serial << endl << INfO_TAB_HEADER2 << endl;
  }

  // PWM table header
  Serial << endl << INFO_MAIN_HEADER2 << endl << INFO_PWM_TITLE << endl << INFO_MAIN_HEADER2 << endl;
  Serial << INfO_TAB_HEADER4 << endl << INfO_TAB_HEADER3 << endl << INfO_TAB_HEADER4 << endl;
  // Build the pwm settings table data
  for (j = 0; j < AVAIL_PWM_CHANNELS; j++) {
    // #1 - PWM
    switch(j) {
      case 0:
        Serial << INFO_FIELD10_80;
      break;
      case 1:
        Serial << INFO_FIELD10_100;
      break;
      case 2:
        Serial << INFO_FIELD10_200;
      break;
    }
    // #2 - DC Min
    Serial << INFO_FIELD5_6A;
    if(dutyCyclePWM[j].minDC < 100)
      Serial << " ";
    Serial << dutyCyclePWM[j].minDC << INFO_FIELD5_6B;
    // #3 - DC Max
    Serial << INFO_FIELD5_6A;
    if(dutyCyclePWM[j].maxDC < 100)
      Serial << " ";
    Serial << dutyCyclePWM[j].maxDC << INFO_FIELD5_6B;
    // #4 - Manual DC
    if(dutyCyclePWM[j].manDC)
      Serial << INFO_FIELD7Y;
    else
      Serial << INFO_FIELD7N;
    Serial << endl << INfO_TAB_HEADER4 << endl;
  }
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




