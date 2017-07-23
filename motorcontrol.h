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

#include <Streaming.h>
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
  int motorDirection;     ///< Current motor direction
};

/**
 * PWM duty cycle settings. All motors using the same
 * PWM channel will be affected by the same settings
 */
struct pwmStatus {
  uint8_t minDC;          ///< Min duty cycle value
  uint8_t maxDC;          ///< Max duty cycle value
  boolean manDC;          ///< Manual duty cycle flag
};

/**
 * \brief  Class that manages the TLE94112 Arduino shield
 * controlling the filament motor
 */
class MotorControl {
  public:

    //! Current motor in use (setting parameters)
    int currentMotor;
    //! Current PWM channel (setting parameters)
    int currentPWM;
    //! Status of the motors parameters and settings
    motorStatus internalStatus[MAX_MOTORS];
    //! Status of the PWM duty cycle
    pwmStatus dutyCyclePWM[AVAIL_PWM_CHANNELS];

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

    /**
     * Reset all the half bridges immediately stopping the motors
     */
    void resetHB(void);

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
     * \brief Set the state flag for duty cycle mode. 
     * 
     * If set to manual the target value (max) is read 
     * from the analog input (pot) else it
     * is set following the internal parameters values.
     * This method applies to any iof the three available
     * PWM channels
     * 
     * \param dc true = manual duty cycle while runnning, 
     * false = use the preset values
     */
    void setPWMManualDC(boolean dc);

    /**
     * \brief Assign a user defined value as min duty cycle
     * 
     * This method applies to any of the three available
     * PWM channels
     * 
     * \param dc Duty cycle minimum value for the selected PWM channel
     */
    void setPWMMinDC(uint8_t dc);

    /**
     * \brief Assign a user defined value as Max duty cycle
     * 
     * This method applies to any of the three available
     * PWM channels
     * 
     * \param dc Duty cycle maximum value for the selected PWM channel
     */
    void setPWMMaxDC(uint8_t dc);

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
//    void motorRun(int minDC, int maxDC, int accdelay, long duration, int motorDirection);

    /**
     * \brief Accelerates to the regime speed then 
     * keep the motor running
     * 
     * \param minDC mnimumn duty cycle value
     * \param maxDC maximum duty cycle value
     * \param accdelay pause ms during the acceleration/deceleration loops
     */
//    void motorStart(int minDC, int maxDC, int accdelay, int motorDirection);

    /**
     * \brief Brake the motor keelping the half bridges high
     */
//    void motorBrake();

    /**
     * \brief Configure the halfbridges of all the motors. 
     * 
     * This method should be called during start global command.
     * The method loop on all motors and call recursively the same
     * polymorphic method that exectues the real setup for the motor
     * based on the direction.
     */
    void motorConfigHB(void);

    /**
     * \brief Configure the halfbridges of the specified motor. 
     * 
     * This method should be called to setup a single motor.
     * If the selected motor is not enabled the command has no effect else
     * it calls the motorConfigHBCW() or motorConfigHBCW() according with the
     * motor selected direction.
     * 
     * \param motor The motor ID to be configured (base 0)
     */
    void motorConfigHB(int motor);

    /**
     * \brief Configure the halfbridges of the specified motor, clowckwise direction
     * 
     * Thie method setup the HB the specified motor according with
     * its currently settings and parameters configured in the internalStatus[] array
     * 
     * \param motor The motor ID to be configured (base 0)
     */
    void motorConfigHBCW(int motor);

    /**
     * \brief Configure the halfbridges of the specified motor, counterclowckwise direction
     * 
     * Thie method setup the HB the specified motor according with
     * its currently settings and parameters configured in the internalStatus[] array
     * 
     * \param motor The motor ID to be configured (base 0)
     */
    void motorConfigHBCCW(int motor);

    /** 
     * \brief Show Current motors configuration in a table and the PWM settings on
     * another to the serial terminal
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
