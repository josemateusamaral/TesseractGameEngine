#include "../pontos/ponto3.h"
#include "../pontos/ponto.h"
#include "../utils/Vec3.h"
#pragma once
class Camera
{
protected:
    Camera(Ponto3 posicao = Ponto3(0,0,0), int dist_f = 500, Vec3 cima = Vec3(0,1,0), Vec3 direita = Vec3(0,0,1), Vec3 normal = Vec3(1,0,0));
public:
    Ponto3 posicao;
    double angulox;
    double anguloy;
    int dist_f;
    static Camera* camera;

    static Camera* criar();    
    void mover(double x = 0.0,double y = 0.0,double z = 0.0);
    void rodarx(double angulo, Ponto3 base);
    void rodary(double angulo, Ponto3 base);
    void rodarz(double angulo, Ponto3 base);

    Ponto* projetar(Ponto3* pontos, int quantidadePontos);

    ~Camera();
};

