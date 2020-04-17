#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer, dx11, window)
{
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->controller = new CameraController(camera, window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights* lights = new Lights();
	lights->AddPointLight({ -2, 0, 0 }, { 1.0f, 1.0f, 1.0f, 1 }, 50);
	lights->AddPointLight({ -2, 0, 10 }, { 0.2f,0.2f, 0.2f, 1 }, 50);
	lights->SetSunDirection({ 1,-1, 1 });
	lights->SetSunColor({ 1,1,1,1 });
	lights->SetSunIntensity(0.2f);
	renderer->SetLights(lights);

	// save the shaders somewhere, remember to clean it up
	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	// object = mesh + material

	Mesh* dev_monkey_mesh = ShittyOBJLoader::Load("Models/monkey.obj", dx11.GetDevice());

	Object* sphere = new Object(dev_monkey_mesh, new Material(defaultShader, dx11));
	Texture* monkey_texture = Texture::CreateTexture("Textures/rocks.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	Texture* monkey_normal = Texture::CreateTexture("Textures/rocks_normal.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	sphere->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	sphere->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	sphere->GetTransform().Translate(0, 0, 6);
	objects.push_back(sphere);

	// ------- TERRAIN
	Material* test_material = new Material(defaultShader, dx11);
	Texture* m_texture = Texture::CreateTexture("rocks.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	test_material->SetTexture(ALBEDO_MATERIAL_TYPE, m_texture, PIXEL_TYPE::PIXEL);

	Mesh* terrain = new Mesh();
	test.generateFromHeightMap("heightmap.png", terrain, dx11.GetDevice());
	objects.push_back(new Object(terrain, test_material));

	// ------ PLAYER
	player = new Player(dev_monkey_mesh, new Material(defaultShader, dx11), window.GetInput(), &test);
	player->GetPlayerObject()->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	player->GetPlayerObject()->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_normal, PIXEL_TYPE::PIXEL);

	controller->SetFollow(&player->GetTransform(), { 0, 10.0f, -10.0f });
	objects.push_back(player);


	//----- GUI SHIET
	gametimer.Start();
	gametimerText = new GUITextObject(dx11, "Test", window.GetWidth() / 2.0f, 0);

	GUI* gui = new GUI(dx11);
	gui->AddGUIObject(gametimerText);
	renderer->SetGUI(gui);
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
	gametimerText->SetString("Timer: " + std::to_string(static_cast<int>(std::floor(gametimer.GetMilisecondsElapsed() / 1000.0))));

	Input* input = window.GetInput();

	if (input->GetKeyDown(DEBUG_CAMERA_KEY))
	{
		input->LockCursor(!input->IsCursorLocked());
		bool following = controller->GetState() == CameraController::State::Follow;
		controller->SetState(following ? CameraController::State::Free : CameraController::State::Follow);
	}

	controller->Update(deltaTime);

	for (auto i : objects)
	{
		if (i->IsEnabled())
		{
			i->Update(deltaTime);
		}
	}

	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();

	DirectX::XMMATRIX view = camera->GetView();
	DirectX::XMMATRIX projection = camera->GetProjection();

	// loop objects in scene
	for (auto i : objects)
	{
		if (i->IsEnabled() && camera->IsBoundsInView(i->GetWorldBounds()))
		{
			i->GetMaterial()->Bind(dx11.GetContext());
			i->Render(renderer, view, projection);
			i->GetMaterial()->Unbind(dx11.GetContext()); 			//unbind material after the use
		}
	}

	renderer->DisplayFrame(camera->GetTransform().GetPosition());
}

void DevScene::FixedUpdate(const float& fixedDeltaTime)
{
	for (auto i : objects)
	{
		i->FixedUpdate(fixedDeltaTime);
	}
}

Scene* DevScene::GetNextScene() const
{
	return nullptr;
}
