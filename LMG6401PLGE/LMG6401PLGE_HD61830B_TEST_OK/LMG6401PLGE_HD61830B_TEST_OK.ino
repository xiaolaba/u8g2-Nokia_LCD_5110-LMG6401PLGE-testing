// xiaolaba, 2012
// LMG6401PLGE 240x128 LCD, controller HD68130B, testing, (!datasheet VEE -15Vm, battery symbol was wrong with polarity)
// atmega168p, rom used 26%, ram used 36%
//
// update 2021-MAY, reccompile, avr-gcc or Arduino IDE
/*
 * update
   ref:
   copy https://github.com/olikraus/u8g2/issues/939
   initialization sequence and cls function taken from lcd_hd61830_lib, cdragos69@yahoo.com
   http://www.skippari.net/lcd/sekalaista/lcd_hd61830_lib.zip
   found from http://forum.lcdinfo.com/viewtopic.php?t=451
   Code from this source - http://juvar.mbnet.fi/blog/?tag=hd61830 - Edited by Gunner to work on older 120x32 LCD with HD61830A00
*/

// pin assignment, uses port pin# if MCU& use avr-gcc directly mega168p / mega328p
///// following, Arduino nano pin#
#define LED     13  // builtin LED, nano/uno/mega ADK
///////// LCD PIN# |Arduino PIN#. 
//#define VSS       GND
//#define VDD       +5V
//#define Vo        between +5V ~ -15V (-12V is also ok), LCD contrast
  #define RS_PIN    12
  #define RW_PIN    11
  #define E_PIN     10
  #define DB0_PIN   9
  #define DB1_PIN   8
  #define DB2_PIN   7
  #define DB3_PIN   6
  #define DB4_PIN   5
  #define DB5_PIN   4
  #define DB6_PIN   3
  #define DB7_PIN   2
//#define /CS_PIN   +5V
//#define /RST7_PIN GND
//#define VEE       -15V
//#define NC        
//#define NC         
//#define NC         
/*
// Mega ADK
#define LED     13  // builtin LED, nano/uno/mega ADK
const byte RS_PIN = 37;
const byte RW_PIN = 36;
const byte E_PIN = 35;

const byte DB0_PIN = 22;
const byte DB1_PIN = 23;
const byte DB2_PIN = 24;
const byte DB3_PIN = 25;
const byte DB4_PIN = 26;
const byte DB5_PIN = 27;
const byte DB6_PIN = 28;
const byte DB7_PIN = 29;
*/


#include <avr/pgmspace.h>

//my ROM strings, stored in flash memory
const char LCM_NAME          [] PROGMEM = {"LCM module: HITACHI LMG6401PLGE. HD61830B / LC7981 LCD controller testing, pin# and connection table"};
const char gridline          [] PROGMEM = {"+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+-----+-----+----+----+----+"};
const char LCM_PIN           [] PROGMEM = {"| A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8 | A9 | A10| A11| A12| A13| A14| A15| A16 | A17 | A18| A19| A20| LMG6401PLGE"};
const char LCM_PIN_FUNCTION  [] PROGMEM = {"| GND| +5V| Vo | RS | R/W| E  | D0 | D1 | D2 | D3 | D4 | D5 | D6 | D7 | /CS| /RST| -15V|  ? | ?  | ?  | Function"};
//const char gridline        [] PROGMEM = {"+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+-----+-----+----+----+----+"};
const char ARDUINO_NANO_PIN  [] PROGMEM = {"| GND| +5V|    | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  |    |     |     |    |    |    | Arduino"};
//const char gridline        [] PROGMEM = {"+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+-----+-----+----+----+----+"};
const char MEGA328_PIN       [] PROGMEM = {"| GND| +5V|    | PB4| PB3| PB2| PB1| PB0| PD7| PD6| PD5| PD4| PD3| PD2|    |     |     |    |    |    | Atmega328P"};
//const char gridline        [] PROGMEM = {"+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+-----+-----+----+----+----+"};

