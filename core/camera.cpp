#include "camera.h"
#include <cmath>

//Definicao do ponteiro para a classe Singleton.
Camera *Camera::camera = nullptr;

/**
 * @brief Construtor da Camera, posição do Pinhole
 * @authors Gustavo Mittelmann, Henrique Heiderscheidt
 * @param posicao posição do pinhole no r3
 * @param dist_f distancia focal
 */
Camera::Camera()
:posicao{Vec3(0,0,0)},dist_f{500}
{
    angulox = 0;
    anguloy = 0;
    hpr = Vec3(0,0,0);
}

void Camera::moveFront(float distance) {
    float h = hpr.y * M_PI / 180.0f * -1.0f;
    float dx = sin(h);
    float dy = cos(h);
    posicao.x += dx * distance;
    posicao.z += dy * distance;
}

void Camera::moveBack(float distance) {
    float h = hpr.y * M_PI / 180.0f * -1.0f;
    float dx = sin(h);
    float dy = cos(h);
    posicao.x -= dx * distance;
    posicao.z -= dy * distance;
}

void Camera::moveLeft(float distance) {
    float h = hpr.y * M_PI / 180.0f * -1.0f;
    float dx = cos(h);
    float dz = -sin(h);
    posicao.x += dx * distance;
    posicao.z += dz * distance;
}

void Camera::moveRight(float distance) {
    float h = hpr.y * M_PI / 180.0f * -1.0f;
    float dx = -cos(h);
    float dz = sin(h);
    posicao.x += dx * distance;
    posicao.z += dz * distance;
}

void Camera::setPos(Vec3 pos){
    this->posicao = pos;
}
        
void Camera::setX(float x){
    this->posicao.x = x;
}

void Camera::setY(float y){
    this->posicao.y = y;
}

void Camera::setZ(float z){
    this->posicao.z = z;
}
        
Vec3 Camera::getPos(){
    return this->posicao;
}

float Camera::getX(){
    return this->posicao.x;
}

float Camera::getY(){
    return this->posicao.y;
}

float Camera::getZ(){
    return this->posicao.z;
}

Camera::~Camera()
{
}