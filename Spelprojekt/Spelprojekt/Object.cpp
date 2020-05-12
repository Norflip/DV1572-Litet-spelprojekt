#include "Object.h"
#include "Renderer.h"
Object::Object(Mesh* mesh, Material* material) : mesh(mesh), material(material), enabled(true)
{
	SetMesh(mesh);
}

Object::Object() : Object(nullptr, nullptr) {}
Object::Object(const Object& other) : Object(other.mesh, other.material) { this->enabled = other.enabled; }
Object::~Object() {}

void Object::SetMesh(Mesh* mesh)
{
	this->mesh = mesh; 
	UpdateLocalBounds();
}

AABB Object::GetWorldBounds() const
{
	DirectX::XMVECTOR min, max, corner;
	DirectX::XMMATRIX worldTransform = transform.GetWorldMatrix();

	DirectX::XMVECTOR corners[8];
	localBounds.GetCorners(corners);

	for (size_t i = 0; i < 8; i++)
	{
		corner = DirectX::XMVector3Transform(corners[i], worldTransform);

		if (i == 0)
			min = max = corner;
		else
		{
			min = DirectX::XMVectorMin(min, corner);
			max = DirectX::XMVectorMax(max, corner);
		}
	}

	return AABB(min, max);
}

void Object::UpdateLocalBounds()
{
	if (this->mesh != nullptr)
	{
		this->localBounds = AABB({ FLT_MAX, FLT_MAX, FLT_MAX }, { -FLT_MAX, -FLT_MAX, -FLT_MAX });
		DirectX::XMVECTOR pos;

		for (size_t i = 0; i < this->mesh->vertexes.size(); i++)
		{
			pos = DirectX::XMLoadFloat3(&this->mesh->vertexes[i].position);
			localBounds.Encapsulate(pos);
		}
	}
}

void Object::Render(Renderer* renderer, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	if(IsVisible())
		renderer->DrawMesh(mesh, transform.GetWorldMatrix(), view, projection);
}
