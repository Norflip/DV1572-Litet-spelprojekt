#pragma once
#include <d3d11_1.h>
#include <dxgi.h>     // används för swapchain och device / deviceContext
#include <DirectXMath.h>

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "DX11Handler.h"
#include "RenderTarget.h"
#include "ConstantBuffers.h"
#include "Lights.h"

#include "HUD.h"
#include "Timer.h"

constexpr float CLEAR_COLOR[3] = { 0.4f,0.4f,0.4f };

class Renderer
{
public:
	Renderer(size_t width, size_t height, DX11Handler&);
	virtual ~Renderer();

	void SetDeferredRenderTarget();
	void SetRenderTarget(RenderTarget* renderTarget);

	void ClearRenderTarget();
	void DrawMesh(Mesh*, DirectX::XMMATRIX world, Camera*);

	Lights* GetLights() const { return this->lights; }
	void SetLights(Lights* lights);
	
	void DisplayFrame(Camera*);
	void ApplyMaterial(Material*);

private:
	void DrawMesh(Mesh*);

private:
	DX11Handler& dx11;
	Shader* lightpass;
	Mesh* screenQuad;
	Lights* lights;
	
	ID3D11Buffer* worldBuffer_ptr;
	WorldConstantBuffer cb_world;

	RenderTarget* backbufferRenderTarget;
	RenderTarget* gbufferRenderTarget;
	RenderTarget* currentRenderTarget;

	HUD* hud;
	Timer timer;
	int counter = 0;
};