#include "../../formas/forma.h"
#include "../../pontos/ponto3.h"
#include "cilindro.h"
#include "../../pontos/ponto.h"
#include "../../utils/Vec3.h"

/**
 * @brief Construtor do sólido Cilindro
 * 
 * @param posicao Ponto do centro do circulo
 * @param lod Nivel de detalhe
 * @param Altura Altura do cilindro
 * 
 * @author Fernanda Martins
 */
Cilindro::Cilindro(Ponto3 posicao, int lod, int Altura)
:Forma(posicao,1)
{
    LOD = lod;
    Ponto3 temp{0,(double)Altura,0};
    altura = temp;
    quantidadePontos = LOD;
    calcular_pontos_base();
    calcular_pontos_3D();
}

void Cilindro::calcular_pontos_base()
{
    Ponto3* pontosTemp = (Ponto3*)malloc(sizeof(Ponto3) * quantidadePontos); 
    int raio = 1;
    double x,y,z, tmp;
    
    int angulo = 0;
    int posicaoArray = 0; 

    for( ; angulo < 360 ; angulo += 360 / (quantidadePontos/2), posicaoArray+=2){
        x = raio * cos( angulo * M_PI / 180 );
        z = raio * sin( angulo * M_PI / 180);
        tmp = posicao.y + altura.y;
        Ponto3 ponto{x,posicao.y,z};
        Ponto3 ponto1{x,tmp,z};
        pontosTemp[posicaoArray] = ponto;
        pontosTemp[posicaoArray+1] = ponto1;
    }
    pontos_base = pontosTemp;
}

void Cilindro::desenhar(Window &window)
{
    calcular_pontos_3D();
    projecao = camera->projetar(pontos,quantidadePontos);
    int index;

    for( int ponto = 0 ; ponto < quantidadePontos-1; ponto++ ){
        
        if ( ponto%2 == 0){
            window.desenha(projecao[ponto],projecao[ponto+1]);
            if (ponto == quantidadePontos-2){
                window.desenha(projecao[ponto],projecao[0]);
            }
            else
                window.desenha(projecao[ponto],projecao[ponto + 2]);
        }
        else{            
            if( ponto == quantidadePontos)
                window.desenha(projecao[ponto],projecao[1]);
            else{
                window.desenha(projecao[ponto],projecao[ponto + 2]);
            }        
        } 
    }
    window.desenha(projecao[quantidadePontos-1],projecao[1]);
}

void Cilindro::setLOD(int lod){
    if(lod){
        if(LOD<360){
            while(true){                
                LOD++;
                if(360%LOD==0){
                    break;
                }
            }
        }
    }
    else{
        if(LOD>3){
            while(true){
                LOD--;
                if(360%LOD==0){
                    break;
                }
            }        
        }
    }

    quantidadePontos = LOD + 1;    
    calcular_pontos_base();

    Ponto3 anguloAntigo = Ponto3(angulo.x,angulo.y,angulo.z);
    angulo.x = 0;
    angulo.y = 0;
    angulo.z = 0;

    girar(anguloAntigo.x,anguloAntigo.y,anguloAntigo.z);

    calcular_pontos_3D();
    projecao = camera->projetar(pontos,quantidadePontos);
    
}

int Cilindro::getLOD(){
    return LOD;
}

ostream & operator<< (ostream &out, const Cilindro &p)
{
    return out;
}