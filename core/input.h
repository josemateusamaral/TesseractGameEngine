#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>
#include <string>
#include "vec3.h"
#include "gui.h"

class Input
{
    private:

        inline static std::unordered_map<std::string, SDL_Keycode> keyMap =
        {
            // special keys
            {"escape", SDLK_ESCAPE},
            {"space", SDLK_SPACE},
            {"enter", SDLK_RETURN},
            {"lshift", SDLK_LSHIFT},
            {"rshift", SDLK_RSHIFT},
            {"lctrl", SDLK_LCTRL},
            {"rctrl", SDLK_RCTRL},
            {"tab", SDLK_TAB},
            {"lalt", SDLK_LALT},
            {"ralt", SDLK_RALT},
            {"backspace", SDLK_BACKSPACE},
            {"delete", SDLK_DELETE},
            {"home", SDLK_HOME},
            {"pageup", SDLK_PAGEUP},
            {"pagedown", SDLK_PAGEDOWN},
            {"delete", SDLK_DELETE},
            {"end", SDLK_END},

            // f keys
            {"f1",  SDLK_F1},
            {"f2",  SDLK_F2},
            {"f3",  SDLK_F3},
            {"f4",  SDLK_F4},
            {"f5",  SDLK_F5},
            {"f6",  SDLK_F6},
            {"f7",  SDLK_F7},
            {"f8",  SDLK_F8},
            {"f9",  SDLK_F9},
            {"f10", SDLK_F10},
            {"f11", SDLK_F11},
            {"f12", SDLK_F12},

            // number keys
            {"0", SDLK_0},
            {"1", SDLK_1},
            {"2", SDLK_2},
            {"3", SDLK_3},
            {"4", SDLK_4},
            {"5", SDLK_5},
            {"6", SDLK_6},
            {"7", SDLK_7},
            {"8", SDLK_8},
            {"9", SDLK_9},

            // letters keys
            {"a", SDLK_a},
            {"b", SDLK_b},
            {"c", SDLK_c},
            {"d", SDLK_d},
            {"e", SDLK_e},
            {"f", SDLK_f},
            {"g", SDLK_g},
            {"h", SDLK_h},
            {"i", SDLK_i},
            {"j", SDLK_j},
            {"k", SDLK_k},
            {"l", SDLK_l},
            {"m", SDLK_m},
            {"n", SDLK_n},
            {"o", SDLK_o},
            {"p", SDLK_p},
            {"q", SDLK_q},
            {"r", SDLK_r},
            {"s", SDLK_s},
            {"t", SDLK_t},
            {"u", SDLK_u},
            {"v", SDLK_v},
            {"w", SDLK_w},
            {"x", SDLK_x},
            {"y", SDLK_y},
            {"z", SDLK_z},

            // direction keys
            {"up", SDLK_UP},
            {"down", SDLK_DOWN},
            {"left", SDLK_LEFT},
            {"right", SDLK_RIGHT}
        };

        inline static std::unordered_map<std::string, SDL_Keycode> mouseMap = {

            {"left",SDL_BUTTON_LEFT},
            {"middle",SDL_BUTTON_MIDDLE},
            {"right",SDL_BUTTON_RIGHT}

        };

        std::unordered_map<SDL_Keycode, std::function<void()>> keyDownBindings;
        std::unordered_map<SDL_Keycode, std::function<void()>> keyUpBindings;
        std::unordered_map<Uint8, std::function<void()>> mouseDownBindings;
        std::unordered_map<Uint8, std::function<void()>> mouseUpBindings;
        std::function<void()> mouseMotionBind;

        bool isMapMouseMotion;

    public:

        Vec3 *mouseMotionVector;
        Vec3 *mousePositionVector;

        Input();
        void bindKey(std::string key, std::string event, std::function<void()> func);
        void bindMouseButton(std::string button, std::string event, std::function<void()> func);
        void bindMouseMotion(std::function<void()> func);
        void process(bool &quit, SDL_Event ev, GUI *gui);
        void setMapMouseMotion(bool state);

};