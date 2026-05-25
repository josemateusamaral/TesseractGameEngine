#include "window.h"
#include "Vec3.h"
#include <bits/stdc++.h>
#include "light.h"

/**
 * @brief Generate a window with a specific width and height
 * 
 * @param width Largura em pixel
 * @param height Altura em pixel
 */
Window::Window(int width,int height)
:width{width}, height{height}
{

    window = SDL_CreateWindow( "Tesseract Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    zBuffer = new float[width*height];
    colorBuffer = new uint32_t[width*height*3];
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

}

/**
 * @brief Refresh the window with the current color buffer
 * 
 */
void Window::refresh()
{
    // copy color buffer to texture
    SDL_UpdateTexture(texture,NULL,colorBuffer,width * sizeof(uint32_t));

    // render texture to screen
    SDL_RenderCopy(renderer,texture,NULL,NULL);

    // present renderer to screen
    SDL_RenderPresent(renderer);
}

/**
 * @brief Função para limpar a janela para o proximo desenho.
 * 
 */
void Window::clean()
{
    // clean renderer
    SDL_RenderClear(renderer);

    // clean zBuffer
    std::fill(this->zBuffer, this->zBuffer + (this->width * this->height), std::numeric_limits<float>::infinity());

    // clear color buffer
    std::fill(colorBuffer,colorBuffer + (width * height),0x00000000);

}

/**
 * @brief Draw line between two points with a specific color
 * 
 * @param v1 ponto v1 
 * @param v2 ponto v2
 * @param r componente vermelho da cor
 * @param g componente verde da cor
 * @param b componente azul da cor
 * 
 * @author Henrique Heiderscheidt, Jose Mateus Amaral
 */
void Window::drawLine(Vec3 &v1, Vec3 &v2, int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer,r,g,b,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, v1.x,v1.y,v2.x,v2.y);
}

/**
 * @brief Draw line between two points
 * 
 * @param v1 ponto v1 
 * @param v2 ponto v2
 * @param r componente vermelho da cor
 * @param g componente verde da cor
 * @param b componente azul da cor
 * 
 * @author Henrique Heiderscheidt
 */
void Window::drawLine(Vec3 &v1, Vec3 &v2)
{
    SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, v1.x,v1.y,v2.x,v2.y);
}

