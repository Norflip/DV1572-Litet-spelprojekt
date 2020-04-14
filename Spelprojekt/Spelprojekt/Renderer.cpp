#include "Renderer.h"

Renderer::Renderer(size_t width, size_t height, DX11Handler& dx11) : dx11(dx11), lights(nullptr)
{
	this->gbufferRenderTarget = new RenderTarget(3, width, height, true);
	this->gbufferRenderTarget->Initalize(dx11.GetDevice());

	this->backbufferRenderTarget = dx11.GetBackbuffer();

	this->lightpass = new Shader();
	this->lightpass->LoadPixelShader(L"Shaders/DeferredLightPass_ps.hlsl", "main", dx11.GetDevice());
	this->lightpass->LoadVertexShader(L"Shaders/DeferredLightPass_vs.hlsl", "main", dx11.GetDevice());

	screenQuad = MeshCreator::CreateScreenQuad(dx11.GetDevice());

	// -------------- WORLD BUFFER
	static_assert(sizeof(cb_world) % 16 == 0, "wrong size");

	HRESULT hr;
	D3D11_BUFFER_DESC bufferDescription;
	D3D11_SUBRESOURCE_DATA subresourceData;
	ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	ZeroMemory(&worldBuffer_ptr, sizeof(worldBuffer_ptr));
	
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = 0;

	// WORLD BUFFER
	bufferDescription.ByteWidth = sizeof(cb_world);
	subresourceData.pSysMem = &cb_world;
	hr = dx11.GetDevice()->CreateBuffer(&bufferDescription, &subresourceData, &worldBuffer_ptr);
	assert(SUCCEEDED(hr));
}

Renderer::~Renderer()
{
}

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
}

void Renderer::ClearRenderTarget()
{
	for (size_t i = 0; i < currentRenderTarget->BufferCount(); i++)
		dx11.GetContext()->ClearRenderTargetView(currentRenderTarget->GetRenderTargetViews()[i], CLEAR_COLOR);

	dx11.GetContext()->ClearDepthStencilView(currentRenderTarget->GetDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void Renderer::DrawMesh(Mesh* mesh, DirectX::XMMATRIX world, Camera* camera)
{
	// update the world buffer content
	cb_world.mvp = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(world, camera->GetView()), camera->GetProjection()));
	cb_world.world = DirectX::XMMatrixTranspose(world);

	dx11.GetContext()->UpdateSubresource(worldBuffer_ptr, 0, 0, &cb_world, 0, 0);
	dx11.GetContext()->VSSetConstantBuffers(0, 1, &worldBuffer_ptr);

	DrawMesh(mesh);
}

void Renderer::SetLights(Lights* lights)
{
	this->lights = lights;
	this->lights->Initialize(dx11.GetDevice());
}

void Renderer::DisplayFrame(Camera* camera)
{
	//Uppdate light constant buffer here
	if (lights != nullptr)
	{
		DirectX::XMFLOAT3 eyePosition;
		DirectX::XMStoreFloat3(&eyePosition, camera->GetTransform().GetPosition());
		lights->UpdateConstantBuffer(eyePosition, dx11.GetContext());
	}
	
	SetRenderTarget(backbufferRenderTarget);
	ClearRenderTarget();

	gbufferRenderTarget->Bind(dx11.GetContext());
	lightpass->Apply(dx11.GetContext());

	DrawMesh(screenQuad);
	dx11.GetSwapChain()->Present(1, 0);
}

void Renderer::ApplyMaterial(Material* material)
{
	material->Apply(dx11.GetContext());
	// update material constant buffer
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
