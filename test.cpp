#include "core/model.h"
#include "core/light.h"
#include "core/tesseract.h"
#include "core/camera.h"
#include "core/gui.h"
#include "core/audio.h"

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
	AmbientLight* ambientLight = new AmbientLight( 0.2, 0.2, 0.2);
	// create point light
	PointLight* pointLight = new PointLight( 0.8, 0.8, 0.8, 0, 0, 30);
	// create directional light
	DirectionalLight* directionalLight = new DirectionalLight( 1, 1, 1, 0, 0, -1);

	// load model
	Model* model = new Model("samples/model_loading/esfere.glb");
	model->setPos( 0, 0, 14);
	model->setScale(3);
	model->renderType = 3;
	engine.scene->addModel(model);
	model->setLight(pointLight);
	model->setLight(ambientLight);

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
		engine.camera->setZ(engine.camera->getZ() + 1);
	});
	engine.input->bindKey("a", "press", [&engine]() {
		engine.camera->setX(engine.camera->getX() + 1);
	});
	engine.input->bindKey("s", "press", [&engine]() {
		engine.camera->setZ(engine.camera->getZ() - 1);
	});
	engine.input->bindKey("d", "press", [&engine]() {
		engine.camera->setX(engine.camera->getX() - 1);
	});
	engine.input->bindKey("q", "press", [&engine]() {
		engine.camera->setY(engine.camera->getY() - 1);
	});
	engine.input->bindKey("e", "press", [&engine]() {
		engine.camera->setY(engine.camera->getY() + 1);
	});
	engine.input->bindKey("t", "release", [&engine]() {
		engine.setCaptureMouse(!engine.getCaptureMouse());
	});

	// bind mouse
	engine.input->bindMouseButton("left", "release", [&model]() {
		//printf("releasing left mouse button");
	});
	engine.input->bindMouseButton("right", "press", [&model]() {
		//printf("pressing right mouse button");
	});
	engine.input->bindMouseMotion([&engine]() {
		engine.camera->hpr.x += engine.input->mouseMotionVector->y * 0.1;
		if(engine.camera->hpr.x < 0) 
			engine.camera->hpr.x = 359;
		engine.camera->hpr.y += engine.input->mouseMotionVector->x * 0.1;
		if(engine.camera->hpr.y < 0) 
			engine.camera->hpr.y = 359;
	});

	//create text
	Text *information = new Text("[ T ] toggle mouse");
	information->setX(10);
	information->setY(30);
	engine.gui->addElement(information);

	//create button
	Button *button = new Button("analitycs");
	button->setX(10);
	button->setY(80);
	button->onClick = []{
	};
	button->onRelease = [&engine]{
		engine.analitycs->fpsMeter->getIsVisible() ? engine.analitycs->fpsMeter->hide() : engine.analitycs->fpsMeter->show();
	};
	engine.gui->addElement(button);

	//create audio
	Audio *audio = new Audio("samples/audio_loading/birds.wav");
	engine.audio->addElement(audio);
	audio->play();

	// game loop
	engine.run([&]() {

		// rotate model
        model->rotate(1, 0, 0);
        
    });
	
	return 0;
}