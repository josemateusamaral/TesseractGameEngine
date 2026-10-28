#include <cmath>
#include <algorithm>
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

    //window data
    int windowWidth = window->getWidth();
    int windowHeight = window->getHeight();

    // camera vector
    float origem[3] = {camera->getX(), camera->getY(), camera->getZ()};
    float a[3] = {mx, my, mz};
    Vec3 cam{origem, a};

    // indexes
    for (int i = 0; i < model->indexCount; i += 3)
    {

        const int i0 = model->indices[i];
        const int i1 = model->indices[i + 1];
        const int i2 = model->indices[i + 2];

        const auto& p0 = model->projection[i0];
        const auto& p1 = model->projection[i1];
        const auto& p2 = model->projection[i2];
        
        // clip distance
        if(p0.z <= 0 || p1.z <= 0 || p2.z <= 0) continue;

        // calculate bounding box
        int topY,bottomY,maxLeft,maxRight;
        // top
        if(p0.y >= p1.y && p0.y >= p2.y){
            topY = p0.y;
        }
        else if(p1.y >= p0.y && p1.y >= p2.y){
            topY = p1.y;
        }
        else{
            topY = p2.y;
        }
        if(topY >= windowHeight) topY = windowHeight - 1;
        // bottom
        if(p0.y <= p1.y && p0.y <= p2.y){
            bottomY = p0.y;
        }
        else if(p1.y <= p0.y && p1.y <= p2.y){
            bottomY = p1.y;
        }
        else{
            bottomY = p2.y;
        }
        if(bottomY < 0) bottomY = 0;
        // left
        if(p0.x <= p1.x && p0.x <= p2.x){
            maxLeft = p0.x;
        }
        else if(p1.x <= p0.x && p1.x <= p2.x){
            maxLeft = p1.x;
        }
        else{
            maxLeft = p2.x;
        }
        if(maxLeft < 0) maxLeft = 0;
        // right
        if(p0.x >= p1.x && p0.x >= p2.x){
            maxRight = p0.x;
        }
        else if(p1.x >= p0.x && p1.x >= p2.x){
            maxRight = p1.x;
        }
        else{
            maxRight = p2.x;
        }
        if(maxRight >= windowWidth) maxRight = windowWidth - 1;
        
        //discart poligon out of the screen or the size of 1 pixel
        if((maxRight - maxLeft) * (topY - bottomY) < 1) continue;

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
        if (model->getBackfaceCulling()) {
            float centerX = (v0.x + v1.x + v2.x) / 3.0f;
            float centerY = (v0.y + v1.y + v2.y) / 3.0f;
            float centerZ = (v0.z + v1.z + v2.z) / 3.0f;

            float viewX = camera->getX() - centerX;
            float viewY = camera->getY() - centerY;
            float viewZ = camera->getZ() - centerZ;
            
            float dot = normal.x * viewX + normal.y * viewY + normal.z * viewZ;
            if (dot <= 0.0f) {
                continue;
            }
        }

        // texture index
        int textureIndex = model->textureIndexes[i/3];
    
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
            (unsigned char*)model->diffuseTextures[textureIndex]->data,
            model->diffuseTextures[textureIndex]->width,
            model->diffuseTextures[textureIndex]->height,            
            //lights
            model->lights,
            model->nLights,
            topY,
            bottomY,
            maxLeft,
            maxRight,
            !model->getBackfaceCulling()
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

    float dist_f = camera->dist_f;

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
        float px = (dist_f * x) / z;
        float py = (dist_f * y) / z;
        projection[i].x = px * -1 + centerX;
        projection[i].y = py * -1 + centerY;
        projection[i].z = z;

    }

}

