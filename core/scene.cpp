#include "scene.h"

Scene::Scene()
{

    bufferSize = 100;
    qtdModels = 0;
    models = new Model*[bufferSize]; 

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