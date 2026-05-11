#pragma once
#include "window.h"
#include "camera.h"

class Tesseract
{

    public:

        Window *window;
        Camera *camera;

        Tesseract(int width = 640, int height = 480);
        ~Tesseract();

};
