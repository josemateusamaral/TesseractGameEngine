#pragma once
#include "window.h"
#include "camera.h"
#include "scene.h"
#include <stdio.h>
#include <functional>

class Tesseract
{

    public:

        Window *window;
        Camera *camera;
        Scene *scene;
        SDL_Event ev;

        bool initialized;
        bool quit;
        int delay;

        Tesseract(int width = 640, int height = 480);
        ~Tesseract();

        bool isRunning();
        void run(function<void()> userUpdate);

};
