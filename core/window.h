#pragma once
#include<SDL2/SDL.h>
#include "../core/ponto.h"

class Window
{
    private:
        int width;
        int height;
        SDL_Window* window = NULL;


    public:
        Window(int width,int height);
        ~Window();
        SDL_Renderer *renderer;

        
        void atualiza();
        void limpar();

        void desenha(Ponto &p1, Ponto &p2);
        void desenha(Ponto &p1, Ponto &p2,int r, int g, int b);
        void desenhar_poligono(Ponto &p1, Ponto &p2, Ponto &p3,int r = 0, int g = 255, int b = 0);

        float area(int x1, int y1, int x2, int y2, int x3, int y3); 
        bool estaDentro(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
};



