#include "DevScene.h"


DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer, window)
{
	camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	camera->UpdateView();

	// save the shaders somewhere, remember to clean it up
	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	// object = mesh + material
	// Mesh* terrainMesh = 
	Mesh* sphereMesh = ShittyOBJLoader::Load("Models/monkey.obj", dx11.GetDevice());
	Object* sphere = new Object(sphereMesh, new Material(defaultShader));

	sphere->GetTransform().Translate(0, 0, 6);
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
	
	// TESTING CAMERA MOVEMENT
	const float cameraSpeed = 0.2f;
	Input* input = window.GetInput();

	float forward = 0.0f;
	if (input->GetKey('w')) forward += 1.0f;
	if (input->GetKey('s')) forward -= 1.0f;

	float right = 0.0f;
	if (input->GetKey('a')) right -= 1.0f;
	if (input->GetKey('d')) right += 1.0f;

	//Logger::Write(std::to_string(rg));

	forward *= cameraSpeed * deltaTime;
	right *= cameraSpeed * deltaTime;
	camera->GetTransform().Translate(right, 0, forward);
	camera->UpdateView();
	//--------------------------------


	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();

	// loop objects in scene
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
