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
#define CAPS_LOCK_LED             0
#define NUM_LOCK_LED              2
#define SCROLL_LOCK_LED           4
#define APPLICATION_LED           6
#define POWER_LED                 8

Adafruit_NeoPixel keyboard_status_leds(POWER_LED+1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define COUNTER_1_RESET_PIN      A1
#define COUNTER_1_CLOCK_PIN      A2


const char* keyboard_map_string[20][8] =
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

const char keyboard_map_char[20][8] =
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
uint8_t last_incoming[20];


int effect = 1;
bool key_down          = false;
volatile bool led_status_update = true;
bool caps_lock_on      = false;
bool scroll_lock_on    = false;
bool num_lock_on       = false;
bool application_on    = false;
bool power_on          = true;

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

  for( uint8_t column=0; column<20; column++)
  {
    last_incoming[column]=0;
  }

  reset_counters();
  
  keyboard_status_leds.begin();
  keyboard_status_leds.setBrightness(100); // (max = 255)

  Keyboard.begin(KeyboardLayout_da_DK);

  HID().setKeyboardLedsStatusReportCallback(keyboardLedsStatusReportCallback);

}



void loop() {
 
  key_press_detected = false;

  for (uint8_t i = 0; i <10; i++)
  {
    incoming1 = read_shift_register( COUNTER_2_CLOCK_PIN);
    decode( i,    incoming1 );
    incoming2 = read_shift_register( COUNTER_1_CLOCK_PIN);
    decode( i+10, incoming2 );

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
      reset_counters();
    }
  }
}

void reset_counters()
{
    digitalWrite(COUNTER_1_RESET_PIN , HIGH);
    digitalWrite(COUNTER_2_RESET_PIN , HIGH);
    __asm__("nop\n\t"); // 62.5ns delay
    digitalWrite(COUNTER_1_RESET_PIN , LOW);
    digitalWrite(COUNTER_2_RESET_PIN , LOW);
 
}

void reset_counter( int reset_pin)
{
    digitalWrite(reset_pin , HIGH);
    __asm__("nop\n\t"); // 62.5ns delay
    digitalWrite(reset_pin , LOW);
 
}

void increment_decade_counters( )
{

    digitalWrite(COUNTER_1_CLOCK_PIN , HIGH);
    digitalWrite(COUNTER_2_CLOCK_PIN , HIGH);
    __asm__("nop\n\t"); // 62.5ns delay
    digitalWrite(COUNTER_1_CLOCK_PIN , LOW);
    digitalWrite(COUNTER_2_CLOCK_PIN , LOW);

}

void increment_decade_counter(int clock_pin )
{
    digitalWrite(clock_pin, HIGH);
    __asm__("nop\n\t"); // 62.5ns delay
    digitalWrite(clock_pin, LOW);

}
uint8_t read_shift_register(int other_counter_clock_pin)
{
  // Get data from 74HC165
  uint8_t incoming1 = 0;
  uint8_t incoming2 = 0;
  uint8_t initial   = 0;

  do
  {
    // read the value from the rows
    incoming1 = read_shift_register2();

    if (!incoming1)
    {
      return 0;
    }
   
    initial   = incoming1;

    // =========================================
    // now scan across the other 9 columns of the other decade counter
    // to eliminate false key presses
    // Is any keypress detected on this decade counter column?

    increment_decade_counter( other_counter_clock_pin );
    for( uint8_t i=0; i<9; i++)
    {
      if (incoming1)
      {
        incoming2 = read_shift_register2();
        incoming1 = incoming1 & incoming2;
      }
      increment_decade_counter( other_counter_clock_pin );
    }
    // Has the shift register changed value while we've been scanning? if so, try again
  } while(initial != read_shift_register2());

  return incoming1;
}

