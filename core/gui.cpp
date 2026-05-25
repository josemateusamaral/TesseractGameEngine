#include "gui.h"
#include <SDL2/SDL_ttf.h>

GUI::GUI()
{

    TTF_Init();
    this->elements = new GUIElement*[this->nMaxElements];

}

GUI::~GUI()
{

}

GUIElement::GUIElement()
{

}

GUIElement::~GUIElement()
{

}

void GUI::addElement(GUIElement* element)
{
    elements[nElements] = element;
    nElements++;
}

void GUI::removeElement(GUIElement* element)
{
    
}




Text::Text(const char* text, const char* fontPath)
{

    font = TTF_OpenFont(fontPath, 24);

    if (!font)
    {
        printf("Erro fonte: %s\n", TTF_GetError());
        return;
    }

    this->setText(text);

    /*
    SDL_Color cor = {255,255,255,255};

    surface = TTF_RenderUTF8_Blended(
        fonte,
        text,
        cor
    );

    if (!surface)
    {
        printf("Erro surface texto: %s\n", TTF_GetError());
        return;
    }

    // converte para uint32 ARGB8888
    SDL_Surface* converted =
        SDL_ConvertSurfaceFormat(
            surface,
            SDL_PIXELFORMAT_ARGB8888,
            0
        );

    SDL_FreeSurface(surface);

    surface = converted;

    width = surface->w;
    height = surface->h;

    TTF_CloseFont(fonte);
    */

}

Text::~Text()
{
    SDL_FreeSurface(surface);
}

void Text::setText(const char* text)
{
    if (surface)
    {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    SDL_Color cor = {255,255,255,255};

    SDL_Surface* temp =
        TTF_RenderUTF8_Blended(
            font,
            text,
            cor
        );

    if (!temp)
    {
        printf("Erro surface texto: %s\n", TTF_GetError());
        return;
    }

    surface = SDL_ConvertSurfaceFormat(
        temp,
        SDL_PIXELFORMAT_ARGB8888,
        0
    );

    SDL_FreeSurface(temp);

    width = surface->w;
    height = surface->h;
}

void Text::render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight)
{
    uint32_t* pixels = (uint32_t*)this->surface->pixels;

    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            int screenX = this->pos.x + x;
            int screenY = this->pos.y + y;

            // clipping
            if (screenX < 0 ||
                screenY < 0 ||
                screenX >= bufferWidth ||
                screenY >= bufferHeight)
            {
                continue;
            }

            uint32_t pixel = pixels[y * this->width + x];

            // alpha
            uint8_t alpha = (pixel >> 24) & 0xFF;

            if (alpha > 0)
            {
                colorBuffer[
                    screenY * bufferWidth + screenX
                ] = pixel;
            }
        }
    }
}