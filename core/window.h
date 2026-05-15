#pragma once
#include<SDL2/SDL.h>
#include "vec3.h"
#include "light.h"

class Window
{
    private:
        int width;
        int height;
        
        SDL_Renderer *renderer;
        SDL_Window* window = NULL;   

    public:

        float* zBuffer;
        float* colorBuffer;

        Window(int width,int height);
        ~Window();

        void atualiza();
        void clean();

        SDL_Window* getWindow();

        void desenha(Vec3 &p1, Vec3 &p2);
        void desenha(Vec3 &p1, Vec3 &p2,int r, int g, int b);
        void desenhar_poligono(Vec3 &p1, Vec3 &p2, Vec3 &p3,int r = 0, int g = 255, int b = 0);

        void desenhar_poligono_texturizado(Vec3 &p1, Vec3 &p2, Vec3 &p3,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3,unsigned char* data, int texW, int texH, Light** lights, int qtdLights);

        float area(int x1, int y1, int x2, int y2, int x3, int y3); 
        bool estaDentro(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
};



