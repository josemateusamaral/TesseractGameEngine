#pragma once
#include<SDL2/SDL.h>
#include <iostream>
#include "window.h"
#include "vec3.h"
#include "camera.h"
#include "texture.h"
#include "light.h"

using namespace std;

class Model
{
    private:

        double tamanho = 1;
        Vec3 posicao;
        bool backfaceCulling = true;

    
        int polygonCount;
        unsigned int* indices = nullptr;
        unsigned int indexCount;
        
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
        Vec3 angulo;
        
        Vec3* pontos_base;
        Vec3* pontos;
        Vec3* projecao;
        Vec3* uvs = nullptr;


        int qtdLights;
        int maxQtdLight;
        Light* lights;


        Model();
        Model(string filePath, Vec3 posicao = Vec3(0, 0, 0), double tamanho = 1);
        void loadModel(string path);
        void draw(Window &window);
        void calcular_pontos_3D();

        //rotation
        void rotate(int rotacaoX, int rotacaoY, int rotacaoZ);

        //position
        void setPos(double x, double y, double z);
        void setPos(Vec3 posicao);
        void setX(double x);
        void setY(double y);
        void setZ(double z);
        Vec3 getPos();

        //scale
        void setScale(double tamanho);
        double getScale();

        //backface culling
        bool getBackfaceCulling();
        void setBackfaceCulling(bool value);

        //light
        void setLight(Light *light);

};

ostream & operator<<(ostream &out, const Model &p);