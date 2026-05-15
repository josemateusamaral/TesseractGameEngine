#include "input.h"
#include <stdio.h>
#include <string>

Input::Input(){

    this->mouseMotionVector = new Vec3(0,0,0);
    this->mousePositionVector = new Vec3(0,0,0);
    this->isMapMouseMotion = false;
    this->mouseMotionBind = NULL;

}

void Input::bindKey(std::string key, std::string event, std::function<void()> func)
{
    auto sdl2_key = this->keyMap.find(key.c_str());
    if(sdl2_key != this->keyMap.end()){
        if(event == "press"){
            keyDownBindings[sdl2_key->second] = func;
        }
        else if(event == "release"){
            keyUpBindings[sdl2_key->second] = func;
        }
    }
}

void Input::bindMouseButton(std::string button, std::string event, std::function<void()> func){
    auto sdl2_mouse_button = this->mouseMap.find(button.c_str());
    if(sdl2_mouse_button != this->mouseMap.end()){
        if(event == "press"){
            mouseDownBindings[sdl2_mouse_button->second] = func;
        }
        else if(event == "release"){
            mouseUpBindings[sdl2_mouse_button->second] = func;
        }
    }
}

void Input::bindMouseMotion(std::function<void()> func){
    this->mouseMotionBind = func;
}

void Input::process(bool &quit, SDL_Event ev)
{

    while (SDL_PollEvent(&ev) != 0)
    {

        if (ev.type == SDL_KEYDOWN)
        {
            auto func = keyDownBindings.find(ev.key.keysym.sym);
            if (func != keyDownBindings.end())
            {
                func->second();
            }
        }

        if (ev.type == SDL_KEYUP)
        {
            auto func = keyUpBindings.find(ev.key.keysym.sym);
            if (func != keyUpBindings.end())
            {
                func->second();
            }
        }

        if (ev.type == SDL_MOUSEMOTION && this->isMapMouseMotion)
        {
            if(this->mouseMotionBind != NULL){
                this->mouseMotionVector->x =  ev.motion.xrel;
                this->mouseMotionVector->y =  ev.motion.yrel;
                this->mousePositionVector->x = ev.motion.x;
                this->mousePositionVector->y = ev.motion.y;
                this->mouseMotionBind();

                printf("\n%d - %d", ev.motion.xrel, ev.motion.yrel);

                this->mouseMotionVector->x = 0;
                this->mouseMotionVector->y = 0;
                this->mousePositionVector->x = 0;
                this->mousePositionVector->y = 0;
            }
        }

        if (ev.type == SDL_MOUSEBUTTONDOWN)
        {
            auto func = this->mouseDownBindings.find(ev.button.button);
            if (func != this->mouseDownBindings.end())
            {
                func->second();
            }
        }

        if (ev.type == SDL_MOUSEBUTTONUP)
        {
            auto func = this->mouseUpBindings.find(ev.button.button);
            if (func != this->mouseUpBindings.end())
            {
                func->second();
            }
        }

    }
}

void Input::setMapMouseMotion(bool state){
    if(state){
        this->isMapMouseMotion = true;
    }else{
        this->isMapMouseMotion = false;
    }
}