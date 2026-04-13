cmake_minimum_required(VERSION 3.10)

project(teste_projecao)

set(CMAKE_CXX_STANDARD 11)

# Encontrar SDL2
find_package(SDL2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    core
)

# Lista de arquivos fonte
set(SOURCES
    teste.cpp

    core/window.cpp
    core/camera.cpp
    core/vec3.cpp
    core/model.cpp
    core/texture.cpp
    core/input.cpp
    core/ponto.cpp
    core/ponto3.cpp
)

# Criar executável
add_executable(teste ${SOURCES})

# Linkar SDL2
target_link_libraries(teste ${SDL2_LIBRARIES} assimp)