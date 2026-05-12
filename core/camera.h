#include "vec3.h"
#pragma once
class Camera
{

public:
    Vec3 posicao;
    double angulox;
    double anguloy;
    int dist_f;
    static Camera* camera;

    static Camera* criar();    

    Camera();

    void mover(double x = 0.0,double y = 0.0,double z = 0.0);
    void rodarx(double angulo, Vec3 base);
    void rodary(double angulo, Vec3 base);
    void rodarz(double angulo, Vec3 base);

    Vec3* projetar(Vec3* pontos, Vec3* projecao, int quantidadePontos);

    ~Camera();
};

