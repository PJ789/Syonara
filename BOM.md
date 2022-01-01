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
 -  4x  ultra bright LEDs, any colour (recommend red, blue, green, yellow)

 _Either_
 -  4x  270 ohm 0.25w resistors acting as current limiting resistors to LEDs

 _or_
 -  1x  330 ohm DIP exclusion Network Resistor array 9pin acting as current limiting resistors to LEDs

 _Option: use fifth LED as power indicator_
 -  1x  ultra bright LEDs, any colour (recommend white) connected to 5v via current limiting resistor (as above)

## Circuit 3: Keyboard RGB back light circuit;
 -  1x  prototype board, to install under keypad

 _Either_
 -  3x NPN transistors as current amplifier to SS micro pinout
 -  3x 1K ohm base input resistors

 _or_
 -  3x IRLZ44N MOSFET as current amplifier to SS micro pinout
 -  3x 10K ohm gate pulldown resistors

Also solder & soldering iron, hot glue & glue gun, heatshrink, and signal wires.
