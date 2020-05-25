#include "IntroScene.h"



IntroScene::IntroScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, bool& exitGame,  Gamemanager* gamemanager) : Scene("IntroScene", renderer, dx11, window), scenes(scenes), exitGame(exitGame)
{
	this->camera = new Camera(90.0f, window.GetWidth(), window.GetHeight());
	this->controller = new CameraController(camera, window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);
	this->nextScene = nullptr;
	Lights& lights = renderer->GetLights();	
	lights.SetSunDirection({ 1, -2, 3 });
	lights.SetSunColor({ 0.98f, 0.96f, 0.73f, 1 });
	lights.SetSunIntensity(0.9f);
		
	// Gamemanager
	this->gamemanager = gamemanager;	
	gamemanager->GetSoundeffectHandler()->SetGlobalVolume(gamemanager->GetCurrentSoundVolume());
	gamemanager->GetMusicHandler()->LoadSound("Monster", "SoundEffects/MonstersInc.wav");
	gamemanager->GetMusicHandler()->SetGlobalVolume(gamemanager->GetCurrentMusicVolume());	
}

IntroScene::~IntroScene()
{
	delete controller;
}

void IntroScene::Load()
{	
	// Set music volume from beginning		
	gamemanager->GetMusicHandler()->PlaySound("Monster", gamemanager->GetCurrentMusicVolume());

	GUI* gui = new GUI(dx11);
	introGUI = new IntroGUI(gui, dx11, controller, this, gamemanager);
	renderer->SetGUI(gui);
		
	Object* glasse = new Object(ObjectLayer::Enviroment, resources.GetModel("playerModel"));
	glasse->GetTransform().Translate(0, 0.65+9, -3);
	glasse->GetTransform().Rotate(0, -0.6, 0);
	entities->InsertObject(glasse);
	

	Object* wagon = new Object(ObjectLayer::Enviroment, resources.GetModel("wagonModel"));
	wagon->GetTransform().Translate(7, -1.75+10, 1);
	wagon->GetTransform().Rotate(0, 0.1, 0);
	wagon->GetTransform().Scale(0.5f, 0.5f, 0.5f);
	entities->InsertObject(wagon);
	controller->SetFollow(&glasse->GetTransform(), { 0, 1, 0 });
	

	//// ------- BACKGROUND

	Object* background = new Object(ObjectLayer::Enviroment, resources.GetModel("backgroundPlane"));
	background->GetTransform().Translate(5, 19, 15);
	background->GetTransform().Rotate(-1.5, 0, 0);
	background->GetTransform().Scale(100, 1, 24);
	//backGround->GetMaterial()->GetTexture(0)->SetSampler(dx11.GetDevice());
	entities->InsertObject(background);

	Terrain ground;
	ground.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice(), false);
	
	Object* terrainObject = new Object(ObjectLayer::None, ground.GetMesh(), resources.GetResource<Material>("terrainMaterial"));
	terrainObject->GetTransform().SetPosition({ -100, -2+8.5f, -10 });
	entities->InsertObject(terrainObject);
}

void IntroScene::Unload()
{
	auto allEntities = entities->AllEntities();
	for (auto i : allEntities)
	{
		delete i;
	}

	entities->Clear();
}

void IntroScene::LoadResources()
{

	Shader* toonShader = new Shader();
	toonShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
	toonShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

	// ------- TERRAIN
	Shader* terrainShader = new Shader();
	terrainShader->LoadPixelShader(L"Shaders/Terrain_ps.hlsl", "main", dx11.GetDevice());
	terrainShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	resources.AddResource("toonShader", toonShader);
	resources.AddResource("terrainShader", terrainShader);

	/*
		TEXTURES
	*/
	Texture* grass_texture = Texture::CreateTexture("Textures/Grass_ColorTest.png", dx11);
	Texture* grass_normal = Texture::CreateTexture("Textures/Grass_Normal.png", dx11);
	Texture* sand_texture = Texture::CreateTexture("Textures/Sand_Color_Test.png", dx11);
	Texture* sand_normal = Texture::CreateTexture("Textures/Sand_Normal_2.png", dx11);

	resources.AddResource("grassTexture", grass_texture);
	resources.AddResource("grassNormal", grass_normal);
	resources.AddResource("sandTexture", sand_texture);
	resources.AddResource("sandNormal", sand_normal);

	/*
		MATERIAL
	*/

	Material* terrainMat = new Material(terrainShader, dx11);
	terrainMat->SetTexture(0, grass_texture, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetTexture(1, sand_texture, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetTexture(2, grass_normal, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetTexture(3, sand_normal, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetSampler(0, dx11.GetDefaultSampler(), SHADER_BIND_TYPE::PIXEL);
	terrainMat->GetMaterialData().hasNormalTexture = true;

	resources.AddResource<Material>("terrainMaterial", terrainMat);

	/*
		MODELS
	*/

	resources.AddModel("playerModel", AssimpHandler::loadFbxObject("Models/Glasse_intro_Pose.fbx", dx11, toonShader));
	resources.AddModel("wagonModel", AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, toonShader));
	resources.AddModel("backgroundPlane", AssimpHandler::loadFbxObject("Models/Background_Plane.fbx", dx11, toonShader));
}

void IntroScene::Update(const float& deltaTime)
{
	this->cameraFocusPosition = { 0,0,5 };

	Scene::Update(deltaTime);
	controller->Update(deltaTime);
	introGUI->Update();
}

Scene* IntroScene::GetNextScene() const
{
	return nextScene;
}


void IntroScene::setNextScene()
{
	Input* input = this->window.GetInput();

	// Change scene logic
	for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes[i]->GetName() == "DevScene")
			nextScene = scenes[i];
	}
}
