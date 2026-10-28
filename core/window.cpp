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
Window::Window(int width, int height, bool vsync, bool fullscreen)
    : width{width},
      height{height},
      vsync{vsync},
      fullscreen{fullscreen}
{
    Uint32 windowFlags = SDL_WINDOW_SHOWN;

    if (fullscreen)
        windowFlags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(
        "Tesseract Game Engine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        windowFlags
    );

    if (!window)
    {
        std::cout << "SDL_CreateWindow Error: "
                  << SDL_GetError() << std::endl;
        return;
    }

    //create renderer
    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
    if (vsync)
        rendererFlags |= SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);
    if (!renderer)
    {
        std::cout << "SDL_CreateRenderer Error: "
                  << SDL_GetError() << std::endl;
        return;
    }

    //create buffers
    zBuffer = new float[width * height];
    colorBuffer = new uint32_t[width * height];

    //create texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    //set background color Uint32
    backgroundColor = (255 << 24) | (255 << 16) | (255 << 8) |255;
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
    std::fill(this->zBuffer, this->zBuffer + (this->width * this->height), 0.0f);

    // clear color buffer
    std::fill(colorBuffer,colorBuffer + (width * height),this->backgroundColor);

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

void Window::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b){
    backgroundColor = (255 << 24) | (r << 16) | (g << 8) | b;
}

void Window::setVSync(bool enabled)
{
    if (vsync == enabled)
        return;

    vsync = enabled;

    SDL_DestroyRenderer(renderer);

    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;

    if (vsync)
        rendererFlags |= SDL_RENDERER_PRESENTVSYNC;

    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    if (!renderer)
    {
        std::cout << "SDL_CreateRenderer Error: "
                  << SDL_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
}

bool Window::isVSyncEnabled()
{
    return vsync;
}

void Window::setFullscreen(bool enabled)
{
    if (fullscreen == enabled)
        return;

    fullscreen = enabled;

    if (fullscreen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        SDL_SetWindowFullscreen(window, 0);
}

bool Window::isFullscreen()
{
    return fullscreen;
}

Window::~Window()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete[] zBuffer;
    delete[] colorBuffer;
}
