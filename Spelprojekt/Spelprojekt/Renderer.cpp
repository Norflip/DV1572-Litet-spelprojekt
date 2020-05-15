#include "Renderer.h"

Renderer::Renderer(size_t width, size_t height, Timer& timer, DX11Handler& dx11) : dx11(dx11), timer(timer), lights()
{
	this->gbufferRenderTarget = new RenderTarget(4, width, height, true);
	this->gbufferRenderTarget->Initalize(dx11.GetDevice());

	this->backbufferRenderTarget = dx11.GetBackbuffer();
	this->gui = nullptr;
	this->lightpass = new Shader();
	this->lightpass->LoadVertexShader(L"Shaders/ScreenQuad_vs.hlsl", "main", dx11.GetDevice());
	this->lightpass->LoadPixelShader(L"Shaders/DeferredLightPass_ps.hlsl", "main", dx11.GetDevice());

	screenQuad = MeshCreator::CreateScreenQuad(dx11.GetDevice());
	worldBuffer_ptr = dx11.CreateBuffer<WorldData>(cb_world);

	ssao.Initialize(width, height, &dx11);
	ssaoRandomTexture = Texture::CreateTexture("Textures/ssaoRandom.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP);

	lights.Initialize(dx11);
}

Renderer::~Renderer() {}

void Renderer::SetDeferredRenderTarget()
{
	this->gbufferRenderTarget->Unbind(dx11.GetContext());
	SetRenderTarget(gbufferRenderTarget);
}

void Renderer::SetRenderTarget(RenderTarget* renderTarget)
{
	this->currentRenderTarget = renderTarget;

	dx11.GetContext()->RSSetViewports(1, &currentRenderTarget->GetViewport());
	dx11.GetContext()->OMSetRenderTargets(currentRenderTarget->BufferCount(), currentRenderTarget->GetRenderTargetViews(), currentRenderTarget->GetDepthStencil());

	if (currentRenderTarget->GetDepthStencilState() != nullptr)
		dx11.GetContext()->OMSetDepthStencilState(currentRenderTarget->GetDepthStencilState(), 0);
}

void Renderer::ClearRenderTarget()
{
	for (size_t i = 0; i < currentRenderTarget->BufferCount(); i++)
		dx11.GetContext()->ClearRenderTargetView(currentRenderTarget->GetRenderTargetViews()[i], CLEAR_COLOR);

	dx11.GetContext()->ClearDepthStencilView(currentRenderTarget->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::DrawMesh(Mesh* mesh, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	// update the world buffer content
	cb_world.mvp = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(world, view), projection));
	cb_world.world = DirectX::XMMatrixTranspose(world);
	cb_world.time = static_cast<float>(timer.GetMilisecondsElapsed()) / 1000.0f;

	dx11.GetContext()->UpdateSubresource(worldBuffer_ptr, 0, 0, &cb_world, 0, 0);
	dx11.GetContext()->VSSetConstantBuffers(0, 1, &worldBuffer_ptr);

	DrawMesh(mesh);
}

void Renderer::DisplayFrame(Camera* camera)
{
	//Uppdate light constant buffer here
	lights.UpdateConstantBuffer(camera, dx11.GetContext());

	SetRenderTarget(backbufferRenderTarget);
	ClearRenderTarget();

	ssao.RenderPass(gbufferRenderTarget);


	// bind ssao_random texture
	ID3D11ShaderResourceView* srv = ssaoRandomTexture->GetSRV();
	ID3D11SamplerState* sampelr = ssaoRandomTexture->GetSampler();
	dx11.GetContext()->PSSetShaderResources(gbufferRenderTarget->BufferCount(), 1, &srv);
	dx11.GetContext()->PSSetSamplers(gbufferRenderTarget->BufferCount(), 1, &sampelr);

	gbufferRenderTarget->Bind(dx11.GetContext());
	lightpass->Bind(dx11.GetContext());


	DrawScreenQuad();

	// GUI PASS
	if (gui != nullptr)
		gui->DrawAll();

	dx11.GetSwapChain()->Present(vSync, 0);


	// unbinds ssao_random texture
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };
	ID3D11SamplerState* const ssrf[1] = { NULL };
	dx11.GetContext()->PSSetShaderResources(gbufferRenderTarget->BufferCount(), 1, pSRV);
	dx11.GetContext()->PSSetSamplers(gbufferRenderTarget->BufferCount(), 1, ssrf);

}

void Renderer::DrawScreenQuad()
{
	DrawMesh(screenQuad);
}

void Renderer::DrawMesh(Mesh* mesh)
{
	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;

	dx11.GetContext()->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	dx11.GetContext()->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	dx11.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dx11.GetContext()->DrawIndexed(mesh->indices.size(), 0, 0);
}