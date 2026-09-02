#include "gui.h"
#include <SDL2/SDL_ttf.h>



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

void GUIElement::setTextColor(uint8_t r, uint8_t g, uint8_t b){
    this->textColor = (255 << 24) | (r << 16) | (g << 8) | b;
    this->setText(this->text);
}

void GUIElement::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b){
    this->backgroundColor = (255 << 24) | (r << 16) | (g << 8) | b;
    this->setText(this->text);
}

void GUIElement::setText(std::string text)
{

    this->text = text;

    if (surface)
    {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    SDL_Color cor = {
        (Uint8)((this->textColor >> 16) & 0xFF), // R
        (Uint8)((this->textColor >> 8) & 0xFF),  // G
        (Uint8)(this->textColor & 0xFF),         // B
        255 
    };

    SDL_Surface* temp =
        TTF_RenderUTF8_Blended(
            font,
            text.c_str(),
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





GUI::GUI()
{

    TTF_Init();
    this->elements = new GUIElement*[this->nMaxElements];

}

GUI::~GUI()
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






Text::Text(std::string text, const char* fontPath)
{

    this->text = text;

    font = TTF_OpenFont(fontPath, 24);
    setTextColor(255,255,255);
    setBackgroundColor(0,0,0);

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

void Text::render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight)
{
    uint32_t* pixels = (uint32_t*)this->surface->pixels;

    for (int y = 0; y < this->height; y++)
    {
        #pragma omp parallel for schedule(static)
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






Button::Button(std::string text, const char* fontPath)
{

    font = TTF_OpenFont(fontPath, 24);
    this->setTextColor(255,255,255);
    this->setBackgroundColor(0,0,0);

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

void Button::render( uint32_t* colorBuffer, int bufferWidth, int bufferHeight)
{
    uint32_t* pixels = (uint32_t*)this->surface->pixels;

    for (int y = 0; y < this->height; y++)
    {
        #pragma omp parallel for schedule(static)
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
                    ] = this->backgroundColor;
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