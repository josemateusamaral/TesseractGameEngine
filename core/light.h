#pragma once
#include "vec3.h"
#include "camera.h"

class Model;

class Light
{
    public:

        float r, g, b;

        Model **attachedModel;
        int qtdAttachedModels;
        int attachedModelBufferSize = 100;

        //shadow map details
        int shadowMapWidth, shadowMapHeight;
        Camera *shadowCamera;
        float* shadowZBuffer;
        Model** shadowEBuffer;

        Light();
        Light(float r, float g, float b);
        void attachModel(Model *model);
        virtual void createShadowMapper() = 0;
        virtual ~Light();

        virtual void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) = 0;

};




class AmbientLight : public Light
{
    public:

        AmbientLight(float r, float g, float b);

        void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) override;
        void createShadowMapper();

};


// create shadow maps for all models that cast shadows
class DirectionalLight : public Light
{
    public:
        
        //direction vector
        float dirX, dirY, dirZ;

        DirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ);
        void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) override;
        void createShadowMapper() override;

};




class PointLight : public Light
{
    public:
        float x, y, z;

        PointLight(float r, float g, float b, float x, float y, float z);

        void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) override;
        void createShadowMapper() override;
};