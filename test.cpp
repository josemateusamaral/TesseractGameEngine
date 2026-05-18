#include "core/model.h"
#include "core/light.h"
#include "core/tesseract.h"
#include "core/camera.h"

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

	// create ambient light
	AmbientLight* ambientLight = new AmbientLight( 1, 1, 1);
	AmbientLight* ambientLight2 = new AmbientLight( 0.1, 0.1, 0.1);
	// create point light
	PointLight* pointLight = new PointLight( 1, 1, 1, 0, 0, 30);


	// load model
	Model* model = new Model("samples/model_loading/esfere.glb");
	model->setPos( 0, 0, 14);
	model->setScale(3);
	model->renderType = 3;
	engine.scene->addModel(model);
	model->setLight(pointLight);
	//model->setLight(ambientLight2);

	// point light place holder
	Model* modelLight = new Model("samples/model_loading/cubo.glb");
	modelLight->setPos( 0, 0, 30);
	modelLight->setScale(3);
	modelLight->renderType = 3;
	engine.scene->addModel(modelLight);
	modelLight->setLight(ambientLight);

	
	// bind keys
	engine.input->bindKey("escape", "release", [&engine]() {
		engine.exit();
	});
	engine.input->bindKey("w", "press", [&engine]() {
		engine.camera->posicao.z += 1;
	});
	engine.input->bindKey("a", "press", [&engine]() {
		engine.camera->posicao.x += 1;
	});
	engine.input->bindKey("s", "press", [&engine]() {
		engine.camera->posicao.z -= 1;
	});
	engine.input->bindKey("d", "press", [&engine]() {
		engine.camera->posicao.x -= 1;
	});
	engine.input->bindKey("q", "press", [&engine]() {
		engine.camera->posicao.y -= 1;
	});
	engine.input->bindKey("e", "press", [&engine]() {
		engine.camera->posicao.y += 1;
	});

	// bind mouse
	engine.setCaptureMouse(true);
	engine.input->bindMouseButton("left", "release", [&model]() {
		printf("soltando botão esquerdo do mouse");
	});
	engine.input->bindMouseButton("right", "press", [&model]() {
		printf("pressionando botao direito do mouse");
	});
	engine.input->bindMouseMotion([&engine]() {
		engine.camera->hpr.x += engine.input->mouseMotionVector->y * 0.1;
		if(engine.camera->hpr.x < 0) 
			engine.camera->hpr.x = 359;
		engine.camera->hpr.y += engine.input->mouseMotionVector->x * 0.1;
		if(engine.camera->hpr.y < 0) 
			engine.camera->hpr.y = 359;
	});

	// game loop
	engine.run([&]() {

		// rotate model
        model->rotate(1, 0, 0);
        
    });
	
	return 0;
}