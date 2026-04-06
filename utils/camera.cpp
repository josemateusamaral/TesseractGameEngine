#include "camera.h"

/**
 * @brief Construtor da Camera, posição do Pinhole
 * @authors Gustavo Mittelmann, Henrique Heiderscheidt
 * @param posicao posição do pinhole no r3
 * @param dist_f distancia focal
 */
Camera::Camera(Ponto3 posicao, int dist_f, Vec3 cima, Vec3 direita, Vec3 normal )
:posicao{posicao},dist_f{dist_f}
{
    angulox = 0;
    anguloy = 0;
}

/**
 * @brief Rodar a camera no eixo x baseado em um ponto de foco no R3.
 * @author Henrique Heiderscheidt
 * @param angulo angulo de rotacao
 * @param base Ponto de foco
 * 
 * @todo Talvez mudar a forma de visualizar a camera.
 */
void Camera::rodarx(double angulo, Ponto3 base){
    posicao.y = posicao.y*cos(angulo)+posicao.z*-sin(angulo);
    posicao.z = posicao.y*sin(angulo) + posicao.z*cos(angulo);
}

/**
 * @brief Rodar a camera no eixo y baseado em um ponto de foco no R3.
 * @author Henrique Heiderscheidt
 * @param angulo angulo de rotacao
 * @param base Ponto de foco
 * 
 * @todo Talvez mudar a forma de visualizar a camera.
 */
void Camera::rodary(double angulo,Ponto3 base){
    posicao.x = ((posicao.x - base.x)*cos(angulo) + (posicao.z-base.z)*-sin(angulo)) + base.x;
    posicao.z = ((posicao.x - base.z)*sin(angulo) + (posicao.z-base.z)*cos(angulo)) + base.z;
    cout << posicao <<endl;
}

/**
 * @brief Rodar a camera no eixo z baseado em um ponto de foco no R3.
 * @author Henrique Heiderscheidt
 * @param angulo Angulo de rotacao
 * @param base Ponto de foco
 * 
 * @todo Talvez mudar a forma de visualizar a camera.
 */
void Camera::rodarz(double angulo, Ponto3 base){
    posicao.x = ((posicao.x - base.x)*cos(angulo)  + (posicao.y - base.y)*-sin(angulo)) + base.x;
    posicao.y = ((posicao.x - base.x)*sin(angulo)  + (posicao.y - base.y)*cos(angulo)) + base.y;

    cout << posicao <<endl;
}

/**
 * @brief Mover a camera no espaco, no momento muda todos os pontos baseados na posicao da camera
 * @authors Gustavo Mittelmann, Henrique Heiderscheidt
 * @param posicao posição do pinhole no r3
 * @param dist_f distancia focal
 */
void Camera::mover(double x,double y,double z){
    posicao.x += x;
    posicao.y += y;
    posicao.z += z;
}

/**
 * 
 * @brief Transformação pontos do 3d para o 2d utilizando o modelo PinHole:
 *        x = ((distância focal * -1) / z ) * x
 *        y = ((distância focal * -1) / z ) * y
 * @authors Jose Mateus Amaral, Gustavo Mittelmann, Henrique Heiderscheidt, Fernanda Martins, Eduardo Brandt, Monique Ellen
 * @param pontos Array de pontos 3D para serem transformados em 2D
 * @param quantidadePontos quantidade de pontos
 * @return Ponto* Array de pontos 2d para serem desenhados
 */

Ponto* Camera::projetar(Ponto3* pontos, int quantidadePontos){
    double y1,y2;
    Ponto* projecaoTemp = (Ponto*)malloc(sizeof(Ponto) * quantidadePontos);
    for( int i = 0 ; i < quantidadePontos ; i++ ){
        y1 = ( ( dist_f*-1 / (pontos[i].z-camera->posicao.z) ) * (pontos[i].x-camera->posicao.x) );
	    y2 = ( ( dist_f*-1 / (pontos[i].z-camera->posicao.z) ) * (pontos[i].y-camera->posicao.y) );
        projecaoTemp[i] = Ponto(y1,y2-240);
    }
	return projecaoTemp;
}

//Definicao do ponteiro para a classe Singleton.
Camera *Camera::camera = nullptr;


/**
 * @brief Singleton da classe camera, só permite uma instacia da classe.
 * @authors Gustavo Mittelmann, Henrique Heiderscheidt
 * @return Camera* 
 */
Camera *Camera::criar(){
    if (camera == nullptr){
        camera = new Camera();
    };
    return camera;
    }


Camera::~Camera()
{
}