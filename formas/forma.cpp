#include "forma.h"
#include "../pontos/ponto3.h"

/**
 * @brief Construtor de um sólido no R3
 * 
 * @param posicao Ponto central da forma no R3
 * @param tamanho Tamanho da forma
 */
Forma::Forma(Ponto3 posicao, double tamanho)
:posicao{posicao},angulo{Ponto3(0.0,0.0,0.0)},tamanho{tamanho}
{
}


/**
 * @brief A partir do centro e do tamanho do sólido, calcula a posição dos pontos restantes.
 * @author Jose Mateus Amaral, Monique
 */
void Forma::calcular_pontos_3D()
{
    double px = posicao.x-camera->posicao.x;
    double py = posicao.y-camera->posicao.y;
    double pz = posicao.z-camera->posicao.z;
    double t = tamanho;

    Ponto3* pontosTemp = (Ponto3*)malloc(sizeof(Ponto3) * quantidadePontos);
    for(int i = 0 ; i < quantidadePontos ; i++ ){
        pontosTemp[i] = pontos_base[i];
        pontosTemp[i].x = px + pontosTemp[i].x * t;
        pontosTemp[i].y = py + pontosTemp[i].y * t;
        pontosTemp[i].z = pz + pontosTemp[i].z * t;
    }

    pontos = pontosTemp;
}


/**
 * @brief Rotacionar os pontos do sólido utilizando matriz de rotação
 * @authors Jose Mateus Amaral, Gustavo Mittelmann, Henrique Heiderscheidt
 * 
 * @param rotacaoX Ângulo de rotação em relação ao eixo X
 * @param rotacaoY Ângulo de rotação em relação ao eixo Y
 * @param rotacaoZ Ângulo de rotação em relação ao eixo Z
 */
void Forma::girar(int rotacaoX, int rotacaoY, int rotacaoZ){

    angulo.x += rotacaoX;
    angulo.y += rotacaoY;
    angulo.z += rotacaoZ;
    double x,y,z,seno,cosseno;

    for( int i = 0 ; i < quantidadePontos ; i++ ){
        
        x = pontos_base[i].x;
        z = pontos_base[i].z;
        seno = sin( rotacaoX * M_PI / 180 );
        cosseno = cos( rotacaoX * M_PI / 180 );
        pontos_base[i].x = x * cosseno - z * seno;
        pontos_base[i].z = z * cosseno + x * seno;
        
        y = pontos_base[i].y;
        z = pontos_base[i].z;
        seno = sin( rotacaoY * M_PI / 180 );
        cosseno = cos( rotacaoY * M_PI / 180 );
        pontos_base[i].y = y * cosseno - z * seno;
        pontos_base[i].z = z * cosseno + y * seno;
        
        x = pontos_base[i].x;
        y = pontos_base[i].y;
        seno = sin( rotacaoZ * M_PI / 180 );
        cosseno = cos( rotacaoZ * M_PI / 180 );
        pontos_base[i].y = y * cosseno - x * seno;
        pontos_base[i].x = x * cosseno + y * seno;
    
    }
}

ostream & operator<< (ostream &out, const Forma &p)
{
    out << "posicao = Point3(" << p.posicao.x << "," << p.posicao.y << "," << p.posicao.z << ")" << endl;
    return out;
}
