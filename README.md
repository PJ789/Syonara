# Syonara
Arduino SS Micro (Leonardo) firmware for a Razer Cynosa Lite or similar single zone RGB keyboard.

This firmware is designed for an SS Micro Arduino MCU board (based on the ATMega32u4, similar to a Leonardo).

The firmware suppports all keys, single zone RGB backlighting, Caps/Num/Shift lock LED functions.

It assumes the complete replacement of the original MCU with an SS Micro, and a keyboard scanning circuit using two 4017 decade counters scanning the matrix columns in tandem, and a 75HC165 shift register providing parallel to serial output from the matrix rows.

HID keyboard functionality is implemented using the Arduino Keyboard library, with the following patches applied;-

arduino/ArduinoCore-avr #446
Adds support for Keyboard LED (Caps Lock, Scroll Lock, Num Lock) 
https://github.com/arduino/ArduinoCore-avr/pull/446

arduino-libraries/Keyboard
Added support for Keyboard Led status codes
https://github.com/arduino-libraries/Keyboard/pull/61

