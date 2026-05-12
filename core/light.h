#pragma once


class Light
{
public:

    float r, g, b;

    Light();
    Light(float r, float g, float b);
    virtual ~Light();

    virtual void applyR() = 0;
    virtual void applyG() = 0;
    virtual void applyB() = 0;
};




class AmbientLight : public Light
{
public:

    AmbientLight(float r, float g, float b);
    void applyR() override;
    void applyG() override;
    void applyB() override;

};




class DirectionalLight : public Light
{
public:
    float dirX, dirY, dirZ;

    DirectionalLight(float r, float g, float b, float dirX, float dirY, float dirZ);

    void applyR() override;
    void applyG() override;
    void applyB() override;
};




class PointLight : public Light
{
public:
    float x, y, z;

    PointLight(float r, float g, float b, float x, float y, float z);

    void applyR() override;
    void applyG() override;
    void applyB() override;
};