@echo off
setlocal
set PATH=C:\msys64\ucrt64\bin;%PATH%
for /f "delims=" %%i in ('pkg-config --cflags gtk+-3.0') do set CFLAGS=%%i
for /f "delims=" %%i in ('pkg-config --libs gtk+-3.0') do set LIBS=%%i
gcc -std=c11 -g %CFLAGS% -Iinclude -o busmate.exe src\main.c src\gui.c src\admin.c src\file_handler.c src\seat_allocation.c %LIBS%
if errorlevel 1 exit /b 1
exit /b 0