#include "../../formas/forma.h"
#include "../../pontos/ponto3.h"
#include "cubo.h"
#include "../../pontos/ponto.h"
#include "../../utils/Vec3.h"
#include <iostream>
using namespace std;


Cubo::Cubo(Ponto3 posicao, double tamanho)
:Forma(posicao,tamanho)
{
    quantidadePontos = 8;
    calcular_pontos_base();
    calcular_pontos_3D();
    
}

Cubo::Cubo(Ponto3 posicao)
:Forma(posicao,1)
{
    quantidadePontos = 8;
    calcular_pontos_base();
    calcular_pontos_3D();

}

/**
 * @brief Criar os pontos do cubo usando o tamanho para calcular a posicao dos pontos
 * 
 * @authors Jose Mateus Amaral, Fernanda Martins
 */
void Cubo::calcular_pontos_base(){
    Ponto3* pontosTemp = (Ponto3*)malloc(sizeof(Ponto3) * 8);
    int t = tamanho;
    pontosTemp[0] = Ponto3(-1*t,-1*t,-1*t);
    pontosTemp[1] = Ponto3(1*t,-1*t,-1*t);
    pontosTemp[2] = Ponto3(1*t,1*t,-1*t);
    pontosTemp[3] = Ponto3(-1*t,1*t,-1*t);
    pontosTemp[4] = Ponto3(-1*t,-1*t,1*t);
    pontosTemp[5] = Ponto3(1*t,-1*t,1*t);
    pontosTemp[6] = Ponto3(1*t,1*t,1*t);
    pontosTemp[7] = Ponto3(-1*t,1*t,1*t);
    pontos_base = pontosTemp;
}

/**
 * @brief Desenhar o cubo nos formato wireframe ou shaded
 * 
 * @param window Janela do SDL
 * 
 * @authors Jose Mateus Amaral, Fernanda Martins
 */
void Cubo::desenhar(Window &window)
{
    // calcular os pontos 3d do objeto que são os pontos no espaco 3d
    calcular_pontos_3D();

    // projetar os pontos do espaço 3d para o espaço 2d
    projecao = camera->projetar(pontos,quantidadePontos);

    // tipo de renderização
    int type = 2;

    switch(renderType){

        // renderizar usando wireframe
        case 1:
            window.desenha(projecao[0],projecao[1]);
            window.desenha(projecao[1],projecao[2]);
            window.desenha(projecao[2],projecao[3]);
            window.desenha(projecao[3],projecao[0]);
            window.desenha(projecao[0],projecao[4]);
            window.desenha(projecao[4],projecao[5]);
            window.desenha(projecao[5],projecao[6]);
            window.desenha(projecao[6],projecao[7]);
            window.desenha(projecao[7],projecao[4]);
            window.desenha(projecao[7],projecao[3]);
            window.desenha(projecao[6],projecao[2]);
            window.desenha(projecao[5],projecao[1]);
            break;

        // renderizar usando poligonos
        case 2: 

            // cor do poligono
            int R = 255;
            int G = 255;
            int B = 255;

            // vetor usado para calcular a iluminação do poligono usando angulo entre vetores
            double angulo;

            // posicao do centro do objeto 
            double a[3] = {posicao.x,posicao.y,posicao.z};
            double b[3];
            double c[3];
            double d[3];

            //posicao da camera
            double origem[3] = {0,0,0};
            Vec3 camToObj{origem,a};

            // listar com indexes que formam poligonos apartir da lista de projecao
            int pontosCubo[24][3] = {
                {1,5,0},
                {5,0,4},
                {1,4,0},
                {1,4,5},

                {1,6,2},
                {1,6,5},
                {1,5,2},
                {2,5,6},

                {3,7,2}, 
                {2,6,7},
                {3,6,2},
                {6,3,7},

                {3,7,4},          
                {0,4,3},
                {0,7,4},
                {0,7,3},                

                {5,7,4},
                {5,7,6}, 
                {4,6,5},
                {4,6,7},
                
                {3,1,2},
                {3,1,0},            
                {0,2,3},
                {0,2,1}
            };

            int i = 0;
            for( ; i < 24 ; i++ ){
                
                // ponto B do poligono
                b[0] = pontos[pontosCubo[i][0]].x;
                b[1] = pontos[pontosCubo[i][0]].y;
                b[2] = pontos[pontosCubo[i][0]].z;

                // ponto C do poligono
                c[0] = pontos[pontosCubo[i][1]].x;
                c[1] = pontos[pontosCubo[i][1]].y;
                c[2] = pontos[pontosCubo[i][1]].z;

                // ponto D do poligono
                d[0] = pontos[pontosCubo[i][2]].x;
                d[1] = pontos[pontosCubo[i][2]].y;
                d[2] = pontos[pontosCubo[i][2]].z;

                //vetor do centro do objeto até o poligono
                Vec3 objToponto1(a,b);

                //tirar produto vetorial usando os vetor BC e BD para calcular o vetor normal do poligono
                Vec3 vetor1{b,c};
                Vec3 vetor2{b,d};
                Vec3 vetorNormal = vetor1.produto_vetorial(vetor2);

                /* verificar se o vetor normal do poligono esta apontando para a direcao 
                certa ou é necessári invertelo*/
                if( objToponto1.angulo_entre_vetores(vetorNormal) < 90 ){
                    vetorNormal = vetorNormal.multiEscalar(-1);
                }
                
                // angulo entre a camera e o vetor normal do poligono
                double cAngulo = camToObj.angulo_entre_vetores(vetorNormal);

                /* desenhar poligono cado o angulo entre a camera e o poligono 
                seja menor de 90 graus o que indicar que o poligono esta com a
                face virada na direcao da camera*/
                if( cAngulo < 90 ){
                    angulo = iluminacao.angulo_entre_vetores(vetorNormal);
                    R = (( corR / 180 ) * angulo);
                    G = (( corG / 180 ) * angulo);
                    B = (( corB / 180 ) * angulo);
                    window.desenhar_poligono(projecao[pontosCubo[i][0]],projecao[pontosCubo[i][1]],projecao[pontosCubo[i][2]],R,G,B);
                }
                
            }
            break;
    }
    
}

ostream & operator<< (ostream &out, const Cubo &p)
{
    out << "Pontos modelo:" << endl;
    for( int pontos_base = 0 ; pontos_base < p.quantidadePontos ; pontos_base++ ){
        out << "Ponto3(" << p.pontos_base[pontos_base].x << "," << p.pontos_base[pontos_base].y << "," << p.pontos_base[pontos_base].z << ")" << endl;
    }

    out << "\nPontos 3D" << endl;
    for( int pontos_3D = 0 ; pontos_3D < p.quantidadePontos ; pontos_3D++ ){
        out << "Ponto3(" << p.pontos[pontos_3D].x << "," << p.pontos[pontos_3D].y << "," << p.pontos[pontos_3D].z << ")" << endl;
    }

    out << "\nPontos 2D" << endl;
    for( int pontos_2D = 0 ; pontos_2D < p.quantidadePontos ; pontos_2D++ ){
        out << "Ponto3(" << p.projecao[pontos_2D].x << "," << p.projecao[pontos_2D].y << ")" << endl;
    }

    return out;
}
