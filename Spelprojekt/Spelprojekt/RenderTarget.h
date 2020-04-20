#pragma once
#include <d3d11.h>    // D3D 11
#include <dxgi.h>     // används för swapchain och device / deviceContext
#include <assert.h>

class RenderTarget
{
public:
	RenderTarget(ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* srv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT viewport);
	RenderTarget(size_t bufferCount, size_t width, size_t height, bool create);
	virtual ~RenderTarget();

	void Initalize(ID3D11Device*);

	void Bind(ID3D11DeviceContext*);
	void Unbind(ID3D11DeviceContext*);

	ID3D11RenderTargetView** GetRenderTargetViews() const { return this->rtv; }

	bool HasDepthBuffer() const { return this->createDepthBuffer; }
	ID3D11DepthStencilView* GetDepthStencil() { return this->dsv; }
	ID3D11DepthStencilState* GetDepthStencilState() const { return this->dss; }

	size_t BufferCount() const { return this->bufferCount; }
	D3D11_VIEWPORT& GetViewport() { return this->viewport; }
	void Cleanup();

private:
	size_t bufferCount;
	size_t width, height;
	bool createDepthBuffer;

	ID3D11RenderTargetView** rtv;
	ID3D11ShaderResourceView** srv;
	ID3D11DepthStencilView* dsv;
	ID3D11DepthStencilState* dss;

	D3D11_VIEWPORT viewport;
};