# Syonara
Arduino SS Micro (Leonardo) firmware for a Razer Cynosa Lite, AK5000, or similar single zone RGB keyboard.

This firmware is designed for an SS Micro Arduino MCU board (based on the ATMega32u4, similar to a Leonardo or Pro Micro).

The firmware suppports all keys, single zone RGB backlighting, Caps/Num/Shift lock Neopixel LED functions.

It assumes the complete replacement of the original MCU circuit with an SS Micro, a keyboard scanning circuit using two 4017 decade counters to scan the 20 matrix columns in tandem, a 74HC165 shift register providing parallel to serial output from the matrix rows, a strip of WS2812 Neopixels to replace the original keyboard status LEDs, and a PWM amplifier circuit to power the RGB backlights.

## Motivation - Why do this to a working keyboard?

 - Open source hardware gives you _complete_ control over the function of your keyboard
 - Better security; you have access to all the source
 - Better performance; code is carefully optimised for nanoseconds level keypress detection & signalling latency
 - Better functionality; add custom keyboard functions, perhaps for gaming or application use, without using proprietary macro applications
 - Eliminates Razer's original bloatware and unnecessary keyboard 'drivers'
 - Adapt the scroll lock key to do something useful :)

## Features

 - Full keyboard function
 - Very low latency operation (330Hz scan rate with -O3 [compiler optimisation](README.md#compiler-optimisation) setting in debug, see notes below)
 - Backlight colour change effects
 - Backlight switches to red/blue/green to indicate prominently when caps lock/num lock/scroll lock are enabled
 - Keyboard status LEDs are replaced by Neopixels
 - Keyboard flickers when keys are pressed to confirm activation
 - Status LED indicates when keystrokes are being sent

## Dependencies

HID keyboard functionality is implemented using the Arduino Keyboard library, with the following patches applied;-

arduino/ArduinoCore-avr #446
Adds support for Keyboard LED (Caps Lock, Scroll Lock, Num Lock) 
https://github.com/arduino/ArduinoCore-avr/pull/446

arduino-libraries/Keyboard
Added support for Keyboard Led status codes
https://github.com/arduino-libraries/Keyboard/pull/61

## Electronics

See also [Bill of Materials](BOM.md) and [Circuit Diagram](README.md#circuit-diagram)

The code requires three circuits are assembled:

### Circuit 1: keyboard scanning circuit

Two 4017 decade counters are used to iterate over the 20 keyboard matrix columns in tandem.

A 74hc165 shift register is used to read the 8 rows of the matrix, and convert the output into a serialized byte value.

The shift register output is read & decoded by the Syonara firmware. (An algorithm in the firmware ensures that there is no misread due to two columns being scanned at the same time).

The scanning circuit is 'hot-wired' onto the original (unused) circuit board, using matrix column/row test points as a solder pads.

### Circuit 2: Caps Lock, Num Lock, Scroll Lock, and Application LEDs circuit

A simple 9 Neopixel strip (taken from a 144 pixels/meter strip) replaces the original LEDs, and is placed on top of the original (unused) keyboard circuit. A piece of insulating tape under the Neopixel strip avoids shorts.

The Syonara firmware sets the correspondng LED colours for each status indication.

### Circuit 3: Keyboard RGB back light circuit

A simple amplifier circuit to drive the keyboard RGB backlight using NPN transistors or MOSFETs. PWM is used to control brightness of each RGB line, allowing colour transition effects.

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

## Circuit Diagram

See [Sayonara Circuit Diagram](Syonara%20Circuit%20Diagram.png)
![Sayonara Circuit Diagram](Syonara%20Circuit%20Diagram.png)

## Compiler Optimisation

Enabling the Arduino GCC -O3 compiler optimisation gives a useful 10% boost to the kwyboard scan rate. To make this change, find platform.txt (see <install folder>\hardware\arduino\avr) and modify the following lines before uploading the sketch;-

> compiler.c.flags=-c -g **-O3** {compiler.warning_flags} -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects
> compiler.c.elf.flags={compiler.warning_flags} **-O3** -g -flto -fuse-linker-plugin -Wl,--gc-sections
> compiler.cpp.flags=-c -g **-O3** {compiler.warning_flags} -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto

## Design Improvements?

- Add a brightness control to the backlight circuit, perhaps using an analog input. 
- Support for standard LED colour setting protocols.
- The circuit design counts over 20 matrix columns, supplying power to each column via two decade counters, then reading the output on the 8 matrix rows as a byte. Because of the keyboard membrane design, this can create false key presses if two keys on the same row/different columns are pressed at the same time (causing power to be fed to a second column) AND another key is simultaneously pressed on either column. The likelihood of this happening is reasonably low; it could be avoided by better keyboard membrane design (requires a new keyboard membrane).
 - Replace the backlight circuit with _moar_ Neopixels
 - In a perfect world, with hindsight, I would have put the decade counter clock/reset pins on the same AVR port, so both could be set at once (saving ~62ns). 
