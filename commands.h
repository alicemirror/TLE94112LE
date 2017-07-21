/**
 *  \file commands.H
 *  \brief Serial commands to control the behaviour of the system.
 *  All the commands are case sensitive
 *  
 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0
 *  Licensed under GNU LGPL 3.0
 */

#ifndef _COMMANDS
#define _COMMANDS

#undef _DEBUG_COMMANDS

// Execution notification
#define CMD_EXEC "executing "
#define CMD_NOCMD "unknown "
#define CMD_SET "setting "
#define CMD_MODE "set "
#define CMD_DIRECTION "Direction "
#define CMD_PWM "PWM: "
#define CMD_WRONGCMD "wrong command "

// Direction control
#define DIRECTION_CW "cw"     ///< clockwise rotation
#define DIRECTION_CCW "ccw"   ///< counterclockwise rotation
#define MOTOR_RAMP "accel"  ///< Enable the acceleration when motor start
#define MOTOR_NORAMP "noaccel"  ///< Disable the acceleration when motor start

// Start/stop
#define MOTOR_START "start" ///< stop all
#define MOTOR_STOP "stop"   ///< stop all

//Duty cycle
#define MANUAL_DC "dcmanual"    ///< Set the duty cycle value depending on the pot
#define AUTO_DC "dcauto"        ///< Set the duty cycle current limits
#define MIN_DC "dcmin"          ///< Set the min duty cycle value via pot
#define MAX_DC "dcmax"          ///< Set the max duty cycle value via pot

// Motor select flag for settings
#define MOTOR_ALL "all"     ///< All motors selected and enabled
#define MOTOR_NONE "none"   ///< No motors disabled and disabled
#define MOTOR_1 "m1"        ///< select motor
#define MOTOR_2 "m2"        ///< select motor
#define MOTOR_3 "m3"        ///< select motor
#define MOTOR_4 "m4"        ///< select motor
#define MOTOR_5 "m5"        ///< select motor
#define MOTOR_6 "m6"        ///< select motor

// Motor enable
#define EN_MOTOR_1 "m1+"        ///< enable motor
#define EN_MOTOR_2 "m2+"        ///< enable motor
#define EN_MOTOR_3 "m3+"        ///< enable motor
#define EN_MOTOR_4 "m4+"        ///< enable motor
#define EN_MOTOR_5 "m5+"        ///< enable motor
#define EN_MOTOR_6 "m6+"        ///< enable motor

// PWM Frequency selector
#define PWM_0    "noPWM" ///< No PWM
#define PWM_80   "80"     ///< PWM 80 Hz
#define PWM_100  "100"    ///<PWM 100 Hz
#define PWM_200  "200"    ///< PWM 200 Hz

// Freewheeling mode
#define FW_ACTIVE "fwactive"    ///< Fereewheeling active
#define FW_PASSIVE "fwpassive"  ///< Freewheeling passive

// Information commands
#define SHOW_CONF "conf"    ///< Dump the current settings

#endif
