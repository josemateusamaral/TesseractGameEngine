#include "renderer.h"
#include <pthread.h>
#include <cmath>

Renderer::Renderer() {
    // Inicialização do renderer
}

Renderer::~Renderer() {
    // Limpeza de recursos do renderer
}

struct RenderThreadData {
    Renderer* renderer;
    Model* model;
    Window* window;
    Camera* camera;

    int start;
    int end;
};

void* renderWorker(void* arg)
{
    RenderThreadData* data = (RenderThreadData*)arg;

    Renderer* renderer = data->renderer;
    Model* model = data->model;
    Window* window = data->window;
    Camera* camera = data->camera;

    int R = 255, G = 255, B = 255;
    float angulo;

    float origem[3] = {
        camera->getX(),
        camera->getY(),
        camera->getZ()
    };

    float a[3] = {
        model->getX(),
        model->getY(),
        model->getZ()
    };

    Vec3 cam{origem, a};

    for (int i = data->start; i < data->end; i += 3)
    {
        int i0 = model->indices[i];
        int i1 = model->indices[i + 1];
        int i2 = model->indices[i + 2];

        if(!model->screenSpaceBuffer[i0] &&
           !model->screenSpaceBuffer[i1] &&
           !model->screenSpaceBuffer[i2])
            continue;

        float b[3] = {
            model->pontos[i0].x,
            model->pontos[i0].y,
            model->pontos[i0].z
        };

        float c[3] = {
            model->pontos[i1].x,
            model->pontos[i1].y,
            model->pontos[i1].z
        };

        float d[3] = {
            model->pontos[i2].x,
            model->pontos[i2].y,
            model->pontos[i2].z
        };

        Vec3 v1(b, c);
        Vec3 v2(b, d);

        Vec3 normal = v1.produto_vetorial(v2);

        if (!(cam.angulo_entre_vetores(normal) > 90 ||
              !model->backfaceCulling))
            continue;

        switch(model->renderType)
        {
            case 1:
                renderer->drawLine(window,
                                   model->projection[i0],
                                   model->projection[i1]);

                renderer->drawLine(window,
                                   model->projection[i1],
                                   model->projection[i2]);

                renderer->drawLine(window,
                                   model->projection[i2],
                                   model->projection[i0]);
                break;

            case 2:

                if (model->shadowCast)
                {
                    angulo = 90;

                    R = model->corR -
                        ((255.0f / 180.0f) * angulo);

                    G = model->corG -
                        ((255.0f / 180.0f) * angulo);

                    B = model->corB -
                        ((255.0f / 180.0f) * angulo);
                }
                else
                {
                    R = model->corR;
                    G = model->corG;
                    B = model->corB;
                }

                renderer->drawBlankPolygon(
                    window,
                    model->projection[i0],
                    model->projection[i1],
                    model->projection[i2],
                    R, G, B
                );

                break;

            case 3:

                renderer->drawTexturedPolygon(
                    window,

                    model->projection[i0],
                    model->projection[i1],
                    model->projection[i2],

                    model->pontos[i0],
                    model->pontos[i1],
                    model->pontos[i2],

                    model->uvs[i0],
                    model->uvs[i1],
                    model->uvs[i2],

                    (unsigned char*)model->diffuseTexture->data,
                    model->diffuseTexture->width,
                    model->diffuseTexture->height,

                    model->lights,
                    model->nLights,

                    model->shadowMapBuffer,
                    100,
                    100,

                    model->shadowCast
                );

                break;
        }
    }

    return nullptr;
}

void Renderer::render(Model *model,Window *window,Camera *camera)
{
    model->calcular_pontos_3D();

    this->project(
        camera,
        model->pontos,
        model->projection,
        model->nVertices,
        model->screenSpaceBuffer,
        window->getHeight(),
        window->getWidth()
    );

    const int NUM_THREADS = 4;

    pthread_t threads[NUM_THREADS];
    RenderThreadData data[NUM_THREADS];

    int triangles = model->indexCount / 3;
    int trianglesPerThread = triangles / NUM_THREADS;

    for(int t = 0; t < NUM_THREADS; t++)
    {
        int startTri = t * trianglesPerThread;

        int endTri = (t == NUM_THREADS - 1)
            ? triangles
            : startTri + trianglesPerThread;

        data[t].renderer = this;
        data[t].model = model;
        data[t].window = window;
        data[t].camera = camera;

        data[t].start = startTri * 3;
        data[t].end   = endTri * 3;

        pthread_create(
            &threads[t],
            nullptr,
            renderWorker,
            &data[t]
        );
    }

    for(int t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], nullptr);
    }
}

