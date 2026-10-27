#include "scene.h"

Scene::Scene()
{
    //models details
    modelsBufferSize = 100;
    qtdModels = 0;
    models = new Model*[modelsBufferSize]; 

}

/**
 * @brief Função para atualizar a janela
 * 
 */
Scene::~Scene()
{

}

void Scene::addModel(Model *model){

    models[qtdModels] = model;
    qtdModels++;

}