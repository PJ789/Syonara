# Syonara
Arduino SS Micro (Leonardo) firmware for a Razer Cynosa Lite or similar single zone RGB keyboard.

This firmware is designed for an SS Micro Arduino MCU board (based on the ATMega32u4, similar to a Leonardo).

The firmware suppports all keys, single zone RGB backlighting, Caps/Num/Shift lock LED functions.

It assumes the complete replacement of the original MCU with an SS Micro, a keyboard scanning circuit using two 4017 decade counters to scan the 20 matrix columns in tandem,  a 75HC165 shift register providing parallel to serial output from the matrix rows, a strip of WS2812 Neopixels to replace the original keyboard status LEDs, and a PWM amplifier circuit to power the RGB backlights.

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

A 74hc165 shift register is used to read the rows of the matrix, and convert into a serialized byte value.

The shift register output is read & decoded by the Syonara firmware. (An algorithm in the firmware ensures that there is no misread due to two columns being scanned at the same time).

### Circuit 2: Caps Lock, Num Lock, Scroll Lock, and Application LEDs circuit

A simple Neopixel strip replaces the original LEDs. A piece of insulating tape under the LEDs avoids shorts.

The Syonara firmware sets the correspondng LED colours for each status indication.

### Circuit 3: Keyboard RGB back light circuit

A simple amplifier circuit to drive the keyboard RGB backlight using transistors or MOSFETs. PWM is used to control brightness of each RGB line, allowing colour transition effects.

The Syonara firmware uses PWM to create RGB colour transition effects.

## MCU Pinouts

Shift register control;

 - Shift/load pin 2
 - Clock pin 3
 - Serial data pin 6

Backlight PWM Control;
 - Red (PWM) pin 9
 - Green (PWM) pin 10
 - Blue (PWM) pin 11

Decade counter control;

 - Counter 1 
   - Reset pin A1
   - Clock pin A2
 
 - Counter 2
   - Reset pin 15
   - Clock pin 16

Caps Lock, Num Lock, Scroll Lock Status LEDs;

 - Neopixel pin A0
   - Caps lock led: 0 
   - Unused led: 1 
   - Num lock led: 2
   - Unused led: 3
   - Scroll lock led: 4 
   - Unused led: 5 
   - Application led: 6 
   - Unused led: 7 
   - Power led: 8 

## Improvements?

Add a brightness control to the backlight circuit, perhaps using an analog input. Support for standard LED colour setting protocols.

