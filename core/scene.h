#pragma once
#include "model.h"
#include "light.h"

class Scene
{

    public:

        Model **models;
        int qtdModels;
        int modelsBufferSize;

        Scene();
        ~Scene();

        void addModel(Model *model);

};



