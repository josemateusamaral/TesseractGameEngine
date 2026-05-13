#include "light.h"
#include "vec3.h"


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

float DirectionalLight::applyR(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return this->r;
}
float DirectionalLight::applyG(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return this->g;
}
float DirectionalLight::applyB(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return this->b;
}




AmbientLight::AmbientLight(float r, float g, float b)
: Light(r, g ,b)
{
    
}

float AmbientLight::applyR(Vec3 p1, Vec3 p2, Vec3 p3)
{
    //printf("APPLY R: %f\n", this->r);
    return this->r;
}
float AmbientLight::applyG(Vec3 p1, Vec3 p2, Vec3 p3)
{
    //printf("APPLY G: %f\n", this->g);
    return this->g;
}
float AmbientLight::applyB(Vec3 p1, Vec3 p2, Vec3 p3)
{    
    //printf("APPLY B: %f\n", this->b);
    return this->b;
}




PointLight::PointLight(float r, float g, float b, float x, float y, float z) 
: Light(r, g, b), x{x}, y{y}, z{z}
{

}

float PointLight::applyR(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return 0.0f;    
}
float PointLight::applyG(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return 0.0f;
}
float PointLight::applyB(Vec3 p1, Vec3 p2, Vec3 p3)
{
    return 0.0f;
}