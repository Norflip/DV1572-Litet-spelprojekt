#include "Renderer.h"

Renderer::Renderer(size_t width, size_t height, Timer& timer, DX11Handler& dx11) : dx11(dx11), timer(timer), lights()
{
	this->gbufferRenderTarget = new RenderTarget(4, width, height, true);
	this->gbufferRenderTarget->Initalize(dx11.GetDevice());

	this->backbufferRenderTarget = dx11.GetBackbuffer();
	this->gui = nullptr;
	this->lightpass = new Shader();
	this->lightpass->LoadPixelShader(L"Shaders/DeferredLightPass_ps.hlsl", "main", dx11.GetDevice());
	this->lightpass->LoadVertexShader(L"Shaders/DeferredLightPass_vs.hlsl", "main", dx11.GetDevice());

	screenQuad = MeshCreator::CreateScreenQuad(dx11.GetDevice());
	worldBuffer_ptr = dx11.CreateBuffer<WorldData>(cb_world);

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
	cb_world.viewproj = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(view, projection));
	cb_world.world = DirectX::XMMatrixTranspose(world);
	cb_world.time = static_cast<float>(timer.GetMilisecondsElapsed()) / 1000.0f;

	for (int i = 0; i < mesh->boneTransforms.size(); i++)
	{
		/*cb_world.boneTransforms[0][0] = mesh->boneTransforms[i].a1;
		cb_world.boneTransforms[0][1] = mesh->boneTransforms[i].b1;
		cb_world.boneTransforms[0][2] = mesh->boneTransforms[i].c1;
		cb_world.boneTransforms[0][3] = mesh->boneTransforms[i].d1;

		cb_world.boneTransforms[1][0] = mesh->boneTransforms[i].a2;
		cb_world.boneTransforms[1][1] = mesh->boneTransforms[i].b2;
		cb_world.boneTransforms[1][2] = mesh->boneTransforms[i].c2;
		cb_world.boneTransforms[1][3] = mesh->boneTransforms[i].d2;

		cb_world.boneTransforms[2][0] = mesh->boneTransforms[i].a3;
		cb_world.boneTransforms[2][1] = mesh->boneTransforms[i].b3;
		cb_world.boneTransforms[2][2] = mesh->boneTransforms[i].c3;
		cb_world.boneTransforms[2][3] = mesh->boneTransforms[i].d3;

		cb_world.boneTransforms[3][0] = mesh->boneTransforms[i].a4;
		cb_world.boneTransforms[3][1] = mesh->boneTransforms[i].b4;
		cb_world.boneTransforms[3][2] = mesh->boneTransforms[i].c4;
		cb_world.boneTransforms[3][3] = mesh->boneTransforms[i].d4;*/

		//cb_world.boneTransforms[i] = DirectX::XMMatrixIdentity();
		//cb_world.boneTransforms[i] = DirectX::XMMatrixTranspose(mesh->boneTransforms[i]);
		cb_world.boneTransforms[i] = mesh->boneTransforms[i];

	}


	dx11.GetContext()->UpdateSubresource(worldBuffer_ptr, 0, 0, &cb_world, 0, 0);
	dx11.GetContext()->VSSetConstantBuffers(0, 1, &worldBuffer_ptr);

	DrawMesh(mesh);
}

void Renderer::DisplayFrame(DirectX::XMVECTOR eye)
{
	//Uppdate light constant buffer here
	DirectX::XMFLOAT3 eyePosition;
	DirectX::XMStoreFloat3(&eyePosition, eye);
	lights.UpdateConstantBuffer(eyePosition, dx11.GetContext());

	SetRenderTarget(backbufferRenderTarget);
	ClearRenderTarget();

	gbufferRenderTarget->Bind(dx11.GetContext());
	lightpass->Bind(dx11.GetContext());

	DrawMesh(screenQuad);

	if (gui != nullptr)
		gui->DrawAll();

	dx11.GetSwapChain()->Present(vSync, 0);
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