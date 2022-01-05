/*
    Syonara - Arduino SS Micro (Leonardo) firmware for Razer Cynosa Lite and similar 
              single zone RGB keybaords
    
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
#include "Keyboard_da_DK.h"
#include <Adafruit_NeoPixel.h>

#define MAX_COLUMNS              20
#define MAX_ROWS                  8

#define DEBUG                     0

#define SHIFT_OR_LOAD_PIN         2
#define CLOCK_PIN                 3
#define SERIAL_PIN                6
#define RED_PIN                   9
#define GREEN_PIN                10
#define BLUE_PIN                 11
#define COUNTER_2_RESET_PIN      15
#define COUNTER_2_CLOCK_PIN      16

#define NEOPIXEL_PIN             A0
#define COUNTER_1_RESET_PIN      A1
#define COUNTER_1_CLOCK_PIN      A2

// Neopixels 
#define CAPS_LOCK_LED             0
#define NUM_LOCK_LED              2
#define SCROLL_LOCK_LED           4
#define APPLICATION_LED           6
#define POWER_LED                 8

#define NUM_LEDS                  9
Adafruit_NeoPixel keyboard_status_leds(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
enum NeoPixelColors {
    Red = 0xFF0000,
    Orange = 0xFFA500,
    Yellow = 0xFFFF00,
    Green = 0x00FF00,
    Blue = 0x0000FF,
    Indigo = 0x4b0082,
    Violet = 0x8a2be2,
    Purple = 0xFF00FF,
    White = 0xFFFFFF,
    Black = 0x000000
};

const char* keyboard_map_string[MAX_COLUMNS][MAX_ROWS] =
{
   //      0      1      2      3      4      5      6      7
   [0] = { "SPCB","SCRL",0,     0,     0,     0,     0,     "PRTS"},
   [1] = { "4",   0,     0,     "N7",  "N8",  "N9",  "N+",  "F5"  },
   [2] = { 0,     0,     0,     0,     "RCTL",0,     "LCTL",0     },
   [3] = { 0  ,   0,     "FUNC","LALT",0,     "ALTG",0,     0     },
   [4] = { "X",   "BACK",0,     "F11", "ENT", "F12", "F9",  "F10" },
   [5] = { "C",   "N4",  "N1",  "UP",  "NUM", "DOWN","DEL", 0     },
   [6] = { "3",   0,     0,     0,     0,     0,     0,     0     },
   [7] = { "WIND",0,     0,     0,     0,     0,     0,     0     },
   [8] = { 0,     "LSHF","RSHF",0,     0,     0,     0,     0     },
   [9] = { "2",   0,     "NENT","UP",  0,     "LEFT","HOME","END" },
  [10] = { "1",   "N6",  "N3",  "N.",  "N*",  "N-",  "PGUP","PGDN"},
  [11] = { "5",   "N5",  "N2",  "N0",  "N/",  "RGHT","INS", 0     },
  [12] = { "Z",   "Å",   "Ö",   "Ä",   "'",   "-",   "+",   "0"   },
  [13] = { "D",   "F7",  "L",   0,     ":.",  "MENU","F8",  "9"   },
  [14] = { "S",   "UMLT","K",   "F6",  ",;",   0,     "´`|","8"   },
  [15] = { "A",   "Y",   "J",   "H",   "M",   "N",   "6",   "7"   },
  [16] = { "TAB", "T",   "F",   "G",   "V",   "B",   "P",   "0"   },
  [17] = { "E",   "F3",  0,     "F4",  0,     0,     "F2",  "I"   },
  [18] = { "W",   "CAPS",0,     "<",   0,     0,     "F1",  "U"   },
  [19] = { "Q",   "BRK", 0,     "ESC", 0,     0,     "½§",  "R"   }
};

const char keyboard_map_char[MAX_COLUMNS][MAX_ROWS] =
{
   //      0             1              2               3             4               5               6             7
   [0] = { ' ',         KEY_SCROLL_LOCK,0,              0,            0,              0,              0,            KEY_PRINT_SCREEN},
   [1] = { '4',         0,              0,              KEY_KP_7,     KEY_KP_8,       KEY_KP_9,       KEY_KP_PLUS,  KEY_F5          },
   [2] = { 0,           0,              0,              0,            KEY_RIGHT_CTRL, 0,              KEY_LEFT_CTRL,0               },
   [3] = { 0  ,         0,              KEY_MENU,       KEY_LEFT_ALT, 0,              KEY_RIGHT_ALT,  0,            0               },
   [4] = { 'x',         KEY_BACKSPACE,  0,              KEY_F11,      KEY_RETURN,     KEY_F12,        KEY_F9,       KEY_F10         },
   [5] = { 'c',         KEY_KP_4,       KEY_KP_1,       KEY_UP_ARROW, KEY_NUM_LOCK,   KEY_DOWN_ARROW, KEY_DELETE,   0               },
   [6] = { '3',         0,              0,              0,            0,              0,              0,            0               },
   [7] = { KEY_LEFT_GUI,0,              0,              0,            0,              0,              0,            0               },
   [8] = { 0,           KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT,0,            0,              0,              0,            0               },
   [9] = { '2',         0,              KEY_KP_ENTER,   KEY_UP_ARROW, 0,              KEY_LEFT_ARROW, KEY_HOME,     KEY_END         },
  [10] = { '1',         KEY_KP_6,       KEY_KP_3,       KEY_KP_DOT,   KEY_KP_ASTERISK,KEY_KP_MINUS,   KEY_PAGE_UP,  KEY_PAGE_DOWN   },
  [11] = { '5',         KEY_KP_5,       KEY_KP_2,       KEY_KP_0,     KEY_KP_SLASH,   KEY_RIGHT_ARROW,KEY_INSERT,   0               },
  [12] = { 'z',         KEY_A_RING,     KEY_ASH,        KEY_SLASHED_O,'\'',           '-',            '+',          '0'             },
  [13] = { 'd',         KEY_F7,         'l',            0,            '.',            KEY_MENU,       KEY_F8,       '9'             },
  [14] = { 's',         KEY_UMLAUT,     'k',            KEY_F6,       ',',            0,              KEY_ACUTE_ACC,'8'             },
  [15] = { 'a',         'y',            'j',            'h',          'm',            'n',            '6',          '7'             },
  [16] = { KEY_TAB,     't',            'f',            'g',          'v',            'b',            'p',          'o'             },
  [17] = { 'e',         KEY_F3,         0,              KEY_F4,       0,              0,              KEY_F2,       'i'             },
  [18] = { 'w',         KEY_CAPS_LOCK,  0,              '<',          0,              0,              KEY_F1,       'u'             },
  [19] = { 'q',         KEY_PAUSE,      0,              KEY_ESC,      0,              0,              '½',          'r'             }
};

bool    key_press_detected;
uint8_t incoming1, incoming2;
uint8_t last_incoming_bytes[MAX_COLUMNS];

int effect = 1;
bool key_down          = false;
volatile bool led_status_update = true;
bool caps_lock_on      = false;
bool scroll_lock_on    = false;
bool num_lock_on       = false;
bool application_led_on    = false;
bool power_led_on          = true;

void setup() {
#if DEBUG
Serial.begin(250000);
while(!Serial);
Serial.println("Running");
#endif  

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

  
  pinMode(      SHIFT_OR_LOAD_PIN,   OUTPUT);
  digitalWrite( SHIFT_OR_LOAD_PIN,   LOW);
  pinMode(      CLOCK_PIN,           OUTPUT);
  digitalWrite( CLOCK_PIN,           LOW);
  pinMode(      SERIAL_PIN,          INPUT);

  pinMode(      SHIFT_OR_LOAD_PIN,   OUTPUT);
  digitalWrite( SHIFT_OR_LOAD_PIN,   LOW);
  
  pinMode(      COUNTER_1_RESET_PIN, OUTPUT);
  digitalWrite( COUNTER_1_RESET_PIN, LOW);
  pinMode(      COUNTER_1_CLOCK_PIN, OUTPUT);
  digitalWrite( COUNTER_1_CLOCK_PIN, LOW);
  pinMode(      COUNTER_2_RESET_PIN, OUTPUT);
  digitalWrite( COUNTER_2_RESET_PIN, LOW);
  pinMode(      COUNTER_2_CLOCK_PIN, OUTPUT);
  digitalWrite( COUNTER_2_CLOCK_PIN, LOW);

  pinMode(      RED_PIN,             OUTPUT);
  digitalWrite( RED_PIN,             LOW);
  pinMode(      GREEN_PIN,           OUTPUT);
  digitalWrite( GREEN_PIN,           HIGH);
  pinMode(      BLUE_PIN,            OUTPUT);
  digitalWrite( BLUE_PIN,            LOW);

  for( uint8_t column=0; column<MAX_COLUMNS; column++)
  {
    last_incoming_bytes[column]=0;
  }

  reset_decade_counters();
  
  keyboard_status_leds.begin();
  keyboard_status_leds.setBrightness(100); // (max = 255)

  Keyboard.begin(KeyboardLayout_da_DK);

  HID().setKeyboardLedsStatusReportCallback(keyboardLedsStatusReportCallback);

}



void loop() {

  key_press_detected = false;

  for (uint8_t column = 0; column < (MAX_COLUMNS/2); column++)
  {
    incoming1 = read_shift_register( COUNTER_2_CLOCK_PIN);
    decode( column,    incoming1 );
    incoming2 = read_shift_register( COUNTER_1_CLOCK_PIN);
    decode( column+10, incoming2 );

    key_press_detected = incoming1||incoming2||key_press_detected;
    
    increment_decade_counters();
  }
  
  // Was any keypress detected?
  if (key_press_detected)
  {
    key_down=  true;
  }
  else // do idle stuff while no key pressed
  {
    // if a releaseAll has been enabled because a key has been pressed
    if (key_down)
    {
      Keyboard.releaseAll();
      key_down = false;
      reset_decade_counters();
    }
  }
}

void reset_decade_counters()
{
  digitalWrite(COUNTER_1_RESET_PIN , HIGH);
  digitalWrite(COUNTER_2_RESET_PIN , HIGH);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"); // 312.5ns: min reset pulse width is <260ns
  digitalWrite(COUNTER_1_RESET_PIN , LOW);
  digitalWrite(COUNTER_2_RESET_PIN , LOW);
 
}

void reset_decade_counter( int reset_pin)
{
  digitalWrite(reset_pin , HIGH);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t"); // 312.5ns: min reset pulse width is <260ns
  digitalWrite(reset_pin , LOW);
 
}

void increment_decade_counters( )
{
  digitalWrite(COUNTER_1_CLOCK_PIN , HIGH);
  digitalWrite(COUNTER_2_CLOCK_PIN , HIGH);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 250.0ns: min clock pulse width is <200ns
  digitalWrite(COUNTER_1_CLOCK_PIN , LOW);
  digitalWrite(COUNTER_2_CLOCK_PIN , LOW);

}

void increment_decade_counter(int clock_pin )
{
  digitalWrite(clock_pin, HIGH);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" ); // 250.0ns: min clock pulse width is <200ns
  digitalWrite(clock_pin, LOW);

}
uint8_t read_shift_register(int other_counter_clock_pin)
{
  // Get data from 74HC165
  uint8_t incoming = 0;
  uint8_t initial  = 0;

  do
  {
    // read the value from the rows
    incoming = read_shift_register_low_level();

    if (!incoming)
    {
      return 0;
    }
   
    initial   = incoming;

    // =========================================
    // now scan across the other 9 columns of the other decade counter
    // to eliminate false key presses
    // Is any keypress detected on this decade counter column?

    increment_decade_counter( other_counter_clock_pin );
    uint8_t column=0;
    for( ; incoming && column<(MAX_COLUMNS-1) ; column++)
    {
      incoming = incoming & read_shift_register_low_level();
      increment_decade_counter( other_counter_clock_pin );
    }
    // at this point incoming is true and column is 9, OR incoming is false and colum is <9, so resync counters
    for( ; column<(MAX_COLUMNS-1) ; column++)
    {
      increment_decade_counter( other_counter_clock_pin );
    }
    // Has the shift register changed value while we've been scanning? if so, try again
  } while(initial != read_shift_register_low_level());

  return incoming;
}

void decode( uint8_t column, uint8_t incoming_byte)
{
  char key;
  uint8_t row;
  uint8_t last_incoming_byte;
  uint8_t all_changed_bits, all_pressed_bits;
  uint8_t row_bit_selector;
   
  last_incoming_byte = last_incoming_bytes[column];

  // we're only interested in changed bits
  all_changed_bits = incoming_byte ^ last_incoming_byte; 

  // is a key pressed, or might a key be released?
  if (all_changed_bits)
  {
    all_pressed_bits = all_changed_bits & incoming_byte;
    
    // ignore those rows where changed bit is not set
    for(row=0,row_bit_selector = 1; !(row_bit_selector & all_changed_bits); row++, row_bit_selector<<=1 );

    // decode changed rows from first to last bit set, stops when row_bit_selector is shifted to zero
    for(; row_bit_selector && (row_bit_selector<=all_changed_bits); row++, row_bit_selector<<=1 )
    {
      key = keyboard_map_char[column][row];
      // is a key pressed that wasn't previously pressed? note the deliberate assignment to application_led_on
      if ( application_led_on = (all_pressed_bits & row_bit_selector))
      {
#if DEBUG
        debugReportPressedKey(column, row, incoming_byte, last_incoming_byte);
#endif
        Keyboard.press(key);
        delay(50); // debounce
      }
      // a key released that was previously pressed has been released
      else 
      {
#if DEBUG
        debugReportReleasedKey(column, row, incoming_byte, last_incoming_byte);
#endif
        Keyboard.release(key);
      }
    }
  }
  last_incoming_bytes[column]=incoming_byte;
}

uint8_t read_shift_register_low_level()
{

  // Loading time
  delayMicroseconds(1); // 1000ns: min input transition rise/fall time is <1000ns
  
  // Enable shifting
  digitalWrite(CLOCK_PIN, HIGH);
  __asm__("nop\n\t"); // 62.5ns delay
  digitalWrite(SHIFT_OR_LOAD_PIN, HIGH);
  __asm__("nop\n\t"); // 62.5ns delay

  // Get data from 74HC165
  uint8_t incoming = 0;
  
  incoming = shiftIn(SERIAL_PIN, CLOCK_PIN, MSBFIRST);
  
  // Enable loading
  digitalWrite(SHIFT_OR_LOAD_PIN, LOW);
  
  return incoming;
}

// Interrupt is called once a millisecond, to update the LEDs

SIGNAL(TIMER0_COMPA_vect) 
{
  static int r,g,b;

  // spread led update workload over 32x1ms timeslots to avoid spikes every millisecond
  switch( (millis() & 0b00011111) )
  {
    case 0b00000000:  // process red pwm output
      switch(effect)
      {
        case 1:
        case 2:     
          r = (millis() % 10240)/20;
          r = (r<256)?r:511-r;
          r = keyboard_status_leds.gamma8(r);
          break;
      }
      if (key_down||caps_lock_on)
      {
        analogWrite(RED_PIN,255 );
      }
      else if (scroll_lock_on||num_lock_on)
      {
        analogWrite(RED_PIN,0 );
      }
      else
      {
        analogWrite(RED_PIN, r );
      }
      break;
    case 0b00001000: // process green pwm output
      switch(effect)
      {
        case 1:
          g = (millis() % 12800)/25;
          g = (g<256)?g:511-g;
          g = keyboard_status_leds.gamma8(g);
          break;
        case 2:
          g = 0;
          break;
      }
      if (key_down||scroll_lock_on)
      {
        analogWrite(GREEN_PIN,255 );
      }
      else if (caps_lock_on||num_lock_on)
      {
        analogWrite(GREEN_PIN  ,0 );
      }
      else
      {
        analogWrite(GREEN_PIN, g );
      }
      break;
    case 0b00010000:  // process blue pwm output
      switch(effect) //blue
      {
        case 1:
          b = (millis() % 11264)/22;
          b = (b<256)?b:511-b;
          b = keyboard_status_leds.gamma8(b);
          break;
        case 2:
          b = 0;
          break;
      }
      if (key_down||num_lock_on)
      {
        analogWrite(BLUE_PIN, 255 );
      }
      else if (scroll_lock_on||caps_lock_on)
      {
        analogWrite(BLUE_PIN,   0 );
      }
      else
      {
        analogWrite(BLUE_PIN,  b );
      }
      break;
    case 0b00011000:  // process keyboard status LED Neopixel output
      if (led_status_update)
      {
        caps_lock_on   = Keyboard.getLedStatus(LED_CAPS_LOCK);
        scroll_lock_on = Keyboard.getLedStatus(LED_SCROLL_LOCK);
        num_lock_on    = Keyboard.getLedStatus(LED_NUM_LOCK);
        led_status_update = false;
      }
      keyboard_status_leds.fill(keyboard_status_leds.Color(r,g,b));
      if (key_down)           keyboard_status_leds.fill(White);
      if (caps_lock_on)       keyboard_status_leds.setPixelColor(CAPS_LOCK_LED,   Red);
      if (scroll_lock_on)     keyboard_status_leds.setPixelColor(SCROLL_LOCK_LED, Green);
      if (num_lock_on)        keyboard_status_leds.setPixelColor(NUM_LOCK_LED,    Blue);
      if (application_led_on) keyboard_status_leds.setPixelColor(APPLICATION_LED, Yellow);
      if (power_led_on)       keyboard_status_leds.setPixelColor(POWER_LED,       keyboard_status_leds.Color(r,g,b));
      
      keyboard_status_leds.show();
      break;
  }
} 

void keyboardLedsStatusReportCallback()
{
  led_status_update = true;
}

void debugReportPressedKey(uint8_t column, uint8_t row, uint8_t incoming_byte, uint8_t last_incoming_byte)
{
  Serial.println("-----------------------------------");
  Serial.print("Row/Column [");
  Serial.print(row);
  Serial.print("/");
  Serial.print(column);
  Serial.println("]");
  Serial.println("Last Incoming/Incoming bits [");
  printByteAsBinary(last_incoming_byte,8);
  Serial.println("/");
  printByteAsBinary(incoming_byte,8);
  Serial.println("]");
  Serial.print("  Pressing [");
  Serial.print(keyboard_map_string[column][row]);
  Serial.println("]");
}

void debugReportReleasedKey(uint8_t column, uint8_t row, uint8_t incoming_byte, uint8_t last_incoming_byte)
{
  Serial.println("-----------------------------------");
  Serial.print("Row/Column [");
  Serial.print(row);
  Serial.print("/");
  Serial.print(column);
  Serial.println("]");
  Serial.println("Last Incoming/Incoming bits [");
  printByteAsBinary(last_incoming_byte,8);
  Serial.println("/");
  printByteAsBinary(incoming_byte,8);
  Serial.println("]");
  Serial.print("  Releasing [");
  Serial.print(keyboard_map_string[column][row]);
  Serial.println("]");
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