struct ProjectThreadData {
    Camera* camera;
    Vec3* vertices;
    Vec3* projection;
    bool* screenSpaceBuffer;
    int start, end;
    int bufferHeight, bufferWidth;
    int centerX, centerY;
    float cosY, sinY, cosP, sinP, cosR, sinR;
    float cx, cy, cz;
};

static void* projectWorker(void* arg) {
    ProjectThreadData* d = static_cast<ProjectThreadData*>(arg);

    for (int i = d->start; i < d->end; i++) {
        float x = d->vertices[i].x - d->cx;
        float y = d->vertices[i].y - d->cy;
        float z = d->vertices[i].z - d->cz;

        float dx = x * d->cosY - z * d->sinY;
        float dz = x * d->sinY + z * d->cosY;
        x = dx; z = dz;

        float dy = y * d->cosP - z * d->sinP;
        dz = y * d->sinP + z * d->cosP;
        y = dy; z = dz;

        dx = x * d->cosR - y * d->sinR;
        dy = x * d->sinR + y * d->cosR;
        x = dx; y = dy;

        float px = (d->camera->dist_f * x) / z;
        float py = (d->camera->dist_f * y) / z;
        d->projection[i].x = px * -1 + d->centerX;
        d->projection[i].y = py * -1 + d->centerY;
        d->projection[i].z = z;

        d->screenSpaceBuffer[i] =
            !((d->projection[i].x > d->bufferWidth || d->projection[i].x < 0) &&
              (d->projection[i].y > d->bufferHeight || d->projection[i].y < 0))
            && z > 0.1f;
    }
    return nullptr;
}

void Renderer::project(Camera *camera, Vec3* vertices, Vec3* projection, int nVertices,
                        bool* screenSpaceBuffer, int bufferHeight, int bufferWidth){

    float pitch = -camera->hpr.x * M_PI / 180.0;
    float yaw   = -camera->hpr.y * M_PI / 180.0;
    float roll  = -camera->hpr.z * M_PI / 180.0;

    float cosY = cos(yaw),   sinY = sin(yaw);
    float cosP = cos(pitch), sinP = sin(pitch);
    float cosR = cos(roll),  sinR = sin(roll);

    float cx = camera->getX();
    float cy = camera->getY();
    float cz = camera->getZ();

    int centerX = bufferWidth / 2;
    int centerY = bufferHeight / 2;

    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    ProjectThreadData threadData[NUM_THREADS];

    int chunk = nVertices / NUM_THREADS;
    int remainder = nVertices % NUM_THREADS;
    int start = 0;

    for (int t = 0; t < NUM_THREADS; t++) {
        int end = start + chunk + (t < remainder ? 1 : 0);

        threadData[t] = { camera, vertices, projection, screenSpaceBuffer,
                           start, end, bufferHeight, bufferWidth,
                           centerX, centerY,
                           cosY, sinY, cosP, sinP, cosR, sinR,
                           cx, cy, cz };

        pthread_create(&threads[t], nullptr, projectWorker, &threadData[t]);
        start = end;
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], nullptr);
    }
}

