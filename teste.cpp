#include <SDL2/SDL.h>
#include <stdio.h>
#include "core/window.h"
#include "core/ponto3.h"
#include "core/model.h"
#include <iostream>
#include "core/camera.h"

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

		// Events
		bool quit = false;
		SDL_Event ev;

		// Create window
		Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT);

		// Create camera
		Camera *camera = new Camera();

		// load model
		Model model{"samples/model_loading/cubo.glb"};
		model.setPos(2, -2, 14);
		model.setScale(3);
		model.setBackfaceCulling(true);
		model.renderType = 3;

		// scene
		Model *render[1] = {&model};

		// pointlight
		Ponto3 light = Ponto3(model.getPos().x, model.getPos().y, model.getPos().z);

		while (!quit)
		{

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
					if (ev.key.keysym.sym == SDLK_a)
					{
						camera->mover(1);
					}
					if (ev.key.keysym.sym == SDLK_d)
					{
						camera->mover(-1);
						// mover para direita
					}
					if (ev.key.keysym.sym == SDLK_w)
					{
						camera->mover(0, 0, 1);
						// mover para tras
					}
					if (ev.key.keysym.sym == SDLK_s)
					{
						camera->mover(0, 0, -1);
					}
					if (ev.key.keysym.sym == SDLK_0)
					{
						camera->mover(0, -1, 0);
					}
					if (ev.key.keysym.sym == SDLK_1)
					{
						camera->mover(0, 1, 0);
					}

					if (ev.key.keysym.sym == SDLK_e)
					{
						camera->rodarx(0.34, model.getPos());
					}
					if (ev.key.keysym.sym == SDLK_q)
					{
						camera->rodary(0.34, model.getPos());
					}
					if (ev.key.keysym.sym == SDLK_z)
					{
						camera->rodarz(0.34, model.getPos());
					}
				}
			}

			//render
			window.clean();
			render[0]->draw(window);
			window.atualiza();
			SDL_Delay(32);

		}
		SDL_Quit();
	}
	return 0;
}