void Renderer::drawTexturedPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3, Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, unsigned char* data, int texW, int texH, Light** lights, int nLights, int topY, int bottomY, int maxLeft, int maxRight, bool isTwoSided)
{
    const int width = window->getWidth();

    // Inclusive bounding box limits
    const int px = maxLeft;
    const int py = topY;
    const int sizeX = maxRight - maxLeft + 1;
    const int sizeY = topY - bottomY + 1;

    if (sizeX <= 0 || sizeY <= 0)
        return;

    const auto edge = [](const Vec3& a, const Vec3& b, float x, float y) -> float {
        return (b.x - a.x) * (y - a.y) - (b.y - a.y) * (x - a.x);
    };

    const float areaTotal = edge(p1, p2, p3.x, p3.y);

    if (std::fabs(areaTotal) < 1e-8f)
        return;

    const float invArea = 1.0f / areaTotal;

    // Calculate lighting once per triangle
    float lightEffetR = 0.0f;
    float lightEffetG = 0.0f;
    float lightEffetB = 0.0f;

    for (int i = 0; i < nLights; i++) {
        lights[i]->apply(v1, v2, v3,lightEffetR, lightEffetG, lightEffetB,isTwoSided);
    }
    lightEffetR = std::clamp(lightEffetR, 0.0f, 1.0f);
    lightEffetG = std::clamp(lightEffetG, 0.0f, 1.0f);
    lightEffetB = std::clamp(lightEffetB, 0.0f, 1.0f);

    // Calculate the reciprocal depth of each vertex.
    // Positive depth is measured in camera space.
    const float iz1 = 1.0f / p1.z;
    const float iz2 = 1.0f / p2.z;
    const float iz3 = 1.0f / p3.z;

    #pragma omp parallel for collapse(2) schedule(static)
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {

            const int px_atual = px + x;
            const int py_atual = py - y;

            // Pixel position in screen space
            const float fx = static_cast<float>(px_atual);
            const float fy = static_cast<float>(py_atual);

            // Calculate the oriented edge functions
            const float e1 = edge(p2, p3, fx, fy);
            const float e2 = edge(p3, p1, fx, fy);
            const float e3 = edge(p1, p2, fx, fy);

            // A pixel is inside the triangle if all edge functions
            // have the same sign as the triangle's oriented area.
            if (areaTotal > 0.0f) {
                if (e1 < 0.0f || e2 < 0.0f || e3 < 0.0f)
                    continue;
            } else {
                if (e1 > 0.0f || e2 > 0.0f || e3 > 0.0f)
                    continue;
            }

            // Calculate barycentric weights
            const float w1 = e1 * invArea;
            const float w2 = e2 * invArea;
            const float w3 = e3 * invArea;

            const int bufferIndex = py_atual * width + px_atual;

            // Perspective-correct depth interpolation:
            // interpolate 1/z instead of z directly.
            const float invZ =
                w1 * iz1 +
                w2 * iz2 +
                w3 * iz3;

            if (invZ <= 0.0f)
                continue;

            // A larger 1/z value represents a closer pixel.
            // The Z-buffer must be initialized to zero.
            if (invZ <= window->zBuffer[bufferIndex])
                continue;

            window->zBuffer[bufferIndex] = invZ;

            // Perspective-correct UV interpolation:
            // interpolate u/z and v/z, then divide by 1/z.
            float u =
                (w1 * uv1.x * iz1 +
                 w2 * uv2.x * iz2 +
                 w3 * uv3.x * iz3) 
                / invZ;

            float v =
                (w1 * uv1.y * iz1 +
                 w2 * uv2.y * iz2 +
                 w3 * uv3.y * iz3) 
                / invZ;

            // Wrap texture coordinates to the [0, 1) range
            u -= std::floor(u);
            v -= std::floor(v);

            // Convert UV coordinates to texture pixel coordinates
            int tx = static_cast<int>(u * (texW - 1));
            int ty = static_cast<int>(v * (texH - 1));
            tx = std::clamp(tx, 0, texW - 1);
            ty = std::clamp(ty, 0, texH - 1);
            const int textureIndex = (ty * texW + tx) * 3;

            //get texture color and apply light effects
            const Uint32 rt = static_cast<Uint32>(data[textureIndex] * lightEffetR);
            const Uint32 gt = static_cast<Uint32>(data[textureIndex + 1] * lightEffetG);
            const Uint32 bt = static_cast<Uint32>(data[textureIndex + 2] * lightEffetB);

            // Write the final pixel color to the color buffer
            window->colorBuffer[bufferIndex] = (255u << 24) | (rt << 16) | (gt << 8) | bt;
        }
    }
}