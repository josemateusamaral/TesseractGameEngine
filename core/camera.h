#include "vec3.h"
#pragma once
class Camera
{

    private:
        Vec3 posicao;

    public:
        Vec3 hpr;
        float angulox;
        float anguloy;
        int dist_f;
        static Camera* camera;

        Camera();
        ~Camera();
        void project(Vec3* pontos, Vec3* projection, int nVertices, bool* projectionClippingBuffer);

        void setPos(Vec3 pos);
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        Vec3 getPos();
        float getX();
        float getY();
        float getZ();

};

