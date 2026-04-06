#include "Vec3.h"
#include <iostream>
#include <cmath>

using namespace std;




Vec3::Vec3():
    x{0}, y{0}, z{0}
{
}

Vec3::Vec3(double x, double y, double z):
    x{x}, y{y}, z{z}
{
}

Vec3::Vec3(double a[], double b[])
{
    x = b[0] - a[0];
    y = b[1] - a[1];
    z = b[2] - a[2];
}

Vec3::Vec3(const Vec3 &v):
    x{v.x}, y{v.y}, z{v.z}
{
}




double Vec3::modulo()
{
    return sqrt( x*x + y*y + z*z );
}

Vec3 Vec3::versor()
{
    Vec3 tmp(*this);
    tmp.x /= this->modulo();
    tmp.y /= this->modulo();
    tmp.z /= this->modulo();
    return tmp;
}

Vec3 Vec3::projetar(Vec3 &v)
{
    Vec3 tmp{v};
    double multiplicador = this->produto_escalar(v) / v.produto_escalar(v);
    tmp.x *= multiplicador;
    tmp.y *= multiplicador;
    tmp.z *= multiplicador;
    return tmp;
}

double Vec3::produto_escalar(const Vec3 &v)
{
    double produto_escalar = x*v.x + y*v.y + z*v.z;
    return produto_escalar;
}

Vec3 Vec3::produto_vetorial(const Vec3 &v)
{
    Vec3 tmp;
    tmp.x = (y * v.z) - (z * v.y);
    tmp.y = ((x * v.z) - (z * v.x)) * -1;
    tmp.z = (x * v.y) - (y * v.x);
    return tmp;
}

double Vec3::produto_misto(const Vec3 &v1, const Vec3 &v2)
{
    Vec3 tmp = this->produto_vetorial(v1);
    double produto = tmp.produto_escalar(v2);
    return produto;
}

bool Vec3::verificar_proporcionalidade(const Vec3 &v)
{
    return ( ( (x / v.x) == (y / v.y) ) && ( (y / v.y) == (z / v.z) ) );
}

double Vec3::angulo_entre_vetores(Vec3 &v)
{
    double produto_escalar = this->produto_escalar(v);
    double produto_dos_modulos = this->modulo() * v.modulo();
    double angulo = acos( produto_escalar / produto_dos_modulos ) * 180 / 3.14159265;
    return angulo;
}



Vec3 Vec3::multiEscalar(double escalar)
{
    Vec3 tmp;
    tmp.x = x*escalar;
    tmp.y = y*escalar;
    tmp.z = z*escalar;
    return tmp;
}




Vec3 Vec3::operator+=(const Vec3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3 Vec3::operator+(const Vec3 &v) 
{
    Vec3 tmp(*this);
    tmp += v;
    return tmp;
}

Vec3 Vec3::operator-=(const Vec3 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vec3 Vec3::operator-(const Vec3 &v) 
{
    Vec3 tmp(*this);
    tmp -= v;
    return tmp;
}





ostream & operator<< (ostream &out, const Vec3 &v)
{
    out << "Vec3(" << v.x << "," << v.y << "," << v.z << ")";
    return out;
}
