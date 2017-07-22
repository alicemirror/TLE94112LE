# Infineon TLE94112LE Test
## Interactive test platform for the infineon TLE94112LE Shield for Arduino

![Workbench platform](images/IMG_20170722_080932.jpg)

## Design note
This if the software application created with the Arduino IDE 1.8.3 to run the
test platform.

This application is specific for the hardware building but can be easily modified
to run with different kind of brushed motors and other inductive components.

The test platform has been created to manage interactively most (almost all) of
the features and functions of the motor controller shield. This version
uses 6 brushed geared micro motors running at 6V max (power is set to 5.5V).

Warning: using and higher power for motors a the LCD backlight should be powered
bt the Arduino 5V to avoid damage.

![Test platform detail](images/IMG_20170720_231205.jpg)

### Functional components
The platform has a LCD display to interactively show the motor settings. Every
single motor can be set separately or all together. The TLE94112LE shield motors
are powered by a 5.5V regulated DC-DC small board while a potentiometer connected
to the Arduino channel A0 is used to dynamically modify the PWM duty cycle.

Please note that the duty cycle configuration is related to the PWM even also the
software permit setting different duty cycle values for every single motor it is
not working due the limitation of the board. The duty cycle setting should be
used selecting all the motors together. This issue will be updated in future
versions.

### MCU
![Test platform detail](images/IMG_20170529_162044.jpg)

This software is compatible with Arduino UNO R3, Genuino 101 and Arduino 101 but
the best performances are reached with the Infineon XMC1100 Boot kit Arduino 
compatible board.

## Commands
Excluding the potentiometer on the board all the controls are sent via terminal 
through a USB-to-serial connection. The available commands are listed below:

### Direction control
- __cw__ : clockwise rotation
- __ccw__ : counterclockwise rotation
- __accel__ : Enable the acceleration when motor start
- __noaccel__ : Disable the acceleration when motor start

### Action commands
- __start__ : start running
- __stop__ : stop all
- __reset__ : reset the system to the default

### Duty cycle
- __dcmanual__ : Set the duty cycle value depending on the pot
- __dcauto__ : Set the duty cycle current limits
- __dcmin__ : Set the min duty cycle value via pot
- __dcmax__ : Set the max duty cycle value via pot

### Motor select flag for settings
- __all__ : All motors selected and enabled
- __none__ : No motors disabled and disabled
- __m1__ : select motor
- __m2__ : select motor
- __m3__ : select motor
- __m4__ : select motor
- __m5__ : select motor
- __m6__ : select motor

### Motor enable
- __m1+__ : enable motor
- __m2+__ : enable motor
- __m3+__ : enable motor
- __m4+__ : enable motor
- __m5+__ : enable motor
- __m6+__ : enable motor

### PWM Frequency selector
- __noPWM__ : No PWM
- __80__ : PWM 80 Hz
- __100__ : PWM 100 Hz
- __200__ : PWM 200 Hz

### Freewheeling mode
- __fwactive__ : Fereewheeling active
- __fwpassive__ : Freewheeling passive

### Information commands
- __conf__ : Dump the current settings

Below the defaault settings table shown on the terminal after a _conf_ command.

> *********************************************************
>                 TLE94112LE Motors Configuration
> *********************************************************
> __________________________________________________________
> Motor|Enabled|Accel|Active FW|DC Min|DC Max|DC Man|Dir|PWM
> -----+-------+-----+---------+------+------+------+---+---
>  M1  |   No  |  No |   Yes   |  50  | 255  |   No | CW| No
> -----+-------+-----+---------+------+------+------+---+---
>  M2  |   No  |  No |   Yes   |  50  | 255  |   No | CW| No
> -----+-------+-----+---------+------+------+------+---+---
>  M3  |   No  |  No |   Yes   |  50  | 255  |   No | CW| No
> -----+-------+-----+---------+------+------+------+---+---
>  M4  |   No  |  No |   Yes   |  50  | 255  |   No | CW| No
> -----+-------+-----+---------+------+------+------+---+---
>  M5  |   No  |  No |   Yes   |  50  | 255  |   No | CW| No
> -----+-------+-----+---------+------+------+------+---+---
>  M6  |   No  |  No |   Yes   |  50  | 255  |   No | CW| No
> -----+-------+-----+---------+------+------+------+---+---