void decode( uint8_t column, uint8_t incoming_byte)
{
  char key;
  uint8_t bit_selector;
  uint8_t last_incoming_byte;
  
  last_incoming_byte = last_incoming[column];

  // is a key pressed, or might a key be released?
  if (incoming_byte || last_incoming_byte)
  {

    for(uint8_t row = 0; row<8; row++)
    {
      key = keyboard_map_char[column][row];
      if (key)
      {
        bit_selector = (1<<row);
        if (incoming_byte & bit_selector )
        {
          if ( !(  last_incoming_byte & bit_selector ) )
          {
#if DEBUG
            Serial.println("-----------------------------------");
            Serial.print("R/C [");
            Serial.print(row);
            Serial.print("/");
            Serial.print(column);
            Serial.println("]");
            Serial.print("I/LI [");
            Serial.print(incoming_byte,BIN);
            Serial.println("/");
            Serial.print(last_incoming_byte,BIN);
            Serial.println("]");
            Serial.print("  Pressing [");
            Serial.print(keyboard_map_string[column][row]);
            Serial.println("]");
#endif
            Keyboard.press(key);
            application_on = true;
            delay(50); // debounce
          }
        }
        else if ( last_incoming_byte & bit_selector )
        {
#if DEBUG
            Serial.println("-----------------------------------");
            Serial.print("R/C [");
            Serial.print(row);
            Serial.print("/");
            Serial.print(column);
            Serial.println("]");
            Serial.print("I/LI [");
            Serial.print(incoming_byte,BIN);
            Serial.println("/");
            Serial.print(last_incoming_byte,BIN);
            Serial.println("]");
            Serial.print("  Releasing [");
            Serial.print(keyboard_map_string[column][row]);
            Serial.println("]");
#endif
            Keyboard.release(key);
            application_on = false;
        }
      }
    }
  }
  last_incoming[column]=incoming_byte;
}

uint8_t read_shift_register2()
{

  // Enable shifting
  digitalWrite(CLOCK_PIN, HIGH);
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
  int r,g,b;

  // spread led update workload over 32x1ms timeslots to avoid spikes every millisecond
  switch( (millis() & 0b00011111) )
  {
    case 0b00000000:  // red
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
        switch(effect)
        {
          case 1:
          case 2:     
            r = (millis() % 10240)/20;
            analogWrite(RED_PIN,   (r<256)?r:511-r );
            break;
        }
      }
      break;
    case 0b00001000: // green
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
        switch(effect)
        {
          case 1:
            g = (millis() % 12800)/25;
            analogWrite(GREEN_PIN, (g<256)?g:511-g );
            break;
          case 2:
            analogWrite(GREEN_PIN, 0 );
            break;
        }
      }
      break;
    case 0b00010000:
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
        switch(effect) //blue
        {
          case 1:
            b = (millis() % 11264)/22;
            analogWrite(BLUE_PIN,  (b<256)?b:511-b );
            break;
          case 2:
            analogWrite(BLUE_PIN, 0 );
            break;
        }
      }
      break;
    case 0b00011000:
      if (led_status_update)
      {
        caps_lock_on   = Keyboard.getLedStatus(LED_CAPS_LOCK);
        scroll_lock_on = Keyboard.getLedStatus(LED_SCROLL_LOCK);
        num_lock_on    = Keyboard.getLedStatus(LED_NUM_LOCK);
        keyboard_status_leds.setPixelColor(CAPS_LOCK_LED,   keyboard_status_leds.Color((caps_lock_on)?255:0,   0,                       0));
        keyboard_status_leds.setPixelColor(SCROLL_LOCK_LED, keyboard_status_leds.Color(0,                      (scroll_lock_on)?255:0,  0));
        keyboard_status_leds.setPixelColor(NUM_LOCK_LED,    keyboard_status_leds.Color(0,                      0,                       (num_lock_on)?255:0));
        led_status_update = false;
      }
      keyboard_status_leds.setPixelColor(APPLICATION_LED, keyboard_status_leds.Color((application_on)?255:0, (application_on)?255:0, 0));
      keyboard_status_leds.setPixelColor(POWER_LED,       keyboard_status_leds.Color((power_on)?255:0,       (power_on)?255:0,       (power_on)?255:0));
      keyboard_status_leds.show();
      break;
  }
} 

void keyboardLedsStatusReportCallback()
{
  led_status_update = true;
}
