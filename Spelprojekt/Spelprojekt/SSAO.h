#pragma once
#include "DX11Handler.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderTarget.h"

class Renderer;

class SSAO
{
public:
	SSAO(size_t width, size_t height);
	virtual ~SSAO();

	void Initialize(DX11Handler* dx11);
	void RenderPass(Renderer* renderer, RenderTarget* gbuffer);
	ID3D11ShaderResourceView* GetOutputSRV() const;

private:
	RenderTarget* renderTarget;
	Texture* randomTexture;
	Shader* shader;

private:
	size_t width, height;
	DX11Handler* dx11;
};