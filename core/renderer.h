#pragma once
#include "camera.h"
#include "model.h"
#include "window.h"

class Renderer
{

    public:

        Renderer();
        ~Renderer();

        void render(Model *model, Window *window, Camera *camera);
        void project(Camera *camera, Vec3* vertices, Vec3* projection, int nVertices, bool* screenSpaceBuffer, int bufferHeight, int bufferWidth);
        void drawTexturedPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3 ,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3,unsigned char* data, int texW, int texH, Light** lights, int nLights);
        void drawLine(Window* window, Vec3 &p1, Vec3 &p2);
        void drawLine(Window* window, Vec3 &p1, Vec3 &p2, int r, int g, int b);
        void drawBlankPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3,int r = 255, int g = 255, int b = 255);
        bool isPixelInsidePolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);        
        float area(int x1, int y1, int x2, int y2, int x3, int y3); 


        // shadow mapping
        void createShadowMap(Camera *camera, float* zBuffer, Model** eBuffer, Model* model, int shadowMapWidth, int shadowMapHeight);
        void drawShadowMap(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3 ,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, float* shadowZBuffer, Model** shadowEBuffer, Model* model, int shadowWindowWidth, int shadowWindowHeight, int shadowMapHeight, int shadowMapWidth);

};