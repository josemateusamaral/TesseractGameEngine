#include "../formas/forma.h"
#include "../pontos/ponto3.h"
#include "model.h"
#include "../../pontos/ponto.h"
#include "Vec3.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

/**
 * @brief Construtor do sólido geométrico Esfera
 * 
 * @param posicao posição do centro no R3
 * @param tamanho Raio da esfera
 * @param lod Quantidade de pontos, para distorção
 *
 * @authors Jose Mateus Amaral
 */
Model::Model(Ponto3 posicao, double tamanho, int lod)
:Forma(posicao,tamanho),LOD{lod}
{
    iluminacao = Vec3(1,1,0);
    faixas = LOD / 2 + 1;
    quantidadePontos = LOD * faixas;
    calcular_pontos_base();
    calcular_pontos_3D();

}

Model::Model(Ponto3 posicao,int lod)
:Forma(posicao,1),LOD{lod}
{
    iluminacao = Vec3(1,1,0);
    faixas = LOD / 2 + 1;
    quantidadePontos = LOD * faixas;
    calcular_pontos_base();
    calcular_pontos_3D();

}

void Model::loadModel(string path)
{
    // ler o arquivo

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_FlipUVs
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Erro: " << importer.GetErrorString() << std::endl;
    }

}

/**
 * @brief Calcular os pontos de uma esfera no R3
 * 
 * @param posicao posição do centro no R3
 * @param tamanho Raio da esfera
 * @param lod Quantidade de pontos, para distorção
 *
 * @authors Jose Mateus Amaral, Monique
 */
void Model::calcular_pontos_base()
{
    // lista tempontoraria de pontos
    Ponto3* pontosTemp = (Ponto3*)malloc(sizeof(Ponto3) * quantidadePontos);

    /* lista auxiliar de pontos que armazenara um circulo que ira ser rotacionado 
    e a cada rotação cada um dos pontos desta lista sera copiado para a lista tempe */
    Ponto3* paraCalculo = (Ponto3*)malloc(sizeof(Ponto3) * LOD);

    /* raio  do circulo que será rotacionado */
    int raio = tamanho;
    int p = 0;
    double x,y,z;
    int ps = 0;

    // quantidade de angulo que o circulo será rotacionado
    int rotacao = 360 / LOD;
    double seno = sin( rotacao * M_PI / 180 );
    double cosseno = cos( rotacao * M_PI / 180 );

    // criar os pontos do circulo e armazenalos na lista
    for( int i = 0 ; i < 360 ; i += 360 / LOD,p++){
        x = raio * cos( i * M_PI / 180 );
        y = raio * sin( i * M_PI / 180 );
        Ponto3 ponto{x,y,0};
        paraCalculo[p] = ponto;
    }

    /* rotacionar o circulo e a cada rotação, copiar todos os pontos da
    lista do circulo para a lista temp que contem os pontos finais da esfera */
    for( int a = 0; a < faixas; a++){
        for( int j = 0 ; j < p ; j++, ps++ ){
            x = paraCalculo[j].x;
            z = paraCalculo[j].z;

            // rodar o ponto no plano xOz
            paraCalculo[j].x = x * cosseno - z * seno;
            paraCalculo[j].z = z * cosseno + x * seno;

            // copiar o ponto da lista do circulo para a lista final
            pontosTemp[ps] = Ponto3(paraCalculo[j].x,paraCalculo[j].y,paraCalculo[j].z);
        }
    }

    pontos_base = pontosTemp;    
}

/**
 * @brief rendericar a esfera nos formatos Wireframe ou Shaded
 *
 * @authors Jose Mateus Amaral, Monique
 */
