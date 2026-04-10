#include <SDL2/SDL.h>
#include <stdio.h>
#include "utils/janela.h"
#include "pontos/ponto.h"
#include "pontos/ponto3.h"
#include "formas/forma.h"
#include "utils/Vec3.h"
#include "utils/model.h"
#include <iostream>
#include "utils/camera.h"

using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{
	// Eventos e loop principal
	bool quit = false;
	SDL_Event ev;

	Camera *camera = new Camera();

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		// Create window
		Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT);

		// load model
		Model model{Ponto3(6, -5, 20), 2, 1};
		model.loadModel("samples/planets/cubo.glb");
		model.girar(10, 10, 10);
		model.renderType = 1;
		model.corR = 255;
		model.corG = 255;
		model.corB = 0;
		model.comSombra = false;
		model.setBackface_Culling(true);

		// scene
		Forma *render[1] = {&model};

		// pointlight
		Ponto3 light = Ponto3(model.posicao.x, model.posicao.y, model.posicao.z);

		while (!quit)
		{

			window.limpar();
			render[0]->desenhar(window);

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
						camera->rodarx(0.34, model.posicao);
					}
					if (ev.key.keysym.sym == SDLK_q)
					{
						camera->rodary(0.34, model.posicao);
					}
					if (ev.key.keysym.sym == SDLK_z)
					{
						camera->rodarz(0.34, model.posicao);
					}
				}
			}

			window.atualiza();
			SDL_Delay(32);
		}
		SDL_Quit();
	}
	return 0;
}