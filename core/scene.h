#pragma once
#include "model.h"

class Scene
{

    public:

        int qtdModels;
        int bufferSize;

        Model **models;

        Scene();
        ~Scene();

        void addModel(Model *model);

};



