#include "RenderTarget.h"
#include <iostream>

RenderTarget::RenderTarget(ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* srv, ID3D11DepthStencilView* dsv, D3D11_VIEWPORT viewport) : bufferCount(1), viewport(viewport), dss(nullptr)
{
	this->rtv = new ID3D11RenderTargetView * [1]{ rtv };
	this->srv = new ID3D11ShaderResourceView * [1]{ srv };
	this->dsv = dsv;

	this->bufferCount = 1;
}

RenderTarget::RenderTarget(size_t bufferCount, size_t width, size_t height, bool createDepthBuffer) :
	bufferCount(bufferCount), width(width), height(height), createDepthBuffer(createDepthBuffer), dss(nullptr)
{

}

RenderTarget::~RenderTarget()
{
	Cleanup();
}

void RenderTarget::Initalize(ID3D11Device* device)
{
	rtv = new ID3D11RenderTargetView * [bufferCount];
	srv = new ID3D11ShaderResourceView * [bufferCount];

	HRESULT hr;

	/*
		VIEWPORT
	*/
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;


	if (this->createDepthBuffer)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDsc;
		ZeroMemory(&depthStencilStateDsc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDsc.DepthEnable = true;
		depthStencilStateDsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDsc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		depthStencilStateDsc.StencilEnable = false;
		depthStencilStateDsc.StencilReadMask = 0xFF;
		depthStencilStateDsc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.s
		depthStencilStateDsc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDsc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDsc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilStateDsc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilStateDsc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDsc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDsc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilStateDsc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT createDepthStencilResult = device->CreateDepthStencilState(&depthStencilStateDsc, &dss);
		assert(SUCCEEDED(createDepthStencilResult));

		//context->OMSetDepthStencilState(depthStencilState, 1);



		ID3D11Texture2D* depthTex;

		// DEPTH TEXTURE
		D3D11_TEXTURE2D_DESC depthTexDesc;
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;


		hr = device->CreateTexture2D(&depthTexDesc, 0, &depthTex);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Format = depthTexDesc.Format;
		dsvDesc.Flags = 0;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;


		//ZeroMemory(&dsv, sizeof(ID3D11DepthStencilView));

		hr = device->CreateDepthStencilView(depthTex, &dsvDesc, &dsv);
		assert(SUCCEEDED(hr));

		depthTex->Release();
		depthTex = 0;
	}

	/*
		RENDER TEXTURES
	*/
	ID3D11Texture2D** targetTextures = new ID3D11Texture2D * [bufferCount];
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (size_t i = 0; i < bufferCount; i++)
	{
		hr = device->CreateTexture2D(&textureDesc, nullptr, &targetTextures[i]);
		assert(SUCCEEDED(hr));
	}

	/*
		RENDER TARGET VIEWS
	*/
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (size_t i = 0; i < bufferCount; i++)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateRenderTargetView(targetTextures[i], &rtvDesc, &rtv[i]);
		assert(SUCCEEDED(hr));
	}

	/*
		SHADER RESOURCE VIEWS
	*/
	D3D11_SHADER_RESOURCE_VIEW_DESC shrDesc;
	ZeroMemory(&shrDesc, sizeof(shrDesc));

	shrDesc.Format = textureDesc.Format;
	shrDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shrDesc.Texture2D.MostDetailedMip = 0;
	shrDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	for (size_t i = 0; i < bufferCount; i++)
	{
		hr = device->CreateShaderResourceView(targetTextures[i], &shrDesc, &srv[i]);
		assert(SUCCEEDED(hr));
	}


	for (size_t i = 0; i < bufferCount; i++)
	{
		if (targetTextures[i])
		{
			targetTextures[i]->Release();
			targetTextures[i] = 0;
		}
	}
	delete[] targetTextures;
}

void RenderTarget::Bind(ID3D11DeviceContext* context)
{
	for (size_t i = 0; i < bufferCount; i++)
		context->PSSetShaderResources(i, 1, &srv[i]);
}

void RenderTarget::Unbind(ID3D11DeviceContext* context)
{
	for (size_t i = 0; i < bufferCount; i++)
	{
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		//ZeroMemory(&nullSRV, sizeof(nullSRV));

		context->PSSetShaderResources(i, 1, nullSRV);
	}
}

void RenderTarget::Cleanup()
{
	for (size_t i = 0; i < bufferCount; i++)
	{
		if (srv[i])
		{
			srv[i]->Release();
			srv[i] = 0;
		}

		if (rtv[i])
		{
			rtv[i]->Release();
			rtv[i] = 0;
		}
	}

	delete dsv;
	delete dss;

	delete[] srv;
	delete[] rtv;
}
