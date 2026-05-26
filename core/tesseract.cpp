#include "tesseract.h"
#include <stdio.h>
#include <functional>

Tesseract::Tesseract(int width, int height)
{

    // inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        this->quit = true;
        this->initialized = false;
        return;

    }else{
        this->quit = false;
        this->initialized = true;
    }

    this->delay = 16;
    this->window = new Window(width, height);
    this->scene = new Scene();
    this->camera = new Camera();
    this->input = new Input();
    this->gui = new GUI();
    this->analitycs = new Analitycs(this->gui);
    this->audio = new AudioManager();

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
    
    Uint32 lastFrameTicktime = SDL_GetTicks();
    Uint32 deltaTime; 
    Uint32 sdlTick; 
    int fps = 0;

    float updateFPSCount = 0.0;

    while (!this->quit) {

        // process inputs
        this->input->process(this->quit, this->ev, this->gui);

        // scripts
        if (userUpdate) {
            userUpdate();
        }

        // render scene
        this->window->clean();
        for(int i = 0; i < this->scene->qtdModels; i++){
            Model* model = this->scene->models[i];
            model->draw(*window, this->camera);
        }

        // update fps meter
        if(this->analitycs->fpsMeter->getIsVisible()){
            sdlTick = SDL_GetTicks();
            deltaTime = sdlTick - lastFrameTicktime;
            fps = 1000 / deltaTime;
            lastFrameTicktime = sdlTick;
            updateFPSCount += deltaTime;
            if(updateFPSCount > 1000){
                Text* fpsText = dynamic_cast<Text*>(this->gui->elements[0]);
                if (fpsText)
                {
                    char buffer[10];
                    sprintf(buffer, "FPS: %d", fps);
                    fpsText->setText(buffer);
                }
                updateFPSCount = 0.0f;
            }
        }

        // render gui
        for(int i = 0; i < this->gui->nElements; i++){
            if(this->gui->elements[i]->getIsVisible()){
                this->gui->elements[i]->render(this->window->colorBuffer, this->window->getWidth(), this->window->getHeight());
            } 
        }

        // show color buffer
        this->window->refresh();

        // refresh rate delay
        SDL_Delay(this->delay);

    }

    SDL_Quit();

}

void Tesseract::exit(){
    this->quit = true;
}