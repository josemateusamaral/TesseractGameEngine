#pragma once
#include<SDL2/SDL.h>
#include <iostream>
#include "../../pontos/ponto3.h"
#include "../../formas/forma.h"
#include "../../utils/janela.h"
#include "../../utils/Vec3.h"

using namespace std;

class Cilindro : virtual public Forma
{
    private:
        int LOD;
        Ponto3 altura;
        
    public:
        
        Cilindro(Ponto3 posicao, int lod, int Altura);
        
        void desenhar(Window &window);
        void calcular_pontos_base();

        void setLOD(int lod);
        int getLOD();
};

ostream & operator<<(ostream &out, const Cilindro &p);