#include "ponto.h"

/**
 * @brief Construtor da classe ponto no r2
 * 
 * @param x coordenada x, soma 400 para centralizar na tela 800x800
 * @param y coordenada y
 * 
 * @author Henrique Heiderscheidt
 */
Ponto::Ponto(double x, double y)
:x{x+400},y{y+400}
{
}

Ponto::Ponto()
{
    
}

/**
 * @brief Contrutor de cópia do ponto no r2
 * 
 * @param p Ponto no R2
 * 
 * @author Henrique Heiderscheidt
 */
Ponto::Ponto(const Ponto & p){
    x = p.x;
    y = p.y;
}


ostream & operator<< (ostream &out, const Ponto &p)
{
    out << "(" << p.x << "," << p.y << ")";
    return out;
}

