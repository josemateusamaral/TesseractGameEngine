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

void Camera::project(Vec3* vertices, Vec3* projection, int nVertices){

    for( int i = 0 ; i < nVertices ; i++ ){
        
        // transform to camera space
        double x = vertices[i].x - this->posicao.x;
        double y = vertices[i].y - this->posicao.y;
        double z = vertices[i].z - this->posicao.z;

        // apply camera inverse rotation
        double pitch = -this->hpr.x * M_PI / 180.0;
        double yaw   = -this->hpr.y * M_PI / 180.0;
        double roll  = -this->hpr.z * M_PI / 180.0;
        // yaw - y
        double cosY = cos(yaw);
        double sinY = sin(yaw);
        double dx = x * cosY - z * sinY;
        double dz = x * sinY + z * cosY;
        x = dx;
        z = dz;
        // pitch - x
        double cosP = cos(pitch);
        double sinP = sin(pitch);
        double dy = y * cosP - z * sinP;
        dz = y * sinP + z * cosP;
        y = dy;
        z = dz;
        // roll - z
        double cosR = cos(roll);
        double sinR = sin(roll);
        dx = x * cosR - y * sinR;
        dy = x * sinR + y * cosR;
        x = dx;
        y = dy;

        // project perpective
        double px = (this->dist_f * x) / z;
        double py = (this->dist_f * y) / z;
        projection[i].x = px * -1 + 320;
        projection[i].y = py * -1 + 240;
        projection[i].z = z;
    
    }

}

Camera::~Camera()
{
}