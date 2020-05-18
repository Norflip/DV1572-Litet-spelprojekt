#include "SSAO.h"
#include "Renderer.h"

SSAO::SSAO(size_t width, size_t height) : width(width), height(height), dx11(nullptr), shader(nullptr), randomTexture(nullptr)
{
	
}

SSAO::~SSAO()
{
	delete this->shader;
	delete this->randomTexture;
}

void SSAO::Initialize(DX11Handler* dx11)
{
	this->dx11 = dx11;
	this->randomTexture = Texture::CreateTexture("Textures/ssaoRandom.jpg", *dx11, true, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

	this->renderTarget = new RenderTarget(1, width, height, false);
	this->renderTarget->Initalize(dx11->GetDevice());

	this->shader = new Shader();
	this->shader->LoadVertexShader(L"Shaders/ScreenQuad_vs.hlsl", "main", dx11->GetDevice());
	this->shader->LoadPixelShader(L"Shaders/SSAO_ps.hlsl", "main", dx11->GetDevice());
}

void SSAO::RenderPass(Renderer* renderer, RenderTarget* gBuffer)
{
	
	//ID3D11ShaderResourceView* srvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
	//dx11->GetContext()->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, srvs);


	// unbind gbuffer?
	//gBuffer->Unbind(dx11->GetContext());

	// bind ssao_random texture + normal and position buffer
	/*ID3D11ShaderResourceView* srv = randomTexture->GetSRV();
	ID3D11SamplerState* sampler = randomTexture->GetSampler();
	ID3D11ShaderResourceView** buffers = gBuffer->GetShaderResourceViews();

	dx11->GetContext()->PSSetShaderResources(0, 1, &buffers[2]);
	dx11->GetContext()->PSSetShaderResources(1, 1, &buffers[3]);
	dx11->GetContext()->PSSetShaderResources(2, 1, &srv);
	dx11->GetContext()->PSSetSamplers(0, 1, &sampler);*/



	renderer->SetRenderTarget(renderTarget);
	renderer->ClearRenderTarget();


	// bind SSAO shader
	shader->Bind(dx11->GetContext());


	renderer->DrawScreenQuad();
	renderTarget->Unbind(dx11->GetContext());
	
	// bind to output

	// return shaderResourceView from the renderTarget


	// unbinds ssao_random texture

	//ID3D11ShaderResourceView* pSRV[1] = { nullptr };
	//ID3D11SamplerState* ssrf[1] = { nullptr };

	//dx11->GetContext()->PSSetShaderResources(0, 1, pSRV);
	//dx11->GetContext()->PSSetShaderResources(1, 1, pSRV);
	//dx11->GetContext()->PSSetShaderResources(2, 1, pSRV);
	//dx11->GetContext()->PSSetSamplers(0, 1, ssrf);
}

ID3D11ShaderResourceView* SSAO::GetOutputSRV() const
{
	return this->renderTarget->GetShaderResourceViews()[0];
}
