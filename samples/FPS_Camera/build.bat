
@echo off
setlocal

set "BUILD_DIR=build"

echo.
echo ============================
echo       BUILD TYPE
echo ============================
echo.
echo [1] DEBUG
echo [2] RELEASE
echo.

choice /c 12 /n /m "Select build type: "

if errorlevel 2 (
    set "BUILD_TYPE=Release"
    set "CLEAN_BUILD=1"
) else (
    set "BUILD_TYPE=Debug"
    set "CLEAN_BUILD=0"
)

echo.
echo Selected: %BUILD_TYPE%

echo.
echo -- PREPARING BUILD DIRECTORY --

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo.
echo -- CONFIGURING %BUILD_TYPE% BUILD --

cmake -S . -B "%BUILD_DIR%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

if errorlevel 1 (
    echo.
    echo CMAKE CONFIGURATION FAILED!
    pause
    exit /b 1
)

echo.
echo -- COMPILING --

cmake --build "%BUILD_DIR%" --parallel %NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo.
    echo COMPILATION FAILED!
    pause
    exit /b 1
)

echo.
echo -- COPYING PROJECT FILES --

robocopy . "%BUILD_DIR%" /E /XD "%BUILD_DIR%" ".git" ".vs" "CMakeFiles" /XF "CMakeLists.txt" "build.bat" "build.sh" "main.cpp" "CMakeCache.txt" "cmake_install.cmake" "Makefile" "install_manifest.txt" "CTestTestfile.cmake" "compile_commands.json" /R:1 /W:1

if errorlevel 8 (
    echo.
    echo FILE COPYING FAILED!
    pause
    exit /b 1
)

if "%CLEAN_BUILD%"=="1" (
    echo.
    echo -- CLEANING CMAKE FILES --

    if exist "%BUILD_DIR%\CMakeFiles" rmdir /s /q "%BUILD_DIR%\CMakeFiles"
    if exist "%BUILD_DIR%\CMakeCache.txt" del /q "%BUILD_DIR%\CMakeCache.txt"
    if exist "%BUILD_DIR%\cmake_install.cmake" del /q "%BUILD_DIR%\cmake_install.cmake"
    if exist "%BUILD_DIR%\Makefile" del /q "%BUILD_DIR%\Makefile"
    if exist "%BUILD_DIR%\CTestTestfile.cmake" del /q "%BUILD_DIR%\CTestTestfile.cmake"
    if exist "%BUILD_DIR%\install_manifest.txt" del /q "%BUILD_DIR%\install_manifest.txt"
    if exist "%BUILD_DIR%\compile_commands.json" del /q "%BUILD_DIR%\compile_commands.json"
)

echo.
echo -- EXECUTING --

if exist "%BUILD_DIR%\main.exe" (
    pushd "%BUILD_DIR%"
    main.exe
    popd
) else (
    echo main.exe NOT FOUND!
    pause
    exit /b 1
)

pause
