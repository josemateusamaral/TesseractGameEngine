#include "tesseract.h"

Tesseract::Tesseract(int width, int height)
{

    // inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        quit = true;
        initialized = false;
        return;

    }else{
        quit = false;
        initialized = true;
    }

    delay = 32;
    window = new Window(width, height);
    scene = new Scene();
    Camera *camera = new Camera();

}

Tesseract::~Tesseract()
{

}

void Tesseract::update()
{

    // update keymap
	while (SDL_PollEvent(&ev) != 0)
	{
        // pressed keys
    	if (ev.type == SDL_KEYDOWN)
		{
			if (ev.key.keysym.sym == SDLK_ESCAPE)
			{
				this->quit = true;
			}
		}
	}


    //render
    this->window->clean();
    for(int i = 0; i < this->scene->qtdModels; i++){
        Model model = this->scene->models[i];
        model.draw(*window);
    }
	this->window->atualiza();

    //delay -> 32ms = 60fps
    SDL_Delay(this->delay);

}

bool Tesseract::isRunning(){
    return this->initialized;
}