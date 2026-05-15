#include "input.h"
#include <stdio.h>
#include <string>

void Input::bindKey(std::string key, std::string event, std::function<void()> func)
{
    auto sdl2_key = this->keyMap.find(key.c_str());
    if(sdl2_key != keyMap.end()){
        if(event == "press"){
            keyDownBindings[sdl2_key->second] = func;
        }
        else if(event == "release"){
            keyUpBindings[sdl2_key->second] = func;
        }
    }
}

void Input::process(bool &quit, SDL_Event ev)
{

    while (SDL_PollEvent(&ev) != 0)
    {

        if (ev.type == SDL_KEYDOWN)
        {
            auto it = keyDownBindings.find(ev.key.keysym.sym);
            if (it != keyDownBindings.end())
            {
                it->second();
            }
        }

        if (ev.type == SDL_KEYUP)
        {
            auto it = keyUpBindings.find(ev.key.keysym.sym);
            if (it != keyUpBindings.end())
            {
                it->second();
            }
        }

        if (ev.type == SDL_MOUSEMOTION)
        {
            int dx = ev.motion.xrel;
            int dy = ev.motion.yrel;
            //printf(dx + " - " + dy);
        }

        if (ev.type == SDL_MOUSEBUTTONDOWN)
        {
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                printf("Down Botao esquerdo\n");
            }

            if(ev.button.button == SDL_BUTTON_RIGHT)
            {
                printf("Down Botao direito\n");
            }

            if(ev.button.button == SDL_BUTTON_MIDDLE)
            {
                printf("Down Botao do meio\n");
            }
        }

        if (ev.type == SDL_MOUSEBUTTONUP)
        {
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                printf("Botao esquerdo\n");
            }

            if(ev.button.button == SDL_BUTTON_RIGHT)
            {
                printf("Botao direito\n");
            }

            if(ev.button.button == SDL_BUTTON_MIDDLE)
            {
                printf("Botao do meio\n");
            }
        }

    }
}