@echo off

echo -- CLEANING --
del test.exe 2>nul

echo -- BUILDING --
cmake -G "MinGW Makefiles"

echo -- COMPILING --
mingw32-make

echo -- EXECUTING --
test.exe