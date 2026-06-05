#pragma once
#include "model.h"
#include "light.h"

class Scene
{

    public:

        Model **models;
        int qtdModels;
        int modelsBufferSize;

        Light **shadowCasters;
        int qtdShadowCasters;
        int shadowCastersBufferSize;

        Scene();
        ~Scene();

        void addModel(Model *model);
        void addShadowCaster(Light *light);

};



