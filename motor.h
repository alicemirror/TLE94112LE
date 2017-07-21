/* 
 *  \file motor.h
 *  \brief DC motor parameters for the Infineon DC motor controller Arduino shield
 *  TLE94112 from Infineon
 *  
 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0
 *  Licensed under GNU LGPL 3.0
 */

#ifndef _MOTOR
#define _MOTOR

//! Application title shown on startup
#define APP_TITLE "Infineon TLE94112LE Test Ver.1.0.4"

//! For high current coupling half bridges together 1&2, 3&4
#undef _HIGHCURRENT
#undef _MOTORDEBUG

//! Avoid too many openload error messages when starting acceleration
//! by default it is ignored
#define _IGNORE_OPENLOAD

#define INVERT_DIRECTION_DELAY 300  ///< Delay in ms when the motor should invert direction
#define ACCELERATION_DELAY 5        ///< Delay between acceleration steps

#define MOTOR_DIRECTION_CW 1      ///< Clockwise
#define MOTOR_DIRECTION_CCW 2     ///< Counterclockwise

#define MOTOR_FW_ACTIVE true       ///< Active freewheeling
#define MOTOR_FW_PASSIVE false     ///< Passive freewheeling

#define MOTOR_ENABLED 1
#define MOTOR_DISABLED 0

#define RAMP_ON true     ///< Acceleration enabled on start
#define RAMP_OFF false    ///< Acceleration disabled on start

#define MOTOR_PWM_ON 1  ///< Motor run with PWM
#define MOTOR_PWM_OFF 0 ///< Motor runs without PWM

#define DUTYCYCLE_MIN 50    ///< Minimum duty cycle for motor start. Depends on motor characteristics
#define DUTYCYCLE_MAX 255   ///< Maximum duty cycle

/**
 * When _HIGHCURRENT is set every motor needs 2+2 half bridges to double the needed power
 */
#undef _HIGHCURRENT

#ifdef _HIGHCURRENT
  //! In high current mode every pole of the motors is connected to two half bridges
  #define MAX_MOTORS 3
  //! Calculate the first half bridge number depending
  //! on the motor number
  #define calcHB1(x) ((x - 1) * 4 + 1)
  //! Calculate the second half bridge number depending
  //! on the motor number
  #define calcHB2(x) ((x - 1) * 4 + 3)
#else
  //! In normal current mode every motor uses two half bridges
  #define MAX_MOTORS 6
  //! Calculate the first half bridge number depending
  //! on the motor number
  #define calcHB1(x) ((x - 1) * 2 + 1)
  //! Calculate the first half bridge number depending
  //! on the motor number
  #define calcHB2(x) ((x - 1) * 2 + 2)
#endif

//! Error title
#define TLE_ERROR_MSG "TLE94112 Error"
#define TLE_NOERROR "No Error"
#define TLE_SPIERROR "SPI"
#define TLE_LOADERROR "Open"
#define TLE_UNDERVOLTAGE "Under V"
#define TLE_OVERVOLTAGE "Over V"
#define TLE_POWERONRESET "Pwr rst" 
#define TLE_TEMPSHUTDOWN "Temp shutdown"
#define TLE_TEMPWARNING "Warn high temp"

// ======================================================================
//        LCD String
// ======================================================================

//! Application title shown on startup
#define L_APP_NAME1 "Infineon test"
#define L_APP_NAME2 "TLE94112 Shield"


#endif
