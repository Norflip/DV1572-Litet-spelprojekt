#include "Lights.h"

Lights::Lights(size_t width, size_t height) : lightBuffer_ptr(nullptr), width(width), height(height) 
{
	lightConstantBuffer.ssao_scale = 1.5f;
	lightConstantBuffer.ssao_bias = 0.1f;
	lightConstantBuffer.ssao_intensity = 3.0f;
	lightConstantBuffer.ssao_radius = 1.5f;

	sunCamera = new Camera(90.0f, width, height);
}

Lights::~Lights() {}

void Lights::Initialize(DX11Handler* dx11)
{
	lightBuffer_ptr = dx11->CreateBuffer<LightData>(lightConstantBuffer);
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

void Lights::UpdateConstantBuffer(Camera* camera, ID3D11DeviceContext* context)
{
	auto it = pointLightMap.begin();
	size_t index = 0;

	while (it != pointLightMap.end() && index < MAX_LIGHTS)
	{
		lightConstantBuffer.pointLights[index] = (*it).second;
		index++;
		it++;
	}

	//lightConstantBuffer.sunView = DirectX::XMMatrixTranspose(sunCamera->GetView());
	//lightConstantBuffer.sunProjection = DirectX::XMMatrixTranspose(sunCamera->GetOrthographic());

	//DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(40.0f, 40.0f, 1.0f, 50.0f);

	lightConstantBuffer.sunView = DirectX::XMMatrixTranspose(sunCamera->GetView());
	lightConstantBuffer.sunProjection = DirectX::XMMatrixTranspose(sunCamera->GetOrthographic());

	//lightConstantBuffer.worldToView = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(camera->GetView(), camera->GetProjection()));
	lightConstantBuffer.worldToView = DirectX::XMMatrixTranspose(camera->GetView());
	lightConstantBuffer.pointLightCount = pointLightMap.size();
	lightConstantBuffer.screenSize = { static_cast<float>(width), static_cast<float>(height) };

	DirectX::XMStoreFloat3(&lightConstantBuffer.eyePosition, camera->GetTransform().GetPosition());
	context->UpdateSubresource(lightBuffer_ptr, 0, 0, &lightConstantBuffer, 0, 0);
	context->PSSetConstantBuffers(LIGHT_CONSTANT_BUFFER_SLOT, 1, &lightBuffer_ptr);
}

void Lights::UpdateCameras(DirectX::XMVECTOR focus, const AABB& bounds)
{
	const float sunOffsetDistance = 100.0f;
	DirectX::XMVECTOR sunDirection = DirectX::XMLoadFloat3(&lightConstantBuffer.sunDirection);
	
	focus = DirectX::XMVectorAdd(focus, { 0.0f, 0.0f, height / 2.0f });

	DirectX::XMVECTOR offset = DirectX::XMVectorAdd(
		focus, DirectX::XMVectorScale(DirectX::XMVector3Normalize(sunDirection), sunOffsetDistance * -1)
	);

	//lightConstantBuffer.sunView = DirectX::XMMatrixLookAtLH(offset, focusPosition, { 0,1,0 });

	//offset = DirectX::XMVectorAdd(focus, { -20, 2, -1 });
	sunCamera->GetTransform().SetPosition(offset);
	sunCamera->GetTransform().LookAtCamera(focus);
	sunCamera->UpdateView();


	DirectX::XMVECTOR targetPos = bounds.GetCenter();
	float sceneBoundsRadius = bounds.GetRadius();
	DirectX::XMVECTOR lightPosition = DirectX::XMVectorScale(sunDirection, -2.0f * sceneBoundsRadius);

	// lookAtMat
	DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(lightPosition, targetPos, { 0,1,0 });


	DirectX::XMFLOAT3 sphereCenterLS;
	DirectX::XMStoreFloat3(&sphereCenterLS, DirectX::XMVector3TransformCoord(targetPos, v));

	float l = sphereCenterLS.x - sceneBoundsRadius;
	float b = sphereCenterLS.y - sceneBoundsRadius;
	float n = sphereCenterLS.z - sceneBoundsRadius;
	float r = sphereCenterLS.x + sceneBoundsRadius;
	float t = sphereCenterLS.y + sceneBoundsRadius;
	float f = sphereCenterLS.z + sceneBoundsRadius;

	DirectX::XMMATRIX p = DirectX::XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	DirectX::XMFLOAT4X4 T;
	T._11 = 0.5f;
	T._22 = -0.5f;
	T._33 = 1.0f;
	T._44 = 1.0f;

	T._41 = 0.5f;
	T._42 = 0.5f;

	tView = v;
	tProjection = p;
	tShadowTransform = t * v * p;
	tShadowTransform = DirectX::XMMatrixIdentity();
}
