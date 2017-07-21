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
#define CMD_WRONGCMD "wrong value "

// Direction control
#define DIRECTION_CW "cw"     ///< clockwise rotation
#define DIRECTION_CCW "ccw"   ///< counterclockwise rotation
#define MOTOR_RAMP "accel"  ///< Enable the acceleration when motor start
#define MOTOR_NORAMP "noaccel"  ///< Disable the acceleration when motor start

// Start/stop
#define MOTOR_START "start" ///< stop all
#define MOTOR_STOP "stop"   ///< stop all

// Motor enable flag
#define MOTOR_1 "m1"        ///< Enable/disable motor
#define MOTOR_2 "m2"        ///< Enable/disable motor
#define MOTOR_3 "m3"        ///< Enable/disable motor
#define MOTOR_4 "m4"        ///< Enable/disable motor
#define MOTOR_5 "m5"        ///< Enable/disable motor
#define MOTOR_6 "m6"        ///< Enable/disable motor
#define MOTOR_ALL "all"     ///< All motors enabled
#define MOTOR_NONE "none"   ///< All motors disabled

// PWM Frequency selector
#define PWM_0    "nopPWM" ///< No PWM
#define PWM_80   "80"     ///< PWM 80 Hz
#define PWM_100  "100"    ///<PWM 100 Hz
#define PWM_200  "200"    ///< PWM 200 Hz

// Freewheeling mode
#define FW_ACTIVE "fwactive"    ///< Fereewheeling active
#define FW_PASSIVE "fwpassive"  ///< Freewheeling passive

// Information commands
#define SHOW_CONF "conf"    ///< Dump the current settings

#endif
