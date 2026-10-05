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
        void project(Camera *camera, Model* model, int bufferHeight, int bufferWidth);
        void drawTexturedPolygon(Window* window, Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3 ,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3,unsigned char* data, int texW, int texH, Light** lights, int nLights, bool* shadowMapBuffer, int shadowMapWidth, int shadowMapHeight, bool shadowCast, int topY, int bottomY, int maxLeft, int maxRight);
        inline float area(int x1, int y1, int x2, int y2, int x3, int y3){
            return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);      
        }
        
        // shadow mapping
        void createShadowMap(Camera *camera, float* zBuffer, Model** eBuffer, Model* model, int shadowMapWidth, int shadowMapHeight);
        void drawShadowMap(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3 ,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3, float* shadowZBuffer, Model** shadowEBuffer, Model* model, int shadowWindowWidth, int shadowWindowHeight, int shadowMapHeight, int shadowMapWidth);

};