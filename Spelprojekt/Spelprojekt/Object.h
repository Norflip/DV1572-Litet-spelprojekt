#pragma once
#include "Transform.h"
//#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "AABB.h"
class Renderer;

class Object
{
public:

	Object();
	Object(const Object& other);
	Object(Mesh* mesh, Material* material);
	virtual ~Object();

	Transform& GetTransform() { return this->transform; }
	void SetMaterial(Material* material) { this->material = material; }
	Material* GetMaterial() const { return this->material; }

	void SetMesh(Mesh* mesh);
	Mesh* GetMesh() const { return this->mesh; }

	AABB GetLocalBounds() const { return this->localBounds; }
	AABB GetWorldBounds() const;

	void UpdateLocalBounds();

	virtual void Render(Renderer*, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
	virtual void Update(const float& deltaTime) {};
	virtual void FixedUpdate(const float& fixedDeltaTime) {};

	void SetEnabled(bool enabled) { this->enabled = enabled; }
	bool IsEnabled() const { return this->enabled; }
	
private:
	Transform transform;
	AABB localBounds;
	
	Material* material;
	Mesh* mesh;

	bool enabled;
};