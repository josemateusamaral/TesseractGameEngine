@echo off

echo -- CLEANING --

if exist test.exe del /q test.exe
if exist CMakeCache.txt del /q CMakeCache.txt
if exist CMakeFiles rmdir /s /q CMakeFiles
if exist Makefile del /q Makefile

echo.
echo -- CONFIGURING RELEASE BUILD --

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release .

if errorlevel 1 (
    echo.
    echo CMAKE CONFIGURATION FAILED!
    pause
    exit /b 1
)

echo.
echo -- COMPILING --

mingw32-make -j%NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo.
    echo COMPILATION FAILED!
    pause
    exit /b 1
)

echo.
echo -- EXECUTING --

test.exe

pause