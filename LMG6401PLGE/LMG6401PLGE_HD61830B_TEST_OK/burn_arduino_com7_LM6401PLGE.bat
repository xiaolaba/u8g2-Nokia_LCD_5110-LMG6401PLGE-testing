set ac=C:\avr8-gnu-toolchain-win32_x86

path %ac%\bin;%ac%\utils\bin;%path%;

::avr-gcc.exe -dumpversion

set HEX=LMG6401PLGE_HD61830B_TEST_OK.m168.hex
::set HEX=LMG6401PLGE_HD61830B_TEST_OK.m168.picoboot_v3b2.bootloader_only.hex
set PCB=arduino
set MCU=atmega168p
set COMPORT=COM7
set BAUD=250000
::set BAUD=115200

avrdude -v -p %MCU% -c %PCB% -P %COMPORT% -b %BAUD% -D -Uflash:w:%HEX%:a


pause

