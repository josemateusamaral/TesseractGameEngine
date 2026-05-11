#include "tesseract.h"

Tesseract::Tesseract(int width, int height)
{

    window = new Window(width, height);
    Camera *camera = new Camera();

}

Tesseract::~Tesseract()
{

}