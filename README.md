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

![Mega2560_u8g2_LCD5110_HelloWorld/img/grey_only_2_lcd_ok.JPG](Mega2560_u8g2_LCD5110_HelloWorld/img/grey_only_2_lcd_ok.JPG)  
