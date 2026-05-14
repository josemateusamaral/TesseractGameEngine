#include "input.h"
#include <stdio.h>
#include <string>

void Input::bindKey(std::string key, std::function<void()> func)
{
    SDL_Keycode sdlKey = SDL_GetKeyFromName(key.c_str());
    if(sdlKey != SDLK_UNKNOWN)
    {
        keyDownBindings[sdlKey] = func;
    }
}

void Input::processar(bool &quit, SDL_Event ev)
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

    }
}

SDL_Keycode Input::stringToKey(std::string key)
{

    auto it = this->keyMap.find(key);

    if(it != keyMap.end())
    {
        return it->second;
    }

    return SDLK_UNKNOWN;
}


/*
EXEMPLO USO

Input input;

input.bindKey(SDLK_ESCAPE, [&quit]() {
    quit = true;
});

input.bindKey(SDLK_w, [camera]() {
    camera->mover(0, 0, 1);
});

input.bindKey(SDLK_s, [camera]() {
    camera->mover(0, 0, -1);
});

input.bindKey(SDLK_a, [camera]() {
    camera->mover(1);
});

input.bindKey(SDLK_d, [camera]() {
    camera->mover(-1);
});

input.bindKey(SDLK_e, [camera, &sol]() {
    camera->rodarx(0.34, sol.posicao);
});

input.processar(quit);
*/