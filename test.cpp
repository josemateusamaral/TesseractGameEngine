#include "core/model.h"
#include "core/light.h"
#include "core/tesseract.h"
#include "core/camera.h"
#include "core/gui.h"
#include "core/audio.h"
#include "core/vec3.h"

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
	engine.window->setBackgroundColor(135, 206, 235); // sky blue

	// create ambient light
	AmbientLight* ambientLight = new AmbientLight(0.4,0.4,0.4);
	// create point light
	PointLight* pointLight = new PointLight(0.4,0.4,0.4,0,0,0);
	// create directional light
	DirectionalLight* directionalLight = new DirectionalLight(0.6,0.6,0.6,0,0,-1);

	// load model
	Model* model = new Model("samples/model_loading/character.glb");
	model->setPos( 0, 0, 30);
	model->setScale(1);
	engine.scene->addModel(model);
	model->setLight(directionalLight);
	model->setLight(ambientLight);

	// load plane
	Model* plane = new Model("samples/model_loading/plane.glb");
	plane->setPos( 0, -9, 30);
	plane->setScale(10);
	engine.scene->addModel(plane);
	plane->setLight(ambientLight);
	plane->setLight(directionalLight);
	// apply texture to plane
	Texture *texture = new Texture("samples/texture_loading/areia.jpg");
	plane->diffuseTexture = texture;
	
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
		printf("\nreleasing left mouse button");
	});
	engine.input->bindMouseButton("right", "release", [&model]() {
		printf("\nreleasing right mouse button");
	});
	engine.input->bindMouseMotion([&engine]() {

		//rotate camera left and right using mouse movement
		engine.camera->hpr.x += engine.input->mouseMotionVector->y * 0.1;
		if(engine.camera->hpr.x == 0) 
			engine.camera->hpr.x = 360;
		if(engine.camera->hpr.x > 360)
			engine.camera->hpr.x = 1;
		
		//rotate camera up and down using mouse movement
		engine.camera->hpr.y += engine.input->mouseMotionVector->x * 0.1;
		if(engine.camera->hpr.y == 0) 
			engine.camera->hpr.y = 360;
		if(engine.camera->hpr.y > 360)
			engine.camera->hpr.y = 1;

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

	//create audio3D
	Audio *audio = new Audio("samples/audio_loading/birds.wav");
	audio->setPos(new Vec3(0, 0, 14));
	engine.audio->addElement(audio);
	//audio->play();

	//create shadow caster
	engine.scene->addShadowCaster(directionalLight);

	// game loop
	engine.run([&]() {

		// rotate model
        model->rotate(1, 0, 0);
        
    });
	
	return 0;
}