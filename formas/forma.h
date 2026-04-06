#pragma once
#include<SDL2/SDL.h>
#include <iostream>
#include "../pontos/ponto3.h"
#include "../pontos/ponto.h"
#include "../utils/janela.h"
#include "../utils/Vec3.h"
#include "../utils/camera.h"


using namespace std;
class Forma 
{
    public:
        Camera* camera = Camera::criar();

        int renderType = 1;
        int corR = 255;
        int corG = 255;
        int corB = 255;
        int comSombra = true;
        Vec3 iluminacao;

        int quantidadePontos;
        double tamanho;
        Ponto3 posicao;
        Ponto3 angulo;
        
        Ponto3* pontos_base;
        Ponto3* pontos;
        Ponto* projecao;

        Forma(Ponto3 posicao,double tamanho);

        virtual void desenhar(Window &window) = 0;
        virtual void calcular_pontos_base() = 0;

        void calcular_pontos_3D();
        void girar(int rotacaoX, int rotacaoY, int rotacaoZ);
};

ostream & operator<<(ostream &out, const Forma &p);
