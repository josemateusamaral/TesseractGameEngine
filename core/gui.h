#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "vec3.h"

class GUIElement
{
    public:

        int width = 50;
        int height = 20;
        Vec3 pos{100,100,0};

        GUIElement();
        ~GUIElement();

        virtual void render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight) = 0;
};

class GUI
{

    public:

        int nElements = 0;
        int nMaxElements = 50;
        GUIElement** elements;

        GUI();
        ~GUI();

        void addElement(GUIElement* element);
        void removeElement(GUIElement* element);

};

class Text : public GUIElement
{
    public:

        SDL_Surface* surface = nullptr;
        TTF_Font* font = nullptr;

        Text(const char* text, const char* fontPath = "core/assets/fonts/opensans.ttf");
        ~Text();
        
        void render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight);
        void setText(const char* text);

};




