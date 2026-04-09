#include "../pontos/ponto3.h"
#include "../pontos/ponto.h"
#include "../utils/Vec3.h"
#pragma once
class Camera
{

public:
    Ponto3 posicao;
    double angulox;
    double anguloy;
    int dist_f;
    static Camera* camera;

    static Camera* criar();    

    Camera();

    void mover(double x = 0.0,double y = 0.0,double z = 0.0);
    void rodarx(double angulo, Ponto3 base);
    void rodary(double angulo, Ponto3 base);
    void rodarz(double angulo, Ponto3 base);

    Ponto* projetar(Ponto3* pontos, int quantidadePontos);

    ~Camera();
};

