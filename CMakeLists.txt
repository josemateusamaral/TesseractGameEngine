cmake_minimum_required(VERSION 3.10)

project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Threads REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    core
)

# implementation files
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

# create executable
add_executable(test ${SOURCES})

# Link SDL2
target_link_libraries(
    test 
    ${SDL2_LIBRARIES} 
    assimp 
    SDL2_ttf
    Threads::Threads
)