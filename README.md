# Syonara
Arduino SS Micro (Leonardo) firmware for a Razer Cynosa Lite or similar single zone RGB keyboard.

This firmware is designed for an SS Micro Arduino MCU board (based on the ATMega32u4, similar to a Leonardo).

The firmware suppports all keys, single zone RGB backlighting, Caps/Num/Shift lock LED functions.

It assumes the complete replacement of the original MCU with an SS Micro, and a keyboard scanning circuit using two 4017 decade counters scanning the matrix columns in tandem, and a 75HC165 shift register providing parallel to serial output from the matrix rows.

## Dependencieis

HID keyboard functionality is implemented using the Arduino Keyboard library, with the following patches applied;-

arduino/ArduinoCore-avr #446
Adds support for Keyboard LED (Caps Lock, Scroll Lock, Num Lock) 
https://github.com/arduino/ArduinoCore-avr/pull/446

arduino-libraries/Keyboard
Added support for Keyboard Led status codes
https://github.com/arduino-libraries/Keyboard/pull/61

## Electronics

The code assumes three circuits are assembled:

### Circuit 1: keyboard scanning circuit

Two 4017 decade counters are used to iterate over the 20 keyboard matrix columns in tandem.

A74hc165 shift register is used to read the rows of the matrix, and convert into a serialized byte value, which is read & decoded. (An algorithm in the firmware ensures that there is no misread due to two columns being scanned at the same time).

### Circuit 2: Caps Lock, Num Lock, Scroll Lock, and Application LEDs circuit

A simple LED current limiting circuit for the LED indicators. Simple high/low switching is used to turn the LEDs on/off as keyboard LED status is changed.

### Circuit 3: Keyboard RGB back light circuit

A simple amplifier circuit tto drive the keyboard RGB backlight using transistors or MOSFETs. PWM is used to control brightness of each RGB line.

