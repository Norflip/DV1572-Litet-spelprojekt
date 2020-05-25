#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, Gamemanager* gamemanager) : Scene("DevScene", renderer, dx11, window), scenes(scenes), gamemanager(gamemanager) // tabort soundeffect
{
	//----- GUI SHIET |  Set gui last |

	gametimerText = new GUIText(dx11, "Time until extraction", window.GetWidth() / 2.0f - 150.0f, 0);
	fpsText = new GUIText(dx11, "Fps", window.GetWidth() / 2.0f - 100.0f, 30);
	totalScore = new GUIText(dx11, "Score", 210.0f, 5.0f);
	totalScore->SetFontSize({ 3.0f, 3.0f });
	totalScore->SetFontColor({ 1,0,0,1 });

	totalEnemies = new GUIText(dx11, "Enemiesleft", 250, 70);
	totalEnemies->SetFontSize({ 2.0f, 2.0f });
	totalEnemies->SetFontColor({ 0.5,1,0,1 });

	this->controller = new CameraController(GetSceneCamera(), window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights& lights = renderer->GetLights();
	lights.SetSunDirection({ 0, -1, 0 });
	lights.SetSunColor({ 0.98f, 0.96f, 0.73f, 1 });
	lights.SetSunIntensity(0.6f);

	//lights->AddPointLight({ -2, 0, 0 }, { 1.0f, 1.0f, 1.0f, 1 }, 50);
	//lights->AddPointLight({ -2, 0, 10 }, { 0.2f,0.2f, 0.2f, 1 }, 50);	

	this->spawner = new SpawnObjects(entities, &terrain, gamemanager, dx11);
	this->spawner->SetMaxEnemies(gamemanager->GetTotalEnemies());
	//this->spawner->SetMaxEnemies(1);

}

DevScene::~DevScene()
{
	delete controller;
}

void DevScene::Load()
{
	// SET TOTAL ENEMIES AND TOTAL TIME TO EXTRACTION
	this->timeUntilEnd = 10.0f; // gamemanager->GetTimer();		// get time from gamemanager

	Timer testSpeed;
	testSpeed.Start();

	// HEALTH
	healthFrame = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 650.0f);
	actionbarLeft = new GUIActionbar(dx11, "Sprites/Actionbar.png", 325.0f, 650.0f);
	actionbarRight = new GUIActionbar(dx11, "Sprites/Actionbar.png", 400.0f, 650.0f);
	score = new GUISprite(dx11, "Sprites/score.png", 10.0f, 19.0f);
	enemies = new GUISprite(dx11, "Sprites/enemiesleft.png", 10, 80);

	//--------------------------------
	// Create GUI for Devscene
	GUI* gui = new GUI(dx11);

	// Exit Wagon
	Object* wagon = new Object(ObjectLayer::Enviroment, resources.GetModel("wagonModel"));
	entities->InsertObject(wagon);
	spawner->PlaceWagon(wagon);






	// GROUNDH MESH
	terrain.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice(), false);
	Object* terrainObject = new Object(ObjectLayer::None, terrain.GetMesh(), resources.GetResource<Material>("terrainMaterial"));
	entities->InsertObject(terrainObject);


	// ------- water shader
	waterTerrain.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice(), true);

	Object* water = new Object(ObjectLayer::None, waterTerrain.GetMesh(), resources.GetResource<Material>("waterMaterial"));
	water->GetTransform().Translate({ 0,5,0, });
	water->GetTransform().SetRotation({ 0,0,0 });
	entities->InsertObject(water);

	water->isWater = true;



	// ------ PLAYER
	AssimpHandler::AssimpData playerModel = *AssimpHandler::loadFbxObject("Animations/Glasse_Idle.fbx", dx11, resources.GetResource<Shader>("animationShader"));
	this->player = new Player(playerModel, controller, spawner, &terrain, gui, gamemanager, wagon, dx11, this);

	this->player->GetTransform().SetPosition({ 55, 4, 55 });
	this->player->GetTransform().Scale(2.0, 2.0, 2.0);
	this->player->SetLayer(ObjectLayer::Player);
	this->controller->SetFollow(&this->player->GetTransform(), { 0, 10.0f, -10.0f });
	entities->InsertObject(this->player);

	// ANIMATION SCHEISSE ///////////////////////
	this->assimpScene = imp.ReadFile("Animations/Glasse_Walk_Cycle.fbx", aiProcess_MakeLeftHanded | aiProcess_Triangulate);
	AssimpHandler::saveAnimationData(assimpScene, this->player->GetMesh()->skeleton, "Walk");

	this->assimpScene = imp.ReadFile("Animations/Glasse_Idle.fbx", aiProcess_MakeLeftHanded | aiProcess_Triangulate);
	AssimpHandler::saveAnimationData(assimpScene, this->player->GetMesh()->skeleton, "Idle");
	this->player->GetMesh()->skeleton->SetFirstAnimation(this->player->GetMesh()->skeleton->animations[1]);

	this->assimpScene = imp.ReadFile("Animations/Glasse_Attack_Right.fbx", aiProcess_MakeLeftHanded | aiProcess_Triangulate);
	AssimpHandler::saveAnimationData(assimpScene, this->player->GetMesh()->skeleton, "Attack");


	Enemy* testEnemy = new Enemy(resources.GetModel("enemyModel"), &terrain, dx11, gamemanager);

	testEnemy->GetTransform().Translate(30, 7, 35);
	testEnemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
	testEnemy->SetTarget(player);
	testEnemy->SetEnabled(false);

	//this->assimpScene = imp.ReadFile("Animations/animanim.fbx", aiProcess_MakeLeftHanded | aiProcess_Triangulate);
	//AssimpHandler::saveAnimationData(assimpScene, testEnemy->GetMesh()->skeleton, "enemyMove");
	//testEnemy->GetMesh()->skeleton->SetFirstAnimation(testEnemy->GetMesh()->skeleton->animations[0]);

	this->spawner->SetEnemyPrefab(testEnemy);
	this->assimpScene = nullptr;


	// ------ WEAPONS
	Projectile* coconutPrefab = new Projectile(resources.GetModel("coconutModel"), gamemanager, &terrain, dx11);
	spawner->SetPickupPrefab(coconutPrefab, WeaponType::Coconut);

	Spoon* spoon = new Spoon(resources.GetModel("spoonModel"), gamemanager, &terrain, dx11);
	spawner->SetPickupPrefab(spoon, WeaponType::Spoon);

	// ------ Leveldesign
	CreateSceneObjects();


	// - - - - - Exit arrow
	arrow = new Object(ObjectLayer::None, resources.GetModel("arrowModel"));
	arrow->GetTransform().Translate(35, 10, 30);
	player->SetArrow(arrow);
	entities->InsertObject(arrow);
	arrow->SetVisible(false);


	// - - - - - GUI OBJECTs sist, pga inget z-värde. 
	// Add objects
	gui->AddGUIObject(gametimerText, "gametimerText");
	gui->AddGUIObject(fpsText, "fpsText");
	gui->AddGUIObject(healthFrame, "healthFrame");
	gui->AddGUIObject(actionbarLeft, "actionbarLeft");
	gui->AddGUIObject(actionbarRight, "actionbarRight");
	gui->AddGUIObject(score, "score");
	gui->AddGUIObject(totalScore, "totalscore");
	gui->AddGUIObject(enemies, "enemiesleft");
	gui->AddGUIObject(totalEnemies, "totalenemiesleft");

	// Set GUI
	renderer->SetGUI(gui);

	gametimer.Start();

	// Play scenemusic
	gamemanager->GetMusicHandler()->StopSound();
	gamemanager->GetMusicHandler()->LoadSound("Levelsound", gamemanager->GetMusicTrack());
	gamemanager->GetMusicHandler()->PlaySound("Levelsound", gamemanager->GetCurrentMusicVolume());

	testSpeed.Stop();
	std::cout << std::endl << "loadTime:  " << testSpeed.GetMilisecondsElapsed() << std::endl;
}

