#include "core/model.h"
#include "core/light.h"
#include "core/tesseract.h"

// Screen dimension
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{

	// initialize engine
	Tesseract engine = Tesseract(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!engine.isRunning()){
		printf("Failed to start Tesseract Game Engine !");
		return -1;
	}

	// load model
	Model* model = new Model("samples/model_loading/esfere.glb");
	model->setPos( 0, 0, 14);
	model->setScale(3);
	model->renderType = 3;
	engine.scene->addModel(model);

	// create ambient light
	AmbientLight* ambientLight = new AmbientLight( 1, 1, 1);
	model->setLight(ambientLight);
	
	// bind keys
	engine.input->bindKey("escape", "release", [&engine]() {
		engine.exit();
	});
	engine.input->bindKey("w", "release", [&model]() {
		model->setZ(model->getPos().z + 1);
	});
	engine.input->bindKey("a", "release", [&model]() {
		model->setX(model->getPos().x + 1);
	});
	engine.input->bindKey("s", "release", [&model]() {
		model->setZ(model->getPos().z - 1);
	});
	engine.input->bindKey("d", "release", [&model]() {
		model->setX(model->getPos().x - 1);
	});

	// bind mouse
	//engine.setCaptureMouse(true);
	engine.setMapMouse(true);


	// game loop
	engine.run([&]() {

		// rotate model
        model->rotate(1, 0, 0);
        
    });
	
	return 0;
}