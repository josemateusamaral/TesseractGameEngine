#pragma once
#include "vec3.h"

class Light
{
public:

    float r, g, b;

    Light();
    Light(float r, float g, float b);
    virtual ~Light();

    virtual void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) = 0;

};




class AmbientLight : public Light
{
public:

    AmbientLight(float r, float g, float b);

    void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) override;

};




class DirectionalLight : public Light
{
public:
    float dirX, dirY, dirZ;

    DirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ);

    void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) override;
};




class PointLight : public Light
{
public:
    float x, y, z;

    PointLight(float r, float g, float b, float x, float y, float z);

    void apply(Vec3 p1, Vec3 p2, Vec3 p3, float &outR, float &outG, float &outB) override;
};