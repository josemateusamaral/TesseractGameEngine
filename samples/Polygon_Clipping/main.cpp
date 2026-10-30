#include "../../core/model.h"
#include "../../core/light.h"
#include "../../core/tesseract.h"
#include "../../core/camera.h"
#include "../../core/gui.h"
#include "../../core/audio.h"
#include "../../core/vec3.h"

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
	engine.window->setBackgroundColor(0,0,0);
	engine.setCaptureMouse(true);
	engine.camera->setPos(Vec3(12.202580,35,-23.123072));
	engine.camera->hpr = Vec3(23.929991,66.240021,0);

	// create ambient light
	AmbientLight* ambientLight = new AmbientLight(0.4,0.4,0.4);
	// create point light
	PointLight* pointLight = new PointLight(0.6,0.6,0.6,-30,19,30);
	// create directional light
	DirectionalLight* directionalLight = new DirectionalLight(0.6,0.6,0.6,0,0,-1);

	//create ground
	Model* ground = new Model("assets/models/polygon.glb");
	ground->setPos(-30, -12, 30);
	ground->setScale(60);
	engine.scene->addModel(ground);
	ground->setLight(ambientLight);
	ground->setLight(pointLight);
	
	// bind keys
	engine.input->bindKey("escape", "release", [&engine]() {
		engine.exit();
	});
	engine.input->bindKey("w", "press", [&engine]() {
		if(!engine.getCaptureMouse())
			return;
		engine.camera->moveFront(0.1f * engine.deltaTime);
	});
	engine.input->bindKey("a", "press", [&engine]() {
		if(!engine.getCaptureMouse())
			return;
		engine.camera->moveLeft(0.1f * engine.deltaTime);
	});
	engine.input->bindKey("s", "press", [&engine]() {
		if(!engine.getCaptureMouse())
			return;
		engine.camera->moveBack(0.1f * engine.deltaTime);
	});
	engine.input->bindKey("d", "press", [&engine]() {
		if(!engine.getCaptureMouse())
			return;
		engine.camera->moveRight(0.1f * engine.deltaTime);
	});
	engine.input->bindKey("q", "press", [&engine]() {
		engine.camera->setY(engine.camera->getY() - 1);
	});
	engine.input->bindKey("e", "press", [&engine]() {
		engine.camera->setY(engine.camera->getY() + 1);
	});
	engine.input->bindKey("f", "release", [&]() {
		
		printf("\nCAMERA POS: %f %f %f",engine.camera->getPos().x,engine.camera->getPos().y,engine.camera->getPos().z);
		printf("\nCAMERA HPR: %f %f %f\n",engine.camera->hpr.x,engine.camera->hpr.y,engine.camera->hpr.z);

		for(int i = 0; i < ground->nVertices; i++){

			printf("\nMODEL VERTICE %d: %f %f %f\n",i,ground->vertices[i].x,ground->vertices[i].y,ground->vertices[i].z);
			printf("\nMODEL PROJECTED VERTICE %d: %f %f %f\n",i,ground->projection[i].x,ground->projection[i].y,ground->projection[i].z);			

		}   

	});
	// bind mouse
	engine.input->bindMouseButton("left", "release", [&]() {

	});
	engine.input->bindMouseButton("right", "release", [&]() {

	});
	engine.input->bindMouseMotion([&engine]() {

		//rotate camera left and right using mouse movement
		engine.camera->hpr.x += engine.input->mouseMotionVector->y * 0.01 * engine.deltaTime;
		if(engine.camera->hpr.x == 0) 
			engine.camera->hpr.x = 360;
		if(engine.camera->hpr.x > 360)
			engine.camera->hpr.x = 1;
		
		//rotate camera up and down using mouse movement
		engine.camera->hpr.y += engine.input->mouseMotionVector->x * 0.01 * engine.deltaTime;
		if(engine.camera->hpr.y == 0) 
			engine.camera->hpr.y = 360;
		if(engine.camera->hpr.y > 360)
			engine.camera->hpr.y = 1;

	});

	// game loop
	engine.run([&]() {
        
    });
	
	return 0;
}