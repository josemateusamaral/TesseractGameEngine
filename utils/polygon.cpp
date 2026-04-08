#include "../pontos/ponto3.h"
#include "polygon.h"

/**
 * @brief Construtor do sólido geométrico Esfera
 * 
 * @param posicao posição do centro no R3
 * @param tamanho Raio da esfera
 * @param lod Quantidade de pontos, para distorção
 *
 * @authors Jose Mateus Amaral
 */
Polygon::Polygon(Ponto3 vertice1, Ponto3 vertice2, Ponto3 vertice3)
{
    this->vertice1 = vertice1;
    this->vertice2 = vertice2;
    this->vertice3 = vertice3;
}