#pragma once
#include <d3d11_1.h>
#include <dxgi.h>     // anv�nds f�r swapchain och device / deviceContext
#include <DirectXMath.h>

#include "Material.h"
#include "Mesh.h"
#include "DX11Handler.h"
#include "RenderTarget.h"
#include "ConstantBuffers.h"
#include "Lights.h"

#include "GUI.h"

constexpr float CLEAR_COLOR[3] = { 0.4f,0.4f,0.4f };

class Renderer
{
public:
	Renderer(size_t width, size_t height, Timer&, DX11Handler&);
	virtual ~Renderer();

	void SetDeferredRenderTarget();
	void SetRenderTarget(RenderTarget* renderTarget);

	void ClearRenderTarget();
	void DrawMesh(Mesh*, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	Lights& GetLights() { return this->lights; }

	void SetGUI(GUI* gui) { this->gui = gui; }
	void DisplayFrame(DirectX::XMVECTOR eye);
	void setVsync(bool vsync) { this->vSync = vSync; }
private:
	void DrawMesh(Mesh*);

private:
	bool vSync = false;
	DX11Handler& dx11;
	Timer& timer;
	Lights lights;

	Shader* lightpass;
	Mesh* screenQuad;

	Material* meshMat;
	GUI* gui;

	ID3D11Buffer* worldBuffer_ptr;
	WorldData cb_world;

	RenderTarget* backbufferRenderTarget;
	RenderTarget* gbufferRenderTarget;
	RenderTarget* currentRenderTarget;
};