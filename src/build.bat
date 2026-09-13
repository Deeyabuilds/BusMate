@echo off
set PATH=C:\msys64\ucrt64\bin;%PATH%
for /f "delims=" %%i in ('pkg-config --cflags gtk+-3.0') do set CFLAGS=%%i
for /f "delims=" %%i in ('pkg-config --libs gtk+-3.0') do set LIBS=%%i
gcc %CFLAGS% -Iinclude -o busmate.exe src\main.c src\gui.c src\admin.c src\file_handler.c %LIBS%
pause