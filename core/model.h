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

        float scale = 1;
        Vec3 position;
        bool backfaceCulling = true;
    
        int polygonCount;
        unsigned int* indices = nullptr;
        unsigned int indexCount;
        
    public:

        //diffuse texture
        Texture* diffuseTexture = nullptr;

        int renderType = 1;
        int corR = 255;
        int corG = 255;
        int corB = 255;
        bool shadowCast = false;

        int nVertices;
        Vec3 angulo;

        // vertices / projection
        Vec3* vertices;
        Vec3* pontos;
        Vec3* projection;
        bool* screenSpaceBuffer;
        Vec3* uvs = nullptr;

        // lights
        int nLights;
        int maxNLight;
        Light** lights;

        Model();
        Model(string filePath, Vec3 position = Vec3(0, 0, 0), float scale = 1);
        void loadModel(string path);
        void draw(Window &window, Camera *camera);
        void calcular_pontos_3D();

        //rotation
        void rotate(int rotacaoX, int rotacaoY, int rotacaoZ);

        //position
        void setPos(float x, float y, float z);
        void setPos(Vec3 posicao);
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        Vec3 getPos();
        float getX();
        float getY();
        float getZ();

        //scale
        void setScale(float tamanho);
        float getScale();

        //backface culling
        bool getBackfaceCulling();
        void setBackfaceCulling(bool value);

        //light
        void setLight(Light *light);

};

ostream & operator<<(ostream &out, const Model &p);