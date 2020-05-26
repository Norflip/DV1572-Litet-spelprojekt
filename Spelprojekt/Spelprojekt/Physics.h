#pragma once
#include "reactphysics3d.h"
#include "Mesh.h"

class Physics
{
public:

	Physics(float gX, float gY, float gZ, float currentTime, float lastFrame, float deltaTime);
	virtual~Physics();

	rp3d::RigidBody* CreateRigidBody(float linearDamping, float angularDamping, float x, float y, float z);
	rp3d::BoxShape* CreateBoxShape();
	rp3d::ConvexMeshShape* ConvertMeshToConvexShape(Mesh*);

private:

	rp3d::DynamicsWorld *world;
	rp3d::Vector3 gravity;

};