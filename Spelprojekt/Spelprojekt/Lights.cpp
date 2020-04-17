#include "Lights.h"

Lights::Lights() : lightBuffer_ptr(nullptr) {}
Lights::~Lights() {}

void Lights::Initialize(DX11Handler& dx11)
{
	lightBuffer_ptr = dx11.CreateBuffer<LightData>(lightConstantBuffer);
}

size_t Lights::AddPointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color, float radius)
{
	size_t id = pointLight_ID++;
	PointLight pl;
	pl.position = position;
	pl.color = color;
	pl.radius = radius;

	pointLightMap.insert({ id, pl });

	dirty = true;
	return id;
}

void Lights::RemovePointLight(size_t id)
{
	auto findResult = pointLightMap.find(id);
	if (findResult != pointLightMap.end())
	{
		pointLightMap.erase(findResult);
		dirty = true;
	}
}

void Lights::SetSun(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT4 color, float intensity)
{
	SetSunDirection(direction);
	SetSunColor(color);
	SetSunIntensity(intensity);
}

void Lights::SetSunDirection(DirectX::XMFLOAT3 direction)
{
	// normalize the direction
	float l = std::sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	lightConstantBuffer.sunDirection = { direction.x / l, direction.y / l, direction.z / l };
}

void Lights::UpdateConstantBuffer(DirectX::XMFLOAT3 eye, ID3D11DeviceContext* context)
{
	auto it = pointLightMap.begin();
	size_t index = 0;

	while (it != pointLightMap.end() && index < MAX_LIGHTS)
	{
		lightConstantBuffer.pointLights[index] = (*it).second;
		index++;
		it++;
	}

	lightConstantBuffer.pointLightCount = pointLightMap.size();
	lightConstantBuffer.eyePosition = eye;

	context->UpdateSubresource(lightBuffer_ptr, 0, 0, &lightConstantBuffer, 0, 0);
	context->PSSetConstantBuffers(CONSTANT_BUFFER_SLOT, 1, &lightBuffer_ptr);
}
