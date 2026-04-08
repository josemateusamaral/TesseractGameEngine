#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>

class Input
{
private:
    std::unordered_map<SDL_Keycode, std::function<void()>> keyDownBindings;

public:
    void bindKey(SDL_Keycode key, std::function<void()> func);
    void processar(bool &quit);
};