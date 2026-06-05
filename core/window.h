#pragma once
#include<SDL2/SDL.h>
#include "vec3.h"
#include "light.h"

class Window
{
    private:
        int width;
        int height;
        SDL_Window* window = NULL;   
        SDL_Texture* texture;
        uint32_t backgroundColor;

    public:

        SDL_Renderer* renderer;
        float* zBuffer;
        uint32_t* colorBuffer;
        

        Window(int width,int height);
        ~Window();

        SDL_Window* getWindow();
        void refresh();
        void clean();
        int getWidth();
        int getHeight();
        void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);

};



