#include "vec3.h"
#pragma once
class Camera
{

public:
    Vec3 posicao;
    Vec3 hpr;
    double angulox;
    double anguloy;
    int dist_f;
    static Camera* camera;

    Camera();
    ~Camera();
    void project(Vec3* pontos, Vec3* projection, int nVertices);

    
};

