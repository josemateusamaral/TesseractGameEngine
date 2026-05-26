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

/**
 * @brief Transform vertices from 3D space to 2D screen using perspective projection
 * @authors Jose Mateus Amaral
 * @param vertices model vertices in 3D space
 * @param nVertices number of vertices in the model
 * @param projection Array of 2D points to be drawn
 * @return Vec3* Array de pontos 2d para serem desenhados
 */

void Camera::project(Vec3* vertices, Vec3* projection, int nVertices, bool* screenSpaceBuffer){

    // camera inverse rotation
    float pitch = -this->hpr.x * M_PI / 180.0;
    float yaw   = -this->hpr.y * M_PI / 180.0;
    float roll  = -this->hpr.z * M_PI / 180.0;
    // yaw - y
    float cosY = cos(yaw);
    float sinY = sin(yaw);
    // pitch - x
    float cosP = cos(pitch);
    float sinP = sin(pitch);
    // roll - z
    float cosR = cos(roll);
    float sinR = sin(roll);

    for( int i = 0 ; i < nVertices ; i++ ){
        
        // transform to camera space
        float x = vertices[i].x - this->posicao.x;
        float y = vertices[i].y - this->posicao.y;
        float z = vertices[i].z - this->posicao.z;
        
        // yaw - y
        float dx = x * cosY - z * sinY;
        float dz = x * sinY + z * cosY;
        x = dx;
        z = dz;
        // pitch - x
        float dy = y * cosP - z * sinP;
        dz = y * sinP + z * cosP;
        y = dy;
        z = dz;
        // roll - z
        dx = x * cosR - y * sinR;
        dy = x * sinR + y * cosR;
        x = dx;
        y = dy;

        if(z<0){
            screenSpaceBuffer[i] = false;
            continue;
        }

        // project perpective
        float px = (this->dist_f * x) / z;
        float py = (this->dist_f * y) / z;
        projection[i].x = px * -1 + 320;
        projection[i].y = py * -1 + 240;
        projection[i].z = z;

        screenSpaceBuffer[i] = !((projection[i].x > 640 || projection[i].x < 0 ) && ( projection[i].y > 480 || projection[i].y < 0));
    
    }

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