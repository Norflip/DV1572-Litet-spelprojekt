#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes) : Scene("DevScene", renderer, dx11, window), scenes(scenes)
{
	//----- GUI SHIET |  Set gui last |

	gametimerText = new GUIText(dx11, "Time until extraction", window.GetWidth() / 2.0f - 150.0f, 0);
	fpsText = new GUIText(dx11, "Fps", window.GetWidth() / 2.0f - 100.0f, 30);


	this->controller = new CameraController(GetSceneCamera(), window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights& lights = renderer->GetLights();
	lights.SetSunDirection({ 1, -1, 0 });
	lights.SetSunColor({ 0.98f, 0.96f, 0.73f, 1 });
	lights.SetSunIntensity(0.6f);

	//lights->AddPointLight({ -2, 0, 0 }, { 1.0f, 1.0f, 1.0f, 1 }, 50);
	//lights->AddPointLight({ -2, 0, 10 }, { 0.2f,0.2f, 0.2f, 1 }, 50);	
	this->timeUntilEnd = 10.0f;
	this->canWin = false;
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
	gui->AddGUIObject(gametimerText, "gametimerText" );
	gui->AddGUIObject(fpsText, "fpsText");
	gui->AddGUIObject(healthFrame, "healthFrame");
	gui->AddGUIObject(actionbarLeft, "actionbarLeft");
	gui->AddGUIObject(actionbarRight, "actionbarRight");

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

	Shader* terrainShader = new Shader();
	terrainShader->LoadPixelShader(L"Shaders/Terrain_ps.hlsl", "main", dx11.GetDevice());
	terrainShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	Texture* grass_texture = Texture::CreateTexture("Textures/Grass.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	Texture* grass_normal = Texture::CreateTexture("Textures/TMP_Grass_NORM.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);


	Texture* sand_texture = Texture::CreateTexture("Textures/TMP_Sand_COLOR.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);	
	Texture* sand_normal = Texture::CreateTexture("Textures/TMP_Sand_NORM.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	Material* terrainMat = new Material(terrainShader, dx11);
	terrainMat->SetTexture(0, grass_texture, PIXEL_TYPE::PIXEL);
	terrainMat->SetTexture(1, sand_texture, PIXEL_TYPE::PIXEL);

	terrainMat->SetTexture(2, grass_normal, PIXEL_TYPE::PIXEL);
	terrainMat->SetTexture(3, sand_normal, PIXEL_TYPE::PIXEL);
	terrainMat->GetMaterialData().hasNormalTexture = true;

	/*Material* test_material = new Material(terrainShader, dx11);
	Texture* m_texture = Texture::CreateTexture("Textures/rocks.jpg", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	test_material->SetTexture(ALBEDO_MATERIAL_TYPE, m_texture, PIXEL_TYPE::PIXEL);
	test_material->GetMaterialData().hasNormalTexture = false;*/

	test.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice());
	AddObject(new Object(test.GetMesh(), terrainMat));


	// ------ PLAYER
	this->player = new Player(dev_monkey_mesh, new Material(defaultShader, dx11), controller, &test, gui, dx11, static_cast<Scene*>(this));
	//player->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	//player->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_normal, PIXEL_TYPE::PIXEL);

	this->controller->SetFollow(&this->player->GetTransform(), { 0, 10.0f, -10.0f });
	AddObject(this->player);

	
	spawnObjects = new SpawnObjects(dx11, static_cast<Scene*>(this), &test, dev_monkey_mesh, new Material(defaultShader, dx11), this->player);
	spawnObjects->SpawnEnemy();
	/*this->enemy = new Enemy(dev_monkey_mesh, new Material(defaultShader, dx11), &test, dx11);
	this->enemy->GetTransform().Translate(5, 12, 15);
	this->enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
	this->enemy->SetTarget(this->player);
	AddObject(this->enemy);*/
	this->player->SetEnemy(spawnObjects->GetEnemy());

	Shader* waterShader = new Shader();
	waterShader->LoadPixelShader(L"Shaders/Water_ps.hlsl", "main", dx11.GetDevice());
	waterShader->LoadVertexShader(L"Shaders/Water_vs.hlsl", "main", dx11.GetDevice());

	Mesh* waterPlane = ShittyOBJLoader::Load("Models/Water_Plane.obj", dx11.GetDevice());
	Object* water = new Object(waterPlane, new Material(waterShader, dx11));
	water->GetTransform().Translate({ 100, 6.0f, 100 });
	water->GetTransform().Scale(2, 2, 2);
	AddObject(water);

		
	this->coconutPickUp = AssimpHandler::loadFbxObject("Models/Coconut.fbx", dx11, defaultShader);
	coconutPickUp->GetTransform().Translate(10, 2, 15);
	AddObject(coconutPickUp);

	//// Testing fbx
	
	CreateSceneObjects();
	//Projectile* testProj = new Projectile("Models/Coconut.fbx", &test, dx11, defaultShader, DirectX::XMVECTOR({ 0,5,0 }), DirectX::XMVECTOR({ 0,0/*MathHelper::PI/2*/,0 }));
	////testProj->GetTransform().Translate(0, 0, 0);
	//AddObject(testProj);
	
	
	Object* wagon = AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, defaultShader);
	wagon->GetTransform().Scale(0.5f, 0.5f, 0.5f);
	wagon->GetTransform().Translate(50, 9.5, 50);
	wagon->GetTransform().Rotate(0.05f, -5, 0);
	AddObject(wagon);

	gametimer.Start();
	//// Testing animation
	//Object* animation = AssimpHandler::loadFbxObject("Models/animation.fbx", dx11, defaultShader);
	//AddObject(animation);
}

void DevScene::Unload()
{
	// @TODO
	gametimer.Restart();
	gametimer.Stop();
}

void DevScene::Update(const float& deltaTime)
{
	spawnObjects->SpawnEnemy();

	Scene::Update(deltaTime);

	//FPS STUFF
	fpsTimer.Start();
	player->NutOnPlayer(coconutPickUp);
	
	fpsTimer.Stop();
	fpsText->SetString("FPS: " + std::to_string((int)(1 / ((fpsTimer.GetMicrosecondsElapsed() / 1000000)))));
	fpsTimer.Restart();
	checkForNextScene();

	gametimerText->SetString("Time until extraction: " + std::to_string(static_cast<int>(gametimer.GetTimeUntilEnd(timeUntilEnd))));
	controller->Update(deltaTime);


	if (gametimer.GetTimeUntilEnd(timeUntilEnd) <= 0.0f)
	{
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

	int size = allObjects.size();

	/*if (canWin && player->GetWorldBounds().Overlaps(allObjects[size-1]->GetWorldBounds()))
	{
		gametimerText->SetString("You won");
		gametimerText->SetPosition(window.GetWidth() / 2.0f - 75.0f, 0.0f);
		SetNextScene(true);
	}*/
}

Scene* DevScene::GetNextScene() const
{
	return nextScene;
}

void DevScene::CreateSceneObjects()
{
	// save the shaders somewhere, remember to clean it up
	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

	// Left beach stand
	Object* beachstand = AssimpHandler::loadFbxObject("Models/BeachstandChips.fbx", dx11, defaultShader);
	beachstand->GetTransform().Scale(0.5, 0.5, 0.5);
	beachstand->GetTransform().Translate(35, 8, 75);
	beachstand->GetTransform().SetRotation({ 0, 1.6, 0 });
	AddObject(beachstand);


	// Boat
	Object* boat = AssimpHandler::loadFbxObject("Models/Boat.fbx", dx11, defaultShader);
	boat->GetTransform().Scale(2.5, 2.5, 2.5);
	boat->GetTransform().Translate(160, 7.5, 30);
	boat->GetTransform().SetRotation({ 0, 1.6, 0 });
	AddObject(boat);

	// Rocks
	Object* rocks = AssimpHandler::loadFbxObject("Models/Rocks.fbx", dx11, defaultShader);
	rocks->GetTransform().Scale(1, 1, 1);
	rocks->GetTransform().Translate(145, 7.5, 65);
	AddObject(rocks);




	// Chairs Left beachside
	Object* chair = AssimpHandler::loadFbxObject("Models/CuteChair.fbx", dx11, defaultShader);
	Object* chairs[5]; 
	for(int i = 0; i < 5; i++)
		chairs[i] = new Object(*chair);

	chairs[0]->GetTransform().Scale(1.25, 1.25, 1.25);
	chairs[0]->GetTransform().Translate(15, 7.5f, 55);
	chairs[0]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(chairs[0]);

	chairs[1]->GetTransform().Scale(1.25, 1.25, 1.25);
	chairs[1]->GetTransform().Translate(14, 7.0f, 58);
	chairs[1]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(chairs[1]);

	chairs[2]->GetTransform().Scale(1.25, 1.25, 1.25);
	chairs[2]->GetTransform().Translate(13, 6.9f, 61);
	chairs[2]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(chairs[2]);

	chairs[3]->GetTransform().Scale(1.25, 1.25, 1.25);
	chairs[3]->GetTransform().Translate(14, 7.0f, 68);
	chairs[3]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(chairs[3]);

	chairs[4]->GetTransform().Scale(1.25, 1.25, 1.25);
	chairs[4]->GetTransform().Translate(13, 6.9f, 71);
	chairs[4]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(chairs[4]);
	
	// Benches left beachside
	Object* bench = AssimpHandler::loadFbxObject("Models/Bench.fbx", dx11, defaultShader);
	Object* benches[2];
	for (int i = 0; i < 2; i++)
		benches[i] = new Object(*bench);

	benches[0]->GetTransform().Scale(0.3, 0.3, 0.3);
	benches[0]->GetTransform().Translate(29, 7, 50);
	benches[0]->GetTransform().SetRotation({ 0,2,0 });
	AddObject(benches[0]);

	benches[1]->GetTransform().Scale(0.3, 0.3, 0.3);
	benches[1]->GetTransform().Translate(25, 7, 95);
	benches[1]->GetTransform().SetRotation({ 0,0.9,0 });
	AddObject(benches[1]);
	
	// Parasoll left beachside
	Object* parasoll = AssimpHandler::loadFbxObject("Models/Parasoll.fbx", dx11, defaultShader);
	Object* parasolls[2];
	for (int i = 0; i < 2; i++)
		parasolls[i] = new Object(*parasoll);

	parasolls[0]->GetTransform().Translate(20, 5.4, 60);
	parasolls[0]->GetTransform().Scale(0.5, 0.5, 0.5);
	parasolls[0]->GetTransform().Rotate(0.1, 0, -0.1);
	AddObject(parasolls[0]);

	parasolls[1]->GetTransform().Translate(17, 3.7, 80);
	parasolls[1]->GetTransform().Scale(0.5, 0.5, 0.5);
	parasolls[1]->GetTransform().Rotate(0, 0, -0.2);
	AddObject(parasolls[1]);
	
	// Balls left beachside
	Object* redball = AssimpHandler::loadFbxObject("Models/Redball.fbx", dx11, defaultShader);
	Object* redballs[2];
	for (int i = 0; i < 2; i++)
		redballs[i] = new Object(*redball);

	redballs[0]->GetTransform().Scale(1, 1, 1);
	redballs[0]->GetTransform().Translate(25, 5.8, 65);
	redballs[0]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(redballs[0]);

	redballs[1]->GetTransform().Scale(1, 1, 1);
	redballs[1]->GetTransform().Translate(125, 6, 155);
	redballs[1]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(redballs[1]);
	
	Object* blueball = AssimpHandler::loadFbxObject("Models/Blueball.fbx", dx11, defaultShader);
	Object* blueballs[2];
	for (int i = 0; i < 2; i++)
		blueballs[i] = new Object(*blueball);

	blueballs[0]->GetTransform().Scale(1, 1, 1);
	blueballs[0]->GetTransform().Translate(15, 6.5, 90);
	blueballs[0]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(blueballs[0]);

	blueballs[1]->GetTransform().Scale(1, 1, 1);
	blueballs[1]->GetTransform().Translate(130, 6.5, 150);
	blueballs[1]->GetTransform().Rotate(0.2f, -5, 0);
	AddObject(blueballs[1]);

	// Palms left beach side 
	Object* palm = AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, defaultShader);
	Object* palms[3];
	for (int i = 0; i < 3; i++)
		palms[i] = new Object(*palm);

	palms[0]->GetTransform().Translate(30, 9, 55);
	palms[0]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(palms[0]);

	palms[1]->GetTransform().Translate(35, 8, 75);
	palms[1]->GetTransform().Scale(0.3, 0.3, 0.3);	
	AddObject(palms[1]);

	palms[2]->GetTransform().Translate(30, 8, 90);
	palms[2]->GetTransform().Scale(0.3, 0.3, 0.3);	
	AddObject(palms[2]);

	// Mountains
	Object* mountain = AssimpHandler::loadFbxObject("Models/Mountain.fbx", dx11, defaultShader);
	Object* mountains[2];
	for (int i = 0; i < 2; i++)
		mountains[i] = new Object(*mountain);

	// Middle mountain
	mountains[0]->GetTransform().Translate(100, 7, 110);
	mountains[0]->GetTransform().Scale(5, 5, 5);	
	AddObject(mountains[0]);

	// Top left mountain
	mountains[1]->GetTransform().Translate(30, 4, 170);
	mountains[1]->GetTransform().Rotate(0, 180, 0);
	mountains[1]->GetTransform().Scale(5, 5, 5);	
	AddObject(mountains[1]);
		

	// Middle palms
	Object* middlePalms[5];
	for (int i = 0; i < 5; i++)
		middlePalms[i] = new Object(*palm);
	// 2 stycken �ver mitten berget
	middlePalms[0]->GetTransform().Translate(55, 8, 145);
	middlePalms[0]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(middlePalms[0]);

	middlePalms[1]->GetTransform().Translate(75, 8.2, 155);
	middlePalms[1]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(middlePalms[1]);

	// 3 under mitten berget
	middlePalms[2]->GetTransform().Translate(110, 9, 55);
	middlePalms[2]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(middlePalms[2]);

	middlePalms[3]->GetTransform().Translate(105, 8, 75);
	middlePalms[3]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(middlePalms[3]);

	middlePalms[4]->GetTransform().Translate(115, 8, 90);
	middlePalms[4]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(middlePalms[4]);

	// palmer h�gra sidan
	Object* rightPalms[3];
	for (int i = 0; i < 3; i++)
		rightPalms[i] = new Object(*palm);

	rightPalms[0]->GetTransform().Translate(150, 8, 160);
	rightPalms[0]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(rightPalms[0]);

	rightPalms[1]->GetTransform().Translate(155, 8, 150);
	rightPalms[1]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(rightPalms[1]);

	rightPalms[2]->GetTransform().Translate(157, 8, 130);
	rightPalms[2]->GetTransform().Scale(0.3, 0.3, 0.3);
	AddObject(rightPalms[2]);

	// Grass
	Object* grass = AssimpHandler::loadFbxObject("Models/Grass.fbx", dx11, defaultShader);
	Object* grassgroup[7];
	for (int i = 0; i < 7; i++)
		grassgroup[i] = new Object(*grass);

	// By Icecream wagon, left beach side
	grassgroup[0]->GetTransform().Translate(48, 9.4, 43);
	grassgroup[0]->GetTransform().SetRotation({ 0, 0, -0.05f });
	grassgroup[0]->GetTransform().Scale(1, 1,1);
	AddObject(grassgroup[0]);

	grassgroup[1]->GetTransform().Translate(45, 9, 77);
	grassgroup[1]->GetTransform().Scale(1, 1, 1);
	AddObject(grassgroup[1]);
	//

	// Right Beach side, under mid mountain
	grassgroup[2]->GetTransform().Translate(115, 10.5, 65);
	grassgroup[2]->GetTransform().Scale(1, 1, 1);
	grassgroup[2]->GetTransform().SetRotation({ 0.05, 0, 0 });
	AddObject(grassgroup[2]);
		
	grassgroup[3]->GetTransform().Translate(135, 10.2, 60);
	grassgroup[3]->GetTransform().Scale(1, 1, 1);
	grassgroup[3]->GetTransform().SetRotation({ 0.05, 0, -0.05f });
	AddObject(grassgroup[3]);
	
	grassgroup[4]->GetTransform().Translate(155, 10, 55);
	grassgroup[4]->GetTransform().Scale(1, 1, 1);
	grassgroup[4]->GetTransform().SetRotation({ 0.05, 0, 0.05f });
	AddObject(grassgroup[4]);

	// Palms right side top
	grassgroup[5]->GetTransform().Translate(158, 10, 135);
	grassgroup[5]->GetTransform().Scale(1, 1, 1);
	grassgroup[5]->GetTransform().SetRotation({ 0.05, 0, -0.05f });
	AddObject(grassgroup[5]);

	grassgroup[6]->GetTransform().Translate(158, 10, 145);
	grassgroup[6]->GetTransform().Scale(1, 1, 1);
	grassgroup[6]->GetTransform().SetRotation({ 0.05, 0, 0.05f });
	AddObject(grassgroup[6]);
	//

	// Surdboards
	Object* sunsetSurfboard = AssimpHandler::loadFbxObject("Models/surfboardSunset.fbx", dx11, defaultShader);
	Object* surfboard = AssimpHandler::loadFbxObject("Models/surfboard.fbx", dx11, defaultShader);
	Object* surfboardTrippy = AssimpHandler::loadFbxObject("Models/surfboardTrippy.fbx", dx11, defaultShader);
		
	sunsetSurfboard->GetTransform().Translate(150, 8.5, 30);
	sunsetSurfboard->GetTransform().Scale(1,1,1);
	sunsetSurfboard->GetTransform().SetRotation({ 0, 0, 0 });
	AddObject(sunsetSurfboard);

	surfboard->GetTransform().Translate(148, 8.5, 28);
	surfboard->GetTransform().Scale(1, 1, 1);
	surfboard->GetTransform().SetRotation({ 0, 0, 0 });
	AddObject(surfboard);

	surfboardTrippy->GetTransform().Translate(146, 8.5, 30);
	surfboardTrippy->GetTransform().Scale(1, 1, 1);
	surfboardTrippy->GetTransform().SetRotation({ 0, 0, 0 });
	AddObject(surfboardTrippy);

	//this->controller->SetFollow(&boat->GetTransform(), { 0, 10.0f, -10.0f });





}

void DevScene::AddSceneObject(Object* obj)
{

}

void DevScene::checkForNextScene()
{
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
		if (winOrLose == false)
		{
			if (scenes[i]->GetName() == "GameOverScene")
			{
				nextScene = scenes[i];
			}
		}
		else if (winOrLose ==  true)
		{
			if (scenes[i]->GetName() == "WinScene")
			{
				nextScene = scenes[i];
			}
		}
	}
}

