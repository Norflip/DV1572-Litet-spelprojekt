#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window) : Scene("DevScene", renderer, dx11, window)
{
	//----- GUI SHIET |  Set gui last |

	// Create timer and set to textobject
	gametimer.Start();
	gametimerText = new GUIText(dx11, "Timer", window.GetWidth() / 2.0f, 0);
	fpsText = new GUIText(dx11, "Fps", window.GetWidth() / 2.0f, 30);


	this->controller = new CameraController(GetSceneCamera(), window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights& lights = renderer->GetLights();
	lights.SetSunDirection({ 1, -1, 0 });
	lights.SetSunColor({ 0.98f, 0.96f, 0.73f, 1 });
	lights.SetSunIntensity(0.6f);

	//lights->AddPointLight({ -2, 0, 0 }, { 1.0f, 1.0f, 1.0f, 1 }, 50);
	//lights->AddPointLight({ -2, 0, 10 }, { 0.2f,0.2f, 0.2f, 1 }, 50);

}

DevScene::~DevScene()
{
	delete controller;
}

void DevScene::Load()
{	

	// HEALTH
	healthFrame = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 700.0f);
	actionbarLeft = new GUIActionbar(dx11, "Sprites/Actionbar.png", 325.0f, 700.0f);
	actionbarRight = new GUIActionbar(dx11, "Sprites/Actionbar.png", 400.0f, 700.0f);

	//--------------------------------
	// Create GUI for Devscene
	GUI* gui = new GUI(dx11);

	// Add objects
	gui->AddGUIObject(gametimerText);
	gui->AddGUIObject(fpsText);
	gui->AddGUIObject(healthFrame);
	gui->AddGUIObject(actionbarLeft);
	gui->AddGUIObject(actionbarRight);

	// Set GUI
	renderer->SetGUI(gui);

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

	test.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice());
	AddObject(new Object(test.GetMesh(), test_material));


	// ------ PLAYER
	this->player = new Player(dev_monkey_mesh, new Material(defaultShader, dx11), controller, &test, gui, dx11, static_cast<Scene*>(this));
	//player->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	//player->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_normal, PIXEL_TYPE::PIXEL);

	this->controller->SetFollow(&this->player->GetTransform(), { 0, 10.0f, -10.0f });
	AddObject(this->player);

	
	this->spawnObjects = new SpawnObjects(dx11, static_cast<Scene*>(this), &test, dev_monkey_mesh, new Material(defaultShader, dx11), this->player);
	AddObject(this->spawnObjects);
	/*this->enemy = new Enemy(dev_monkey_mesh, new Material(defaultShader, dx11), &test, dx11);
	this->enemy->GetTransform().Translate(5, 12, 15);
	this->enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
	this->enemy->SetTarget(this->player);
	AddObject(this->enemy);*/
	//this->player->SetEnemy(spawnObjects->GetEnemy());

	Shader* waterShader = new Shader();
	waterShader->LoadPixelShader(L"Shaders/Water_ps.hlsl", "main", dx11.GetDevice());
	waterShader->LoadVertexShader(L"Shaders/Water_vs.hlsl", "main", dx11.GetDevice());

	Mesh* waterPlane = ShittyOBJLoader::Load("Models/Plane1.obj", dx11.GetDevice());
	Object* water = new Object(waterPlane, new Material(waterShader, dx11));
	water->GetTransform().Translate({ 10, -10, 10 });
	AddObject(water);

		
	this->coconutPickUp = AssimpHandler::loadFbxObject("Models/Coconut.fbx", dx11, defaultShader);
	coconutPickUp->GetTransform().Translate(10, 2, 15);
	AddObject(coconutPickUp);

	//// Testing fbx
	//Object* chair = AssimpHandler::loadFbxObject("Models/CuteChair.fbx", dx11, defaultShader);
	//chair->GetTransform().Translate(15, 5, 10);
	//AddObject(chair);

	//Projectile* testProj = new Projectile("Models/Coconut.fbx", &test, dx11, defaultShader, DirectX::XMVECTOR({ 0,5,0 }), DirectX::XMVECTOR({ 0,0/*MathHelper::PI/2*/,0 }));
	////testProj->GetTransform().Translate(0, 0, 0);
	//AddObject(testProj);
	
	//Object* glasse = AssimpHandler::loadFbxObject("Models/GlasseSmall.fbx", dx11, defaultShader);
	//AddObject(glasse);
	//Object* wagon = AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, defaultShader);
	//wagon->GetTransform().Translate(5, 5, 30);
	//AddObject(wagon);

	//Object* palm = AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, defaultShader);
		//palm->GetTransform().Translate(5, 5, 30);
	//palm->GetTransform().Scale(0.5, 0.5, 0.5);	// new
	//AddObject(palm);

	//Object* coconut = AssimpHandler::loadFbxObject("Models/Coconut.fbx", dx11, defaultShader);
	//coconut->GetTransform().Translate(10, 5, 15);
	//AddObject(coconut);

	//// Testing animation
	//Object* animation = AssimpHandler::loadFbxObject("Models/animation.fbx", dx11, defaultShader);
	//AddObject(animation);
}

void DevScene::Unload()
{
	// @TODO
}

void DevScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);

	//FPS STUFF
	fpsTimer.Start();
	player->NutOnPlayer(coconutPickUp);

	gametimerText->SetString("Timer: " + std::to_string(static_cast<int>(std::floor(gametimer.GetMilisecondsElapsed() / 1000.0))));
	controller->Update(deltaTime);
	
	fpsTimer.Stop();
	fpsText->SetString("FPS: " + std::to_string( (int)(1/((fpsTimer.GetMicrosecondsElapsed()/1000000)))));
	fpsTimer.Restart();
}

Scene* DevScene::GetNextScene() const
{
	return nullptr;
}
