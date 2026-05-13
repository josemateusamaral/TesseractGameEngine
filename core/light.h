#pragma once
#include "vec3.h"

class Light
{
public:

    float r, g, b;

    Light();
    Light(float r, float g, float b);
    virtual ~Light();

    virtual float applyR(Vec3 p1, Vec3 p2, Vec3 p3) = 0;
    virtual float applyG(Vec3 p1, Vec3 p2, Vec3 p3) = 0;
    virtual float applyB(Vec3 p1, Vec3 p2, Vec3 p3) = 0;
};




class AmbientLight : public Light
{
public:

    AmbientLight(float r, float g, float b);
    float applyR(Vec3 p1, Vec3 p2, Vec3 p3) override;
    float applyG(Vec3 p1, Vec3 p2, Vec3 p3) override;
    float applyB(Vec3 p1, Vec3 p2, Vec3 p3) override;

};




class DirectionalLight : public Light
{
public:
    float dirX, dirY, dirZ;

    DirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ);

    float applyR(Vec3 p1, Vec3 p2, Vec3 p3) override;
    float applyG(Vec3 p1, Vec3 p2, Vec3 p3) override;
    float applyB(Vec3 p1, Vec3 p2, Vec3 p3) override;
};




class PointLight : public Light
{
public:
    float x, y, z;

    PointLight(float r, float g, float b, float x, float y, float z);

    float applyR(Vec3 p1, Vec3 p2, Vec3 p3) override;
    float applyG(Vec3 p1, Vec3 p2, Vec3 p3) override;
    float applyB(Vec3 p1, Vec3 p2, Vec3 p3) override;
};