#pragma once
#include<SDL2/SDL.h>
#include <iostream>
using namespace std;
class Ponto3
{
    public:
        double x;
        double y;
        double z;
        Ponto3(double x,double y, double z);
        Ponto3();
};

ostream & operator<<(ostream &out, const Ponto3 &p);