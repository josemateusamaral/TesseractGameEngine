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
	engine.window->setBackgroundColor(0,0,0); // sky blue
	engine.setCaptureMouse(true);
	engine.camera->setPos(Vec3(-6.2,32,21));
	engine.camera->hpr = Vec3(11,90,0);

	// create ambient light
	AmbientLight* ambientLight = new AmbientLight(0.4,0.4,0.4);
	// create point light
	PointLight* pointLight = new PointLight(0.4,0.4,0.4,0,0,0);
	// create directional light
	DirectionalLight* directionalLight = new DirectionalLight(0.6,0.6,0.6,0,0,-1);

	//create table
	Model* table = new Model("samples/model_loading/table.glb");
	table->setPos(-30, -20, 30);
	table->setScale(20);
	engine.scene->addModel(table);
	table->setLight(ambientLight);
	table->setLight(directionalLight);

	//create radio
	Model* radio = new Model("samples/model_loading/radio.glb");
	radio->setPos(-30, 15, 30);
	radio->setScale(20);
	radio->rotate(0, 90, 0);
	engine.scene->addModel(radio);
	radio->setLight(ambientLight);
	radio->setLight(directionalLight);
	Audio *audio = new Audio("samples/audio_loading/radio_broadcast.wav");
	audio->setPos(new Vec3(-30, 21, 30));
	audio->volume = 0.5;
	engine.audio->addElement(audio);
	audio->loop();

	//create interior
	Model* model = new Model("samples/model_loading/interior.glb");
	model->setPos(0, 45, 40);
	model->setScale(140);
	model->rotate(0, 90, 0);
	engine.scene->addModel(model);
	model->setLight(directionalLight);
	model->setLight(ambientLight);

	//create ground
	Model* ground = new Model("samples/model_loading/ground_plane.glb");
	ground->setPos(-30, -12, 30);
	ground->setScale(200);
	engine.scene->addModel(ground);
	ground->setLight(ambientLight);
	ground->setLight(directionalLight);

	//create text
	Text *information = new Text("[ F ] interact");
	information->setX((engine.window->getWidth() / 2) - (information->width / 2));
	information->setY((engine.window->getHeight() /2) + (engine.window->getHeight() /4));
	information->setTextColor(255,255,255);
	engine.gui->addElement(information);

	//create button
	Button *button = new Button("Turn OFF");
	button->setX(10);
	button->setY(80);
	button->setTextColor(255,255,255);
	button->setBackgroundColor(0,0,0);
	button->hide();
	button->onClick = []{
	};
	button->onRelease = [&]{
		if(audio->playing){
			audio->stop();
			button->setText("Turn ON");
		}else{
			audio->loop();
			button->setText("Turn OFF");
		}
	};
	engine.gui->addElement(button);




	//game loop variables
	float radioDistance;
	
	// bind keys
	engine.input->bindKey("escape", "release", [&engine]() {
		engine.exit();
	});
	engine.input->bindKey("w", "press", [&engine]() {
		engine.camera->moveFront(1.0f);
	});
	engine.input->bindKey("a", "press", [&engine]() {
		engine.camera->moveLeft(1.0f);
	});
	engine.input->bindKey("s", "press", [&engine]() {
		engine.camera->moveBack(1.0f);
	});
	engine.input->bindKey("d", "press", [&engine]() {
		engine.camera->moveRight(1.0f);
	});
	engine.input->bindKey("q", "press", [&engine]() {
		engine.camera->setY(engine.camera->getY() - 1);
	});
	engine.input->bindKey("e", "press", [&engine]() {
		engine.camera->setY(engine.camera->getY() + 1);
	});
	engine.input->bindKey("f", "release", [&]() {

		float radioDistance = (radio->getPos() - engine.camera->getPos()).modulo();
		if(radioDistance <= 30){
			if(engine.getCaptureMouse()){
				engine.setCaptureMouse(false);
				button->show();
			}else{
				engine.setCaptureMouse(true);
				button->hide();
			}
		}
		
	});
	// bind mouse
	engine.input->bindMouseButton("left", "release", [&]() {
		//radio->setY(radio->getY() + 1);
		//printf("%f %f %f",radio->getX(),radio->getY(),radio->getZ());
	});
	engine.input->bindMouseButton("right", "release", [&]() {
		//radio->setY(radio->getY() - 1);
		//printf("%f %f %f",radio->getX(),radio->getY(),radio->getZ());
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


	// game loop
	engine.run([&]() {

		//update radio interaction
		radioDistance = (radio->getPos() - engine.camera->getPos()).modulo();
		if(radioDistance <= 30){
			information->show();
		}else{
			information->hide();
		}
        
    });
	
	return 0;
}