// Then set up a table to all ROM strings.
//const char *const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5};
const char *const string_table[] PROGMEM = {LCM_NAME, 
                                            gridline,
                                            LCM_PIN,
                                            LCM_PIN_FUNCTION,
                                            gridline,
                                            ARDUINO_NANO_PIN,
                                            gridline,
                                            MEGA328_PIN,
                                            gridline};
char buffer[sizeof (LCM_PIN)];  // make sure this is large enough for the longest string it must hold

const char DEMO              [] PROGMEM = {"Xiaolaba, 2012-MAY-11, HITACHI LMG6401PLGE. chip HD61830B, testing"};
const char *const romStringPtr  PROGMEM = DEMO;


void setup() {

  Serial.begin(115200);

  //// uart("hello world");
  //Serial.println(F("hello world"));
 
  // number of element in array = total byte of array / byte of any one of element (data_type)
  int number_of_string = sizeof(string_table) / sizeof(string_table[0]);  

  //// print my ROM strings to serial terminal, for debug purpose
  //for (int i = 0; i <= 5; i++) {
  for (int i = 0; i < number_of_string; i++) {
    // copy ROM string to buffer
    strcpy_P(buffer, (char *) pgm_read_ptr( & (string_table[i]) ) );
    Serial.println(buffer);
    //delay(500);
  }

  // print single DEMO string serial terminal, for debug purpose
  strcpy_P(buffer, (char *) pgm_read_ptr( &romStringPtr ));
  Serial.println(buffer);
  
  
  // init MCU IO, mega328p, portB & portD = Ardunio Nano D0 ~ D13, all output pins to HD68130
  DDRD = 0xff;  
  DDRB = 0xff;  

  pinMode(LED, OUTPUT);

  // Permanently tied up E pin, enable LCD
  digitalWrite(E_PIN, HIGH);

  delay(2000);
  // Init LCM, 240x128 dots
  com(word(0x00, 0x3C)); // Display ON, master mode, cursor on/blink, text mode, internal CG-ROM
  com(word(0x01, 0x75)); // 6x8 font (including spacing) vertical 8 dot -1 = 7, horizontal 6 dot -1, -> 0x75
  com(word(0x02, 240/6-1)); // 0~39 = 240 dot/6dot of each char = 40, horizontal characters
  com(word(0x03, 0x1F)); // 120/4 duty cycle (32 lines)
  com(word(0x04, 0x07)); // cursor on 8th line (of character)
  com(word(0x08, 0x00)); // Display start address low
  com(word(0x09, 0x00)); // Display start address high
  com(word(0x0A, 0x00)); // Cursor address low
  com(word(0x0B, 0x00)); // Cursor address high

  cls(); // clear LCD display RAM, LCD screen will be clean up
  
  // each char of buffer, dump to LCD display RAM, screen will be seen
  for (int i=0; i<sizeof(DEMO); i++) {
    com(word(B00001100, buffer[i]));    // buffer loaded DEMO string at previous
  }

/*
  // each char of buffer, dump to LCD display RAM, screen will be seen
  // HD61830 controller interal CGROM, ASCII map 0x20 ~ 0xFF, same as HD44780, 
  com(word(B00001100, 'X'));
  com(word(B00001100, 'i'));
  com(word(B00001100, 'a'));
  com(word(B00001100, 'o'));
  com(word(B00001100, 'l'));
  com(word(B00001100, 'a'));
  com(word(B00001100, 'b'));  
  com(word(B00001100, 'a')); 

  com(word(B00001100, B01001000)); // H
  com(word(B00001100, B01100101)); // e
  com(word(B00001100, B01101100)); // l
  com(word(B00001100, B01101100)); // l
  com(word(B00001100, B01101111)); // o
  */
}



void loop() {
  //dummy loop for testing only
  com(word(B00001100, '>')); // dump to LCD display RAM, screen will be seen
  delay(1000); // visual test for LED blinking
}