void DevScene::Unload()
{
	auto allEntities = entities->AllEntities();
	for (auto i : allEntities)
	{
		delete i;
	}

	entities->Clear();

	// @TODO
	gametimer.Restart();
	gametimer.Stop();
}

void DevScene::LoadResources()
{

	// save the shaders somewhere, remember to clean it up
	Shader* toonShader = new Shader();
	toonShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
	toonShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

	Shader* terrainShader = new Shader();
	terrainShader->LoadPixelShader(L"Shaders/Terrain_ps.hlsl", "main", dx11.GetDevice());
	terrainShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

	Shader* waterShader = new Shader();
	waterShader->LoadPixelShader(L"Shaders/Water_ps.hlsl", "main", dx11.GetDevice());
	waterShader->LoadVertexShader(L"Shaders/Water_vs.hlsl", "main", dx11.GetDevice());

	Shader* animationShader = new Shader();
	animationShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
	animationShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "animation", dx11.GetDevice());

	Shader* billboardShader = new Shader();
	billboardShader->LoadVertexShader(L"Shaders/Billboard_vs.hlsl", "main", dx11.GetDevice());
	billboardShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());

	resources.AddResource("toonShader", toonShader);
	resources.AddResource("terrainShader", terrainShader);
	resources.AddResource("waterShader", waterShader);
	resources.AddResource("animationShader", animationShader);
	resources.AddResource("billboardShader", billboardShader);


	// ------- TERRAIN

	Texture* grass_texture = Texture::CreateTexture("Textures/Grass_ColorTest.png", dx11);
	Texture* grass_normal = Texture::CreateTexture("Textures/Grass_Normal.png", dx11);
	Texture* sand_texture = Texture::CreateTexture("Textures/Sand_Color_Test.png", dx11);
	Texture* sand_normal = Texture::CreateTexture("Textures/Sand_Normal_2.png", dx11);
	Texture* heightMapTexture = Texture::CreateTexture("Textures/map_displacement_map_small.png", dx11);

	resources.AddResource("grassTexture", grass_texture);
	resources.AddResource("grassNormal", grass_normal);
	resources.AddResource("sandTexture", sand_texture);
	resources.AddResource("sandNormal", sand_normal);
	resources.AddResource("heightMapTexture", heightMapTexture);

	/*
		MATERIALS
	*/

	Material* terrainMat = new Material(terrainShader, dx11);
	terrainMat->SetTexture(0, grass_texture, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetTexture(1, sand_texture, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetTexture(2, grass_normal, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetTexture(3, sand_normal, SHADER_BIND_TYPE::PIXEL);
	terrainMat->SetSampler(0, dx11.GetDefaultSampler(), SHADER_BIND_TYPE::PIXEL);
	terrainMat->GetMaterialData().hasNormalTexture = true;

	Material* waterMat = new Material(waterShader, dx11);
	waterMat->SetTexture(0, heightMapTexture, SHADER_BIND_TYPE::PIXEL);
	waterMat->SetSampler(0, dx11.GetDefaultSampler(), SHADER_BIND_TYPE::PIXEL);

	resources.AddResource("terrainMaterial", terrainMat);
	resources.AddResource("waterMaterial", waterMat);

	/*
		MODELS
	*/
	resources.AddModel("coconutModel", AssimpHandler::loadFbxObject("Models/Coconut.fbx", dx11, toonShader));
	resources.AddModel("spoonModel", AssimpHandler::loadFbxObject("Models/Spoon.fbx", dx11, toonShader));
	resources.AddModel("wagonModel", AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, toonShader));
	resources.AddModel("enemyModel", AssimpHandler::loadFbxObject("Animations/animanim.fbx", dx11, toonShader));
	resources.AddModel("arrowModel", AssimpHandler::loadFbxObject("Models/Arrow.fbx", dx11, toonShader));

	resources.AddModel("quadInv", AssimpHandler::loadFbxObject("Models/QuadInv.fbx", dx11, billboardShader));
	resources.AddModel("mountainModel", AssimpHandler::loadFbxObject("Models/Mountain.fbx", dx11, toonShader));
	resources.AddModel("rockModel", AssimpHandler::loadFbxObject("Models/Rocks.fbx", dx11, toonShader));
	resources.AddModel("boatModel", AssimpHandler::loadFbxObject("Models/Boat.fbx", dx11, toonShader));
	resources.AddModel("kioskModel", AssimpHandler::loadFbxObject("Models/Kiosk.fbx", dx11, toonShader));

	resources.AddModel("sunchairModel", AssimpHandler::loadFbxObject("Models/Sunchair.fbx", dx11, toonShader));
	resources.AddModel("parasollModel", AssimpHandler::loadFbxObject("Models/Umbrella_BlueRed.fbx", dx11, toonShader));
	resources.AddModel("surfboardBlueModel", AssimpHandler::loadFbxObject("Models/SurfboardBlue.fbx", dx11, toonShader));
	resources.AddModel("surfboardOrangeModel", AssimpHandler::loadFbxObject("Models/SurfboardOrange.fbx", dx11, toonShader));
	resources.AddModel("surfboardTrippyModel", AssimpHandler::loadFbxObject("Models/SurfboardTrippy.fbx", dx11, toonShader));
	resources.AddModel("beachballRedModel", AssimpHandler::loadFbxObject("Models/Beachball_Red.fbx", dx11, toonShader));
	resources.AddModel("beachballBlueModel", AssimpHandler::loadFbxObject("Models/Beachball_Blue.fbx", dx11, toonShader));
	resources.AddModel("bushModel", AssimpHandler::loadFbxObject("Models/Bush.fbx", dx11, toonShader));
	resources.AddModel("bungalowModel", AssimpHandler::loadFbxObject("Models/Beachbungalow.fbx", dx11, toonShader));
	resources.AddModel("gateModel", AssimpHandler::loadFbxObject("Models/Gate.fbx", dx11, toonShader));
	resources.AddModel("restplaceModel", AssimpHandler::loadFbxObject("Models/Restplace.fbx", dx11, toonShader));
	resources.AddModel("benchModel", AssimpHandler::loadFbxObject("Models/Bench.fbx", dx11, toonShader));
	resources.AddModel("palmModel", AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, toonShader));
}

