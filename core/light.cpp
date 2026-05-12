#include "light.h"


Light::Light(){
    
}

Light::Light(float r, float g, float b)
: r{r}, g{g}, b{b}
{

}

Light::~Light()
{
    
}




DirectionalLight::DirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ)
: Light(r, g ,b), dirX{dirX}, dirY{dirY}, dirZ{dirZ}
{

}

void DirectionalLight::applyR()
{

}
void DirectionalLight::applyG()
{

}
void DirectionalLight::applyB()
{

}




AmbientLight::AmbientLight(float r, float g, float b)
: Light(r, g ,b)
{

}

void AmbientLight::applyR()
{

}
void AmbientLight::applyG()
{

}
void AmbientLight::applyB()
{

}




PointLight::PointLight(float r, float g, float b, float x, float y, float z) 
: Light(r, g, b), x{x}, y{y}, z{z}
{

}

void PointLight::applyR()
{

}
void PointLight::applyG()
{

}
void PointLight::applyB()
{

}