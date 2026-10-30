cmake_minimum_required(VERSION 3.10)

project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Engine root directory
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# ---------------------------------------------------------
# Dependences
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
    ${ENGINE_DIR}/core
)

# ---------------------------------------------------------
# Source files
# ---------------------------------------------------------

set(SOURCES
    main.cpp

    ${ENGINE_DIR}/core/tesseract.cpp
    ${ENGINE_DIR}/core/window.cpp
    ${ENGINE_DIR}/core/camera.cpp
    ${ENGINE_DIR}/core/vec3.cpp
    ${ENGINE_DIR}/core/model.cpp
    ${ENGINE_DIR}/core/texture.cpp
    ${ENGINE_DIR}/core/input.cpp
    ${ENGINE_DIR}/core/scene.cpp
    ${ENGINE_DIR}/core/light.cpp
    ${ENGINE_DIR}/core/gui.cpp
    ${ENGINE_DIR}/core/analitycs.cpp
    ${ENGINE_DIR}/core/audio.cpp
    ${ENGINE_DIR}/core/renderer.cpp
)

# ---------------------------------------------------------
# Executable
# ---------------------------------------------------------

add_executable(main ${SOURCES})


# ---------------------------------------------------------
# Copy assets
# ---------------------------------------------------------

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ENGINE_DIR}/core/assets
    $<TARGET_FILE_DIR:main>/assets/engine_assets
)


# ---------------------------------------------------------
# Compiler optimizations - Release
# ---------------------------------------------------------


if(MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    target_compile_options(main PRIVATE
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
    main
    ${SDL2_LIBRARIES}
    assimp
    SDL2_ttf
    OpenMP::OpenMP_CXX
)