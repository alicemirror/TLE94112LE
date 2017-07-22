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
- cw : clockwise rotation
- ccw : counterclockwise rotation
- accel : Enable the acceleration when motor start
- noaccel : Disable the acceleration when motor start

### Action commands
- start : start running
- stop : stop all
- reset : reset the system to the default

### Duty cycle
- dcmanual : Set the duty cycle value depending on the pot
- dcauto : Set the duty cycle current limits
- dcmin : Set the min duty cycle value via pot
- dcmax : Set the max duty cycle value via pot

### Motor select flag for settings
- all : All motors selected and enabled
- none : No motors disabled and disabled
- m1 : select motor
- m2 : select motor
- m3 : select motor
- m4 : select motor
- m5 : select motor
- m6 : select motor

### Motor enable
- m1+ : enable motor
- m2+ : enable motor
- m3+ : enable motor
- m4+ : enable motor
- m5+ : enable motor
- m6+ : enable motor

### PWM Frequency selector
- noPWM : No PWM
- 80 : PWM 80 Hz
- 100 : PWM 100 Hz
- 200 : PWM 200 Hz

### Freewheeling mode
- fwactive : Fereewheeling active
- fwpassive : Freewheeling passive

### Information commands
- conf : Dump the current settings



