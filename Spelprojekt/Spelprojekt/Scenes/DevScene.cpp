#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer, window)
{
	camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	camera->UpdateView();

	Lights* lights = new Lights();
	lights->AddPointLight({ -2, 0, 0 }, { 0.9f,0.1f,0.1f,1 }, 0);
	lights->AddPointLight({ -2, 0, 10 }, { 0.1f,0.1f, 0.9f, 1 }, 0);

	renderer->SetLights(lights);



	// save the shaders somewhere, remember to clean it up
	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	// object = mesh + material
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
	Input* input = window.GetInput();

	// dess körs två gånger atm
	if (input->GetKeyDown('f'))
		Logger::Write("f down");

	if (input->GetKeyUp('f'))
		Logger::Write("f up");



	if (input->GetKeyDown('f'))
	{
		input->LockCursor(!input->IsCursorLocked());
	}
	else if (input->IsCursorLocked())
	{

		POINTS delta = input->GetMouseDelta();
		camera->GetTransform().Rotate(delta.y * 0.2f * deltaTime, delta.x * 0.2f * deltaTime, 0);

		// TESTING CAMERA MOVEMENT
		const float cameraSpeed = 2.0f;
		float forwardDelta = 0.0f;
		float rightDelta = 0.0f;

		if (input->GetKey('w')) forwardDelta += 1.0f;
		if (input->GetKey('a')) rightDelta -= 1.0f;
		if (input->GetKey('s')) forwardDelta -= 1.0f;
		if (input->GetKey('d')) rightDelta += 1.0f;

		float len = sqrtf(forwardDelta * forwardDelta + rightDelta * rightDelta);

		if (len != 0.0f)
		{
			DirectX::XMVECTOR forward = camera->GetTransform().TransformDirection(Transform::default_forward);
			DirectX::XMVECTOR right = camera->GetTransform().TransformDirection(Transform::default_right);

			DirectX::XMVECTOR translation = DirectX::XMVectorScale(forward, (forwardDelta / len));
			translation = DirectX::XMVectorAdd(translation, DirectX::XMVectorScale(right, (rightDelta / len)));
			translation = DirectX::XMVectorScale(DirectX::XMVector3Normalize(translation), cameraSpeed * deltaTime);

			camera->GetTransform().Translate(translation);
		}

		camera->UpdateView();
	}

	//--------------------------------


	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();

	// loop objects in scene
	for (auto i : objects)
	{
		if (camera->GetFrustum().AABBInFrustrum(i->GetWorldBounds()))
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
