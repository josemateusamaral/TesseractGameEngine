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
    Uint32 sdlTick; 
    int fps = 0;

    float updateFPSCount = 0.0;

    // create shadow maps for all models that cast shadows
    int shadowMapWidth = this->window->getWidth();
    int shadowMapHeight = this->window->getHeight();
    Camera *shadowCamera = new Camera();
    shadowCamera->setPos(Vec3(0, 0, 0));
    shadowCamera->hpr.x = 1;
    shadowCamera->hpr.y = 1;
    float* shadowZBuffer = new float[shadowMapWidth * shadowMapHeight];
    Model** shadowEBuffer = new Model*[shadowMapWidth * shadowMapHeight];

    while (!this->quit) {

        // process inputs
        this->input->process(this->quit, this->ev, this->gui);

        // scripts
        if (userUpdate) {
            userUpdate();
        }

        // clear color buffer and z buffer
        this->window->clean();
        
        // create shadow maps
        if(this->scene->qtdShadowCasters > 0){
            
            for(int i = 0; i < this->scene->qtdShadowCasters; i++){
                
                Light* light = this->scene->shadowCasters[i];
                std::fill(light->shadowZBuffer, light->shadowZBuffer + (light->shadowMapWidth * light->shadowMapHeight), std::numeric_limits<float>::infinity());
                
                for(int i = 0; i < this->scene->qtdModels; i++){
                    Model* model = this->scene->models[i];
                    std::fill(model->shadowMapBuffer,model->shadowMapBuffer + (100*100),false);
                    this->renderer->createShadowMap(light->shadowCamera, light->shadowZBuffer, light->shadowEBuffer, model, light->shadowMapWidth, light->shadowMapHeight);
                }
            }
        }

        // render scene
        for(int i = 0; i < this->scene->qtdModels; i++){
            this->renderer->render(this->scene->models[i], this->window, this->camera);
        }

        // update fps meter
        sdlTick = SDL_GetTicks();
        this->deltaTime = sdlTick - lastFrameTicktime;
        if(this->analitycs->fpsMeter->getIsVisible()){
            fps = 1000 / this->deltaTime;
            lastFrameTicktime = sdlTick;
            updateFPSCount += this->deltaTime;
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
        //SDL_Delay(this->delay);

    }

    SDL_Quit();

}

void Tesseract::exit(){
    this->quit = true;
}