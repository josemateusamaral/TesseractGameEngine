#include "scene.h"

Scene::Scene()
{
    //models details
    modelsBufferSize = 100;
    qtdModels = 0;
    models = new Model*[modelsBufferSize]; 

    //shadow casters details
    shadowCastersBufferSize = 100;
    qtdShadowCasters = 0;
    shadowCasters = new Light*[shadowCastersBufferSize];

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

void Scene::addShadowCaster(Light *light){

    shadowCasters[qtdShadowCasters] = light;
    qtdShadowCasters++;
    for(int i = 0; i < light->qtdAttachedModels; i++){
        light->attachedModel[i]->shadowCast = true;
        light->createShadowMapper();
    }

}