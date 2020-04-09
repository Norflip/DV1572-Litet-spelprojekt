#pragma once
//#define WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h> 

#include <d3d11_1.h>
#include <dxgi.h> 
#include <assert.h>
#include <d3dcompiler.h>

#pragma comment(lib, "gdi32")
#pragma comment(lib, "d3d11") 
#pragma comment(lib, "d3dcompiler.lib")

/*
#pragma comment( lib, "gdi32" )           // dxgi.h ?
#pragma comment( lib, "d3d11" )           // Direct3D 11
#pragma comment( lib, "dxgi" )
#pragma comment( lib, "d3dcompiler.lib" ) // för att kompilera shaders	dxguid.lib
*/

class DX11Handler
{
public:

	DX11Handler();
	virtual ~DX11Handler();

	void Initialize(size_t width, size_t height, HWND);

	ID3D11Device* GetDevice() const { return this->device; }
	ID3D11DeviceContext* GetDeviceContext() const { return this->context; }
	D3D11_VIEWPORT GetViewport() const { return this->windowViewport; }

	void SetFullscreen(bool fullscreen);
	void SetWireframeMode(bool);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	ID3D11RasterizerState* rasterizerState;
	D3D11_VIEWPORT windowViewport;

	DXGI_SWAP_CHAIN_DESC swapChainDescription;
};