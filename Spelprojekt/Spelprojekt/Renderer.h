#pragma once
#include <d3d11_1.h>
#include <dxgi.h>     // anv�nds f�r swapchain och device / deviceContext
#include <DirectXMath.h>

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "DX11Handler.h"
#include "RenderTarget.h"
#include "ConstantBuffers.h"
#include "Lights.h"

#include "GUI.h"
#include "GUITextObject.h"


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

	void SetGUI(GUI* gui) { this->gui = gui; }	

	void DisplayFrame(Camera*);
	void ApplyMaterial(Material*);

private:
	void DrawMesh(Mesh*);

private:
	DX11Handler& dx11;
	Shader* lightpass;
	Mesh* screenQuad;
	Lights* lights;
	
	Material* meshMat;
	GUI* gui;

	ID3D11Buffer* worldBuffer_ptr;
	WorldConstantBuffer cb_world;

	RenderTarget* backbufferRenderTarget;
	RenderTarget* gbufferRenderTarget;
	RenderTarget* currentRenderTarget;
};