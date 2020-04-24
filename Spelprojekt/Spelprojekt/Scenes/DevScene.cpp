#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer, dx11, window)
{
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->controller = new CameraController(camera, window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights* lights = new Lights();
	//lights->AddPointLight({ -2, 0, 0 }, { 1.0f, 1.0f, 1.0f, 1 }, 50);
	//lights->AddPointLight({ -2, 0, 10 }, { 0.2f,0.2f, 0.2f, 1 }, 50);

	lights->SetSunDirection({ 1, -1, 0 });
	lights->SetSunColor({ 1,0,0,1 });
	lights->SetSunIntensity(0.3f);

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
	sphere->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_normal, PIXEL_TYPE::PIXEL);

	sphere->GetTransform().Translate(0, 3, 6);
	AddObject(sphere);

	// ------- TERRAIN
	Material* test_material = new Material(defaultShader, dx11);
	Texture* m_texture = Texture::CreateTexture("Textures/rocks.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	test_material->SetTexture(ALBEDO_MATERIAL_TYPE, m_texture, PIXEL_TYPE::PIXEL);
	test_material->GetMaterialData().hasNormalTexture = false;

	Mesh* terrain = new Mesh();
	test.generateFromHeightMap("Textures/heightmap.png", terrain, dx11.GetDevice());
	AddObject(new Object(terrain, test_material));

	// ------ PLAYER
	player = new Player("Models/GlasseSmall.fbx", controller, &test, dx11, defaultShader);
	player->GetTransform().SetRotation({ 0, 0, 0 });
	controller->SetFollow(&player->GetTransform(), { 0, 10.0f, -10.0f });
	AddObject(player);

	//----- GUI SHIET

	// Create timer and set to textobject
	gametimer.Start();
	gametimerText = new GUITextObject(dx11, "Test", window.GetWidth() / 2.0f, 0);
	
	// New
	m_HealthFrame = new GUISpriteObject(dx11, 10.0f, 700.0f);
	m_HealthFrame->SetWICSprite(dx11, "Sprites/Frame.png");	

	m_Healthbar = new GUISpriteObject(dx11, 10.0f, 700.0f);
	m_Healthbar->SetWICSprite(dx11, "Sprites/Healthbar.png");
	m_Healthbar->HealthBar(100.0f, 100.0f);

	// Create GUI for Devscene
	GUI* gui = new GUI(dx11);

	// Add objects
	gui->AddGUIObject(gametimerText);
	gui->AddGUIObject(m_Healthbar);
	gui->AddGUIObject(m_HealthFrame);	

	// Set GUI
	renderer->SetGUI(gui);


	// Testing fbx load using assimp
	/*Object* cube = AssimpHandler::loadFbxObject("Models/testCube2.fbx", dx11.GetDevice(), dx11, defaultShader);
	cube->GetTransform().Translate(15, 5, 10);
	objects.push_back(cube);*/

	// Testing fbx
	Object* chair = AssimpHandler::loadFbxObject("Models/SunChair.fbx", dx11, defaultShader);
	chair->GetTransform().Translate(15, 5, 10);
	AddObject(chair);


	//Projectile* testProj = new Projectile("Models/monkey.obj", &test, dx11, defaultShader, DirectX::XMVECTOR({ 0,0,0 }), DirectX::XMVECTOR({ 0,0,0 }));
	//testProj->GetTransform().Translate(15, 5, 10);
	//AddObject(testProj);

	Object* glasse = AssimpHandler::loadFbxObject("Models/GlasseSmall.fbx", dx11, defaultShader);
	AddObject(glasse);

	Object* palm = AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, defaultShader);
	palm->GetTransform().Translate(5, 5, 30);
	AddObject(palm);
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

	for (auto i : allObjects)
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

	for (auto shader : sortedObjects)
	{
		shader.first->Bind(dx11.GetContext());

		for (auto material : shader.second)
		{
			material.first->Bind(dx11.GetContext());

			for (auto object : material.second)
				object->Render(renderer, view, projection);

			material.first->Unbind(dx11.GetContext());
		}
	}

	renderer->DisplayFrame(camera->GetTransform().GetPosition());
}

void DevScene::FixedUpdate(const float& fixedDeltaTime)
{
	for (auto i : allObjects)
	{
		i->FixedUpdate(fixedDeltaTime);
	}
}

Scene* DevScene::GetNextScene() const
{
	return nullptr;
}
