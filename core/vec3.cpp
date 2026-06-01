#include "vec3.h"
#include <iostream>
#include <cmath>

using namespace std;

Vec3::Vec3():
    x{0}, y{0}, z{0}
{
}

Vec3::Vec3(float x, float y, float z):
    x{x}, y{y}, z{z}
{
}

Vec3::Vec3(float a[], float b[])
{
    x = b[0] - a[0];
    y = b[1] - a[1];
    z = b[2] - a[2];
}

Vec3::Vec3(const Vec3 &v):
    x{v.x}, y{v.y}, z{v.z}
{
}




float Vec3::modulo()
{
    return sqrt( x*x + y*y + z*z );
}

Vec3 Vec3::versor()
{
    float m = this->modulo();
    if(m == 0.0) return Vec3(0,0,0);
    return Vec3( x / m, y / m, z / m);
}

Vec3 Vec3::projetar(Vec3 &v)
{
    Vec3 tmp{v};
    float multiplicador = this->produto_escalar(v) / v.produto_escalar(v);
    tmp.x *= multiplicador;
    tmp.y *= multiplicador;
    tmp.z *= multiplicador;
    return tmp;
}

float Vec3::produto_escalar(const Vec3 &v)
{
    float produto_escalar = x*v.x + y*v.y + z*v.z;
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

float Vec3::produto_misto(const Vec3 &v1, const Vec3 &v2)
{
    Vec3 tmp = this->produto_vetorial(v1);
    float produto = tmp.produto_escalar(v2);
    return produto;
}

bool Vec3::verificar_proporcionalidade(const Vec3 &v)
{
    return ( ( (x / v.x) == (y / v.y) ) && ( (y / v.y) == (z / v.z) ) );
}

float Vec3::angulo_entre_vetores(Vec3 &v)
{
    float produto_escalar = this->produto_escalar(v);
    float produto_dos_modulos = this->modulo() * v.modulo();
    float angulo = acos( produto_escalar / produto_dos_modulos ) * 180 / 3.14159265;
    return angulo;
}



Vec3 Vec3::multiEscalar(float escalar)
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
