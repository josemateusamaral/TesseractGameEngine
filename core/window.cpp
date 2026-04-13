#include "window.h"
#include "vec3.h"
#include <bits/stdc++.h>

/**
 * @brief Construtor de uma janela SDL2
 * 
 * @param width Largura em pixel
 * @param height Altura em pixel
 */
Window::Window(int width,int height)
{
    window = SDL_CreateWindow( "Window(s)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}

/**
 * @brief Função para atualizar a janela
 * 
 */
void Window::atualiza()
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderPresent(renderer);
}

/**
 * @brief Função para limpar a janela para o proximo desenho.
 * 
 */
void Window::limpar()
{
    SDL_RenderClear(renderer);

}

/**
 * @brief Método para desenhar uma linha do ponto p1 até p2
 * 
 * @param p1 ponto p1
 * @param p2 ponto p2
 * 
 * @author Henrique Heiderscheidt
 */
void Window::desenha(Ponto &p1, Ponto &p2, int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer,r,g,b,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, p1.x,p1.y,p2.x,p2.y);
}

void Window::desenha(Ponto &p1, Ponto &p2)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, p1.x,p1.y,p2.x,p2.y);
}

/**
 * @brief Desenhar os poligonos de um solido geometrico
 * 
 * @param p1 Ponto 1
 * @param p2 Ponto 2
 * @param p3 Ponto 3
 * @param r Gradiente Vermelho
 * @param g Gradiente Verde
 * @param b Gradiente Azul
 * 
 * @author Jose Mateus Amaral
 */
void Window::desenhar_poligono(Ponto &p1, Ponto &p2, Ponto &p3,int r, int g, int b){

    // meio do poligono
    //double middleX = (((p1.x + p2.x + p3.x) - (400 * 3))/3);
    //double middleY = (((p1.y + p2.y + p3.y) - (400 * 3))/3);
    //Ponto meio{middleX,middleY};

    // quadrado ao redor do poligono
    double topY,bottomY,maxLeft,maxRight;

    //Encontra o ponto mais acima do triangulo
    if(p1.y > p2.y && p1.y > p3.y){
        topY = p1.y;
    }
    else if(p2.y > p1.y && p2.y > p3.y){
        topY = p2.y;
    }
    else{
        topY = p3.y;
    }

    //Encontra o ponto mais abaixo do triangulo
    if(p1.y < p2.y && p1.y < p3.y){
        bottomY = p1.y;
    }
    else if(p2.y < p1.y && p2.y < p3.y){
        bottomY = p2.y;
    }
    else{
        bottomY = p3.y;
    }

    //Encontra o ponto mais a esquerda do triangulo
    if(p1.x < p2.x && p1.x < p3.x){
        maxLeft = p1.x;
    }
    else if(p2.x < p1.x && p2.x < p3.x){
        maxLeft = p2.x;
    }
    else{
        maxLeft = p3.x;
    }

    //Encontra o ponto mais a esquerda do triangulo
    if(p1.x > p2.x && p1.x > p3.x){
        maxRight = p1.x;
    }
    else if(p2.x > p1.x && p2.x > p3.x){
        maxRight = p2.x;
    }
    else{
        maxRight = p3.x;
    }

    // inicios do desenho do poligono
    double px = maxLeft; //pixel inicial do poligono no eixo x
    double py = topY; //pixel inicial do poligono no eixo y

    // dimensoes do poligono
    double sizeX = maxRight - maxLeft;//Largura do poligono
    double sizeY = topY - bottomY;//Altura do poligono

    SDL_SetRenderDrawColor(renderer,r,g,b,255);

    //Percorre todo quadrado em volta do poligono, pensando na otimização 
    for( int x = 0 ; x < sizeX ; x++ ){
        for( int y = 0 ; y < sizeY ; y++ ){
            //Se o ponto está dentro do poligono, então ele é desenhado
            if(estaDentro(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,px+x,py-y)){
                SDL_RenderDrawPoint(renderer,px + x,py - y);
            }
        }
    }
}

/**
 * @brief calcular a area de um triangulo no R2
 * 
 * @param x1 int x do vertice 1
 * @param y1 int y do vertice 1
 * @param x2 int x do vertice 2
 * @param y2 int y do vertice 2
 * @param x3 int x do vertice 3
 * @param y3 int y do vertice 1
 * 
 * @author Jose Mateus Amaral
 */
float Window::area(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

/**
 * @brief Verifica se o ponto está dentro do poligono
 * 
 * @param x1 x1 poligono
 * @param y1 y1 poligono
 * @param x2 x2 poligono
 * @param y2 y2 poligono
 * @param x3 x3 poligono
 * @param y3 y3 poligono
 * @param x Posição x do ponto
 * @param y Posição y do ponto
 * @return true Se está dentro do poligono
 * @return false Se está fora do poligono
 * 
 * @author Jose Mateus Amaral
 */
bool Window::estaDentro(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
{  
    // calcular area do poligono
    float areaPoligono = area(x1, y1, x2, y2, x3, y3);

    //calcular area de um triangulo formado pelos pontos 2 e 3 do poligono e o ponto sendo testado
    float triangulo1 = area(x, y, x2, y2, x3, y3);

    //calcular area de um triangulo formado pelos pontos 1 e 3 do poligono e o ponto sendo testado
    float triangulo2 = area(x1, y1, x, y, x3, y3);

    //calcular area de um triangulo formado pelos pontos 1 e 2 do poligono e o ponto sendo testado
    float triangulo3 = area(x1, y1, x2, y2, x, y);

    // retornar se a area do poligono é igual a soma das areas dos triangulos formados com o ponto sendo testado
    return ( areaPoligono == triangulo1 + triangulo2 + triangulo3 );
}

Window::~Window()
{
    SDL_DestroyWindow(window);
}