/**
 * @brief Draw polygon between three points with a specific color
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
void Window::drawBlankPolygon(Vec3 &p1, Vec3 &p2, Vec3 &p3,int r, int g, int b){

    // polygon boundbox
    float topY,bottomY,maxLeft,maxRight;
    //top
    if(p1.y > p2.y && p1.y > p3.y){
        topY = p1.y;
    }
    else if(p2.y > p1.y && p2.y > p3.y){
        topY = p2.y;
    }
    else{
        topY = p3.y;
    }
    //bottom
    if(p1.y < p2.y && p1.y < p3.y){
        bottomY = p1.y;
    }
    else if(p2.y < p1.y && p2.y < p3.y){
        bottomY = p2.y;
    }
    else{
        bottomY = p3.y;
    }
    //left
    if(p1.x < p2.x && p1.x < p3.x){
        maxLeft = p1.x;
    }
    else if(p2.x < p1.x && p2.x < p3.x){
        maxLeft = p2.x;
    }
    else{
        maxLeft = p3.x;
    }
    //right
    if(p1.x > p2.x && p1.x > p3.x){
        maxRight = p1.x;
    }
    else if(p2.x > p1.x && p2.x > p3.x){
        maxRight = p2.x;
    }
    else{
        maxRight = p3.x;
    }

    // top left corner of the boundbox
    float px = maxLeft; 
    float py = topY;
    float sizeX = maxRight - maxLeft;
    float sizeY = topY - bottomY;

    // set polygon color
    SDL_SetRenderDrawColor(renderer,r,g,b,255);

    // loop through the pixels of the boundbox and check if they are inside the polygon
    for( int x = 0 ; x < sizeX ; x++ ){
        for( int y = 0 ; y < sizeY ; y++ ){
            if(isPixelInsidePolygon(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,px+x,py-y)){
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
bool Window::isPixelInsidePolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
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

void Window::drawTexturedPolygon(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3, Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, unsigned char* data, int texW, int texH, Light** lights, int nLights) 
{

    // polygon boundbox
    int topY,bottomY,maxLeft,maxRight;
    // top
    if(p1.y > p2.y && p1.y > p3.y){
        topY = p1.y;
    }
    else if(p2.y > p1.y && p2.y > p3.y){
        topY = p2.y;
    }
    else{
        topY = p3.y;
    }
    if(topY >= this->height) topY = this->height - 1;
    // bottom
    if(p1.y < p2.y && p1.y < p3.y){
        bottomY = p1.y;
    }
    else if(p2.y < p1.y && p2.y < p3.y){
        bottomY = p2.y;
    }
    else{
        bottomY = p3.y;
    }
    if(bottomY < 0) bottomY = 0;
    // left
    if(p1.x < p2.x && p1.x < p3.x){
        maxLeft = p1.x;
    }
    else if(p2.x < p1.x && p2.x < p3.x){
        maxLeft = p2.x;
    }
    else{
        maxLeft = p3.x;
    }
    if(maxLeft < 0) maxLeft = 0;

    // right
    if(p1.x > p2.x && p1.x > p3.x){
        maxRight = p1.x;
    }
    else if(p2.x > p1.x && p2.x > p3.x){
        maxRight = p2.x;
    }
    else{
        maxRight = p3.x;
    }
    if(maxRight >= this->width) maxRight = this->width - 1;

    // top left corner of the boundbox
    float px = maxLeft; 
    float py = topY; 
    float sizeX = maxRight - maxLeft;
    float sizeY = topY - bottomY;

    // calculate the area of the polygon
    float areaTotal = area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

    if (areaTotal == 0) return;

    // calculate lights effect
    float lightEffetR = 0.0f;
    float lightEffetG = 0.0f;
    float lightEffetB = 0.0f;
    for (int i = 0; i < nLights; i++) { 
        lights[i]->apply( v1, v2, v3, lightEffetR, lightEffetG, lightEffetB);
    }

    // loop through the bounding box of the triangle
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {

            // pixel screen space position
            int px_atual = px + x;
            int py_atual = py - y;

            // test area
            float a1 = area(px_atual, py_atual, p2.x, p2.y, p3.x, p3.y);
            float a2 = area(p1.x, p1.y, px_atual, py_atual, p3.x, p3.y);
            float a3 = area(p1.x, p1.y, p2.x, p2.y, px_atual, py_atual);

            //float w1 = edge(p2.x,p2.y,p3.x,p3.y,x,y);
            //float w2 = edge(p3.x,p3.y,p1.x,p1.y,x,y);
            //float w3 = edge(p1.x,p1.y,p2.x,p2.y,x,y);
            //if(w1 >= 0 && w2 >= 0 && w3 >= 0)

            // verify if the the pixel is inside the polygon
            if (abs(areaTotal - (a1 + a2 + a3)) < 0.01) {
                
                // baricentric weights
                float w1 = a1 / areaTotal;
                float w2 = a2 / areaTotal;
                float w3 = a3 / areaTotal;

                // test zbuffer
                float z = w1 * p1.z + w2 * p2.z + w3 * p3.z;
                int bufferIndex = ( py_atual * this->width ) + px_atual;
                if (z < this->zBuffer[bufferIndex]) {
                    this->zBuffer[bufferIndex] = z;
                } else {
                    continue;
                }

                // Interpola o UV para este pixel exato
                float u = w1 * uv1.x + w2 * uv2.x + w3 * uv3.x;
                float v = w1 * uv1.y + w2 * uv2.y + w3 * uv3.y;

                // Enrola o UV para sempre ficar entre 0.0 e 1.0 (permite textura repetida)
                u = u - floor(u);
                v = v - floor(v);

                // Converte UV para coordenada de pixel na imagem
                int tx = (int)(u * (texW - 1));
                int ty = (int)(v * (texH - 1));

                // Clamping de segurança extra contra problemas de arredondamento de float
                if (tx < 0) tx = 0;
                if (tx >= texW) tx = texW - 1;
                if (ty < 0) ty = 0;
                if (ty >= texH) ty = texH - 1;

                // Texture index (assuming format RGB, 3 bytes per pixel)
                int idx = (ty * texW + tx) * 3;

                // fill color buffer
                Uint32 rt = data[idx] * lightEffetR;
                Uint32  gt = data[idx + 1] * lightEffetG;
                Uint32 bt = data[idx + 2] * lightEffetB;
                colorBuffer[bufferIndex] = (255 << 24) | (rt << 16) | (gt << 8) | bt;

            }
        }
    }
}

void Window::createBoundBox(Vec3 &p1, Vec3 &p2, Vec3 &p3, float &minX, float &maxX, float &minY, float &maxY) {
    minX = std::min({p1.x, p2.x, p3.x});
    maxX = std::max({p1.x, p2.x, p3.x});
    minY = std::min({p1.y, p2.y, p3.y});
    maxY = std::max({p1.y, p2.y, p3.y});
}

float Window::edge( float ax, float ay, float bx, float by, float px, float py){
    return ( px - ax ) * ( by - ay ) - ( py - ay ) * ( bx - ax );
}

int Window::getWidth(){
    return this->width;
}

int Window::getHeight(){
    return this->height;
}

SDL_Window* Window::getWindow(){
    return this->window;
}

Window::~Window()
{
    SDL_DestroyWindow(window);
}