void DevScene::Update(const float& deltaTime)
{

	this->cameraFocusPosition = player->GetTransform().GetPosition();
	billBoard->GetTransform().SetPosition({ player->GetTransform().GetPosition().m128_f32[0],player->GetTransform().GetPosition().m128_f32[1] + 6, player->GetTransform().GetPosition().m128_f32[2] });
	this->spawner->Update(deltaTime);
	Scene::Update(deltaTime);


	// fixa

	// Change all weapons to vector
	//for(auto i : coconuts)
	//	player->UpdateHands(i);

	//for (auto i : spoons)
	//	player->UpdateHands(i);

	//
	UpdateGUI(deltaTime);
}

void DevScene::FixedUpdate(const float& fixedDeltaTime)
{
	Scene::FixedUpdate(fixedDeltaTime);

	this->player->GetMesh()->skeleton->AddKeyframe();

}

Scene* DevScene::GetNextScene() const
{
	return nextScene;
}

void DevScene::CreateSceneObjects()
{



	billBoard = new Object(ObjectLayer::Enviroment, resources.GetModel("quadInv"));
	billBoard->GetTransform().Translate(55, 12, 55);
	billBoard->GetTransform().Scale(1, 1, 1);
	billBoard->GetTransform().SetRotation({ 0,0, 0 });
	entities->InsertObject(billBoard);

	if (true)
	{
		// save the shaders somewhere, remember to clean it up
		Shader* defaultShader = new Shader();
		defaultShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
		defaultShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

		////////////////////////// MOUNTAINS /////////////////////////////
		Object* mountain = new Object(ObjectLayer::Enviroment, resources.GetModel("mountainModel"));
		Object* mountains[2];
		for (int i = 0; i < 2; i++) {
			mountains[i] = new Object(*mountain);
			entities->InsertObject(mountains[i]);
		}

		// Middle mountain
		mountains[0]->GetTransform().Translate(100.0f, 6.0f, 110.0f);

		// Top left mountain
		mountains[1]->GetTransform().Translate(30.0f, 4.0f, 170.0f);
		mountains[1]->GetTransform().Rotate(0.0f, 180.0f, 0.0f);


		////////////////////////// ROCKS /////////////////////////////
		Object* rocks = new Object(ObjectLayer::Enviroment, resources.GetModel("rockModel"));
		entities->InsertObject(rocks);
		rocks->GetTransform().Rotate(0, 180, 0);
		rocks->GetTransform().Translate(195, 5, 145);


		////////////////////////// BOATS /////////////////////////////
		Object* boat = new Object(ObjectLayer::Enviroment, resources.GetModel("boatModel"));
		boat->GetTransform().Translate(205, 6.2f, 115);
		boat->GetTransform().SetRotation({ 0.1, 1.7, -0.15 });
		entities->InsertObject(boat);


		////////////////////////// STANDS /////////////////////////////
		// Left beach stand
		Object* beachstand = new Object(ObjectLayer::Enviroment, resources.GetModel("kioskModel"));
		Object* beachstands[3];
		for (int i = 0; i < 3; i++) {
			beachstands[i] = new Object(*beachstand);
			entities->InsertObject(beachstands[i]);
		}

		beachstands[0]->GetTransform().Translate(135, 7, 48);

		beachstands[1]->GetTransform().Translate(125, 7, 48);
		beachstands[1]->GetTransform().SetRotation({ -0.1, -0.1, 0 });

		beachstands[2]->GetTransform().Translate(60, 7, 55);
		beachstands[2]->GetTransform().SetRotation({ -0.1, 0.8, 0 });


		////////////////////////// SUNCHAIR /////////////////////////////		
		// Chairs Left beachside
		Object* chair = new Object(ObjectLayer::Enviroment, resources.GetModel("sunchairModel"));
		Object* sunChairs[21];
		for (int i = 0; i < 21; i++) {
			sunChairs[i] = new Object(*chair);
			entities->InsertObject(sunChairs[i]);
		}

		sunChairs[0]->GetTransform().Translate(200, 6.5, 40);
		sunChairs[0]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[1]->GetTransform().Translate(195, 6.5, 39.8);
		sunChairs[1]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[2]->GetTransform().Translate(190, 6.5, 39.6);
		sunChairs[2]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[3]->GetTransform().Translate(175, 6.5, 39.6);
		sunChairs[3]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[4]->GetTransform().Translate(170, 6.5, 39.4);
		sunChairs[4]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[5]->GetTransform().Translate(165, 6.5, 39.2);
		sunChairs[5]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[6]->GetTransform().Translate(150, 6.5, 39.2);
		sunChairs[6]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[7]->GetTransform().Translate(145, 6.5, 39.2);
		sunChairs[7]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[8]->GetTransform().Translate(140, 6.5, 39.2);
		sunChairs[8]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[9]->GetTransform().Translate(120, 6.5, 38);
		sunChairs[9]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[10]->GetTransform().Translate(115, 6.5, 37.6);
		sunChairs[10]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[11]->GetTransform().Translate(110, 6.5, 37.4);
		sunChairs[11]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[12]->GetTransform().Translate(90, 6.5, 34);
		sunChairs[12]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[13]->GetTransform().Translate(85, 6.5, 34);
		sunChairs[13]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[14]->GetTransform().Translate(80, 6.5, 34);
		sunChairs[14]->GetTransform().Rotate(-0.1f, -6.3, 0);

		sunChairs[15]->GetTransform().Translate(35, 6.5, 57);
		sunChairs[15]->GetTransform().Rotate(0.0f, -5.7, 0);

		sunChairs[16]->GetTransform().Translate(40, 6.5, 55);
		sunChairs[16]->GetTransform().Rotate(0.0f, -5.5, 0);

		sunChairs[17]->GetTransform().Translate(42, 6.5, 51);
		sunChairs[17]->GetTransform().Rotate(0.0f, -5.3, 0);

		sunChairs[18]->GetTransform().Translate(58, 6.5, 39);
		sunChairs[18]->GetTransform().Rotate(-0.1f, -5.7, 0);

		sunChairs[19]->GetTransform().Translate(62, 6.5, 37);
		sunChairs[19]->GetTransform().Rotate(-0.1f, -5.7, 0);

		sunChairs[20]->GetTransform().Translate(66, 6.5, 35);
		sunChairs[20]->GetTransform().Rotate(-0.1f, -5.7, 0);


		////////////////////////// PARASOLLS /////////////////////////////
		// Parasoll left beachside
		Object* parasoll = new Object(ObjectLayer::Enviroment, resources.GetModel("parasollModel"));
		Object* parasolls[7];

		for (int i = 0; i < 7; i++) {
			parasolls[i] = new Object(*parasoll);
			entities->InsertObject(parasolls[i]);
		}

		parasolls[0]->GetTransform().Translate(192.5, 5.3, 40);
		parasolls[1]->GetTransform().Translate(172.5, 5.4, 40);
		parasolls[2]->GetTransform().Translate(147.5, 5.5, 40);
		parasolls[3]->GetTransform().Translate(117.5, 5.6, 38);
		parasolls[3]->GetTransform().Rotate(-0.05, 0, 0);
		parasolls[4]->GetTransform().Translate(87.5, 5.6, 35);
		parasolls[4]->GetTransform().Rotate(-0.05, 0, 0);
		parasolls[5]->GetTransform().Translate(45, 5.6, 55);
		parasolls[5]->GetTransform().Rotate(-0.05, 0, 0);
		parasolls[6]->GetTransform().Translate(65, 6, 40);
		parasolls[6]->GetTransform().Rotate(-0.05, 0, 0);


		////////////////////////// SURFBOARDS /////////////////////////////
		Object* SurfboardBlue = new Object(ObjectLayer::Enviroment, resources.GetModel("surfboardBlueModel"));
		Object* SurfboardsBlue[2];
		for (int i = 0; i < 2; i++) {
			SurfboardsBlue[i] = new Object(*SurfboardBlue);
			entities->InsertObject(SurfboardsBlue[i]);
		}

		SurfboardsBlue[0]->GetTransform().Translate(130, 7, 35);
		SurfboardsBlue[1]->GetTransform().Translate(168, 7.3, 210);


		Object* SurfboardOrange = new Object(ObjectLayer::Enviroment, resources.GetModel("surfboardOrangeModel"));
		Object* SurfboardsOrange[2];
		for (int i = 0; i < 2; i++) {
			SurfboardsOrange[i] = new Object(*SurfboardOrange);
			entities->InsertObject(SurfboardsOrange[i]);
		}

		SurfboardsOrange[0]->GetTransform().Translate(128, 7, 33);
		SurfboardsOrange[1]->GetTransform().Translate(168, 7.3, 205);



		Object* SurfboardTrippy = new Object(ObjectLayer::Enviroment, resources.GetModel("surfboardTrippyModel"));
		entities->InsertObject(SurfboardTrippy);
		SurfboardTrippy->GetTransform().Translate(130, 7, 37);


		////////////////////////// BEACHBALLS /////////////////////////////
		// Balls left beachside
		Object* redball = new Object(ObjectLayer::Enviroment, resources.GetModel("beachballRedModel"));
		Object* redballs[3];
		for (int i = 0; i < 3; i++) {
			redballs[i] = new Object(*redball);
			entities->InsertObject(redballs[i]);
		}

		redballs[0]->GetTransform().Translate(100, 7, 38);
		redballs[0]->GetTransform().Rotate(0.2f, -5, 0);

		redballs[1]->GetTransform().Translate(160, 7, 190);
		redballs[1]->GetTransform().Rotate(0.2f, -5, 0);

		redballs[2]->GetTransform().Translate(35, 6, 45);
		redballs[2]->GetTransform().Rotate(0.2f, -5, 0);


		Object* blueball = new Object(ObjectLayer::Enviroment, resources.GetModel("beachballBlueModel"));
		Object* blueballs[3];
		for (int i = 0; i < 3; i++) {
			blueballs[i] = new Object(*blueball);
			entities->InsertObject(blueballs[i]);
		}

		blueballs[0]->GetTransform().Translate(105, 6.5, 35);
		blueballs[0]->GetTransform().Rotate(0.2f, -5, 0);

		blueballs[1]->GetTransform().Translate(155, 6.5, 195);
		blueballs[1]->GetTransform().Rotate(0.2f, -5, 0);

		blueballs[2]->GetTransform().Translate(40, 6.2, 40);
		blueballs[2]->GetTransform().Rotate(0.2f, -5, 0);




		////////////////////////// BEACHBALLS /////////////////////////////
		Object* bush = new Object(ObjectLayer::Enviroment, resources.GetModel("bushModel"));
		Object* bushes[13];
		for (int i = 0; i < 13; i++) {
			bushes[i] = new Object(*bush);
			entities->InsertObject(bushes[i]);
		}


		// 2 Stands bottom beach //
		bushes[0]->GetTransform().Translate(142, 7, 50);
		bushes[0]->GetTransform().Rotate(0.05f, -5, 0);

		bushes[1]->GetTransform().Translate(120, 7, 47);
		bushes[1]->GetTransform().Rotate(0.05f, -5, 0);
		////

		// Stand bottom beach left side
		bushes[2]->GetTransform().Translate(75, 6.5, 43);
		bushes[2]->GetTransform().Rotate(0.05f, -5, 0);

		bushes[3]->GetTransform().Translate(70.0f, 6.5f, 45.0f);
		bushes[3]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);
		////

		// By restplace //
		bushes[4]->GetTransform().Translate(112.0f, 7.0f, 132.0f);
		bushes[4]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);

		bushes[5]->GetTransform().Translate(115.0f, 8.0f, 148.0f);
		bushes[5]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);
		////


		// By rocks //
		bushes[6]->GetTransform().Translate(195.0f, 8.0f, 143.0f);
		bushes[6]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);

		bushes[7]->GetTransform().Translate(200.0f, 8.0f, 152.0f);
		bushes[7]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);

		// Bottom rocks by beach
		bushes[8]->GetTransform().Translate(190.0f, 7.0f, 120.0f);
		bushes[8]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);

		bushes[9]->GetTransform().Translate(195.0f, 7.0f, 120.0f);
		bushes[9]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);
		////

		// By benches - by rocks
		bushes[10]->GetTransform().Translate(165.0f, 7.0f, 122.0f);
		bushes[10]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);
		////

		// Top beach
		bushes[11]->GetTransform().Translate(143.0f, 7.5f, 188.0f);
		bushes[11]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);

		bushes[12]->GetTransform().Translate(163.0f, 7.5f, 185.0f);
		bushes[12]->GetTransform().Rotate(0.05f, -5.0f, 0.0f);


		////////////////////////// Fireplace stuff /////////////////////////////

		Object* bungalow = new Object(ObjectLayer::Enviroment, resources.GetModel("bungalowModel"));
		bungalow->GetTransform().Translate(30.0f, 7, 213.0f);
		bungalow->GetTransform().SetRotation({ 0.0f, -0.9f, 0.0f });
		entities->InsertObject(bungalow);

		Object* gate = new Object(ObjectLayer::Enviroment, resources.GetModel("gateModel"));
		gate->GetTransform().Translate(60.0f, 6.2, 198);
		gate->GetTransform().SetRotation({ 0.0f, -2.2f, -0.1f });
		entities->InsertObject(gate);

		Object* restplace = new Object(ObjectLayer::Enviroment, resources.GetModel("restplaceModel"));
		restplace->GetTransform().Translate(107.0f, 7.5f, 140.0f);
		restplace->GetTransform().SetRotation({ 0.0f, -1.5f, 0.0f });
		entities->InsertObject(restplace);


		// Benches left beachside
		Object* bench = new Object(ObjectLayer::Enviroment, resources.GetModel("benchModel"));
		Object* benches[4];
		for (int i = 0; i < 4; i++)
			benches[i] = new Object(*bench);

		benches[0]->GetTransform().Translate(170.0f, 7.0f, 118.0f);
		benches[0]->GetTransform().SetRotation({ 0.0f, 0.4f, 0.0f });

		benches[1]->GetTransform().Translate(157.0f, 7.0f, 122.0f);
		benches[1]->GetTransform().SetRotation({ 0.0f, 0.3f, 0.0f });

		benches[2]->GetTransform().Translate(140.0f, 7.0f, 195.0f);
		benches[2]->GetTransform().SetRotation({ 0.0f, -2.5f, 0.0f });

		benches[3]->GetTransform().Translate(170.0f, 7.0f, 190.0f);
		benches[3]->GetTransform().SetRotation({ 0.0f, 2.5f, 0.0f });

		for (int i = 0; i < 4; i++)
			entities->InsertObject(benches[i]);

		////////////////////////// PALMS /////////////////////////////




		// Palms left beach side 
		Object* palm = new Object(ObjectLayer::Tree, resources.GetModel("palmModel"));
		Object* palms[22];
		for (int i = 0; i < 22; i++) {
			palms[i] = new Object(*palm);
			entities->InsertObject(palms[i]);
		}

		// Bottom beach //
		palms[0]->GetTransform().Translate(200.0f, 7.8f, 60.0f);
		palms[1]->GetTransform().Translate(180.0f, 7.8f, 75.0f);
		palms[2]->GetTransform().Translate(160.0f, 7.8f, 60.0f);
		palms[3]->GetTransform().Translate(145.0f, 7.8f, 73.0f);
		palms[4]->GetTransform().Translate(125.0f, 7.8f, 60.0f);
		palms[5]->GetTransform().Translate(105.0f, 7.8f, 75.0f);
		/////

		// bottom beach left
		palms[6]->GetTransform().Translate(85.0f, 7.8f, 58.0f);
		palms[7]->GetTransform().Translate(83.0f, 7.8f, 82.0f);
		palms[8]->GetTransform().Translate(60.0f, 8.0f, 90.0f);
		palms[9]->GetTransform().Translate(60.0f, 7.8f, 65.0f);
		////

		// left of middle rock
		palms[10]->GetTransform().Translate(55.0f, 7.8f, 115.0f);
		palms[11]->GetTransform().Translate(38.0f, 7.8f, 95.0f);
		palms[12]->GetTransform().Translate(45.0f, 7.8f, 140.0f);
		////

		// top rock
		palms[13]->GetTransform().Translate(70.0f, 7.8f, 165.0f);
		palms[14]->GetTransform().Translate(80.0f, 7.8f, 140.0f);
		palms[15]->GetTransform().Translate(105.0f, 7.8f, 165.0f);
		palms[16]->GetTransform().Translate(90.0f, 8.0f, 195.0f);
		palms[17]->GetTransform().Translate(115.0f, 7.8f, 200.0f);
		////

		// Left by rocks
		palms[18]->GetTransform().Translate(145.0f, 8.0f, 175.0f);
		palms[19]->GetTransform().Translate(170.0f, 7.8f, 160.0f);
		palms[20]->GetTransform().Translate(150.0f, 7.8f, 145.0f);
		palms[21]->GetTransform().Translate(200.0f, 7.8f, 180.0f);

	}


	spawner->SpawnInitial();

}

