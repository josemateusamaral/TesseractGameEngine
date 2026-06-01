#pragma once
#include "gui.h"

class Analitycs
{

    public:

        Text *fpsMeter;
        GUI *gui;

        Analitycs(GUI *gui);
        ~Analitycs();

        void render();


};