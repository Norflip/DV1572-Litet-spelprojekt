#pragma once
#include <reactphysics3d.h>
#include "Object.h"
#include "Transform.h"
#include "Logger.h"
#include <unordered_map>

class Physics
{
public:

	Physics();
	virtual~Physics();

	//rp3d::RigidBody* CreateRigidBody(float linearDamping, float angularDamping, float x, float y, float z);
	rp3d::ConvexMeshShape* ConvertMeshToConvexShape(Mesh*);
	rp3d::ProxyShape* AddCollision(rp3d::ConvexMeshShape* shape, rp3d::RigidBody* body, float mass);
	rp3d::Transform ConvertTransform(const Transform& transform);

	rp3d::Vector3 Convert(DirectX::XMVECTOR v3);

	rp3d::CollisionBody* CreateCollisionBodyBox(Object* object);
	rp3d::CollisionBody* CreateCollisionBodyConvex(Object* object);
	rp3d::CollisionBody* CreateCollisionBodyCapsule(Object* object, float radius, float xOffset = 0, float zOffset = 0, float height = 100.0f);
	void UpdateCollisionBody(rp3d::CollisionBody* body, const Transform& transform);

	rp3d::CollisionWorld* GetWorld() const { return this->world; }

private:
	std::unordered_map<Mesh*, rp3d::ConvexMeshShape*> meshShapeCache;
	rp3d::CollisionWorld* world;
	rp3d::Vector3 gravity;

};