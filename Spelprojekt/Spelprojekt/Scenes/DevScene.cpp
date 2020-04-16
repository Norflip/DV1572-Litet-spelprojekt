#include "DevScene.h"


DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer, window)
{
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->controller = new CameraController(camera, window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights* lights = new Lights();
	lights->AddPointLight({ -2, 0, 0 }, { 0.9f,0.1f,0.1f,1 }, 50);
	lights->AddPointLight({ -2, 0, 10 }, { 0.1f,0.1f, 0.9f, 1 }, 50);

	lights->SetSunDirection({ 1,-1, 1 });
	lights->SetSunColor({ 1,1,1,1 });
	lights->SetSunIntensity(0.2f);

	renderer->SetLights(lights);

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

	controller->SetFollow(&sphere->GetTransform(), { 0, 10.0f, -10.0f });

	TerrainGenerator test;
	Mesh* terrain = new Mesh();
	test.generateFromHeightMap("heightmap.png", terrain, dx11.GetDevice());
	Object* terrainObject = new Object(terrain, new Material(defaultShader));
	sphere->GetTransform().Translate(0, 0, 0);
	terrainObject->GetTransform().Translate(2, 2, 22);
	//objects.push_back(sphere);
	objects.push_back(terrainObject);
}

DevScene::~DevScene()
{
	delete controller;
	delete camera;
}

void DevScene::Load()
{

}

void DevScene::Unload()
{

}

void DevScene::Update(const float& deltaTime)
{
	Input* input = window.GetInput();

	if (input->GetKeyDown(DEBUG_CAMERA_KEY))
	{
		input->LockCursor(!input->IsCursorLocked());
		bool following = controller->GetState() == CameraController::State::Follow;
		controller->SetState(following ? CameraController::State::Free : CameraController::State::Follow);
	}

	controller->Update(deltaTime);

	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();

	// loop objects in scene
	for (auto i : objects)
	{
		if (camera->InView(i->GetWorldBounds()))
		{
			renderer->ApplyMaterial(i->GetMaterial());
			i->Render(renderer, camera);
		}
	}

	renderer->DisplayFrame(camera);
}

void DevScene::FixedUpdate(const float& fixedDeltaTime)
{

}

Scene* DevScene::GetNextScene() const
{
	return nullptr;
}
