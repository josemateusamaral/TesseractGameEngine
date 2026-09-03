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
	engine.setCaptureMouse(true);
	engine.camera->setX(-79);
	engine.camera->setY(73);
	engine.camera->setZ(-5.5);

	// create ambient light
	AmbientLight* ambientLight = new AmbientLight(0.4,0.4,0.4);
	// create point light
	PointLight* pointLight = new PointLight(0.4,0.4,0.4,0,0,0);
	// create directional light
	DirectionalLight* directionalLight = new DirectionalLight(0.6,0.6,0.6,0,0,-1);

	//create audio3D
	Audio *audio = new Audio("samples/audio_loading/birds.wav");
	audio->setPos(new Vec3( -62, 23, 30));
	engine.audio->addElement(audio);
	audio->loop();

	// load plane
	Model* plane = new Model("samples/model_loading/room.glb");
	plane->setPos( 0, -9, 30);
	plane->setScale(100);
	plane->rotate(0, 0, 90);
	engine.scene->addModel(plane);
	plane->setLight(ambientLight);
	plane->setLight(directionalLight);

	// load plane
	Model* character = new Model("samples/model_loading/character.glb");
	character->setPos( -62, 23, 30);
	character->rotate(180,0,0);
	character->setScale(3);
	engine.scene->addModel(character);
	character->setLight(ambientLight);
	character->setLight(directionalLight);

	// load plane
	Model* ground = new Model("samples/model_loading/plane_sub.glb");
	ground->setPos(-93, -6, 30);
	ground->setScale(100);
	engine.scene->addModel(ground);
	ground->setLight(ambientLight);
	ground->setLight(directionalLight);
	// apply texture to plane
	Texture *texture = new Texture("samples/texture_loading/areia.jpg");
	ground->diffuseTexture = texture;
	
	// bind keys
	engine.input->bindKey("escape", "release", [&engine]() {
		engine.exit();
	});
	engine.input->bindKey("w", "press", [&engine]() {
		//movver camera frente
		float speed = 3.0f;
		float h = engine.camera->hpr.y * M_PI / 180.0f * -1.0f;
		float dx = sin(h);
		float dy = cos(h);
		engine.camera->setX(
			engine.camera->getX() + dx * speed
		);
		engine.camera->setZ(
			engine.camera->getZ() + dy * speed
		);
	});
	engine.input->bindKey("a", "press", [&engine]() {
		engine.camera->setX(engine.camera->getX() + 1);
	});
	engine.input->bindKey("s", "press", [&engine]() {
		//mover camera para tras
		float speed = -3.0f;
		float h = engine.camera->hpr.y * M_PI / 180.0f * -1.0f;
		float dx = sin(h);
		float dy = cos(h);
		engine.camera->setX(
			engine.camera->getX() + dx * speed
		);
		engine.camera->setZ(
			engine.camera->getZ() + dy * speed
		);
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
	engine.input->bindMouseButton("left", "release", [&plane]() {

	});
	engine.input->bindMouseButton("right", "release", [&engine]() {

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
	information->setTextColor(255,0,0);
	engine.gui->addElement(information);

	//create button
	Button *button = new Button("analitycs");
	button->setX(10);
	button->setY(80);
	button->setTextColor(0,255,0);
	button->setBackgroundColor(79,6,102);
	button->onClick = []{
	};
	button->onRelease = [&engine]{
		engine.analitycs->fpsMeter->getIsVisible() ? engine.analitycs->fpsMeter->hide() : engine.analitycs->fpsMeter->show();
	};
	engine.gui->addElement(button);

	// game loop
	engine.run([&]() {

		// rotate model
        //model->rotate(1, 0, 0);
        
    });
	
	return 0;
}