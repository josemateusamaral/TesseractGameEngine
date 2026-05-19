#pragma once
#include<SDL2/SDL.h>
#include "vec3.h"
#include "light.h"

class Window
{
    private:
        int width;
        int height;
        
        SDL_Renderer* renderer;
        SDL_Window* window = NULL;   
        SDL_Texture* texture;

    public:

        float* zBuffer;
        uint32_t* colorBuffer;

        Window(int width,int height);
        ~Window();

        void refresh();
        void clean();

        SDL_Window* getWindow();

        void drawLine(Vec3 &p1, Vec3 &p2);
        void drawLine(Vec3 &p1, Vec3 &p2, int r, int g, int b);
        void drawBlankPolygon(Vec3 &p1, Vec3 &p2, Vec3 &p3,int r = 255, int g = 255, int b = 255);
        void drawTexturedPolygon(Vec3 &p1, Vec3 &p2, Vec3 &p3, Vec3 &v1, Vec3 &v2, Vec3 &v3 ,Vec3 &uv1, Vec3 &uv2, Vec3 &uv3,unsigned char* data, int texW, int texH, Light** lights, int nLights);
        void createBoundBox(Vec3 &p1, Vec3 &p2, Vec3 &p3, double &minX, double &maxX, double &minY, double &maxY);
        float area(int x1, int y1, int x2, int y2, int x3, int y3); 
        bool isPixelInsidePolygon(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
        float edge(float ax, float ay,float bx, float by,float px, float py);

};



