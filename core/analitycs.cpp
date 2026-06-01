#include "analitycs.h"
#include "gui.h"

Analitycs::Analitycs(GUI *gui){

    this->gui = gui;

    // fps meter
    fpsMeter = new Text(" ");
	fpsMeter->setX(10);
	fpsMeter->setY(4);
    fpsMeter->hide();
    gui->addElement(fpsMeter);

}

Analitycs::~Analitycs(){

}

