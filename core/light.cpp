#include "light.h"
#include "vec3.h"
#include "model.h"

Light::Light(){

    this->attachedModel = new Model*[this->attachedModelBufferSize];
    this->qtdAttachedModels = 0;

}
Light::Light(float r, float g, float b)
: r{r}, g{g}, b{b}
{
    this->attachedModel = new Model*[this->attachedModelBufferSize];
    this->qtdAttachedModels = 0;
}
void Light::attachModel(Model *model){
    this->attachedModel[qtdAttachedModels] = model;
    qtdAttachedModels++;
}
Light::~Light()
{
    
}




AmbientLight::AmbientLight(float r, float g, float b)
: Light(r, g ,b)
{
    
}
void AmbientLight::apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB)
{
    outR += this->r;
    outG += this->g;
    outB += this->b;
}
void AmbientLight::createShadowMapper()
{
    // ambient lights do not cast shadows in this implementation
}




DirectionalLight::DirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ)
: Light(r, g ,b), dirX{dirX}, dirY{dirY}, dirZ{dirZ}
{

}

void DirectionalLight::apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB)
{
    // polygon edges
    Vec3 v1 = p2 - p1;
    Vec3 v2 = p3 - p1;

    // polygon normal
    Vec3 normal = v1.produto_vetorial(v2).versor();

    // light direction
    Vec3 lightDir(dirX, dirY, dirZ);
    lightDir = lightDir.versor();

    // Lambert
    float intensity = normal.produto_escalar(lightDir);
    intensity = std::max(0.0f, intensity);

    // apply color
    outR += r * intensity;
    outG += g * intensity;
    outB += b * intensity;
}

void DirectionalLight::createShadowMapper()
{
    this->shadowMapWidth = 640;
    this->shadowMapHeight = 480;
    this->shadowCamera = new Camera();
    this->shadowCamera->setPos(Vec3(0, 0, 0));
    this->shadowCamera->hpr.x = 1;
    this->shadowCamera->hpr.y = 1;
    this->shadowZBuffer = new float[shadowMapWidth * shadowMapHeight];
    this->shadowEBuffer = new Model*[shadowMapWidth * shadowMapHeight];
}




PointLight::PointLight(float r, float g, float b, float x, float y, float z) 
: Light(r, g, b), x{x}, y{y}, z{z}
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void PointLight::apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB)
{
    // edges
    Vec3 v1 = p2 - p1;
    Vec3 v2 = p3 - p1;

    // normal
    Vec3 normal = v1.produto_vetorial(v2).versor();

    // polygon center
    Vec3 center(
        (p1.x + p2.x + p3.x) / 3.0,
        (p1.y + p2.y + p3.y) / 3.0,
        (p1.z + p2.z + p3.z) / 3.0
    );

    // light direction
    Vec3 lightDir(
        this->x - center.x,
        this->y - center.y,
        this->z - center.z
    );
    lightDir = lightDir.versor();

    // Lambert
    float intensity = normal.produto_escalar(lightDir);
    intensity = std::max(0.0f, intensity);

    // apply color
    outR += this->r * intensity;
    outG += this->g * intensity;
    outB += this->b * intensity;

}

void PointLight::createShadowMapper()
{
    // point lights do not cast shadows in this implementation
}