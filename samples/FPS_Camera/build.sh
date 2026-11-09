```bash
#!/bin/bash

BUILD_DIR="build"

echo
echo "============================"
echo "       BUILD TYPE"
echo "============================"
echo
echo "[1] DEBUG"
echo "[2] RELEASE"
echo

read -p "Select build type: " OPTION

case "$OPTION" in
    1)
        BUILD_TYPE="Debug"
        CLEAN_BUILD=0
        ;;
    2)
        BUILD_TYPE="Release"
        CLEAN_BUILD=1
        ;;
    *)
        echo "Invalid option!"
        exit 1
        ;;
esac

echo
echo "Selected: $BUILD_TYPE"

echo
echo "-- PREPARING BUILD DIRECTORY --"

mkdir -p "$BUILD_DIR"

echo
echo "-- CONFIGURING $BUILD_TYPE BUILD --"

cmake -S . -B "$BUILD_DIR" \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

if [ $? -ne 0 ]; then
    echo
    echo "CMAKE CONFIGURATION FAILED!"
    exit 1
fi

echo
echo "-- COMPILING --"

cmake --build "$BUILD_DIR" --parallel "$(nproc)"

if [ $? -ne 0 ]; then
    echo
    echo "COMPILATION FAILED!"
    exit 1
fi

echo
echo "-- COPYING PROJECT FILES --"

rsync -a \
    --exclude="$BUILD_DIR/" \
    --exclude=".git/" \
    --exclude=".vs/" \
    --exclude="CMakeFiles/" \
    --exclude="CMakeLists.txt" \
    --exclude="build.bat" \
    --exclude="build.sh" \
    --exclude="main.cpp" \
    --exclude="CMakeCache.txt" \
    --exclude="cmake_install.cmake" \
    --exclude="Makefile" \
    --exclude="install_manifest.txt" \
    --exclude="CTestTestfile.cmake" \
    --exclude="compile_commands.json" \
    ./ "$BUILD_DIR/"

if [ $? -ne 0 ]; then
    echo
    echo "FILE COPYING FAILED!"
    exit 1
fi

if [ "$CLEAN_BUILD" -eq 1 ]; then
    echo
    echo "-- CLEANING CMAKE FILES --"

    rm -rf "$BUILD_DIR/CMakeFiles"
    rm -f "$BUILD_DIR/CMakeCache.txt"
    rm -f "$BUILD_DIR/cmake_install.cmake"
    rm -f "$BUILD_DIR/Makefile"
    rm -f "$BUILD_DIR/CTestTestfile.cmake"
    rm -f "$BUILD_DIR/install_manifest.txt"
    rm -f "$BUILD_DIR/compile_commands.json"
fi

echo
echo "-- EXECUTING --"

if [ -f "$BUILD_DIR/main" ]; then
    cd "$BUILD_DIR" || exit 1
    ./main
else
    echo "main NOT FOUND!"
    exit 1
fi
```