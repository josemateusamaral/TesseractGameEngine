#include "input.h"

void Input::bindKey(SDL_Keycode key, std::function<void()> func)
{
    keyDownBindings[key] = func;
}

void Input::processar(bool &quit)
{
    SDL_Event ev;

    while (SDL_PollEvent(&ev) != 0)
    {
        if (ev.type == SDL_QUIT)
        {
            quit = true;
        }

        if (ev.type == SDL_KEYDOWN)
        {
            auto it = keyDownBindings.find(ev.key.keysym.sym);

            if (it != keyDownBindings.end())
            {
                it->second(); // 🔥 executa a função associada
            }
        }
    }
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