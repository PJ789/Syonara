# Syonara
Arduino SS Micro (Leonardo) firmware for a Razer Cynosa Lite, AK5000, or similar single zone RGB keyboard.

This firmware is designed for an SS Micro Arduino MCU board (based on the ATMega32u4, similar to a Leonardo, Pro Micro, or Teensy 2.0).

The firmware suppports all keys, single zone RGB backlighting, Caps/Num/Shift lock Neopixel LED functions.

It assumes the complete replacement of the original MCU circuit with an SS Micro, a keyboard scanning circuit using two 4017 decade counters to scan the 20 matrix columns in tandem, a 74HC165 shift register providing parallel to serial output from the matrix rows, a strip of WS2812 Neopixels to replace the original keyboard status LEDs, and a PWM amplifier circuit to power the RGB backlights.

## Motivation - Why do this to a working keyboard?

 - Open source hardware gives you _complete_ control over the function of your keyboard
 - Better security; you have access to all the source
 - Better performance; code is carefully optimised for millisecond level keypress detection & signalling latency
 - Better functionality; add custom keyboard functions, perhaps for gaming or application use, without using proprietary macro applications
 - Eliminates Razer's original Synapse bloatware and unnecessary keyboard 'drivers'
 - Adapt the fn & scroll lock key to do something useful :)

