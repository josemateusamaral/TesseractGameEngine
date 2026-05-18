#include "tesseract.h"
#include <stdio.h>
#include <functional>

Tesseract::Tesseract(int width, int height)
{

    // inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        this->quit = true;
        this->initialized = false;
        return;

    }else{
        this->quit = false;
        this->initialized = true;
    }

    this->delay = 32;
    this->window = new Window(width, height);
    this->scene = new Scene();
    this->camera = new Camera();
    this->input = new Input();

}

Tesseract::~Tesseract()
{

}

void Tesseract::setCaptureMouse(bool state){
    if(state){
        this->isCaptureMouse = true;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        this->input->setMapMouseMotion(true);
    }else{
        this->isCaptureMouse = false;
        SDL_SetRelativeMouseMode(SDL_FALSE);
        this->input->setMapMouseMotion(false);
    }
}

bool Tesseract::getCaptureMouse(){
    return this->isCaptureMouse;
}

bool Tesseract::isRunning(){
    return this->initialized && !this->quit;
}

void Tesseract::run(function<void()> userUpdate) {
    
    while (!this->quit) {

        // process inputs
        this->input->process(this->quit, this->ev);

        // scripts
        if (userUpdate) {
            userUpdate();
        }

        // render
        this->window->clean();
        for(int i = 0; i < this->scene->qtdModels; i++){
            Model* model = this->scene->models[i];
            model->draw(*window, this->camera);
        }
        this->window->refresh();

        // refresh rate delay
        SDL_Delay(this->delay);

    }

    SDL_Quit();

}

void Tesseract::exit(){
    this->quit = true;
}