#include "renderer.h"

Renderer::Renderer() {
    // Inicialização do renderer
}

Renderer::~Renderer() {
    // Limpeza de recursos do renderer
}

void Renderer::render(Model *model, Window *window, Camera *camera) {

    // project vertices
    this->project(camera, model, window->getHeight(), window->getWidth());

    //model data
    float mx = model->getX();
    float my = model->getY();
    float mz = model->getZ();
    float sm = model->getScale();

    // camera vector
    float origem[3] = {camera->getX(), camera->getY(), camera->getZ()};
    float a[3] = {mx, my, mz};
    Vec3 cam{origem, a};

    // indexes
    for (int i = 0; i < model->indexCount; i += 3)
    {

        int i0 = model->indices[i];
        int i1 = model->indices[i + 1];
        int i2 = model->indices[i + 2];
        
        // clip distance
        if(model->projection[i0].z <= 1 || model->projection[i1].z <= 1 || model->projection[i2].z <= 1) continue;

        // screen space test
        if(!model->screenSpaceBuffer[i0] && !model->screenSpaceBuffer[i1] && !model->screenSpaceBuffer[i2]) continue;


        //polygon
        Vec3 v0 = model->vertices[i0];
        v0.x = mx + (v0.x * sm);
        v0.y = my + (v0.y * sm);
        v0.z = mz + (v0.z * sm);
        Vec3 v1 = model->vertices[i1];
        v1.x = mx + (v1.x * sm);
        v1.y = my + (v1.y * sm);
        v1.z = mz + (v1.z * sm);
        Vec3 v2 = model->vertices[i2];
        v2.x = mx + (v2.x * sm);
        v2.y = my + (v2.y * sm);
        v2.z = mz + (v2.z * sm);

        //calculate camera vector
        float b[3] = {v0.x, v0.y, v0.z};
        float c[3] = {v1.x, v1.y, v1.z};
        float d[3] = {v2.x, v2.y, v2.z};
        Vec3 vector1(b, c);
        Vec3 vector2(b, d);
        Vec3 normal = vector1.produto_vetorial(vector2);

        // backface culling
        //if (!(cam.angulo_entre_vetores(normal) > 90 || !model->backfaceCulling)) continue;
    
        this->drawTexturedPolygon(
            //window
            window,
            //projections
            model->projection[i0],
            model->projection[i1],
            model->projection[i2],
            //vertices
            v0,
            v1,
            v2,
            //uvs
            model->uvs[i0],
            model->uvs[i1],
            model->uvs[i2],
            //texture
            (unsigned char*)model->diffuseTexture->data,
            model->diffuseTexture->width,
            model->diffuseTexture->height,
            //lights
            model->lights,
            model->nLights,
            //shadow map
            model->shadowMapBuffer,
            100,//height
            100,//width
            model->shadowCast
        );
    }

}

void Renderer::project(Camera *camera, Model* model, int bufferHeight, int bufferWidth){

    //model data
    Vec3* vertices = model->vertices;
    Vec3* projection = model->projection;
    int nVertices = model->nVertices;
    bool* screenSpaceBuffer = model->screenSpaceBuffer;
    float mx = model->getX();
    float my = model->getY();
    float mz = model->getZ();
    float sm = model->getScale();

    // camera inverse rotation
    float pitch = -camera->hpr.x * M_PI / 180.0;
    float yaw   = -camera->hpr.y * M_PI / 180.0;
    float roll  = -camera->hpr.z * M_PI / 180.0;
    // yaw - y
    float cosY = cos(yaw);
    float sinY = sin(yaw);
    // pitch - x
    float cosP = cos(pitch);
    float sinP = sin(pitch);
    // roll - z
    float cosR = cos(roll);
    float sinR = sin(roll);

    float cx = camera->getX();
    float cy = camera->getY();
    float cz = camera->getZ();

    int centerX = bufferWidth / 2;
    int centerY = bufferHeight / 2;

    #pragma omp parallel for simd
    for( int i = 0 ; i < nVertices ; i++ ){
        
        // transform to camera space
        float x = ((vertices[i].x * sm) + mx) - cx;
        float y = ((vertices[i].y * sm) + my) - cy;
        float z = ((vertices[i].z * sm) + mz) - cz;

        // yaw - y
        float dx = x * cosY - z * sinY;
        float dz = x * sinY + z * cosY;
        x = dx;
        z = dz;

        // pitch - x
        float dy = y * cosP - z * sinP;
        dz = y * sinP + z * cosP;
        y = dy;
        z = dz;

        // roll - z
        dx = x * cosR - y * sinR;
        dy = x * sinR + y * cosR;
        x = dx;
        y = dy;

        // project perpective
        float px = (camera->dist_f * x) / z;
        float py = (camera->dist_f * y) / z;
        projection[i].x = px * -1 + centerX;
        projection[i].y = py * -1 + centerY;
        projection[i].z = z;

        screenSpaceBuffer[i] = projection[i].x >= 0 && projection[i].x < bufferWidth && projection[i].y >= 0 && projection[i].y < bufferHeight;    
    }

}

