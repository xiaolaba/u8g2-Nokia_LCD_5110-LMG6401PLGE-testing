  //ref :https://juvar.fi/?tag=lcd
  
    
    
    /* initialization sequence and cls function taken from lcd_hd61830_lib, cdragos69@yahoo.com
       http://www.skippari.net/lcd/sekalaista/lcd_hd61830_lib.zip
       found from http://forum.lcdinfo.com/viewtopic.php?t=451
    */
    // pin assignment
    const byte E_PIN = 4;
    const byte RW_PIN = 3;
    const byte RS_PIN = 2;
    const byte DB7_PIN = 12;
    const byte DB6_PIN = 11;
    const byte DB5_PIN = 10;
    const byte DB4_PIN = 9;
    const byte DB3_PIN = 8;
    const byte DB2_PIN = 7;
    const byte DB1_PIN = 6;
    const byte DB0_PIN = 5;
    
    void setup() {
      // Permanently tied up E pin
      digitalWrite(E_PIN, HIGH);
      delay(2000);
      com(word(0x00, 0x38)); // Display ON, master mode, cursor off char blink, text mode, internal CG-ROM
      com(word(0x01, 0x7a)); // 6x8 font
      com(word(0x02, 0x27)); // 40 horizontal characters
      com(word(0x03, 0x7F)); // 1/128 duty cycle (128 lines)
      com(word(0x04, 0x07)); // 8-lines cursor
      com(word(0x08, 0x00)); // Display start address low
      com(word(0x09, 0x00)); // Display start address high
      com(word(0x0A, 0x00)); // Cursor address low
      com(word(0x0B, 0x00)); // Cursor address high
      cls();


      com(word(B00001100, B01001000)); //letter H
      com(word(B00001100, B01100101)); //letter e
      com(word(B00001100, B01101100)); //letter l
      com(word(B00001100, B01101100)); //letter l
      com(word(B00001100, B01101111)); //letter o
      com(word(B00001100, B00100000)); //space
      com(word(B00001100, B01110111)); //letter w
      com(word(B00001100, B01101111)); //letter o
      com(word(B00001100, B01110010)); //letter r
      com(word(B00001100, B01101100)); //letter l
      com(word(B00001100, B01100100)); //letter d

      //LCD_init_graph();
    
    }
    void loop() {
      // nothing here
      //cls();
      //delay(1000);
      //com(word(B00001100, B01001111)); //letter H
      
    }
    
    void cls() {
      // clear screen
      com(word(0x0A, 0x00)); // Cursor address low
      com(word(0x0B, 0x00)); // Cursor address high
      for(int i=0;i<320;i++) com(word(0x0C,0)); // reset all text memory
      com(word(0x0A, 0x00)); // Cursor address low
      com(word(0x0B, 0x00)); // Cursor address high
    }

//RS : Register Select, 1:instruction, 0:data
#define set_instruction digitalWrite(RS_PIN, HIGH);
#define set_data        digitalWrite(RS_PIN, LOW);

// R/W : Read / Write : 1:read, 0:write
#define set_write digitalWrite(RW_PIN, LOW);
#define set_read  digitalWrite(RW_PIN, HIGH);

// E : falling edge, data is latched
#define latch  digitalWrite(E_PIN, LOW);
#define unlatch  digitalWrite(E_PIN, HIGH);
    
    void com(word dat) {
      digitalWrite(LED_BUILTIN, HIGH); // data flow indication on
      delay(500);

      //unlatch;
      //delay (1);
      
      //set_write;
      digitalWrite(RW_PIN, LOW);
      
      //set_instruction;
      digitalWrite(RS_PIN, HIGH);

      digitalWrite(DB7_PIN, bitRead(dat, 15));
      digitalWrite(DB6_PIN, bitRead(dat, 14));
      digitalWrite(DB5_PIN, bitRead(dat, 13));
      digitalWrite(DB4_PIN, bitRead(dat, 12));
      digitalWrite(DB3_PIN, bitRead(dat, 11));
      digitalWrite(DB2_PIN, bitRead(dat, 10));
      digitalWrite(DB1_PIN, bitRead(dat, 9));
      digitalWrite(DB0_PIN, bitRead(dat, 8));

      //latch;
      //delay (1);
      //unlatch;
      //delay (1); 
      
      //set_write;
      digitalWrite(RW_PIN, LOW);
      //set_data;     
      digitalWrite(RS_PIN, LOW);

      digitalWrite(DB7_PIN, bitRead(dat, 7));
      digitalWrite(DB6_PIN, bitRead(dat, 6));
      digitalWrite(DB5_PIN, bitRead(dat, 5));
      digitalWrite(DB4_PIN, bitRead(dat, 4));
      digitalWrite(DB3_PIN, bitRead(dat, 3));
      digitalWrite(DB2_PIN, bitRead(dat, 2));
      digitalWrite(DB1_PIN, bitRead(dat, 1));
      digitalWrite(DB0_PIN, bitRead(dat, 0));

      //latch;
      //delay (1);      
      //unlatch;
      //delay (1); 
      
      digitalWrite(LED_BUILTIN, LOW); // data flow indication off
      //wait for busy flag to go away
      digitalWrite(E_PIN, HIGH);
      digitalWrite(RW_PIN, HIGH);
      digitalWrite(RS_PIN, HIGH);
      do {
        delay(10);
      } while (digitalRead(DB7_PIN) == HIGH);
    }



void LCD_init_graph()
{
    com(word(0x0B, 0x00)); // Cursor address high
    
    com(word(0x00, 0x3c));          // Display ON, master mode, graphic mode
    com(word(0x01, 0x75));          // 8 bits / character
    com(word(0x02, 0x1D));          // 30 horizontal bytes - 240 pixels
    com(word(0x03, 0x4F));          // 1/128 duty cycle (128 lines)
    com(word(0x04, 0x07));          // Cursor at bottom line
    com(word(0x08, 0x00)); // Display start address low
    com(word(0x09, 0x00));  // Display start address high
    com(word(0x0a, 0x00));  // Cursor address low
    com(word(0x0b, 0x00)); // Cursor address high
//    current_x=0;                        // initial current position
//    current_y=0;
}
