#include <cmath>
#include <algorithm>
#include "renderer.h"

namespace {
    constexpr float NEAR_CLIP = 0.01f;
}

// Clipped vertex
struct ClipVertex {
    Vec3 camera;
    Vec3 screen;
    Vec3 world;
    Vec3 uv;
};

// Bounding box
struct Bounds {
    int topY;
    int bottomY;
    int left;
    int right;
    bool visible;
};

Renderer::Renderer() {
    // Inicialização do renderer
}

Renderer::~Renderer() {
    // Limpeza de recursos do renderer
}

void Renderer::render(Model *model, Window *window, Camera *camera) {

    // Project vertices
    this->project(camera, model, window->getHeight(), window->getWidth());

    // Model data
    float mx = model->getX();
    float my = model->getY();
    float mz = model->getZ();
    float sm = model->getScale();

    // Window data
    int windowWidth = window->getWidth();
    int windowHeight = window->getHeight();
    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;

    // Camera data
    float cx = camera->getX();
    float cy = camera->getY();
    float cz = camera->getZ();
    float distF = camera->dist_f;
    // Camera inverse rotation
    float pitch = -camera->hpr.x * M_PI / 180.0;
    float yaw   = -camera->hpr.y * M_PI / 180.0;
    float roll  = -camera->hpr.z * M_PI / 180.0;
    float cosY = cos(yaw);
    float sinY = sin(yaw);
    float cosP = cos(pitch);
    float sinP = sin(pitch);
    float cosR = cos(roll);
    float sinR = sin(roll);

    // Transform a world vertex to camera space
    auto toCameraSpace = [&](const Vec3& vertex) {

        float x = vertex.x - cx;
        float y = vertex.y - cy;
        float z = vertex.z - cz;
        // Yaw
        float dx = x * cosY - z * sinY;
        float dz = x * sinY + z * cosY;
        x = dx;
        z = dz;
        // Pitch
        float dy = y * cosP - z * sinP;
        dz = y * sinP + z * cosP;
        y = dy;
        z = dz;
        // Roll
        dx = x * cosR - y * sinR;
        dy = x * sinR + y * cosR;
        x = dx;
        y = dy;

        Vec3 result;
        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    };

    // Linear interpolation
    auto interpolate = [](const Vec3& a, const Vec3& b, float t) {

        Vec3 result;

        result.x = a.x + (b.x - a.x) * t;
        result.y = a.y + (b.y - a.y) * t;
        result.z = a.z + (b.z - a.z) * t;

        return result;
    };

    // Calculate intersection with the near clipping plane
    auto getIntersection = [&](const ClipVertex& a, const ClipVertex& b) {

        float t = (NEAR_CLIP - a.camera.z) / (b.camera.z - a.camera.z);

        ClipVertex result;

        result.camera = interpolate(a.camera, b.camera, t);
        result.camera.z = NEAR_CLIP;

        result.world = interpolate(a.world, b.world, t);
        result.uv = interpolate(a.uv, b.uv, t);

        // Project intersection
        result.screen.x = centerX - (distF * result.camera.x) / NEAR_CLIP;
        result.screen.y = centerY - (distF * result.camera.y) / NEAR_CLIP;
        result.screen.z = NEAR_CLIP;

        return result;
    };

    // Calculate triangle bounding box
    auto calculateBounds = [&](const Vec3& a, const Vec3& b, const Vec3& c) {

        Bounds bounds{0, 0, 0, 0, false};

        float minX = std::min(a.x, std::min(b.x, c.x));
        float maxX = std::max(a.x, std::max(b.x, c.x));

        float minY = std::min(a.y, std::min(b.y, c.y));
        float maxY = std::max(a.y, std::max(b.y, c.y));

        // Discard polygons completely outside the screen
        if (maxX < 0 || minX >= windowWidth || maxY < 0 || minY >= windowHeight) {
            return bounds;
        }

        bounds.left = static_cast<int>(std::clamp(std::floor(minX), 0.0f, static_cast<float>(windowWidth - 1)));
        bounds.right = static_cast<int>(std::clamp(std::ceil(maxX), 0.0f, static_cast<float>(windowWidth - 1)));
        bounds.bottomY = static_cast<int>(std::clamp(std::floor(minY), 0.0f, static_cast<float>(windowHeight - 1)));
        bounds.topY = static_cast<int>(std::clamp(std::ceil(maxY), 0.0f, static_cast<float>(windowHeight - 1)));

        // Discard polygons with a size smaller than one pixel
        if ((bounds.right - bounds.left) * (bounds.topY - bounds.bottomY) < 1) {
            return bounds;
        }

        bounds.visible = true;

        return bounds;
    };

    // Process triangles
    for (int i = 0; i < model->indexCount; i += 3) {

        const int i0 = model->indices[i];
        const int i1 = model->indices[i + 1];
        const int i2 = model->indices[i + 2];

        const Vec3 &p0 = model->projection[i0];
        const Vec3 &p1 = model->projection[i1];
        const Vec3 &p2 = model->projection[i2];

        // Check which vertices are in front of the camera
        const bool inside0 = p0.z >= NEAR_CLIP;
        const bool inside1 = p1.z >= NEAR_CLIP;
        const bool inside2 = p2.z >= NEAR_CLIP;

        const bool needsClipping = !(inside0 && inside1 && inside2);

        // Discard triangles completely behind the camera
        if (!inside0 && !inside1 && !inside2)
            continue;

        // For normal triangles, calculate bounds before transforming vertices
        Bounds originalBounds{0, 0, 0, 0, false};

        if (!needsClipping) {
            originalBounds = calculateBounds(p0, p1, p2);

            if (!originalBounds.visible)
                continue;
        }

        // World vertices
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

        // Calculate face normal
        float b[3] = {v0.x, v0.y, v0.z};
        float c[3] = {v1.x, v1.y, v1.z};
        float d[3] = {v2.x, v2.y, v2.z};
        Vec3 vector1(b, c);
        Vec3 vector2(b, d);
        Vec3 normal = vector1.produto_vetorial(vector2);

        // Backface culling
        if (model->getBackfaceCulling()) {

            float centerX = (v0.x + v1.x + v2.x) / 3.0f;
            float centerY = (v0.y + v1.y + v2.y) / 3.0f;
            float centerZ = (v0.z + v1.z + v2.z) / 3.0f;

            float viewX = camera->getX() - centerX;
            float viewY = camera->getY() - centerY;
            float viewZ = camera->getZ() - centerZ;

            float dot = normal.x * viewX +
                        normal.y * viewY +
                        normal.z * viewZ;

            if (dot <= 0.0f)
                continue;
        }

        // Texture index
        int textureIndex = model->textureIndexes[i / 3];

        const bool isTwoSided = !model->getBackfaceCulling();

        // Clipped polygon
        ClipVertex polygon[4];
        int polygonCount = 0;

        if (!needsClipping) {

            // Fast path: original triangle is completely visible
            polygon[0].screen = p0;
            polygon[1].screen = p1;
            polygon[2].screen = p2;

            polygon[0].world = v0;
            polygon[1].world = v1;
            polygon[2].world = v2;

            polygon[0].uv = model->uvs[i0];
            polygon[1].uv = model->uvs[i1];
            polygon[2].uv = model->uvs[i2];

            polygonCount = 3;

        } else {

            // Prepare original vertices in camera space
            ClipVertex input[3];

            input[0].world = v0;
            input[1].world = v1;
            input[2].world = v2;

            input[0].uv = model->uvs[i0];
            input[1].uv = model->uvs[i1];
            input[2].uv = model->uvs[i2];

            input[0].camera = toCameraSpace(v0);
            input[1].camera = toCameraSpace(v1);
            input[2].camera = toCameraSpace(v2);

            input[0].screen = p0;
            input[1].screen = p1;
            input[2].screen = p2;

            // Sutherland-Hodgman clipping against the near plane
            ClipVertex previous = input[2];
            bool previousInside = previous.camera.z >= NEAR_CLIP;

            for (int j = 0; j < 3; j++) {

                const ClipVertex& current = input[j];

                bool currentInside = current.camera.z >= NEAR_CLIP;

                // The edge intersects the clipping plane
                if (currentInside != previousInside) {

                    float t = (NEAR_CLIP - previous.camera.z) / (current.camera.z - previous.camera.z);

                    // Avoid duplicated vertices exactly on the clipping plane
                    if (t > 0.0f && t < 1.0f) {
                        polygon[polygonCount++] = getIntersection(previous, current);
                    }
                }

                // Keep vertices inside the camera
                if (currentInside) {
                    polygon[polygonCount++] = current;
                }

                previous = current;
                previousInside = currentInside;
            }
        }

        // Discard invalid polygons
        if (polygonCount < 3)
            continue;

        // Triangulate the resulting polygon using a triangle fan
        // A clipped triangle can generate either 3 or 4 vertices
        for (int j = 1; j < polygonCount - 1; j++) {

            Bounds bounds = needsClipping
                ? calculateBounds(
                    polygon[0].screen,
                    polygon[j].screen,
                    polygon[j + 1].screen
                )
                : originalBounds;

            // Draw a triangle using the existing rasterizer
            if (!bounds.visible)
                continue;

            this->drawTexturedPolygon(

                // Window
                window,

                // Projections
                polygon[0].screen,
                polygon[j].screen,
                polygon[j+1].screen,

                // World vertices
                polygon[0].world,
                polygon[j].world,
                polygon[j+1].world,

                // UVs
                polygon[0].uv,
                polygon[j].uv,
                polygon[j+1].uv,

                // Texture
                (unsigned char*)model->diffuseTextures[textureIndex]->data,
                model->diffuseTextures[textureIndex]->width,
                model->diffuseTextures[textureIndex]->height,

                // Lights
                model->lights,
                model->nLights,

                // Polygon bounds
                bounds.topY,
                bounds.bottomY,
                bounds.left,
                bounds.right,

                // BackfaceCulling is disabled ? It is used to calculate lighting
                isTwoSided
            );
            
        }
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

        // Store camera-space depth
        projection[i].z = z;

        // Avoid projecting vertices behind or too close to the camera
        if (z < NEAR_CLIP) {
            projection[i].x = centerX;
            projection[i].y = centerY;
            continue;
        }

        // Project perspective
        float px = (dist_f * x) / z;
        float py = (dist_f * y) / z;

        projection[i].x = px * -1 + centerX;
        projection[i].y = py * -1 + centerY;

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

    const auto edge = [](const Vec3& a, const Vec3& b, float x, float y) -> float {
        return (b.x - a.x) * (y - a.y) - (b.y - a.y) * (x - a.x);
    };

    // Polygon area
    const float areaTotal = edge(p1, p2, p3.x, p3.y);
    const float invArea = 1.0f / areaTotal;

    // Calculate lighting once per triangle
    float lightEffetR = 0.0f;
    float lightEffetG = 0.0f;
    float lightEffetB = 0.0f;
    // apply all lights
    for (int i = 0; i < nLights; i++) {
        lights[i]->apply(v1, v2, v3,lightEffetR, lightEffetG, lightEffetB,isTwoSided);
    }
    // Clamp light effect between 0 and 1
    lightEffetR = std::clamp(lightEffetR, 0.0f, 1.0f);
    lightEffetG = std::clamp(lightEffetG, 0.0f, 1.0f);
    lightEffetB = std::clamp(lightEffetB, 0.0f, 1.0f);

    // Calculate the reciprocal depth of each vertex.
    const float iz1 = 1.0f / p1.z;
    const float iz2 = 1.0f / p2.z;
    const float iz3 = 1.0f / p3.z;

    #pragma omp parallel for collapse(2) schedule(static)
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {

            // Pixel position in screen space
            const int px_atual = px + x;
            const int py_atual = py - y;
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

            // Color and Depth buffers position
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