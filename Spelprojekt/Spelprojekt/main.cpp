#include "reactphysics3d.h"
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Gravity vector 
	rp3d::Vector3 gravity(0.0, -9.81, 0.0);

	// Create the dynamics world 
	rp3d::DynamicsWorld world(gravity);

	// Initial position and orientation of the rigid body 
	rp3d::Vector3 initPosition(0.0, 3.0, 0.0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);

	// Create a rigid body in the world 
	rp3d::RigidBody* body;
	body = world.createRigidBody(transform);

	Application application (hInstance); //sets up an application with hInstance
	application.Run(); //Starts the rendering loop
	return 0;
}