void Renderer::drawTexturedPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3, Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, unsigned char* data, int texW, int texH, Light** lights, int nLights, bool* shadowMapBuffer, int shadowMapWidth, int shadowMapHeight, bool shadowCast) 
{

    // polygon boundbox
    int topY,bottomY,maxLeft,maxRight;
    // top
    if(p1.y >= p2.y && p1.y >= p3.y){
        topY = p1.y;
    }
    else if(p2.y >= p1.y && p2.y >= p3.y){
        topY = p2.y;
    }
    else{
        topY = p3.y;
    }
    if(topY >= window->getHeight()) topY = window->getHeight() - 1;
    // bottom
    if(p1.y <= p2.y && p1.y <= p3.y){
        bottomY = p1.y;
    }
    else if(p2.y <= p1.y && p2.y <= p3.y){
        bottomY = p2.y;
    }
    else{
        bottomY = p3.y;
    }
    if(bottomY < 0) bottomY = 0;
    // left
    if(p1.x <= p2.x && p1.x <= p3.x){
        maxLeft = p1.x;
    }
    else if(p2.x <= p1.x && p2.x <= p3.x){
        maxLeft = p2.x;
    }
    else{
        maxLeft = p3.x;
    }
    if(maxLeft < 0) maxLeft = 0;

    // right
    if(p1.x >= p2.x && p1.x >= p3.x){
        maxRight = p1.x;
    }
    else if(p2.x >= p1.x && p2.x >= p3.x){
        maxRight = p2.x;
    }
    else{
        maxRight = p3.x;
    }
    if(maxRight >= window->getWidth()) maxRight = window->getWidth() - 1;

    // top left corner of the boundbox
    int px = maxLeft; 
    int py = topY;  
    int sizeX = maxRight - maxLeft;
    int sizeY = topY - bottomY;

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
    #pragma omp parallel for collapse(2) schedule(static)
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {

            // pixel screen space position
            int px_atual = px + x;
            int py_atual = py - y;

            //test area
            float a1 = this->area(px_atual, py_atual, p2.x, p2.y, p3.x, p3.y);
            float a2 = this->area(p1.x, p1.y, px_atual, py_atual, p3.x, p3.y);
            float a3 = this->area(p1.x, p1.y, p2.x, p2.y, px_atual, py_atual);

            // verify if the the pixel is inside the polygon
            if (abs(areaTotal - (a1 + a2 + a3)) < 0.1) {
                
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





void Renderer::createShadowMap(Camera *camera, float* zBuffer, Model** eBuffer, Model* model, int shadowMapWidth, int shadowMapHeight){//, Vec3* vertices, Vec3* projection, int nVertices, bool* screenSpaceBuffer, int bufferHeight, int bufferWidth, char* dataShadowMap, int shadowMapWidth, int shadowMapHeight){
    
    model->calcular_pontos_3D();

    // project vertices
    this->project(camera, model, shadowMapHeight, shadowMapWidth);

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
    if(p1.y >= p2.y && p1.y >= p3.y){
        topY = p1.y;
    }
    else if(p2.y >= p1.y && p2.y >= p3.y){
        topY = p2.y;
    }
    else{
        topY = p3.y;
    }
    if(topY >= shadowWindowHeight) topY = shadowWindowHeight - 1;
    // bottom
    if(p1.y <= p2.y && p1.y <= p3.y){
        bottomY = p1.y;
    }
    else if(p2.y <= p1.y && p2.y <= p3.y){
        bottomY = p2.y;
    }
    else{
        bottomY = p3.y;
    }
    if(bottomY < 0) bottomY = 0;
    // left
    if(p1.x <= p2.x && p1.x <= p3.x){
        maxLeft = p1.x;
    }
    else if(p2.x <= p1.x && p2.x <= p3.x){
        maxLeft = p2.x;
    }
    else{
        maxLeft = p3.x;
    }
    if(maxLeft < 0) maxLeft = 0;

    // right
    if(p1.x >= p2.x && p1.x >= p3.x){
        maxRight = p1.x;
    }
    else if(p2.x >= p1.x && p2.x >= p3.x){
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
            if (abs(areaTotal - (a1 + a2 + a3)) < 0.5) {
                
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

}
