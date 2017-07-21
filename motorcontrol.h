/**
 *  \file motorcontrol.h
 *  \brief Arduino class to control Infineon DC Motor Shield with TLE94112
 *  
 *  \author Enrico Miglino <balearicdynamics@gmail.com> \n
 *  Balearic Dynamics sl <www.balearicdynamics.com> SPAIN
 *  \date July 2017
 *  \version 1.0
 *  Licensed under GNU LGPL 3.0 
 */

#ifndef _MOTORCONTROL
#define _MOTORCONTROL

#include <TLE94112.h>
#include "motor.h"

/**
 * Internal status of a generic motor
 */
struct motorStatus {
  boolean useRamp;        ///< Use acceleration/deceleration cycle when starting
  uint8_t channelPWM;     ///< PWM channel for this motor
  boolean isEnabled;      ///< Motor enabled status
  boolean isRunning;      ///< Motor running status (should be enabled)
  boolean freeWheeling;   ///< Free wheeling active or passive
  int minDC;              ///< Min duty cycle value
  int maxDC;              ///< Max duty cycle value
  int accdelay;           ///< Delay in steps (ms) during the acceleration / deceleration ramp
  int motorDirection;     ///< Current motor direction
};

/**
 * \brief  Class that manages the TLE94112 Arduino shield
 * controlling the filament motor
 */
class MotorControl {
  public:
    /** 
     * \brief Initialization and motor settings 
     * 
     * The initialization method support two hardcoded modes, depending on the
     * kind of geared motor it is used.\n
     * In _HIGHCURRENT mode every motor uses two half bridges couple together for every 
     * pole if more than 0.9A is needed (< 0.18)\n
     * The standard usage mode is in low current mode with a single half bridge every motor pole
     */
    void begin(void);

    //! \brief stop the motor control
    void end(void);

    /**
     * \brief initialize the motor default settings and disable all the motors
     * Launched when the class is initialised.
     */
    void reset(void);

    //! Status of the motors parameters and settings
    motorStatus internalStatus[MAX_MOTORS];

    //! Current motor in use (setting parameters)
    int currentMotor;

    /**
     * \brief Set the desired PWM channel to the current motor if one
     * or to all motors
     * 
     * \param pwmCh The selected PWM channel
     */
    void setPWM(uint8_t pwmCh);

     /**
      * \brief Set the desired direction for the selected motor (or all)
      * 
      * \param dir The selected direction, CW or CCW
      */
    void setMotorDirection(int dir);

    /**
     * Enable or disable the acceleration/deceleration sequence when motor
     * start or invert direction
     * 
     * \param acc Acceleration flag
     */
    void setMotorRamp(boolean acc);

    /**
     * Enable or disable the freewheeling flag
     * 
     * \param fw Freewheeling flag
     */
    void setMotorFreeWheeling(boolean fw);

    /**
     * \brief Accelerates to the regime speed for filament release then 
     * keep the regime speed for the needed number of milliseconds
     * to release a lenght of filament then decelerate until motor stop
     * 
     * \note The feedExtruder() speed is slower than the normal filamentFeed()
     * method. The typical duration for this method should be FEED_EXTRUDER_DELAY
     * This method is uninterruptable as it uses internal delays
     * 
     * \param duration the numer of ms to feed at the regime speed
     */
    void rotateCW(long duration);
  
    /**
     * \brief Accelerates to the regime speed for filament release then 
     * keep the regime speed for the needed number of milliseconds
     * to release a lenght of filament then decelerate until motor stop
     * 
     * \note The filamentFeed() speed is faster than the normal feedExtruder()
     * method. This method is uninterruptable as it uses internal delays
     * 
     * \param duration the numer of ms to feed at the regime speed
     */
    void filamentFeed(long duration);

    /**
     * \brief Accelerates to the regime speed for filament release then 
     * keep the regime speed until the motor is stopped
     * 
     * \note This method is interruptable and doe not check for errors
     * internally. It runs indefinitely until a stop command is sent
     */
    void filamentContFeed(void);

    /**
     * \brief Accelerates to the regime speed for filament load then 
     * keep the regime speed for the needed number of milliseconds
     * to release a lenght of filament then decelerate until motor stop
     * 
     * \note The filamentLoad()is the complimentary method than filamentFeed(0
     * as it runs in the opposite direction
     * 
     * \param duration the numer of ms to feed at the regime speed
     */
    void filamentLoad(long duration);

    /**
     * \brief Accelerates to the regime speed for filament load then 
     * keep the regime speed until the motor is stopped
     * 
     * \note This method is interruptable and doe not check for errors
     * internally. It runs indefinitely until a stop command is sent
     */
    void filamentContLoad(void);

    /**
     * \brief Accelerates to the regime speed then 
     * keep the regime speed for the needed number of milliseconds
     * then decelerate until motor stop
     * 
     * \note This method is used by the uninterruptable methods feedExtruder().
     * filamentFeed() and filamentLoad(). It is a public method for convenience
     * but it is not expected to be used in the normal usage
     * 
     * \param minDC mnimumn duty cycle value
     * \param maxDC maximum duty cycle value
     * \param accdelay pause ms during the acceleration/deceleration loops
     * \param duration numer of ms at the regime speed
     */
    void motorRun(int minDC, int maxDC, int accdelay, long duration, int motorDirection);

    /**
     * \brief Accelerates to the regime speed then 
     * keep the motor running
     * 
     * \param minDC mnimumn duty cycle value
     * \param maxDC maximum duty cycle value
     * \param accdelay pause ms during the acceleration/deceleration loops
     */
    void motorStart(int minDC, int maxDC, int accdelay, int motorDirection);

    /**
     * \brief Brake the motor keelping the half bridges high
     */
    void motorBrake();

    /** 
     * \brief Show Current motors configuration
     */
     void showInfo(void);

    /**
     * Check if an error occured.
     * 
     * \note This method should be used for test the error condition only as it does not
     * 
     * \return true if tehre is an error
     */
    boolean tleCheckDiagnostic(void);

    /**
     * Check the error condition and detect the kind of error (if any) then reset it
     * 
     * \return The error string
     * \todo Check the harfbridge generating the specific error 
     */
    void tleDiagnostic(void);

};

#endif
