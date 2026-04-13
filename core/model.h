#pragma once
#include<SDL2/SDL.h>
#include <iostream>
#include "../core/ponto3.h"
#include "../core/ponto.h"
#include "window.h"
#include "vec3.h"
#include "camera.h"

using namespace std;

class Model
{
    private:
        int LOD;
        int faixas;

        int polygonCount;
        Ponto3* polygons = nullptr;

        unsigned int* indices = nullptr;
        unsigned int indexCount;

        bool backface_culling = true;
        Ponto* uvs = nullptr;
        
    public:

        Camera* camera = Camera::criar();

        int renderType = 1;
        int corR = 255;
        int corG = 255;
        int corB = 255;
        int comSombra = true;
        Vec3 iluminacao;

        int quantidadePontos;
        double tamanho = 1;
        Ponto3 posicao;
        Ponto3 angulo;
        
        Ponto3* pontos_base;
        Ponto3* pontos;
        Ponto* projecao;

        void calcular_pontos_3D();
        void girar(int rotacaoX, int rotacaoY, int rotacaoZ);



        
        Model(string filePath, Ponto3 posicao = Ponto3(0, 0, 0), double tamanho = 1, int lod = 1);
        Model(Ponto3 posicao, double tamanho, int lod);
        Model(Ponto3 posicao, int lod);

        void desenhar(Window &window);
        void calcular_pontos_base();
        void loadModel(string path);

        void setPos(double x, double y, double z);
        void setPos(Ponto3 posicao);
        void setX(double x);
        void setY(double y);
        void setZ(double z);

        void setTamanho(double tamanho);
        double getTamanho();

        void setLOD(int lod);
        int getLOD();

        bool getBackface_Culling();
        void setBackface_Culling(bool value);

};

ostream & operator<<(ostream &out, const Model &p);