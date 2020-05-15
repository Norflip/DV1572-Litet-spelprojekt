#include "SSAO.h"

SSAO::SSAO(size_t width, size_t height, DX11Handler* dx11, Mesh* screenquad) : width(width), height(height), dx11(dx11), screenquad(screenquad)
{
	this->randomTexture = Texture::CreateTexture("Textures/ssaoRandom.jpg", *dx11, true, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);
	this->shader = new Shader();
	this->shader->LoadVertexShader(L"Shaders/ScreenQuad_vs.hlsl", "main", dx11->GetDevice());
	this->shader->LoadPixelShader(L"Shaders/SSAO_ps.hlsl", "main", dx11->GetDevice());
}

SSAO::~SSAO()
{
	delete this->shader;
	delete this->randomTexture;
}

void SSAO::RenderPass(RenderTarget* gbuffer)
{
	// bind ssao_random texture
	ID3D11ShaderResourceView* srv = randomTexture->GetSRV();
	ID3D11SamplerState* sampelr = randomTexture->GetSampler();
	dx11->GetContext()->PSSetShaderResources(gbuffer->BufferCount(), 1, &srv);
	dx11->GetContext()->PSSetSamplers(gbuffer->BufferCount(), 1, &sampelr);

	// bind two buffers
	// bind SSAO shader
	// bind to output





	// return shaderResourceView from the renderTarget

	// unbinds ssao_random texture
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };
	ID3D11SamplerState* const ssrf[1] = { NULL };
	dx11->GetContext()->PSSetShaderResources(gbuffer->BufferCount(), 1, pSRV);
	dx11->GetContext()->PSSetSamplers(gbuffer->BufferCount(), 1, ssrf);
}