void cls() {
  // clear screen
  com(word(0x0A, 0x00)); // Cursor address low
  com(word(0x0B, 0x00)); // Cursor address high
  for (int i = 0; i < 240; i++) com(word(0x0C,0)); // LCD display RAM, cleanup
  com(word(0x0A, 0x00)); // Cursor address low
  com(word(0x0B, 0x00)); // Cursor address high
}



// routine of sending instruciton/data to controller HD61830
// HD61830 (1.4MHZ)  / HD61830B (2.4MHZ)
void com(word dat) {
  digitalWrite(LED, HIGH); // LED on, data flow indication on, for eyes

  // 8bit comamnd to HD61830 Instruction-Register
  digitalWrite(RW_PIN, LOW);
  digitalWrite(RS_PIN, HIGH);
  digitalWrite(DB7_PIN, bitRead(dat, 15));
  digitalWrite(DB6_PIN, bitRead(dat, 14));
  digitalWrite(DB5_PIN, bitRead(dat, 13)); 
  digitalWrite(DB4_PIN, bitRead(dat, 12));
  digitalWrite(DB3_PIN, bitRead(dat, 11));
  digitalWrite(DB2_PIN, bitRead(dat, 10));
  digitalWrite(DB1_PIN, bitRead(dat, 9));
  digitalWrite(DB0_PIN, bitRead(dat, 8));

  // 8bit data to HD61830 Data-Register
  digitalWrite(RW_PIN, LOW);
  digitalWrite(RS_PIN, LOW);
  digitalWrite(DB7_PIN, bitRead(dat, 7));
  digitalWrite(DB6_PIN, bitRead(dat, 6));
  digitalWrite(DB5_PIN, bitRead(dat, 5));
  digitalWrite(DB4_PIN, bitRead(dat, 4));
  digitalWrite(DB3_PIN, bitRead(dat, 3));
  digitalWrite(DB2_PIN, bitRead(dat, 2));
  digitalWrite(DB1_PIN, bitRead(dat, 1));
  digitalWrite(DB0_PIN, bitRead(dat, 0));

  //wait for busy flag to go away
  digitalWrite(E_PIN, HIGH);
  digitalWrite(RW_PIN, HIGH);
  digitalWrite(RS_PIN, HIGH);
  do {
    delay(10);
  } while (digitalRead(DB7_PIN) == HIGH); // check LCD busy flag



  // debug purpose, dump dat (16bit command/data to serial terminal
  Serial.println(F("\nBits of dat (16 bit) ")); //binary format
  Serial.print(bitRead(dat, 15));
  Serial.print(bitRead(dat, 14));
  Serial.print(bitRead(dat, 13));
  Serial.print(bitRead(dat, 12));
  Serial.print(bitRead(dat, 11));
  Serial.print(bitRead(dat, 10));
  Serial.print(bitRead(dat, 9));
  Serial.print(bitRead(dat, 8));
  Serial.print(" "); 
  Serial.print(bitRead(dat, 7));
  Serial.print(bitRead(dat, 6));
  Serial.print(bitRead(dat, 5));
  Serial.print(bitRead(dat, 4));
  Serial.print(bitRead(dat, 3));
  Serial.print(bitRead(dat, 2));
  Serial.print(bitRead(dat, 1));
  Serial.print(bitRead(dat, 0));
  Serial.print(" ");

  // hex formaat
  Serial.print((", dat = LCD given instruction | data = "));

  byte MSB = (dat >> 8) & 0xFF;
  byte LSB = dat & 0xFF;

  // ASCII format
  Serial.print("\nMSB: "); Serial.print(MSB, BIN); Serial.print(" 0x"); Serial.print(MSB, HEX);
  Serial.print(", LSB: "); Serial.print(LSB, BIN); Serial.print(" 0x"); Serial.print(LSB, HEX); Serial.print(" ASCII= "); Serial.print(char(LSB));
  Serial.println();

  // done
  digitalWrite(LED, LOW); // LED off, data flow indication off

}
