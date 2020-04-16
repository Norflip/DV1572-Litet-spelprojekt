#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "AABB.h"

class Object
{
public:

	Object(const Object& other);
	Object(Mesh* mesh, Material* material);
	virtual ~Object();

	Transform& GetTransform() { return this->transform; }
	Material* GetMaterial() const { return this->material; }
	Mesh* GetMesh() const { return this->mesh; }

	AABB GetLocalBounds() const { return this->localBounds; }
	AABB GetWorldBounds() const;

	void Render(Renderer*, Camera*);
	void UpdateLocalBounds();

private:
	Transform transform;
	AABB localBounds;
	
	Material* material;
	Mesh* mesh;
};