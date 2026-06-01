#include "window.h"
#include "Vec3.h"
#include <bits/stdc++.h>
#include "light.h"

/**
 * @brief Generate a window with a specific width and height
 * 
 * @param width Largura em pixel
 * @param height Altura em pixel
 */
Window::Window(int width,int height)
:width{width}, height{height}
{

    window = SDL_CreateWindow( "Tesseract Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    zBuffer = new float[width*height];
    colorBuffer = new uint32_t[width*height*3];
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

}

/**
 * @brief Refresh the window with the current color buffer
 * 
 */
void Window::refresh()
{
    // copy color buffer to texture
    SDL_UpdateTexture(texture,NULL,colorBuffer,width * sizeof(uint32_t));

    // render texture to screen
    SDL_RenderCopy(renderer,texture,NULL,NULL);

    // present renderer to screen
    SDL_RenderPresent(renderer);
}

/**
 * @brief Função para limpar a janela para o proximo desenho.
 * 
 */
void Window::clean()
{
    // clean renderer
    SDL_RenderClear(renderer);

    // clean zBuffer
    std::fill(this->zBuffer, this->zBuffer + (this->width * this->height), std::numeric_limits<float>::infinity());

    // clear color buffer
    std::fill(colorBuffer,colorBuffer + (width * height),0x00000000);

}

int Window::getWidth(){
    return this->width;
}

int Window::getHeight(){
    return this->height;
}

SDL_Window* Window::getWindow(){
    return this->window;
}

Window::~Window()
{
    SDL_DestroyWindow(window);
}

