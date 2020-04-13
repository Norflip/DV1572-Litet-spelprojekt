#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

class Object
{
public:

	Object(const Object& other);
	Object(Mesh* mesh, Material* material);
	virtual ~Object();

	Transform& GetTransform() { return this->transform; }
	Material* GetMaterial() const { return this->material; }
	Mesh* GetMesh() const { return this->mesh; }

	void Render(Renderer*, Camera*);

private:
	Transform transform;
	Material* material;
	Mesh* mesh;
};