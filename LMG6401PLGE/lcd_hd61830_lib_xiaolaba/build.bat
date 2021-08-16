:: install MinGW, https://sourceforge.net/projects/mingw/
:: goto C:\MinGW\msys\1.0
:: run msys.bat
:: dosbox will be opened
:: cd "C:\Users\user0\Desktop\InpOut32_c_test_app\VC_test_app"
:: then run this build.bat
:: tested Win10, 2021-AUG-11

g++ -static-libgcc -static-libstdc++ LCD_LIB.CPP -o LCD_LIB.exe
pause
