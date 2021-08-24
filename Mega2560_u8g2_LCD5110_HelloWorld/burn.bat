:: by xiaolaba, 2021-AUG-10

:: download https://www.microchip.com/en-us/development-tools-tools-and-software/gcc-compilers-avr-and-arm
:: AVR 8-bit Toolchain v3.62 ¡V Windows
:: installed as,
:: gcc version 5.4.0 (AVR_8_bit_GNU_Toolchain_3.6.2_1778)

:: add tool chain to path
path=%PATH%;C:\avr8-gnu-toolchain-win32_x86\utils\bin;C:\avr8-gnu-toolchain-win32_x86\bin

:: show gcc version 5.4.0 (AVR_8_bit_GNU_Toolchain_3.6.2_1778)
avr-gcc -v


:: clean up old project file
make clean

:: build hex firmware
:: display redirect to file of std, speeding up, no screen showing
make hex >> std

:: use makefile to burn hex
make writeflash

:: remove files after hex build
rm std
rm *.o
rm *.ee.hex
rm *.out
rm *.map

::goto end
:: burn firmware, compile with mega328p
set mcu=atmega328p
set target=Mega2560_u8g2_LCD5110_HelloWorld.ino.with_bootloader.standard.hex
avrdude -c usbtiny -p %mcu% -U flash:w:%target%:a

:end
pause