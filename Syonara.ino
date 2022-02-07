/*
    Syonara - Arduino SS Micro (Leonardo) firmware for Razer Cynosa Lite and similar 
              single zone RGB keyboards

    Copyright 2022 Peter John 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "Keyboard.h"
#include <Adafruit_NeoPixel.h>
#include "EEPROM.h"

#define MAX_COLUMNS              20
#define MAX_ROWS                  8

#define DEBUG                     0

#define SHIFT_OR_LOAD_PIN         2
#define CLOCK_PIN                 3
#define SERIAL_PIN                6
#define RED_PIN                   9
#define GREEN_PIN                10
#define BLUE_PIN                 11
#define COUNTER_1_CLOCK_PIN      14
#define COUNTER_1_2_RESET_PIN    15
#define COUNTER_2_CLOCK_PIN      16

#define NEOPIXEL_PIN             A0

// Neopixels 
#define CAPS_LOCK_LED             0
#define NUM_LOCK_LED              2
#define SCROLL_LOCK_LED           4
#define APPLICATION_LED           6
#define POWER_LED                 8

#define NUM_LEDS                  9
Adafruit_NeoPixel keyboard_status_leds(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
enum NeoPixelColors {
    Red    = 0xff0000,
    Orange = 0xffa500,
    Yellow = 0xffff00,
    Green  = 0x00ff00,
    Blue   = 0x0000ff,
    Indigo = 0x4b0082,
    Violet = 0x8a2be2,
    Purple = 0xff00ff,
    White  = 0xffffff,
    Black  = 0x000000
};

#if DEBUG
const char* keyboard_map_string[MAX_COLUMNS][MAX_ROWS] =
{
   //      0              1                 2                 3              4                 5                 6               7
   [0] = { " ",           "KEY_SCROLL_LOCK",0,                0,             0,                0,                0,              "KEY_PRINT_SCREEN"},
   [1] = { "4",           0,                0,                "KEY_KP_7",    "KEY_KP_8",       "KEY_KP_9",       "KEY_KP_PLUS",  "KEY_F5"          },
   [2] = { 0,             0,                0,                0,             "KEY_RIGHT_CTRL", 0,                "KEY_LEFT_CTRL",0                 },
   [3] = { 0  ,           0,                "KEY_RIGHT_GUI",  "KEY_LEFT_ALT",0,                "KEY_RIGHT_ALT",  0,              0                 },
   [4] = { "x",           "KEY_BACKSPACE",  0,                "KEY_F11",     "KEY_RETURN",     "KEY_F12",        "KEY_F9",       "KEY_F10"         },
   [5] = { "c",           "KEY_KP_4",       "KEY_KP_1",       0,             "KEY_NUM_LOCK",   "KEY_DOWN_ARROW", "KEY_DELETE",   0                 },
   [6] = { "3",           0,                0,                0,             0,                0,                0,              0                 },
   [7] = { "KEY_LEFT_GUI",0,                0,                0,             0,                0,                0,              0                 },
   [8] = { 0,             "KEY_LEFT_SHIFT", "KEY_RIGHT_SHIFT",0,             0,                0,                0,              0                 },
   [9] = { "2",           0,                "KEY_KP_ENTER",   "KEY_UP_ARROW",0,                "KEY_LEFT_ARROW", "KEY_HOME",     "KEY_END"         },
  [10] = { "1",           "KEY_KP_6",       "KEY_KP_3",       "KEY_KP_DOT",  "KEY_KP_ASTERISK","KEY_KP_MINUS",   "KEY_PAGE_UP",  "KEY_PAGE_DOWN"   },
  [11] = { "5",           "KEY_KP_5",       "KEY_KP_2",       "KEY_KP_0",    "KEY_KP_SLASH",   "KEY_RIGHT_ARROW","KEY_INSERT",   0                 },
  [12] = { "z",           "[",              ";",              "\'",          "\\",             "/",              "-",            "0"               },
  [13] = { "d",           "KEY_F7",         "l",              0,             ".",              "KEY_MENU",       "KEY_F8",       "9"               },
  [14] = { "s",           "]",              "k",              "KEY_F6",      ",",              0,                "=",            "8"               },
  [15] = { "a",           "y",              "j",              "h",           "m",              "n",              "6",            "7"               },
  [16] = { "KEY_TAB",     "t",              "f",              "g",           "v",              "b",              "p",            "o"               },
  [17] = { "e",           "KEY_F3",         0,                "KEY_F4",      0,                0,                "KEY_F2",       "i"               },
  [18] = { "w",           "KEY_CAPS_LOCK",  0,                "KEY_ISO",     0,                0,                "KEY_F1",       "u"               },
  [19] = { "q",           "KEY_PAUSE",      0,                "KEY_ESC",     0,                0,                "`",            "r"               }
};
#endif

#define KEY_ISO    0xEC
#define KEY_BSLASH 0xB9
const char keyboard_map_char[MAX_COLUMNS][MAX_ROWS] =
{
   //      0             1              2               3             4               5               6             7
   [0] = { ' ',         KEY_SCROLL_LOCK,0,              0,            0,              0,              0,            KEY_PRINT_SCREEN},
   [1] = { '4',         0,              0,              KEY_KP_7,     KEY_KP_8,       KEY_KP_9,       KEY_KP_PLUS,  KEY_F5          },
   [2] = { 0,           0,              0,              0,            KEY_RIGHT_CTRL, 0,              KEY_LEFT_CTRL,0               },
   [3] = { 0  ,         0,              KEY_RIGHT_GUI,  KEY_LEFT_ALT, 0,              KEY_RIGHT_ALT,  0,            0               },
   [4] = { 'x',         KEY_BACKSPACE,  0,              KEY_F11,      KEY_RETURN,     KEY_F12,        KEY_F9,       KEY_F10         },
   [5] = { 'c',         KEY_KP_4,       KEY_KP_1,       0,            KEY_NUM_LOCK,   KEY_DOWN_ARROW, KEY_DELETE,   0               },
   [6] = { '3',         0,              0,              0,            0,              0,              0,            0               },
   [7] = { KEY_LEFT_GUI,0,              0,              0,            0,              0,              0,            0               },
   [8] = { 0,           KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT,0,            0,              0,              0,            0               },
   [9] = { '2',         0,              KEY_KP_ENTER,   KEY_UP_ARROW, 0,              KEY_LEFT_ARROW, KEY_HOME,     KEY_END         },
  [10] = { '1',         KEY_KP_6,       KEY_KP_3,       KEY_KP_DOT,   KEY_KP_ASTERISK,KEY_KP_MINUS,   KEY_PAGE_UP,  KEY_PAGE_DOWN   },
  [11] = { '5',         KEY_KP_5,       KEY_KP_2,       KEY_KP_0,     KEY_KP_SLASH,   KEY_RIGHT_ARROW,KEY_INSERT,   0               },
  [12] = { 'z',         '[',            ';',            '\'',         '\\',           '/',            '-',          '0'             },
  [13] = { 'd',         KEY_F7,         'l',            0,            '.',            KEY_MENU,       KEY_F8,       '9'             },
  [14] = { 's',         ']',            'k',            KEY_F6,       ',',            0,              '=',          '8'             },
  [15] = { 'a',         'y',            'j',            'h',          'm',            'n',            '6',          '7'             },
  [16] = { KEY_TAB,     't',            'f',            'g',          'v',            'b',            'p',          'o'             },
  [17] = { 'e',         KEY_F3,         0,              KEY_F4,       0,              0,              KEY_F2,       'i'             },
  [18] = { 'w',         KEY_CAPS_LOCK,  0,              KEY_ISO,      0,              0,              KEY_F1,       'u'             },
  [19] = { 'q',         KEY_PAUSE,      0,              KEY_ESC,      0,              0,              '`',          'r'             }
};

#define DEBOUNCE_TIMEOUT 10
uint32_t key_debounce_times[MAX_COLUMNS][MAX_ROWS];

uint8_t last_incoming_bytes[MAX_COLUMNS];
uint8_t last_shift_register_byte;

byte anti_ghost[MAX_COLUMNS];

#define EFFECT_CHANGE_KEY KEY_PAUSE
#define MAX_EFFECT 15
uint8_t effect                  = 0;
bool keyboard_activity_detected = false;
volatile bool led_status_update = true;
bool caps_lock_on               = false;
bool scroll_lock_on             = false;
bool num_lock_on                = false;
bool application_led_on         = false;
bool power_led_on               = true;

void setup() {
#if DEBUG
Serial.begin(250000);
//while(!Serial);
Serial.println(F("Running"));
#endif  

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A  =  0xAF;
  TIMSK0 |= _BV(OCIE0A);

  
  pinMode(      SHIFT_OR_LOAD_PIN,   OUTPUT);
  digitalWrite( SHIFT_OR_LOAD_PIN,   LOW);
  pinMode(      CLOCK_PIN,           OUTPUT);
  digitalWrite( CLOCK_PIN,           LOW);
  pinMode(      SERIAL_PIN,          INPUT);

  pinMode(      SHIFT_OR_LOAD_PIN,   OUTPUT);
  digitalWrite( SHIFT_OR_LOAD_PIN,   LOW);
  
  pinMode(      COUNTER_1_CLOCK_PIN, OUTPUT);
  digitalWrite( COUNTER_1_CLOCK_PIN, LOW);
  pinMode(      COUNTER_1_2_RESET_PIN, OUTPUT);
  digitalWrite( COUNTER_1_2_RESET_PIN, LOW);
  pinMode(      COUNTER_2_CLOCK_PIN, OUTPUT);
  digitalWrite( COUNTER_2_CLOCK_PIN, LOW);

  pinMode(      RED_PIN,             OUTPUT);
  digitalWrite( RED_PIN,             LOW);
  pinMode(      GREEN_PIN,           OUTPUT);
  digitalWrite( GREEN_PIN,           HIGH);
  pinMode(      BLUE_PIN,            OUTPUT);
  digitalWrite( BLUE_PIN,            LOW);

  memset(last_incoming_bytes, 0, sizeof(last_incoming_bytes));
  memset(key_debounce_times,  0, sizeof(key_debounce_times));
  memset(anti_ghost, 0, sizeof(anti_ghost));

  reset_decade_counters();
  
  keyboard_status_leds.begin();
  keyboard_status_leds.setBrightness(100); // (max = 255)

  Keyboard.begin();

  HID().setKeyboardLedsStatusReportCallback(keyboardLedsStatusReportCallback);

  effect = EEPROM.read(0);
  EEPROM.update(0, effect = (effect>MAX_EFFECT)?0:effect );

}

#if DEBUG
unsigned long cycles = 0;
unsigned long cycle_count_start  = millis();
#endif
void loop() {
  // static variables for better performance
  static bool    key_activity_detected;
  register uint8_t column = 0;


  Keyboard.releaseAll();
  reset_decade_counters();
  column = 0;
  keyboard_activity_detected = false;

#if DEBUG
Serial.println("Waiting for keyboard activity ");
#endif
  do
  {
#if DEBUG
cycles++;
#endif
   // this code is not pretty, but it is intended to run as fast as possible
    column=0;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=1;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=2;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=3;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=4;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=5;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=6;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=7;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=8;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();
    column=9;    if (read_shift_register_low_level() ) break;
    increment_decade_counters();

  }
  while(!last_shift_register_byte);
#if DEBUG
Serial.print("Key press detected on column ");
Serial.println(column);
#endif
  clearAntiGhosting();
  
  do
  {
#if DEBUG
cycles++;
#endif
    keyboard_activity_detected = false;
    for (;  column != (MAX_COLUMNS/2); column++, increment_decade_counters())
    {
      // if there is any keyboard activity... keys pressed or keys released scan & decode the keys
      if (read_shift_register_low_level()) // keys pressed
      {
        decode( column,                 read_shift_register( increment_decade_counter2 ) );
        refreshAntiGhosting();
        decode( column+(MAX_COLUMNS/2), read_shift_register( increment_decade_counter1 ) );
        refreshAntiGhosting();

        keyboard_activity_detected = true;
      }
      else // have any keys been released?
      {
        if (last_incoming_bytes[column]) // keys released? cols 0-9
        {
          decode( column,                 read_shift_register( increment_decade_counter2 ) );
          keyboard_activity_detected = true;
          refreshAntiGhosting();
        }
        if (last_incoming_bytes[column+(MAX_COLUMNS/2)])  // keys released? cols 10-19
        {
          decode( column+(MAX_COLUMNS/2), read_shift_register( increment_decade_counter1 ) );
          keyboard_activity_detected = true;
          refreshAntiGhosting();
        }
      }
    }
    column = 0;
  }
  while(keyboard_activity_detected);

}

void reset_decade_counters()
{
//  digitalWrite(COUNTER_1_2_RESET_PIN , HIGH);
  PORTB |= (1<<PB1);// pin15
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 500.0ns min  pulse width is <500ns
//  digitalWrite(COUNTER_1_2_RESET_PIN , LOW);
  PORTB &= ~(1<<PB1);// pin15
 
}

void increment_decade_counters( )
{
//  digitalWrite(COUNTER_1_CLOCK_PIN , HIGH);
//  digitalWrite(COUNTER_2_CLOCK_PIN , HIGH);
  PORTB |= ((1<<PB2)|(1<<PB3));// pin14 | pin16
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 250.0ns: min clock pulse width is <200ns
//  digitalWrite(COUNTER_2_CLOCK_PIN , LOW);
  PORTB &= ~((1<<PB2)|(1<<PB3));// pin14 | pin16

}

void increment_decade_counter1()
{
//  digitalWrite(COUNTER_1_CLOCK_PIN, HIGH);
  PORTB |= (1<<PB3);// pin14
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 250.0ns: min clock pulse width is <200ns
//  digitalWrite(COUNTER_1_CLOCK_PIN, LOW);
  PORTB &= ~(1<<PB3);// pin14

}

void increment_decade_counter2()
{
//  digitalWrite(COUNTER_2_CLOCK_PIN, HIGH);
  PORTB |= (1<<PB2);// pin16
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 250.0ns: min clock pulse width is <200ns
//  digitalWrite(COUNTER_2_CLOCK_PIN, LOW);
  PORTB &= ~(1<<PB2);// pin16

}

uint8_t read_shift_register( void (*increment_other_decade_counter)(void) )
{
  // static variables for better performance
  // Get data from 74HC165
  register uint8_t incoming_byte;
  static uint8_t initial_byte;
  static uint8_t result_byte;
  register uint8_t other_decade_counter_increments;

  do
  {
    // if zero return (no further processing necessary: its zero)
    if (!last_shift_register_byte) return 0;
   
    incoming_byte = last_shift_register_byte;
    initial_byte  = incoming_byte;

    // =========================================
    // now scan across the other 9 columns of the other decade counter
    // to eliminate false key presses

    (*increment_other_decade_counter)();

    for( other_decade_counter_increments=1; other_decade_counter_increments!=(MAX_COLUMNS/2) ; other_decade_counter_increments++,(*increment_other_decade_counter)())
    {
      incoming_byte &= read_shift_register_low_level();
      if (!incoming_byte) break;
    }

    // at this point incoming is still true and a loop of the other counter is complete, OR incoming is false, so resync counters
    for( ; other_decade_counter_increments!=(MAX_COLUMNS/2) ; other_decade_counter_increments++,(*increment_other_decade_counter)());

    result_byte = incoming_byte;
    // We're back in sync. Has the shift register changed value while we've been away? if so, try again
  } while(initial_byte != read_shift_register_low_level() );

  return result_byte;
}

void decode( uint8_t column, uint8_t incoming_byte)
{
  // static variables for better performance
  static char     key;
  register uint8_t row;
  register uint8_t last_incoming_byte;
  register uint8_t all_changed_bits;
  register uint8_t all_pressed_bits;
  register uint8_t row_bit_selector;
  static uint32_t *key_debounce_time_ptr;
   
  last_incoming_byte = last_incoming_bytes[column];

  if (incoming_byte==last_incoming_byte) return; // nothing to do, nothing pressed or released

  // we're only interested in changed bits
  all_changed_bits = incoming_byte ^ last_incoming_byte; 
  all_pressed_bits = all_changed_bits & incoming_byte;
  
  // ignore those rows where changed bit is not set
  for(row=0; !(1 & all_changed_bits); row++, all_changed_bits>>=1 );

  // decode changed rows from first to last bit set, stops when all_changed_bits is shifted to zero
  for(; all_changed_bits; row++, all_changed_bits>>=1 )
  {
    if (1 & all_changed_bits) // a change affects this row
    {
      row_bit_selector=(1<<row);
      key_debounce_time_ptr = &key_debounce_times[column][row];
      if (
          (!(*key_debounce_time_ptr)) // if no debounce to consider
          ||
          ((*key_debounce_time_ptr) && (millis()-(*key_debounce_time_ptr))>DEBOUNCE_TIMEOUT) // debounce has expired
         )
      {
        key = keyboard_map_char[column][row];
        // is a key pressed that wasn't previously pressed? note the deliberate assignment to application_led_on
        if ((application_led_on = (all_pressed_bits & row_bit_selector)))
        {
          if (!(anti_ghost[column]&row_bit_selector))
          {
#if DEBUG
            debugReportKey(column, row, incoming_byte, last_incoming_byte);
#endif
            Keyboard.press(key);
            if (key==((char)EFFECT_CHANGE_KEY)) EEPROM.update(0,(effect=(effect<MAX_EFFECT)?effect+1:0));
          }
#if DEBUG
          else
          {
            debugReportAntiGhost(column, row, incoming_byte, last_incoming_byte);
          }
#endif
        }
        // a key that was previously pressed has been released
        else 
        {
#if DEBUG
          debugReportKey(column, row, incoming_byte, last_incoming_byte);
#endif
          Keyboard.release(key);
        }
        (*key_debounce_time_ptr) = millis();
      }
      else // handle debounce by setting the incoming bit to the last incoming bit
      {
#if DEBUG
        Serial.println("Debounce!");
#endif
        if (last_incoming_byte & row_bit_selector)
          incoming_byte |=   row_bit_selector;
        else
          incoming_byte &=  ~row_bit_selector;
      }
    }
  }
  last_incoming_bytes[column]=incoming_byte;
}

uint8_t read_shift_register_low_level()
{
  // Loading time
  // note, its possible that if 10K pulldown resistors are used this could be reduced to 1us or less
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 625ns delay
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 625ns delay
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 625ns delay
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 625ns delay
  
  // Enable shifting, clock high, shift or load high
  //digitalWrite(CLOCK_PIN, HIGH);
  PORTD |= (1<<PD0);// pin3
  __asm__("nop\n\t"); // 62.5ns delay
  //digitalWrite(SHIFT_OR_LOAD_PIN, HIGH);
  PORTD |= (1<<PD1);// pin2
  __asm__("nop\n\t"); // 62.5ns delay

  // Get data from 74HC165
  last_shift_register_byte = shiftIn(SERIAL_PIN, CLOCK_PIN, MSBFIRST);
  
  // Enable loading
  //digitalWrite(SHIFT_OR_LOAD_PIN, LOW);
  PORTD &= ~(1<<PD1);// pin2

  return last_shift_register_byte;
}


enum BacklightEffects {
  ColourTransitions = 0, // 0 - colour backlight transitions
  RedTransitions,        // 1 - red transitions only
  GreenTransitions,      // 2 - green transitions only
  BlueTransitions,       // 3 - blue transitions only
  RedGreenTransitions,   // 4 - red-green transitions only
  RedBlueTransitions,    // 5 - red-blue transitions only
  BlueGreenTransitions,  // 6 - blue-green transitions only
  SolidWhite,            // 7 - solid white backlight, no transitions
  SolidRed,              // 8 - solid red backlight, no transitions
  SolidGreen,            // 9 - solid green backlight, no transitions
  SolidBlue,             // 10 - solid blue backlight, no transitions
  SolidYellow,           // 11 - solid yellow backlight, no transitions
  SolidMagenta,          // 12 - solid magenta backlight, no transitions
  SolidCyan,             // 13 - solid cyan backlight, no transitions
  BacklightOff,          // 14 - backlight effects off
  Rainbow                // 15 - rainbow colour cycle
};

// Interrupt is called once a millisecond, to update the LEDs
// Be careful in this function, it is easy to crash the MCU (requiring a reset to recover)

SIGNAL(TIMER0_COMPA_vect) 
{
  static int16_t  r,g,b; // 16 bit ints, by design
  static uint8_t  leds;
  static uint32_t backlight_color;

  // spread led update workload over 64x1ms timeslots to avoid spikes every millisecond
  // effects:
  switch( (millis() & 0b00111111) )
  {
    case 0b00000000:  // process red pwm output
      r = 0;
      switch(effect)
      {
        case ColourTransitions...BlueGreenTransitions:
          switch(effect)
          {
            case ColourTransitions:
            case RedTransitions:
            case RedGreenTransitions:
            case RedBlueTransitions:
              r = map((millis() & 0b0011111111000000),0,0b0011111111000000,-150,150);
              r = 100+abs(r);
              r = keyboard_status_leds.gamma8(r);
              break;
            default:
              break;
          }
          break;
        case SolidWhite...SolidCyan:
          switch(effect)
          {
            case SolidWhite:
            case SolidRed:
            case SolidYellow:
            case SolidMagenta:
              r = 150;           
              break;
            default:
              break;
          }
          break;
        case Rainbow:
          r = (keyboard_status_leds.ColorHSV(millis() & 0xFFFF) & 0xFF0000)>>16;
          break;
        default:
          break;
      }
 
      r = ( scroll_lock_on || num_lock_on  ) ?   0:r;
      r = ( keyboard_activity_detected || caps_lock_on ) ? 254:r;

      analogWrite(RED_PIN, r );

      break;
    case 0b00010000: // process green pwm output
      g = 0;
      switch(effect)
      {
        case ColourTransitions...BlueGreenTransitions:
          switch(effect)
          {
            case ColourTransitions:
            case GreenTransitions:
            case RedGreenTransitions:
            case BlueGreenTransitions:
              g = map((millis() & 0b0001111111000000),0,0b0001111111000000,-150,150);
              g = 100+abs(g);
              g = keyboard_status_leds.gamma8(g);
              break;
            default:
              break;
          }
          break;
        case SolidWhite...SolidCyan:
          switch(effect)
          {
            case SolidWhite:
            case SolidGreen:
            case SolidYellow:
            case SolidCyan:
              g = 150;
              break;          
            default:
              break;
          }
          break;
        case Rainbow:
          g = (keyboard_status_leds.ColorHSV(millis() & 0xFFFF) & 0x00FF00)>>8;
          break;
        default:
          break;
      }
    
      g = ( caps_lock_on || num_lock_on   ) ?   0:g;
      g = ( keyboard_activity_detected || scroll_lock_on) ? 254:g;
      
      analogWrite(GREEN_PIN, g );

      break; 
 
    case 0b00100000:  // process blue pwm output
       b = 0;
      switch(effect)
      {
        case ColourTransitions...BlueGreenTransitions:
          switch(effect)
          {
              case ColourTransitions:
              case BlueTransitions: 
              case RedBlueTransitions: 
              case BlueGreenTransitions: 
                b = map((millis() & 0b0000111111000000),0,0b0000111111000000,-150,150);
                b = 100+abs(b);
                b = keyboard_status_leds.gamma8(b);
              break;
            default:
              break;
          }
          break;
        case SolidWhite...SolidCyan:
          switch(effect)
          {
            case SolidWhite:
            case SolidBlue:
            case SolidMagenta:
            case SolidCyan:
              b = 150;
              break;          
            default:
              break;
          }
          break;
        case Rainbow:
          b = keyboard_status_leds.ColorHSV(millis() & 0xFFFF) & 0x0000FF;
          break;
        default:
          break;
      }
 
      b = ( scroll_lock_on || caps_lock_on )?   0:b;
      b = ( keyboard_activity_detected || num_lock_on  )? 254:b;

      analogWrite(BLUE_PIN,  b );

      break;
    case 0b00110000:  // process keyboard status LED Neopixel output
      if (led_status_update)
      {
        caps_lock_on   = Keyboard.getLedStatus(LED_CAPS_LOCK);
        scroll_lock_on = Keyboard.getLedStatus(LED_SCROLL_LOCK);
        num_lock_on    = Keyboard.getLedStatus(LED_NUM_LOCK);
        led_status_update = false;
      }
      backlight_color = keyboard_status_leds.Color(r,g,b);
      if (keyboard_activity_detected)
      {
        for(leds=0; leds<NUM_LEDS; leds+=2)
          keyboard_status_leds.setPixelColor(leds, White);
      }
      else
      {
        for(leds=0; leds<NUM_LEDS; leds+=2)
          keyboard_status_leds.setPixelColor(leds, backlight_color);
      }

      if (caps_lock_on)       keyboard_status_leds.setPixelColor(CAPS_LOCK_LED,   Red);
      if (scroll_lock_on)     keyboard_status_leds.setPixelColor(SCROLL_LOCK_LED, Green);
      if (num_lock_on)        keyboard_status_leds.setPixelColor(NUM_LOCK_LED,    Blue);
      if (application_led_on) keyboard_status_leds.setPixelColor(APPLICATION_LED, Yellow);
      if (power_led_on)       keyboard_status_leds.setPixelColor(POWER_LED,       backlight_color);

      keyboard_status_leds.show();
      break;
  }
} 

void keyboardLedsStatusReportCallback()
{
  led_status_update = true;
}

void clearAntiGhosting()
{
    memset(anti_ghost, 0, sizeof(anti_ghost));
}
void refreshAntiGhosting()
{
  register uint8_t column = 0;
  register uint8_t row = 0;

  clearAntiGhosting();
  
  setRowAntiGhosting();    
  setColumnAntiGhosting();    
  setMultiKeyAntiGhosting();    
}
void setRowAntiGhosting()
{
  register uint8_t column             = 0;
  register uint8_t column_inner       = 0;
  register uint8_t last_incoming_byte = 0;
  register uint8_t anti_ghost_mask    = 0;
  
  for( column=0; column<MAX_COLUMNS && anti_ghost_mask!=0b11111111; column++)
  {
    last_incoming_byte = last_incoming_bytes[column];
    // check if more than one row (bit) is set for this column
    if (last_incoming_byte & (last_incoming_byte-1))
    {
      // more than one row is set for this column, add rows to maks
      anti_ghost_mask|=last_incoming_byte;
    }
  }
  if ( anti_ghost_mask )
  {
    // use mask to lock out affected rows to prevent ghosting on other columns
    for( column=0; column<MAX_COLUMNS; column++)
    {
      anti_ghost[column]|=anti_ghost_mask;
    }
  }
}
void setColumnAntiGhosting(  )
{
  register uint8_t column             = 0;
  register uint8_t row_bit_selector   = 0;
  register uint8_t row_bit_counter    = 0;

  for( row_bit_selector=1; row_bit_selector; row_bit_selector<<=1)
  {
    row_bit_counter = 0;
    
    // check if more than one column has this row (bit) set
    for( column=0; column<MAX_COLUMNS && row_bit_counter!=2; column++)
    {
      if (last_incoming_bytes[column]&row_bit_selector) row_bit_counter++;
    }

    // more than one column has this row bit set
    // lock out affected columns to prevent ghosting on other rows
    if (row_bit_counter==2)
    {
      for( column=0; column<MAX_COLUMNS; column++)
      {
        if (last_incoming_bytes[column]&row_bit_selector) anti_ghost[column]=0b11111111;
      }
    }
  }

}
void setMultiKeyAntiGhosting()
{
  register uint8_t column             = 0;
  register uint8_t row_bit_selector   = 0;
  register uint8_t key_counter        = 0;
  register uint8_t anti_ghost_mask    = 0;
  
  // check if more than one key is pressed
  for( column=0; column<MAX_COLUMNS && key_counter!=2; column++)
  {
    if ( last_incoming_bytes[column]!=0) key_counter+=1;
  }
  
  if (key_counter==2)
  {
    // more than one key is active on different columns
    // lock out all columns/rows combos to prevent ghosting
    for( column=0; column<MAX_COLUMNS; column++)
    {
      anti_ghost_mask |= last_incoming_bytes[column];
    }

    for( column=0; column<MAX_COLUMNS; column++)
    {
      if ( last_incoming_bytes[column]!=0) anti_ghost[column] |= anti_ghost_mask;
    }
  }
}
#if DEBUG
void debugReportKey(uint8_t column, uint8_t row, uint8_t incoming_byte, uint8_t last_incoming_byte)
{
  Serial.println(F("-----------------------------------"));
  Serial.println(F("Key change"));
  debugReport( column,  row,  incoming_byte,  last_incoming_byte);
}
void debugReportAntiGhost(uint8_t column, uint8_t row, uint8_t incoming_byte, uint8_t last_incoming_byte)
{
  Serial.println(F("-----------------------------------"));
  Serial.println(F("Anti-ghosting hit, keystroke suppressed"));
  debugReport( column,  row,  incoming_byte,  last_incoming_byte);
}
void debugReport(uint8_t column, uint8_t row, uint8_t incoming_byte, uint8_t last_incoming_byte)
{
  Serial.print(F("Row/Column ["));
  Serial.print(row);
  Serial.print(F("/"));
  Serial.print(column);
  Serial.println(F("]"));
  if (incoming_byte>last_incoming_byte)
  {
    Serial.print(F("PRESSING ["));
  }
  else
  {
    Serial.print(F("RELEASING ["));
  }  
  Serial.print(keyboard_map_string[column][row]);
  Serial.println(F("]"));
  Serial.print(F("Cycles since last report ["));
  Serial.print(cycles);
  Serial.println(F("]"));
  Serial.print(F("Time since last report ["));
  Serial.print(((float)(millis()-cycle_count_start)));
  Serial.println(F("ms]"));
  Serial.print(F("Estimated scan rate ["));
  Serial.print((((float)cycles)*1000.0)/((float)(millis()-cycle_count_start)));
  Serial.println(F("hz]"));
  Serial.print(F("Current effect ["));
  Serial.print(effect);
  Serial.println(F("]"));
  cycles = 0;
  cycle_count_start  = millis();
}
void printByteAsBinary(uint8_t number, uint8_t bits)
{
  uint8_t bit_index;

  bits = (bits>8)?8:bits;

  if (bits == 0)
  {
    return;
  }
  
  if (bits == 8)
  {
    Serial.println("  76543210");
    Serial.print("0b");
  }

  bit_index=1<<(bits-1);
  Serial.print((number & bit_index) ? '1' : '0');
  printByteAsBinary(number,bits-1);
}
#endif
