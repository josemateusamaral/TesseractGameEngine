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

void GUIElement::hide(){
    this->isVisible = false;
}

void GUIElement::show(){
    this->isVisible = true;
}

bool GUIElement::getIsVisible(){
    return this->isVisible;
}

void GUIElement::setPos(Vec3 pos){
    this->pos = pos;
}
        
void GUIElement::setX(int x){
    this->pos.x = x;
}

void GUIElement::setY(int y){
    this->pos.y = y;
}
        
Vec3 GUIElement::getPos(){
    return this->pos;
}

int GUIElement::getX(){
    return this->pos.x;
}

int GUIElement::getY(){
    return this->pos.y;
}



void GUI::addElement(GUIElement* element)
{
    elements[nElements] = element;
    nElements++;
}

void GUI::removeElement(GUIElement* element)
{
    
}

void GUI::processMouseClick(string button, int x, int y){

    for(int i = 0; i < this->nElements; i++ ){

        if(
            x >= this->elements[i]->getX() &&
            x <= this->elements[i]->getX() + this->elements[i]->width &&
            y >= this->elements[i]->getY() &&
            y <= this->elements[i]->getY() + this->elements[i]->height
        ){
            this->elements[i]->press();
        }
    }

}

void GUI::processMouseRelease(string button, int x, int y){
    
    for(int i = 0; i < this->nElements; i++ ){
        this->elements[i]->release();
    }

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
            int screenX = this->getX() + x;
            int screenY = this->getY() + y;

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

void Text::press(){
    this->pressed = true;
}

void Text::release(){
    this->pressed = false;
}




Button::Button(const char* text, const char* fontPath)
{

    font = TTF_OpenFont(fontPath, 24);

    if (!font)
    {
        printf("Erro fonte: %s\n", TTF_GetError());
        return;
    }

    this->setText(text);

}

Button::~Button()
{
    SDL_FreeSurface(surface);
}

void Button::setText(const char* text)
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

void Button::render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight)
{
    uint32_t* pixels = (uint32_t*)this->surface->pixels;

    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            int screenX = this->getX() + x;
            int screenY = this->getY() + y;

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
            }else{
                if(this->pressed){
                    colorBuffer[
                        screenY * bufferWidth + screenX
                    ] = (60 << 24) | (60 << 16) | (60 << 8) | 60;
                }else{
                    colorBuffer[
                        screenY * bufferWidth + screenX
                    ] = (150 << 24) | (150 << 16) | (150 << 8) | 150;
                }
                
            }
        }
    }
}

void Button::press(){
    if(!this->pressed){
        this->pressed = true;
        if(this->onClick != nullptr){
            this->onClick();
        }
    }
}

void Button::release(){
    if(this->pressed){
        this->pressed = false;
        if(this->onRelease != nullptr){
            this->onRelease();
        }
    }
}