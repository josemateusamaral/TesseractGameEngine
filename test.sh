#!/bin/bash

echo "cleaning files..;"
rm -f test

echo "Creating Makefile..."
cmake CMakeLists.txt

echo "Compiling..."
make

echo "Executing..."
./test