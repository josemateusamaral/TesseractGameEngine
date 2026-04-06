#include "../../formas/forma.h"
#include "../../pontos/ponto3.h"
#include "cone.h"
#include "../../pontos/ponto.h"
#include "../../utils/Vec3.h"

/**
 * @brief Construtor do sólido geométrico Cone
 * 
 * @param posicao Posição do centro do círculo no R3
 * @param lod Quantidade de pontos, para distorção.
 * @param tAltura Altura do cone
 *
 * @authors Jose Mateus Amaral
 */
Cone::Cone(Ponto3 posicao,int lod,int tAltura)
:Forma(posicao,1)
{
    LOD = lod;
    Ponto3 temp{0,(double)tAltura,0};
    altura = temp;
    quantidadePontos = LOD + 1;
    calcular_pontos_base();
    calcular_pontos_3D();
}

/**
 * @brief calcular os pontos do cone que tem posição relativa a posição do cone
 *
 * @authors Jose Mateus Amaral
 */
void Cone::calcular_pontos_base()
{
    // lista para abrigar os pontos temporariamente
    Ponto3* pontosTemp = (Ponto3*)malloc(sizeof(Ponto3) * quantidadePontos);  

    int raio = 1;
    double x,y,z;
    int angulo = 0;
    int posicaoArray = 0; 

    // criar um circulo para ser a base do cone
    for( ; angulo < 360 ; angulo += 360 / (quantidadePontos -1), posicaoArray++ ){
        x = raio * cos( angulo * M_PI / 180 );
        z = raio * sin( angulo * M_PI / 180);
        Ponto3 ponto{x,posicao.y,z};
        pontosTemp[posicaoArray] = ponto;
    }

    // adicionar ponto da altura do cone que é o ponto onde todos os pontos da base são ligados
    pontosTemp[quantidadePontos-1] = altura;

    pontos_base = pontosTemp;
}

/**
 * @brief desenhar os pontos da projeção do cone no render da janela
 *
 * @param Window &window janela para renderizar
 *
 * @authors Jose Mateus Amaral
 */
void Cone::desenhar(Window &window)
{
    calcular_pontos_3D();
    projecao = camera->projetar(pontos,quantidadePontos);
    int index;

    // ligar cada um dos pontos da base com o ponto ao seu lado e liga-lo tambem ao ponto da altura
    for( int ponto = 1 ; ponto < quantidadePontos-1; ponto++ ){
        
        // ligar ponto atual ao ponto ao seu lado
        window.desenha(projecao[ponto],projecao[ponto - 1]);
        
        // se for o ultimo ponto da base, liga-lo ao primeiro da base
        if( ponto == quantidadePontos - 2){
            window.desenha(projecao[ponto],projecao[0]);
        }

        // ligar ponto atual ao ponto da altura
        window.desenha(projecao[ponto],projecao[quantidadePontos-1]);
    }

    // ligar primeiro ponto da base a o ponto da altura
    window.desenha(projecao[0],projecao[quantidadePontos-1]);

}

/**
 * @brief desenhar os pontos da projeção do cone no render da janela
 *
 * @param int lod seletor de alteração do lod
 *
 * @authors Jose Mateus Amaral
 */
void Cone::setLOD(int lod){

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
        if(LOD>=3){
            while(true){
                LOD--;
                if(360%LOD==0){
                    break;
                }
            }        
        }
    }

    // recriar os pontos da esfera usando o novo lod
    quantidadePontos = LOD + 1;    
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

int Cone::getLOD(){
    return LOD;
}

ostream & operator<< (ostream &out, const Cone &p)
{
    return out;
}
