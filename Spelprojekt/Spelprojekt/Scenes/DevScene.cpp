#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer)
{
	camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	camera->UpdateView();

	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	Mesh* sphereMesh = ShittyOBJLoader::Load("Models/sphere.obj", dx11.GetDevice());
	Object* sphere = new Object(sphereMesh, new Material(defaultShader));

	sphere->GetTransform().Translate(0, 0, 4);
	objects.push_back(sphere);
}

DevScene::~DevScene()
{
}

void DevScene::Load()
{

}

void DevScene::Unload()
{

}

void DevScene::Update(const float& deltaTime)
{
	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();

	// loop objects
	for (auto i : objects)
	{
		renderer->ApplyMaterial(i->GetMaterial());
		i->Render(renderer, camera);
	}

	renderer->DisplayFrame();
}

void DevScene::FixedUpdate(const float& fixedDeltaTime)
{

}

Scene* DevScene::GetNextScene() const
{
	return nullptr;
}
