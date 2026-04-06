// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include "utils/janela.h"
#include "pontos/ponto.h"
#include "pontos/ponto3.h"
#include "formas/cubo/cubo.h"
#include "formas/esfera/esfera.h"
#include "formas/cone/cone.h"
#include "formas/cilindro/cilindro.h"
#include "formas/forma.h"
#include "utils/Vec3.h"
#include <iostream>
#include "utils/camera.h"

using namespace std;

struct Planeta
{
	Esfera modelo;
	double angulo = 0;
	double distancia = 10;
	double velocidade = 1;
};

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{
	// Eventos e loop principal
	bool quit = false;
	SDL_Event ev;

	Camera *camera = camera->criar();

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		// Create window
		Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT);

		// carregar textura do fundo
		SDL_Texture *textura;
		SDL_Surface *Loading_Surf;
		Loading_Surf = SDL_LoadBMP("fundo.bmp");
		textura = SDL_CreateTextureFromSurface(window.renderer, Loading_Surf);

		// criar objetos para testes de renderizacao
		Cubo cuboX{Ponto3(5, 5, 10), 1.0};
		Cubo cuboZ{Ponto3(0, 5, 10), 1.0};
		Cubo cuboY{Ponto3(-5, 5, 10), 1.0};
		Cone cone{Ponto3(5, -1, 10), 20, 1};
		Cilindro cilindro{Ponto3(-5, -1, 10), 20, 3};

		// struc que guarda os dados do planeta como: modelo3d, velocidade, tamanho da orbita
		struct Planeta
		{
			Forma *modelo;
			Forma *orbitar;
			double angulo;
			double raio;
			double velocidade;
		};

		// modelo3d do sol
		Esfera sol{Ponto3(7, -16, 60), 2, 60};
		sol.girar(10, 10, 10);
		sol.renderType = 2;
		sol.corR = 255;
		sol.corG = 255;
		sol.corB = 0;
		sol.comSombra = false;

		// modelo 3d de um cubo verde que pode ser interpretado como um satelite
		Cubo cuboSatelite{Ponto3(5, 5, 10), 1};
		Planeta satelite;
		cuboSatelite.renderType = 2;
		cuboSatelite.corR = 0;
		cuboSatelite.corG = 255;
		cuboSatelite.corB = 0;
		satelite.modelo = &cuboSatelite;
		satelite.angulo = 0;
		satelite.raio = 30;
		satelite.velocidade = 3;
		satelite.orbitar = &sol;

		// modelo 3d de marte
		Esfera marte{Ponto3(0, -2, 10), 1.5, 60};
		marte.renderType = 2;
		marte.girar(10, 10, 10);
		marte.corR = 255;
		marte.corG = 180;
		marte.corB = 180;
		Planeta planetaMarte;
		planetaMarte.modelo = &marte;
		planetaMarte.angulo = 0;
		planetaMarte.raio = 8;
		planetaMarte.velocidade = 1;
		planetaMarte.orbitar = &sol;

		// modelo 3d da terra
		Esfera terra{Ponto3(0, -2, 10), 1.5, 60};
		terra.renderType = 2;
		terra.girar(10, 10, 10);
		terra.corR = 180;
		terra.corG = 180;
		terra.corB = 255;
		Planeta planetaTerra;
		planetaTerra.modelo = &terra;
		planetaTerra.angulo = 0;
		planetaTerra.raio = 20;
		planetaTerra.velocidade = 2;
		planetaTerra.orbitar = &sol;

		// modelo3d da lua
		Esfera lua{Ponto3(0, -2, 10), 1, 60};
		lua.renderType = 2;
		lua.girar(10, 10, 10);
		lua.corR = 255;
		lua.corG = 255;
		lua.corB = 255;
		Planeta planetaLua;
		planetaLua.modelo = &lua;
		planetaLua.angulo = 0;
		planetaLua.raio = 3;
		planetaLua.velocidade = -5;
		planetaLua.orbitar = &terra;

		// lista de structs dos planetas
		struct Planeta planetas[4] = {planetaTerra, planetaLua, planetaMarte, satelite};

		// objetos para renderizar
		Forma *render[5] = {&terra, &marte, &sol, &lua, &cuboSatelite};

		double angulo = 0;
		double raio = 8;
		double npx, npz;
		double b[3];

		// ponto de luz
		double a[3] = {sol.posicao.x, sol.posicao.y, sol.posicao.z};

		while (!quit)
		{

			window.limpar();

			///////////////////////////////////////////////////
			// DEMO PLANETARIO - INICIO

			// colocar fundo de estrelas
			SDL_RenderCopy(window.renderer, textura, NULL, NULL);

			// girar cubo do satelite
			cuboSatelite.girar(1, 1, 1);

			// girar planetas ao redor do sol e atualizar sua iluminacao
			for (int i = 0; i < 4; i++)
			{
				planetas[i].angulo += static_cast<int>(planetas[i].velocidade) % 360;

				// nova posicao do planeta
				npx = planetas[i].orbitar->posicao.x + (planetas[i].raio * cos((planetas[i].angulo + (100 * i)) * M_PI / 180));
				npz = planetas[i].orbitar->posicao.z + (planetas[i].raio * sin((planetas[i].angulo + (100 * i)) * M_PI / 180));

				// atualizar posicao do planeta
				planetas[i].modelo->posicao.x = npx;
				planetas[i].modelo->posicao.z = npz;
				planetas[i].modelo->posicao.y = planetas[i].orbitar->posicao.y;

				// atualizar iluminacao do planeta
				b[0] = planetas[i].modelo->posicao.x;
				b[1] = planetas[i].modelo->posicao.y;
				b[2] = planetas[i].modelo->posicao.z;
				planetas[i].modelo->iluminacao = Vec3(b, a);
			}

			// ordenar a lista de planetas para começar renderizando de traz para frente
			Forma *swap;
			for (int j = 0; j < 5; j++)
			{
				for (int k = 0; k < 5; k++)
				{
					if (render[k]->posicao.z < render[k + 1]->posicao.z)
					{
						swap = render[k + 1];
						render[k + 1] = render[k];
						render[k] = swap;
					}
				}
			}

			// renderizar objetos
			for (int l = 0; l < 5; l++)
			{
				render[l]->desenhar(window);
			}

			// DEMO PLANETARIO - FIM
			////////////////////////////////////q

			// testes de desenho das formas
			//cilindro.girar(1, 1, 1);
			//cilindro.desenhar(window);
			//cone.girar(1, 1, 1);
			//cone.desenhar(window);
			//cuboX.girar(5, 0, 0);
			//cuboX.desenhar(window);
			//cuboY.girar(0, 5, 0);
			//cuboY.desenhar(window);
			//cuboZ.girar(0, 0, 5);
			//cuboZ.desenhar(window);

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
						// cout<<camera->posicao.x<<"\n";
						// mover para esquerda
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
						camera->rodarx(0.34, sol.posicao);
					}
					if (ev.key.keysym.sym == SDLK_q)
					{
						camera->rodary(0.34, sol.posicao);
					}
					if (ev.key.keysym.sym == SDLK_z)
					{
						camera->rodarz(0.34, sol.posicao);
					}
				}
			}

			window.atualiza();
			SDL_Delay(1000 / 24);
		}
		SDL_Quit();
	}
	return 0;
}
