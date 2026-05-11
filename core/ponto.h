#pragma once
#include<SDL2/SDL.h>
#include <iostream>
using namespace std;
class Ponto
{
    public:
        double x;
        double y;
        double z;
        Ponto(double x,double y,double z);
        Ponto(const Ponto & p);
        Ponto();
};

ostream & operator<<(ostream &out, const Ponto &p);