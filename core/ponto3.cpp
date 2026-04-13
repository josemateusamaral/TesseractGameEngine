#include "ponto3.h"

/**
 * @brief Ponto no R3 (x,y,z)
 * 
 * @param x Posicao X
 * @param y Posicao Y
 * @param z Posicao Z
 * 
 * @author Henrique Heiderscheidt
 */
Ponto3::Ponto3(double x, double y, double z )
:x{x},y{y}, z{z}
{

}

/**
 * @brief Construtor de um ponto padrão 0,0,10
 * 
 * @author Henrique Heiderscheidt
 */
Ponto3::Ponto3()
:x{0},y{0}, z{10}
{

}

ostream & operator<< (ostream &out, const Ponto3 &p)
{
    out << "(" << p.x << "," << p.y << "," << p.z <<")";
    return out;
}