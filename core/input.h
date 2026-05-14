#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>
#include <string>

class Input
{
private:

    inline static std::unordered_map<std::string, SDL_Keycode> keyMap =
    {
        {"escape", SDLK_ESCAPE},
        {"space", SDLK_SPACE},
        {"enter", SDLK_RETURN},

        {"w", SDLK_w},
        {"a", SDLK_a},
        {"s", SDLK_s},
        {"d", SDLK_d},

        {"up", SDLK_UP},
        {"down", SDLK_DOWN},
        {"left", SDLK_LEFT},
        {"right", SDLK_RIGHT}
    };

    std::unordered_map<SDL_Keycode, std::function<void()>> keyDownBindings;

public:

    void bindKey(std::string key, std::function<void()> func);
    void processar(bool &quit, SDL_Event ev);
    SDL_Keycode stringToKey(std::string key);

};