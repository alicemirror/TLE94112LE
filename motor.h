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
#define APP_TITLE "Infineon TLE94112LE Test Ver.1.0.1"

// System status (ID and description)
#define SYS_READY "Ready"       // System ready
#define SYS_RUN "Running"       // Filament in use
#define SYS_LOAD "Loading"         // Roll loaded
#define SYS_STARTED "Started"   // Application started
// Status IDs
#define STAT_NONE 0
#define STAT_READY 1
#define STAT_LOAD 2
#define STAT_RUN 3

//! For high current coupling half bridges together 1&2, 3&4
#undef _HIGHCURRENT
#undef _MOTORDEBUG

//! Avoid too many openload error messages when starting acceleration
//! by default it is ignored
#define _IGNORE_OPENLOAD

#define INVERT_DIRECTION_DELAY 300  ///< Delay in ms when the motor should invert direction
#define ACCELERATION_DELAY 5        ///< Delay between acceleration steps

#define DIRECTION_FEED 1    ///< Motor rotates to release filament
#define DIRECTION_LOAD 2    ///< Motor rotates to load filament

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

#endif
