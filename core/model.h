#pragma once
#include<SDL2/SDL.h>
#include <iostream>
#include "../core/ponto3.h"
#include "../core/ponto.h"
#include "window.h"
#include "vec3.h"
#include "camera.h"
#include "texture.h"

using namespace std;

class Model
{
    private:

        double tamanho = 1;
        Ponto3 posicao;
        bool backfaceCulling = true;

    
        int polygonCount;
        Ponto3* polygons = nullptr;
        unsigned int* indices = nullptr;
        unsigned int indexCount;
        
        Ponto* uvs = nullptr;
        
    public:

        Camera* camera = Camera::criar();

        Texture* texture = nullptr;

        int renderType = 1;
        int corR = 255;
        int corG = 255;
        int corB = 255;
        int comSombra = false;
        Vec3 iluminacao;

        int quantidadePontos;
        Ponto3 angulo;
        
        Ponto3* pontos_base;
        Ponto3* pontos;
        Ponto* projecao;

        Model(string filePath, Ponto3 posicao = Ponto3(0, 0, 0), double tamanho = 1);
        void loadModel(string path);
        void draw(Window &window);
        void calcular_pontos_3D();
        void rotate(int rotacaoX, int rotacaoY, int rotacaoZ);

        //position
        void setPos(double x, double y, double z);
        void setPos(Ponto3 posicao);
        void setX(double x);
        void setY(double y);
        void setZ(double z);
        Ponto3 getPos();

        //scale
        void setScale(double tamanho);
        double getScale();

        //backface culling
        bool getBackfaceCulling();
        void setBackfaceCulling(bool value);

};

ostream & operator<<(ostream &out, const Model &p);