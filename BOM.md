# Bill of Materials

## MCU
 -  1x SS Micro ATMega32u4 (Leonardo, Pro Micro or similar)
 -  1x USB cable soldered directly to the SS Micro USB tabs, to install under keypad

## Circuit 1: keyboard scanning circuit;
 -  1x  prototype board, to install under keypad (nb; assembly should minimize height at all cost)
 -  2x  4017 decade counters
 -  1x  74hc165 shift registers
 -  20x 1N4148 signal diodes to protect outputs from decade counters
 -  8x  1N4148 signal diodes to protect inputs to shift register (recommended)
 -  1x  10K ohm DIP exclusion Network Resistor array 9pin acting as pull down resistors on the 8 shift register inputs

 _or replace last item with_
 -  8x  10K ohm 0.25w resistors acting as pull down resistors on the 8 shift register inputs

## Circuit 2: Caps Lock, Num Lock, Scroll Lock, and Application LEDs circuit
 -  9x WS2812 Neopixel LEDs in a strip, stuck where the original LEDs are situated
 -  1x piece of insulating tape to go under the Neopixels to avoid shorts

## Circuit 3: Keyboard RGB back light switch circuit;
 -  1x prototype board, to install under keypad

 _Either_
 -  3x NPN transistors as current amplifier to SS micro pinout
 -  3x 1K ohm base input resistors

 _or_
 -  3x IRLZ44N MOSFET as current amplifier to SS micro pinout
 -  3x 10K ohm gate pulldown resistors

Also solder & soldering iron, hot glue & glue gun, heatshrink, and signal wires.
