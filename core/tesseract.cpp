#include "tesseract.h"
#include <stdio.h>
#include <functional>
#include <limits>

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
	this->camera->hpr.x = 1;
	this->camera->hpr.y = 1;
    this->input = new Input();
    this->gui = new GUI();
    this->analitycs = new Analitycs(this->gui);
    this->audio = new AudioManager(this->camera);
    this->renderer = new Renderer();

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
    Uint32 fpsUpdateTime = lastFrameTicktime;
    Uint32 frameCount = 0;
    int fps = 0;

    while (!this->quit) {

        // Calculate frame delta time
        Uint32 currentTick = SDL_GetTicks();
        this->deltaTime = currentTick - lastFrameTicktime;
        lastFrameTicktime = currentTick;

        // Process inputs
        this->input->process(this->quit, this->ev, this->gui);

        // Run user scripts
        if (userUpdate) {
            userUpdate();
        }

        // Clear color and depth buffers
        this->window->clean();

        // Render scene
        for (int i = 0; i < this->scene->qtdModels; i++) {
            this->renderer->render(this->scene->models[i],this->window,this->camera);
        }

        // Update FPS counter
        frameCount++;
        currentTick = SDL_GetTicks();

        if (currentTick - fpsUpdateTime >= 1000) {

            fps = frameCount * 1000 / (currentTick - fpsUpdateTime);

            //update fps counter
            if (this->analitycs->fpsMeter->getIsVisible()) {
                Text* fpsText = dynamic_cast<Text*>(this->gui->elements[0]);
                if (fpsText) {
                    char buffer[32];
                    sprintf(buffer, "FPS: %d", fps);
                    fpsText->setText(buffer);
                }
            }

            frameCount = 0;
            fpsUpdateTime = currentTick;
        }

        // Render GUI
        for (int i = 0; i < this->gui->nElements; i++) {
            if (this->gui->elements[i]->getIsVisible()) {
                this->gui->elements[i]->render(this->window->colorBuffer,this->window->getWidth(),this->window->getHeight());
            }
        }

        // Present frame
        this->window->refresh();
    }

    SDL_Quit();
}


void Tesseract::exit(){
    this->quit = true;
}