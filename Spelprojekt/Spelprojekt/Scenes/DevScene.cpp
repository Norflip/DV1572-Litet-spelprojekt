#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene(renderer, dx11, window)
{
	//----- GUI SHIET

	// Create timer and set to textobject
	gametimer.Start();
	gametimerText = new GUIText(dx11, "Timer", window.GetWidth() / 2.0f, 0);

	// HEALTH
	healthFrame = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 700.0f);
	healthbar = new GUISprite(dx11, "Sprites/Healthbar.png", 10.0f, 700.0f);
	healthbar->HealthBar(100.0f, 50.0f);

	actionbarLeft = new GUIActionbar(dx11, "Sprites/Actionbar.png", 325.0f, 700.0f);

	// Create GUI for Devscene
	GUI* gui = new GUI(dx11);

	// Add objects
	gui->AddGUIObject(gametimerText);
	gui->AddGUIObject(healthbar);
	gui->AddGUIObject(healthFrame);
	gui->AddGUIObject(actionbarLeft);
	
	// Set GUI
	renderer->SetGUI(gui);

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
	this->player = new Player(dev_monkey_mesh, new Material(defaultShader, dx11), controller, &test, gui, dx11);
	//player->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	//player->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_normal, PIXEL_TYPE::PIXEL);

	this->controller->SetFollow(&player->GetTransform(), { 0, 10.0f, -10.0f });
	AddObject(player);

	this->enemy = new Enemy(dev_monkey_mesh, new Material(defaultShader, dx11), &test, dx11);
	this->enemy->GetTransform().Translate(5, 5, 3);
	this->enemy->SetTarget(this->player);
	AddObject(this->enemy);

	// Testing fbx
	Object* chair = AssimpHandler::loadFbxObject("Models/CuteChair.fbx", dx11, defaultShader);
	chair->GetTransform().Translate(15, 5, 10);
	AddObject(chair);

	Projectile* testProj = new Projectile("Models/Coconut.fbx", &test, dx11, defaultShader, DirectX::XMVECTOR({ 0,5,0 }), DirectX::XMVECTOR({ 0,0/*MathHelper::PI/2*/,0 }));
	//testProj->GetTransform().Translate(0, 0, 0);
	AddObject(testProj);

<<<<<<< Updated upstream
	Object* glasse = AssimpHandler::loadFbxObject("Models/GlasseSmall.fbx", dx11, defaultShader);
	AddObject(glasse);
	Object* wagon = AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, defaultShader);
	wagon->GetTransform().Translate(5, 5, 30);
	AddObject(wagon);
=======
		
	this->coconutPickUp = AssimpHandler::loadFbxObject("Models/Coconut.fbx", dx11, defaultShader);
	coconutPickUp->GetTransform().Translate(10, 3, 15);
	AddObject(coconutPickUp);
>>>>>>> Stashed changes

	Object* palm = AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, defaultShader);
	AddObject(palm);

	Object* coconut = AssimpHandler::loadFbxObject("Models/Coconut.fbx", dx11, defaultShader);
	coconut->GetTransform().Translate(10, 5, 15);
	AddObject(coconut);

	// Testing animation
	Object* animation = AssimpHandler::loadFbxObject("Models/animation.fbx", dx11, defaultShader);
	AddObject(animation);
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
	if (coconutPickUp->GetWorldBounds().Overlaps(player->GetWorldBounds()))
	{
		//RemoveObject(coconutPickUp);
		Logger::Write(LOG_LEVEL::Info, "Inside nut");
	}
	else {
		Logger::Write(LOG_LEVEL::Info, "NOT Inside nut");
	}

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
