cmake_minimum_required(VERSION 3.10)

project(teste_projecao)

set(CMAKE_CXX_STANDARD 11)

# Encontrar SDL2
find_package(SDL2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    utils
    pontos
    formas
    formas/cubo
    formas/esfera
    formas/cilindro
    formas/cone
)

# Lista de arquivos fonte
set(SOURCES
    teste.cpp

    utils/janela.cpp
    utils/camera.cpp
    utils/Vec3.cpp
    utils/model.cpp
    utils/polygon.cpp
    utils/input.cpp

    pontos/ponto.cpp
    pontos/ponto3.cpp

    formas/forma.cpp
    formas/cubo/cubo.cpp
    formas/esfera/esfera.cpp
    formas/cilindro/cilindro.cpp
    formas/cone/cone.cpp
)

# Criar executável
add_executable(teste ${SOURCES})

# Linkar SDL2
target_link_libraries(teste ${SDL2_LIBRARIES} assimp)