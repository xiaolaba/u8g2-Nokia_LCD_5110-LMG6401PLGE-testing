# u8g2-Nokia_LCD_5110-LMG6401PLGE-testing
try to uses u8g2 LCD library, some good some odd

### [LMG6401PLGE](LMG6401PLGE), HD61830, controller, testing NG, did not know why
all files/modified and build.bat available

HITACHI LCD LMG6401PLGE, 240x128
chips:
HD61830, controller
LC7940, x-driver x3, 80 col x3 = 240 pixel of width
LC7942, y-driver x2, 64 row x2 = 128 pixel of height

```
copy from,
https://www.youtube.com/watch?v=WmnEPF2JpjA
https://sergeev.io/projects/lc7981_driver/

https://github.com/LongHairedHacker/avr-lc7981

http://www.skippari.net/lcd/sekalaista/lcd_hd61830_lib.zip
http://forum.lcdinfo.com/viewtopic.php?t=451 HD61830 programing (VB); graphic LCD
https://juvar.fi/?p=34, Controlling graphic dot matrix LCD display

http://www.skippari.net/lcd/sekalaista/lcd_hd61830_lib.zip
http://forum.lcdinfo.com/viewtopic.php?t=451 HD61830 programing (VB); graphic LCD
https://juvar.fi/?p=34, Controlling graphic dot matrix LCD display


modified Makefile and codes
setup build.bat for win10 and Microchio AVR8 toolchain

build is ok
testing, NG
LCD failure ? or library is not supporting HD61830/LC7940/LC7942 ? did not know

HITACHI LCD LMG6401PLGE, 240x128
chips:
HD61830, controller
LC7940, x-driver x3, 80 col x3 = 240 pixel of width
LC7942, y-driver x2, 64 row x2 = 128 pixel of height

2021-AUG-10 by xiaolaba

```



### Nokia 5110 LCD, controller PCD8544  
testing ok, notes,  
```
//// xiaolaba, testing, 2021-AUG-10
//// software SPI mode, any port pin is ok
//// dc : Data/Command pin, or RS (Register Select) pin
//// U8G2_Rx, LCD image orientations
//U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);  // Nokia 5110 DisplayU8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 7, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);  // Nokia 5110 Display
//U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R1, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);  // Nokia 5110 Display
U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R2, /* clock=*/ 7, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);  // Nokia 5110 Display
//U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R3, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);  // Nokia 5110 Display

//// hardware SPI mode, CLK / DATA pin is specific for every MCU
//U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R0,                                  /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);   // Nokia 5110 Display
//U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R1,                                  /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);   // Nokia 5110 Display
//U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R2,                                  /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);   // Nokia 5110 Display
//U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R3,                                  /* cs=*/ 8, /* dc=*/ 10, /* reset=*/ 9);   // Nokia 5110 Display

// lcd2image produced image array
// change 1st line to uses flash memory for ROM stroage, save RAM used
// const unsigned char gImage_xiao3[528] = { /* 0X20,0X01,0X54,0X00,0X30,0X00, */
// const unsigned char gImage_xiao3[528] PROGMEM = { /* 0X20,0X01,0X54,0X00,0X30,0X00, */
#include "img\xiao1.h"
#include "img\xiao2.h"
#include "img\xiao3.h"
#include "img\xiao4.h"
```

![Mega2560_u8g2_LCD5110_HelloWorld/img/grey_only_2_lcd_ok.JPG](Mega2560_u8g2_LCD5110_HelloWorld/img/grey_only_2_lcd_ok.JPG)  

complete project files,
[Mega2560_u8g2_LCD5110_HelloWorld](Mega2560_u8g2_LCD5110_HelloWorld)

demo video,
[https://www.youtube.com/watch?v=h8Ii3jQoLog](https://www.youtube.com/watch?v=h8Ii3jQoLog)
