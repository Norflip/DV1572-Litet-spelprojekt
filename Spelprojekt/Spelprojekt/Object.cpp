#include "Object.h"

Object::Object(Mesh* mesh, Material* material) : mesh(mesh), material(material) 
{
	UpdateLocalBounds();
}

Object::Object(const Object& other) : Object(other.mesh, other.material) {}

Object::~Object() {}

AABB Object::GetWorldBounds() const
{
	DirectX::XMVECTOR min, max, ch;
	DirectX::XMMATRIX w = transform.GetWorldMatrix();

	DirectX::XMVECTOR corners[8];
	localBounds.GetCorners(corners);

	for (size_t i = 0; i < 8; i++)
	{
		ch = DirectX::XMVector3Transform(corners[i], w);

		if (i == 0)
			min = max = corners[i];
		else
		{
			min = DirectX::XMVectorMin(min, ch);
			max = DirectX::XMVectorMax(max, ch);
		}
	}

	return AABB(min, max);
}

void Object::Render(Renderer* renderer, Camera* camera)
{
	renderer->DrawMesh(mesh, transform.GetWorldMatrix(), camera);
}

void Object::UpdateLocalBounds()
{
	if (this->mesh != nullptr)
	{
		this->localBounds = AABB({ FLT_MAX, FLT_MAX, FLT_MAX }, { -FLT_MAX, -FLT_MAX, -FLT_MAX });

		DirectX::XMMATRIX rot = this->transform.GetWorldMatrix();
		DirectX::XMVECTOR pos;

		for (size_t i = 0; i < this->mesh->vertexes.size(); i++)
		{
			pos = DirectX::XMLoadFloat3(&this->mesh->vertexes[i].position);
			localBounds.Encapsulate(pos);
		}
	}
}
