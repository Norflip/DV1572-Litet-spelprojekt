#pragma once
#include <d3d11_1.h>
#include <dxgi.h>     // anv�nds f�r swapchain och device / deviceContext

class Renderer
{
public:
	Renderer(ID3D11Device*, ID3D11DeviceContext*);
	virtual ~Renderer();

private:

};