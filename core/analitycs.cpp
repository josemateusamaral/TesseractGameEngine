#include "analitycs.h"
#include "gui.h"

Analitycs::Analitycs(GUI *gui){

    this->gui = gui;

    // fps meter
    fpsMeter = new Text(" ");
	fpsMeter->pos.x = 10;
	fpsMeter->pos.y = 4;
    fpsMeter->isVisible = false;
    gui->addElement(fpsMeter);

}

Analitycs::~Analitycs(){

}

