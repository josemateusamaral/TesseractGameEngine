#include <stdio.h>
#include "core/vec3.h"
#include "core/model.h"
#include <iostream>
#include "core/tesseract.h"

using namespace std;

// Screen dimension
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{

	//initialize engine
	Tesseract tesseract = Tesseract(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!tesseract.isRunning()){
		printf("Fail to start Tesseract Game Engine !");
		return -1;
	}

	// load model
	Model model{"samples/model_loading/cubo.glb"};
	model.setPos(0, 0, 14);
	model.setScale(3);
	model.setBackfaceCulling(true);
	model.renderType = 3;
	tesseract.scene->addModel(&model);

	// create light point
	Vec3 light = Vec3(model.getPos().x, model.getPos().y, model.getPos().z);

	tesseract.run([&]() {

        model.rotate(1, 1, 1);
        
    });
	
	return 0;
}