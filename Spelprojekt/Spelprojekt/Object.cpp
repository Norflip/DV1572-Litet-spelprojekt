#include "Object.h"

Object::Object(Mesh* mesh, Material* material) : mesh(mesh), material(material) {}

Object::Object(const Object& other) : mesh(other.mesh), material(other.material) {}

Object::~Object() {}

void Object::Render(Renderer* renderer, Camera* camera)
{
	renderer->DrawMesh(mesh, transform.GetWorldMatrix(), camera);
}
