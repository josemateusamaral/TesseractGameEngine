cmake_minimum_required(VERSION 3.10)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------
# Dependências
# ---------------------------------------------------------

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenMP REQUIRED)

# ---------------------------------------------------------
# Include directories
# ---------------------------------------------------------

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    core
)

# ---------------------------------------------------------
# Source files
# ---------------------------------------------------------

set(SOURCES
    test.cpp

    core/tesseract.cpp
    core/window.cpp
    core/camera.cpp
    core/vec3.cpp
    core/model.cpp
    core/texture.cpp
    core/input.cpp
    core/scene.cpp
    core/light.cpp
    core/gui.cpp
    core/analitycs.cpp
    core/audio.cpp
    core/renderer.cpp
)

# ---------------------------------------------------------
# Executable
# ---------------------------------------------------------

add_executable(test ${SOURCES})

# ---------------------------------------------------------
# Compiler optimizations - Release
# ---------------------------------------------------------

if(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    target_compile_options(test PRIVATE
        $<$<CONFIG:Release>:
            -O3
            -march=native
            -mtune=native
            -ffast-math
            -funroll-loops
            -fomit-frame-pointer
            -DNDEBUG
        >
    )

endif()

# ---------------------------------------------------------
# Link libraries
# ---------------------------------------------------------

target_link_libraries(
    test
    ${SDL2_LIBRARIES}
    assimp
    SDL2_ttf
    OpenMP::OpenMP_CXX
)