#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include "../../pontos/ponto3.h"
#include "../../formas/forma.h"
#include "../../utils/janela.h"

using namespace std;

class Cone : virtual public Forma
{

    private:
        int LOD;
        Ponto3 altura;

    public:
        Cone(Ponto3 posicao,int tamanho, int tAltura);

        void desenhar(Window &window);
        void calcular_pontos_base();

        void setLOD(int lod);
        int getLOD();

};

ostream & operator<<(ostream &out, const Cone &p);