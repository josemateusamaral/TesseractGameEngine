#pragma once
#include "../core/ponto3.h"

using namespace std;

class Polygon
{
    private:
        Ponto3 vertice1;
        Ponto3 vertice2;
        Ponto3 vertice3;
        
    public:
        Polygon();
        Polygon(Ponto3 vertice1, Ponto3 vertice2, Ponto3 vertice3);
};

ostream & operator<<(ostream &out, const Polygon &p);