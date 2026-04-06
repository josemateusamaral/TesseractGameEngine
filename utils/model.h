#pragma once
#include<SDL2/SDL.h>
#include <iostream>
#include "../../pontos/ponto3.h"
#include "../../formas/forma.h"
#include "../../utils/janela.h"
#include "../../utils/Vec3.h"

using namespace std;

class Model : virtual public Forma
{
    private:
        int LOD;
        int faixas;
        
    public:
        
        Model(Ponto3 posicao, double tamanho, int lod);
        Model(Ponto3 posicao, int lod);

        void desenhar(Window &window);
        void calcular_pontos_base();
        void loadModel(string path);

        void setLOD(int lod);
        int getLOD();
};

ostream & operator<<(ostream &out, const Model &p);