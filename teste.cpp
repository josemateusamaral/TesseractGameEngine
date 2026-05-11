#include <SDL2/SDL.h>
#include <stdio.h>
#include "core/ponto3.h"
#include "core/model.h"
#include <iostream>
#include "core/tesseract.h"

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{

		Tesseract tesseract = Tesseract(SCREEN_WIDTH, SCREEN_HEIGHT);

		// Events
		bool quit = false;
		SDL_Event ev;

		// load model
		Model model{"samples/model_loading/cubo.glb"};
		model.setPos(0, 0, 14);
		model.setScale(3);
		model.setBackfaceCulling(true);
		model.renderType = 3;

		// pointlight
		Ponto3 light = Ponto3(model.getPos().x, model.getPos().y, model.getPos().z);

		while (!quit){

			model.rotate(1,1,1);

			// entradas do teclado
			while (SDL_PollEvent(&ev) != 0)
			{

				if (ev.type == SDL_KEYDOWN)
				{
					if (ev.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = true;
					}
				}
			}

			//render
			tesseract.window->clean();
			model.draw(*tesseract.window);
			tesseract.window->atualiza();
			
			SDL_Delay(32);

		}
		SDL_Quit();
	}
	return 0;
}