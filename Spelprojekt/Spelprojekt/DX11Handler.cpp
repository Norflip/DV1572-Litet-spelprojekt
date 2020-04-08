#include "DX11Handler.h"

DX11Handler::DX11Handler() : device(nullptr), context(nullptr), swapchain(nullptr), rasterizerState(nullptr)
{
}

DX11Handler::~DX11Handler()
{
}

void DX11Handler::Initialize(size_t width, size_t height, HWND hwnd)
{
	/*
		Use adapter output to calculate numerator & denominator
		IDXGIFactory* pFactory;
		IDXGIAdapter* pAdapter;
		IDXGIOutput* pAdapterOutput;
	*/


	/////////////////				SWAPCHAIN INITIALIZE				/////////////////
	
	// Swapchain: swaps between two buffers
	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDescription.BufferCount = 2;	 // one back buffer

	swapChainDescription.BufferDesc.Width = width;
	swapChainDescription.BufferDesc.Height = height;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 60; // sets framerate to 60 as max
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // dunno DXGI_USAGE_SHADER_INPUT
	swapChainDescription.OutputWindow = hwnd;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT swapflags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef  _DEBUG
	swapflags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0 };	// 

	HRESULT resultCreateDevAndSwap = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapflags, featureLevel, 1, D3D11_SDK_VERSION, &swapChainDescription, &swapchain, &device, nullptr, &context);
	assert(SUCCEEDED(resultCreateDevAndSwap));



	/////////////////				END SWAPCHAIN INITIALIZE				/////////////////
	/*
		viewport
		Depth buffer
	*/

	SetFillMode(D3D11_FILL_SOLID);
}

void DX11Handler::SetFillMode(D3D11_FILL_MODE mode)
{
	/////////////////				RASTERDESC INITIALIZE				/////////////////
	
	D3D11_RASTERIZER_DESC rasterizerDescription;
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDescription.FillMode = mode; //if we want wireframe, fill etc
	rasterizerDescription.CullMode = D3D11_CULL_NONE;

	/* Filips nasty shit >:D
	rasterizerDescription.DepthClipEnable = false;
	rasterizerDescription.FrontCounterClockwise = false;
	rasterizerDescription.MultisampleEnable = false;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	*/

	HRESULT resultCreateRasterizer = device->CreateRasterizerState(&rasterizerDescription, &rasterizerState);
	assert(SUCCEEDED(resultCreateRasterizer));


}
