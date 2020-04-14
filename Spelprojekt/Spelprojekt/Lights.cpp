#include "Lights.h"

Lights::Lights() : lightBuffer_ptr(nullptr) {}
Lights::~Lights() {}

void Lights::Initialize(ID3D11Device* device)
{
	if (lightBuffer_ptr == nullptr)
	{
		static_assert(sizeof(lightConstantBuffer) % 16 == 0, "wrong size");

		HRESULT hr;
		D3D11_BUFFER_DESC bufferDescription;
		D3D11_SUBRESOURCE_DATA subresourceData;
		ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		ZeroMemory(&lightBuffer_ptr, sizeof(lightBuffer_ptr));

		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = 0;

		// LIGHT BUFFER
		bufferDescription.ByteWidth = sizeof(lightConstantBuffer);
		subresourceData.pSysMem = &lightConstantBuffer;

		hr = device->CreateBuffer(&bufferDescription, &subresourceData, &lightBuffer_ptr);
		assert(SUCCEEDED(hr));
	}
}

size_t Lights::AddPointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color, float attenuation)
{
	size_t id = pointLight_ID++;
	PointLight pl;
	pl.position = position;
	pl.color = color;
	pl.power = attenuation;

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
