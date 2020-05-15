#pragma once
#include "DX11Handler.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

class SSAO
{
public:
	SSAO();
	virtual ~SSAO();

	void Initialize(size_t width, size_t height, DX11Handler* dx11);
	void RenderPass(RenderTarget* gbuffer);

private:
	RenderTarget* renderTarget;
	Texture* randomTexture;
	Shader* shader;

private:
	size_t width, height;
	DX11Handler* dx11;
};