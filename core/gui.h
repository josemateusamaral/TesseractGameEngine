#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include "vec3.h"

class GUIElement
{
    
    private:
        bool isVisible = true;
        Vec3 pos{100,100,0};

    public:
        
        function<void()> onClick = nullptr;
        function<void()> onRelease = nullptr;
        
        bool pressed = false;
        bool isSystemControlled = false;
        int width = 50;
        int height = 20;

        GUIElement();
        ~GUIElement();

        void hide();
        void show();
        bool getIsVisible();
        void setPos(Vec3 pos);
        void setX(int x);
        void setY(int y);
        Vec3 getPos();
        int getX();
        int getY();

        virtual void render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight) = 0;
        virtual void press() = 0;
        virtual void release() = 0;


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

        void processMouseClick(string button, int x, int y);
        void processMouseRelease(string button, int x, int y);

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
        void press();
        void release();

};

class Button : public GUIElement
{
    public:

        SDL_Surface* surface = nullptr;
        TTF_Font* font = nullptr;

        Button(const char* text, const char* fontPath = "core/assets/fonts/opensans.ttf");
        ~Button();
        
        void render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight);
        void setText(const char* text);
        void press();
        void release();

};