void DevScene::checkForNextScene()
{
	// Används inte längre
	// Change scene logic

	if (controller->getInput()->GetKeyDown('i'))
	{
		for (int i = 0; i < scenes.size(); i++)
		{
			if (scenes[i]->GetName() == "GameOverScene")
			{
				nextScene = scenes[i];
			}

			if (scenes[i]->GetName() == "WinScene")
			{
				nextScene = scenes[i];
			}

		}
	}
}

void DevScene::SetNextScene(bool winOrLose)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes[i]->GetName() == "EndScene")
		{
			scenes[i]->setWinOrLose(winOrLose);
			nextScene = scenes[i];
		}
	}
}

void DevScene::UpdateGUI(const float& deltaTime)
{
	//FPS STUFF
	fpsTimer.Start();

	totalEnemies->SetString(std::to_string(this->spawner->CountEnemiesRemaining()));
	totalScore->SetString(std::to_string(player->GetPoints()));

	gametimerText->SetString("Timer: " + std::to_string(static_cast<int>(std::floor(gametimer.GetMilisecondsElapsed() / 1000.0))));
	controller->Update(deltaTime);

	fpsTimer.Stop();
	fpsText->SetString("FPS: " + std::to_string((int)(1 / ((fpsTimer.GetMicrosecondsElapsed() / 1000000)))));
	fpsTimer.Restart();
	checkForNextScene();

	gametimerText->SetString("Time until extraction: " + std::to_string(static_cast<int>(gametimer.GetTimeUntilEnd(timeUntilEnd))));

	if (gametimer.GetTimeUntilEnd(timeUntilEnd) <= 0.0f || this->spawner->CountEnemiesRemaining() == 0)
	{
		arrow->SetVisible(true);
		gametimerText->SetString("Move to exit");
		gametimerText->SetPosition(window.GetWidth() / 2.0f - 80.0f, 0.0f);
		canWin = true;
	}

	if (player->GetPlayerHealth() <= 0.0f)
	{
		gametimerText->SetString("You lost");
		gametimerText->SetPosition(window.GetWidth() / 2.0f - 75.0f, 0.0f);
		SetNextScene(false);
	}

	if (canWin && player->GetWorldBounds().Overlaps(this->player->GetWinArea()->GetWorldBounds()))
	{
		gametimerText->SetString("You won");
		gametimerText->SetPosition(window.GetWidth() / 2.0f - 75.0f, 0.0f);
		SetNextScene(true);
	}
}