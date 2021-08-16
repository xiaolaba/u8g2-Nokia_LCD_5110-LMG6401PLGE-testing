:: by xiaolaba, 2021-AUG-10

:: add tool chain to path
path=%PATH%;C:\avr8-gnu-toolchain-win32_x86\utils\bin;C:\avr8-gnu-toolchain-win32_x86\bin

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

goto end
:: burn firmware
set mcu=atmega328p
set target=LMG6401PLGE_lcd.hex
avrdude -c usbtiny -p %mcu% -U flash:w:%target%:a

:end
pause