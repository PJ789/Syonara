# Bill of Materials

## MCU
 -  1x SS Micro ATMega32u4 (Leonardo, Pro Micro or similar)
 -  1x USB cable soldered directly to the SS Micro USB tabs, to install under keypad

## Circuit 1: keyboard scanning circuit;
 -  1x  prototype board, to install under keypad (nb; assembly should minimize height at all cost)
 -  2x  MC14017/CD4017 decade counters, or equivalent
 -  1x  74HC165 shift registers
 -  20x 1N4148 signal diodes to protect outputs from decade counters
 -  8x  1N4148 signal diodes to protect inputs to shift register (recommended)
 -  1x  9 pin 47K ohm SIP [resistor network array acting](https://www.youtube.com/watch?v=_DJ_xLOxnpE) as pull down resistors on the 8 shift register inputs. _if you have the option, 10K ohm would probably be a better choice for faster shift register operation_

 _or replace last item with_
 -  8x  47K ohm 0.25w resistors acting as pull down resistors on the 8 shift register inputs _if you have the option, 10K ohm would probably be a better choice for faster shift register operation_

## Circuit 2: Caps Lock, Num Lock, Scroll Lock, and Application LEDs circuit
 -  9x WS2812 Neopixel LEDs from a 144 pixels/meter strip, stuck where the original LEDs are situated
 -  1x piece of insulating tape to go under the Neopixels to avoid shorts

## Circuit 3: Keyboard RGB back light switch circuit;
 -  1x prototype board, to install under keypad

 _Either_
 -  3x NPN transistors as current amplifier to SS micro pinout
 -  3x 1K ohm base input resistors

 _or_
 -  3x IRLZ44N MOSFET as current amplifier to SS micro pinout
 -  3x 10K ohm gate pulldown resistors

Also solder & soldering iron, hot glue & glue gun, heatshrink, and signal wires

## Coloured Wires;
 -  20x reels of multi-coloured signal wire

 _or_
 -  1x recycled & stripped Centronics cables as a source of multi-coloured signal wires

## Mounting;
 -  hot glue or plastic rivets to secure components in place
 - a hacksaw may be needed to adjust the height of the white plastic LED lense, I found I needed to trim mine in half to stop the keyboard bulging