void Model::desenhar(Window &window)
{
    // calcular pontos do R2 calculando primeiramente os pontos do R3
    calcular_pontos_3D();
    projecao = camera->projetar(pontos,quantidadePontos);
    
    // posicao do ponto ao lado do ponto atual
    int index;

    // tipo de renderização
    int type = 0;

    // cor dos poligonos
    int R = 255;
    int G = 255;
    int B = 255;

    // angulo entre 
    double angulo;

    // listas usadas para a criação de vetores
    double a[3] = {posicao.x,posicao.y,posicao.z};
    double b[3];
    double c[3];

    // vetor da camera até o objeto
    double origem[3] = {0,0,0};
    Vec3 camToObj{origem,a};

    // percorrer a lista de pontos da projeção e renderiza-los no modo WIREFRAME ou SHADED
    for(int j = 0; j < quantidadePontos; j++)
    {

        // calcular posição do proximo ponto que sera ligado ao ponto atual
        if( ( ( j + 1 ) % LOD ) == 0 ) {
            index = j - LOD + 1;
        }
        else{
            index = j + 1;
        }

        switch(renderType){

            // renderizar usando o modo wireframe
            case 1:
                window.desenha( projecao[j],projecao[index]);
                if( j > LOD - 1 ){
                    window.desenha( projecao[j], projecao[ j - LOD ]);
                }  
                break;

            // renderizar usando o modo shaded
            case 2:
                
                // vertice B do poligono
                b[0] = pontos[j].x;
                b[1] = pontos[j].y;
                b[2] = pontos[j].z;

                //verticar C do poligono
                c[0] = pontos[index].x;
                c[1] = pontos[index].y;
                c[2] = pontos[index].z;

                // vetores do centro do objeto até o poligono
                Vec3 objToponto1(a,b);
                Vec3 objToponto2(a,c);

                /* calcular a cor do poligono usando o angulo entre o vetor normal
                do poligono e o vetor de iluminação */
                if(camToObj.angulo_entre_vetores(objToponto1) > 90 && camToObj.angulo_entre_vetores(objToponto2) > 90){
                    if( j > LOD - 1 ){

                        /* calcular a cor do poligono usando o algulo entre o vetor de iluminação 
                        e o vetor normal do poligono */
                        if(comSombra){
                            angulo = ( objToponto1.angulo_entre_vetores(iluminacao) + objToponto2.angulo_entre_vetores(iluminacao) ) / 2 ;
                            R = corR - (( 255 / 180 ) * angulo);
                            G = corG - (( 255 / 180 ) * angulo);
                            B = corB - (( 255 / 180 ) * angulo);
                        }

                        // usar a cor normal sem calcular a iluminação
                        else{
                            R = corR;
                            G = corG;
                            B = corB;
                        }

                        //desenhar poligonos
                        window.desenhar_poligono(projecao[j],projecao[j - LOD],projecao[j - LOD + 1],R,G,B);
                        window.desenhar_poligono(projecao[j],projecao[j - 1],projecao[j - LOD],R,G,B);
                    }
                }
                break;       
        }
    }
}

/**
 * @brief configurar o nivel de detalhe da esfera
 * 
 * @param lod nivel de detalhe
 *
 * @authors Jose Mateus Amaral
 */
void Model::setLOD(int lod){

    /* aumentar o nivel de detalhe de lod == 1.
    O nivel maximo de lod para uma esfera é 360 oque significa
    que a esfera é composta por 360 faixas*/
    if(lod){
        if(LOD<=360){
            while(true){
                LOD++;
                if(360%LOD==0){
                    break;
                }
            }
        }
    }

    /* diminuir nivel de detalher se lod == 0.
    O nivel minimo de lod para uma esfera é 6, oque signicar 
    que ela é composta por 6 faixas*/
    else{
        if(LOD>=6){
            while(true){
                LOD--;
                if(360%LOD==0){
                    break;
                }
            }        
        }
    }

    // recriar os pontos da esfera usando o novo lod
    faixas = LOD / 2 + 1;
    quantidadePontos = LOD * faixas;    
    calcular_pontos_base();

    // configurar o angulo da nova esfera para ter o mesmo angulo da antiga
    Ponto3 anguloAntigo = Ponto3(angulo.x,angulo.y,angulo.z);
    angulo.x = 0;
    angulo.y = 0;
    angulo.z = 0;
    girar(anguloAntigo.x,anguloAntigo.y,anguloAntigo.z);

    // calcular os pontos 3d e 2d da esfera
    calcular_pontos_3D();
    projecao = camera->projetar(pontos,quantidadePontos);
    
}
int Model::getLOD(){
    return LOD;
}



ostream & operator<< (ostream &out, const Model &p)
{
    return out;
}
