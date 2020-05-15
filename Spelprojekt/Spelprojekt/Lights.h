#pragma once
#include <DirectXMath.h>
#include <d3d11_1.h>
#include <dxgi.h>
#include "ConstantBuffers.h"
#include "DX11Handler.h"
#include <unordered_map>
#include "Camera.h"

class Lights
{
	const int CONSTANT_BUFFER_SLOT = 1;

public:
	Lights();
	virtual ~Lights();

	void Initialize(DX11Handler& dx11);

	size_t AddPointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color, float attenuation);
	void RemovePointLight(size_t id);

	void SetSun(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT4 color, float intensity);

	void SetSunColor(DirectX::XMFLOAT4 color) { lightConstantBuffer.sunColor = color; }
	void SetSunIntensity(float intensity) { lightConstantBuffer.sunIntensity = intensity; }
	void SetSunDirection(DirectX::XMFLOAT3 direction);

	bool IsDirty() const { return this->dirty; }
	void SetDirty() { this->dirty = true; }

	void UpdateConstantBuffer(Camera* camera, ID3D11DeviceContext*);

private:
	size_t pointLight_ID;
	std::unordered_map<size_t, PointLight> pointLightMap;

	bool dirty;
	LightData lightConstantBuffer;
	ID3D11Buffer* lightBuffer_ptr;
};