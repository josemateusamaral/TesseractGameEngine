#pragma once
#include "window.h"
#include "camera.h"
#include "scene.h"
#include "input.h"
#include <stdio.h>
#include <functional>

class Tesseract
{

    private:
        bool quit;
        bool isCaptureMouse;
        bool isMapMouse;
        bool initialized;
        int delay;

    public:

        Window *window;
        Camera *camera;
        Scene *scene;
        Input *input;
        SDL_Event ev;

        Tesseract(int width = 640, int height = 480);
        ~Tesseract();

        void setCaptureMouse(bool state);
        bool getCaptureMouseState();
        void setMapMouse(bool state);
        bool getMapMouseState();

        bool isRunning();
        void run(function<void()> userUpdate);
        void exit();

};
