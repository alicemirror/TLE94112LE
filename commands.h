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
#define CMD_WRONGCMD "wrong value "

// Running mode
#define MODE_AUTO "auto"     ///< Run in automatic mode
#define MODE_MANUAL "man"    ///< Run in manual mode

// Motor control
#define MOTOR_FEED "cw"             ///< clockwise rotation
#define MOTOR_PULL "ccw"            ///< counterclockwise rotation
#define MOTOR_STOP "stop"           ///< stop all
#define MOTOR_FEED_CONT "cwcont"    ///< continuous clockwise rotation
#define MOTOR_PULL_CONT "ccwcont"   ///< continuous counterclockwise rotation
#define MOTOR_RAMP "accel"          ///< Enable/disable the acceleration when motor start

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
#define PWM_80   "80"    ///< PWM 80 Hz
#define PWM_100  "100"   ///<PWM 100 Hz
#define PWM_200  "200"   ///< PWM 200 Hz

// Information commands
#define SHOW_CONF "conf"    ///< Dump the current settings

#endif
