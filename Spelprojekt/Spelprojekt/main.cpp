#include <reactphysics3d\reactphysics3d.h>

void TestPhysicis();

void TestPhysicis()
{
	reactphysics3d::PhysicsCommon common;
	rp3d::PhysicsWorld* world = common.createPhysicsWorld();

	// YADDA YADDA
}

#include "Application.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	TestPhysicis();

	Application application (hInstance); //sets up an application with hInstance
	application.Run(); //Starts the rendering loop
	return 0;
}