## Features

 - Full keyboard function
 - Very low latency 1ms resolution (752Hz scan rate with -O3 [compiler optimisation](README.md#compiler-optimisation), see notes below and [performance measurements](README.md#performance-measurements))
 - Backlight colour change effects with ultra low performance impact
 - Backlight switches to red/blue/green to indicate prominently when caps lock/num lock/scroll lock are enabled
 - Keyboard status LEDs are replaced by Neopixels
 - Keyboard flickers when keys are pressed to confirm activation
 - Status LED indicates when keystrokes are being sent

## Dependencies

HID keyboard functionality is implemented using the Arduino Keyboard library, with the following patches applied;-

 - arduino/ArduinoCore-avr #446  
Adds support for Keyboard LED (Caps Lock, Scroll Lock, Num Lock) 
https://github.com/arduino/ArduinoCore-avr/pull/446

 - arduino-libraries/Keyboard  
Added support for Keyboard Led status codes
https://github.com/arduino-libraries/Keyboard/pull/61

## Electronics

See also [Bill of Materials](BOM.md) and [Circuit Diagram](README.md#circuit-diagram). See also [Design Improvements](README.md#design-improvements) for potentially _better ways_ to implement a circuit if you don't mind making code changes.

The code requires three circuits are assembled:

1. [Keyboard Scanning Circuit](README.md#circuit-1-keyboard-scanning-circuit)
2. [Caps Lock, Num Lock, Scroll Lock, and Application LEDs Circuit](README.md#circuit-2-caps-lock-num-lock-scroll-lock-and-application-leds-circuit)
3. [Keyboard RGB Back Light Circuit](README.md#circuit-3-keyboard-rgb-back-light-circuit)

### Circuit 1: Keyboard Scanning Circuit

Two 4017 decade counters are used to iterate over the 20 keyboard matrix columns in tandem.

A 74hc165 shift register is used to read the 8 rows of the matrix, and convert the output into a serialized byte value.

The shift register output is read & decoded by the Syonara firmware. (An algorithm in the firmware ensures that there is no misread due to two columns being scanned at the same time).

The scanning circuit is 'hot-wired' onto the original (unused) circuit board, using matrix column/row test points as solder pads.

### Circuit 2: Caps Lock, Num Lock, Scroll Lock, and Application LEDs Circuit

A simple 9 Neopixel strip (taken from a 144 pixels/meter strip) replaces the original LEDs, and is placed on top of the original (unused) keyboard LED circuit. A piece of insulating tape under the Neopixel strip avoids shorts.

The Syonara firmware sets the correspondng LED colours for each status indication.

### Circuit 3: Keyboard RGB Back Light Circuit

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

- Reset Counter 1 & 2
   - Reset pin 15

- Counter 1 
   - Clock pin 14
 
 - Counter 2
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

See [Syonara Circuit Diagram](Syonara%20Circuit%20Diagram.png)
![Syonara Circuit Diagram](Syonara%20Circuit%20Diagram.png)

## Compiler Optimisation

Enabling the Arduino GCC -O3 compiler optimisation gives a useful 10% boost to the keyboard scan rate. To make this change, find platform.txt (see <install folder>\hardware\arduino\avr) and modify the following lines before uploading the sketch;-

> **compiler.c.flags**=-c -g **-O3** {compiler.warning_flags} -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects  
> **compiler.c.elf.flags**={compiler.warning_flags} **-O3** -g -flto -fuse-linker-plugin -Wl,--gc-sections  
> **compiler.cpp.flags**=-c -g **-O3** {compiler.warning_flags} -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto  

## Performance Measurements

 - Scan rate without key press: 752Hz
 - Scan rate during key press:  340Hz
 - Key press -> initial key press detection; 0.0013s (1.3ms)
 - Key press detection -> key press decode; typical 0.0025-0.005s (2.5ms-5.0ms)
 - Key press decode -> key press send; 0.001s (1.0ms)
 - Debounce after key press: 5ms delay (200Hz)

## Design Improvements?

1. Add a brightness control to the backlight circuit, perhaps using an analog input. 
2. Support for standard LED colour setting protocols.
3. The circuit design counts over 20 matrix columns, supplying power to each column via two decade counters, then reading the output on the 8 matrix rows as a byte. Because of the keyboard membrane design, this can create false key presses if two keys on the same row/different columns are pressed at the same time (causing power to be fed to a second column) AND another key is simultaneously pressed on either column. The likelihood of this happening is reasonably low; it could be avoided by better keyboard membrane design (requires a new keyboard membrane).
4. Replace the backlight circuit with _moar_ Neopixels
5. ~~Put the two decade counter clock/reset pins on the same AVR port, so both could be reset or clocked in one operation (saving 62ns). The decade counters are never reset independently of one another. (see also [Syonara circuit diagram V2](design_ideas/Syonara%20Circuit%20Diagram%20V2.png) for an illustration, and idea #10).~~ _done 2022-01-07, designs updated_
6. Implement a per column, or row/column, debounce algorithm
7. Use a third decade counter in a cascade, so to avoid the need to test for a conflict between tandem decade counters. Requires one more 4017 decade counter ic, a bigger circuit board, and some Syonara firmware code changes. However, this could also slow the initial key press -> key press detection time period (because a much slower sequential scan would be needed using the cascaded decade counters).
8. Alternatively, use a second 74HC165 shift register... and hack about with the keyboard membrane to separate columns 1-10 on one shift register, and 11-20 on the other shift register.  Requires one more 74HC165 shift register ic, a bigger circuit board, keyboard membrane hacks (cuts & joints to the tracks), and some Syonara firmware code changes. However, this would accelerate the initial key press -> key press decode time period (because both shift registers could be read in tandem, and no complex decoding is needed). It might also allow debounce to be reduced for one or other shift register.
9. A RPi Pico (26 multi-function GPIO pins), Teensy 2.0 (with 25 I/O pins), or Pro Mico (with 18 I/O pins) could eliminate the need for a shift register. To eliminate the decade counters would require a board with 23 I/O pins (20 to scan columns, 3 needed to drive the row shift register). To eliminate both decade counter & shift register would need 28 (20 rows x 8 columns).
10. ~~The reset for the two decade counters could be wired to a single pin, since they are never individually reset by the Syonara firmware code. This would then free up a pin for other uses, and accelerate the reset operation (see also [Syonara circuit diagram V2](design_ideas/Syonara%20Circuit%20Diagram%20V2.png) for an illustration, and idea #5).~~ _done 2022-01-07, designs updated_
