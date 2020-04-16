#pragma once
#include <d3d11_1.h>
#include <dxgi.h> 
#include <assert.h>
#include <d3dcompiler.h>

#include "Window.h"
#include "RenderTarget.h"

#pragma comment(lib, "gdi32")
#pragma comment(lib, "d3d11") 
#pragma comment( lib, "dxgi" )   
#pragma comment(lib, "d3dcompiler.lib")

class DX11Handler
{
public:

	DX11Handler();
	virtual ~DX11Handler();

	void Initialize(const Window& window);

	ID3D11Device* GetDevice() const { return this->device; }
	ID3D11DeviceContext* GetContext() const { return this->context; }
	IDXGISwapChain* GetSwapChain() const { return this->swapchain; }
	RenderTarget* GetBackbuffer() const { return this->backbuffer; }

	void SetFullscreen(bool fullscreen);
	void SetWireframeMode(bool);

private:
	void CreateBackbufferRenderTarget(size_t width, size_t height);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	ID3D11RasterizerState* rasterizerState;
	
	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	RenderTarget* backbuffer;
};