void Renderer::drawTexturedPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3, Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, unsigned char* data, int texW, int texH, Light** lights, int nLights, bool* shadowMapBuffer, int shadowMapWidth, int shadowMapHeight, bool shadowCast) 
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
    if(topY >= window->getHeight()) topY = window->getHeight() - 1;
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
    if(maxRight >= window->getWidth()) maxRight = window->getWidth() - 1;

    // top left corner of the boundbox
    float px = maxLeft; 
    float py = topY; 
    float sizeX = maxRight - maxLeft;
    float sizeY = topY - bottomY;

    // calculate the area of the polygon
    float areaTotal = this->area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

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
            float a1 = this->area(px_atual, py_atual, p2.x, p2.y, p3.x, p3.y);
            float a2 = this->area(p1.x, p1.y, px_atual, py_atual, p3.x, p3.y);
            float a3 = this->area(p1.x, p1.y, p2.x, p2.y, px_atual, py_atual);

            // verify if the the pixel is inside the polygon
            if (abs(areaTotal - (a1 + a2 + a3)) < 0.01) {
                
                // baricentric weights
                float w1 = a1 / areaTotal;
                float w2 = a2 / areaTotal;
                float w3 = a3 / areaTotal;

                // test zbuffer
                float z = w1 * p1.z + w2 * p2.z + w3 * p3.z;
                int bufferIndex = ( py_atual * window->getWidth() ) + px_atual;
                if (z < window->zBuffer[bufferIndex]) {
                    window->zBuffer[bufferIndex] = z;
                } else {
                    continue;
                }

                // Interpolate UV coordinates with perspective correction
                float iz1 = 1.0f / p1.z;
                float iz2 = 1.0f / p2.z;
                float iz3 = 1.0f / p3.z;
                float u =
                (
                    w1 * uv1.x * iz1 +
                    w2 * uv2.x * iz2 +
                    w3 * uv3.x * iz3
                )
                /
                (
                    w1 * iz1 +
                    w2 * iz2 +
                    w3 * iz3
                );

                float v =
                (
                    w1 * uv1.y * iz1 +
                    w2 * uv2.y * iz2 +
                    w3 * uv3.y * iz3
                )
                /
                (
                    w1 * iz1 +
                    w2 * iz2 +
                    w3 * iz3
                );

                // Enrola o UV para sempre ficar entre 0.0 e 1.0 (permite textura repetida)
                u = u - floor(u);
                v = v - floor(v);

                // shadow mapping test
                float shadowFactor = 1.0f;
                if (shadowMapBuffer != nullptr && shadowCast){
                    // Converte UV para coordenada de pixel na imagem
                    int tx = (int)(u * (shadowMapWidth - 1));
                    int ty = (int)(v * (shadowMapHeight - 1));

                    // Clamping de segurança extra contra problemas de arredondamento de float
                    if (tx < 0) tx = 0;
                    if (tx >= shadowMapWidth) tx = shadowMapWidth - 1;
                    if (ty < 0) ty = 0;
                    if (ty >= shadowMapHeight) ty = shadowMapHeight - 1;

                    // Texture index (assuming format RGB, 3 bytes per pixel)
                    int idx = (ty * shadowMapWidth + tx);
                    if(!shadowMapBuffer[idx]){
                        shadowFactor = 0.5f;
                    }
                }                

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
                Uint32 rt = (data[idx] * lightEffetR) * shadowFactor;
                Uint32  gt = (data[idx + 1] * lightEffetG) * shadowFactor;
                Uint32 bt = (data[idx + 2] * lightEffetB) * shadowFactor;

                // fill color buffer
                window->colorBuffer[bufferIndex] = (255 << 24) | (rt << 16) | (gt << 8) | bt;
                
            }
        }
    }
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
void Renderer::drawLine(Window* window, Vec3 &v1, Vec3 &v2)
{
    SDL_SetRenderDrawColor(window->renderer,255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(window->renderer, v1.x,v1.y,v2.x,v2.y);
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
void Renderer::drawLine(Window* window, Vec3 &v1, Vec3 &v2, int r, int g, int b)
{
    SDL_SetRenderDrawColor(window->renderer,r,g,b,SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(window->renderer, v1.x,v1.y,v2.x,v2.y);
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
void Renderer::drawBlankPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3,int r, int g, int b){

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
    SDL_SetRenderDrawColor(window->renderer,r,g,b,255);

    // loop through the pixels of the boundbox and check if they are inside the polygon
    for( int x = 0 ; x < sizeX ; x++ ){
        for( int y = 0 ; y < sizeY ; y++ ){
            if(this->isPixelInsidePolygon(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,px+x,py-y)){
                SDL_RenderDrawPoint(window->renderer,px + x,py - y);
            }
        }
    }
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
bool Renderer::isPixelInsidePolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
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
float Renderer::area(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}








void Renderer::createShadowMap(Camera *camera, float* zBuffer, Model** eBuffer, Model* model, int shadowMapWidth, int shadowMapHeight){//, Vec3* vertices, Vec3* projection, int nVertices, bool* screenSpaceBuffer, int bufferHeight, int bufferWidth, char* dataShadowMap, int shadowMapWidth, int shadowMapHeight){
    
    model->calcular_pontos_3D();

    // project vertices
    this->project(camera, model->pontos, model->projection, model->nVertices, model->screenSpaceBuffer, shadowMapHeight, shadowMapWidth);

    int R = 255, G = 255, B = 255;
    float angulo;

    // camera vector
    float origem[3] = {camera->getX(), camera->getY(), camera->getZ()};
    float a[3] = {model->getX(), model->getY(), model->getZ()};
    Vec3 cam{origem, a};

    // indexes
    for (int i = 0; i < model->indexCount; i += 3)
    {

        int i0 = model->indices[i];
        int i1 = model->indices[i + 1];
        int i2 = model->indices[i + 2];

        // ignore polygons out off screen space
        if(!model->screenSpaceBuffer[i0] && !model->screenSpaceBuffer[i1] && !model->screenSpaceBuffer[i2]) continue;

        //polygon
        Vec3 p0 = model->pontos[i0];
        Vec3 p1 = model->pontos[i1];
        Vec3 p2 = model->pontos[i2];
        float b[3] = {model->pontos[i0].x, model->pontos[i0].y, model->pontos[i0].z};
        float c[3] = {model->pontos[i1].x, model->pontos[i1].y, model->pontos[i1].z};
        float d[3] = {model->pontos[i2].x, model->pontos[i2].y, model->pontos[i2].z};
        Vec3 v1(b, c);
        Vec3 v2(b, d);
        Vec3 normal = v1.produto_vetorial(v2);

        // backface culling
        if (!(cam.angulo_entre_vetores(normal) > 90 || !model->backfaceCulling)) continue;

        // TEXTURED
        this->drawShadowMap(
            //projections
            model->projection[i0],
            model->projection[i1],
            model->projection[i2],
            //vertices
            model->pontos[i0],
            model->pontos[i1],
            model->pontos[i2],
            //uvs
            model->uvs[i0],
            model->uvs[i1],
            model->uvs[i2],
            //lights
            zBuffer,
            eBuffer,
            model,
            640,
            480,
            100,
            100
        );

    }

    //printf("\nshadow map created...");

}

void Renderer::drawShadowMap(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3 ,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, float* shadowZBuffer, Model** shadowEBuffer, Model* model, int shadowWindowWidth, int shadowWindowHeight, int shadowMapHeight, int shadowMapWidth){

    //printf("\ndrawing shadow map...");

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
    if(topY >= shadowWindowHeight) topY = shadowWindowHeight - 1;
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
    if(maxRight >= shadowWindowWidth) maxRight = shadowWindowWidth - 1;

    // top left corner of the boundbox
    float px = maxLeft; 
    float py = topY; 
    float sizeX = maxRight - maxLeft;
    float sizeY = topY - bottomY;

    // calculate the area of the polygon
    float areaTotal = this->area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

    if (areaTotal == 0) return;

    // loop through the bounding box of the triangle
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {

            // pixel screen space position
            int px_atual = px + x;
            int py_atual = py - y;

            // test area
            float a1 = this->area(px_atual, py_atual, p2.x, p2.y, p3.x, p3.y);
            float a2 = this->area(p1.x, p1.y, px_atual, py_atual, p3.x, p3.y);
            float a3 = this->area(p1.x, p1.y, p2.x, p2.y, px_atual, py_atual);

            // verify if the the pixel is inside the polygon
            if (abs(areaTotal - (a1 + a2 + a3)) < 0.01) {
                
                // baricentric weights
                float w1 = a1 / areaTotal;
                float w2 = a2 / areaTotal;
                float w3 = a3 / areaTotal;

                // test zbuffer
                float z = w1 * p1.z + w2 * p2.z + w3 * p3.z;
                int bufferIndex = ( py_atual * shadowWindowWidth ) + px_atual;
                if (z < shadowZBuffer[bufferIndex]) {
                    shadowZBuffer[bufferIndex] = z;
                } else {
                    continue;
                }

                // Interpola o UV para este pixel exato
                float iz1 = 1.0f / p1.z;
                float iz2 = 1.0f / p2.z;
                float iz3 = 1.0f / p3.z;
                float u =
                (
                    w1 * uv1.x * iz1 +
                    w2 * uv2.x * iz2 +
                    w3 * uv3.x * iz3
                )
                /
                (
                    w1 * iz1 +
                    w2 * iz2 +
                    w3 * iz3
                );

                float v =
                (
                    w1 * uv1.y * iz1 +
                    w2 * uv2.y * iz2 +
                    w3 * uv3.y * iz3
                )
                /
                (
                    w1 * iz1 +
                    w2 * iz2 +
                    w3 * iz3
                );

                // Enrola o UV para sempre ficar entre 0.0 e 1.0 (permite textura repetida)
                u = u - floor(u);
                v = v - floor(v);

                // Converte UV para coordenada de pixel na imagem
                int tx = (int)(u * (shadowMapWidth - 1));
                int ty = (int)(v * (shadowMapHeight - 1));

                // Clamping de segurança extra contra problemas de arredondamento de float
                if (tx < 0) tx = 0;
                if (tx >= shadowMapWidth) tx = shadowMapWidth - 1;
                if (ty < 0) ty = 0;
                if (ty >= shadowMapHeight) ty = shadowMapHeight - 1;

                // Texture index (assuming format RGB, 3 bytes per pixel)
                model->shadowMapBuffer[(ty * shadowMapWidth + tx)] = true;

            }
        }
    }

    //printf("\nshadow map drawed...");

}
