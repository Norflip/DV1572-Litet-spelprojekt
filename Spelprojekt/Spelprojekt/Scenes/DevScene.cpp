#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes) : Scene("DevScene", renderer, dx11, window), scenes(scenes)
{
	//----- GUI SHIET |  Set gui last |

	gametimerText = new GUIText(dx11, "Time until extraction", window.GetWidth() / 2.0f - 150.0f, 0);
	fpsText = new GUIText(dx11, "Fps", window.GetWidth() / 2.0f - 100.0f, 30);


	this->controller = new CameraController(GetSceneCamera(), window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	Lights& lights = renderer->GetLights();
	lights.SetSunDirection({ 1, -2, 1 });
	lights.SetSunColor({ 0.98f, 0.96f, 0.73f, 1 });
	lights.SetSunIntensity(0.6f);

	//lights->AddPointLight({ -2, 0, 0 }, { 1.0f, 1.0f, 1.0f, 1 }, 50);
	//lights->AddPointLight({ -2, 0, 10 }, { 0.2f,0.2f, 0.2f, 1 }, 50);	
	this->timeUntilEnd = 10.0f;

}

DevScene::~DevScene()
{
	delete controller;
}

void DevScene::Load()
{		
	this->canWin = false;

	// HEALTH
	healthFrame = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 650.0f);
	actionbarLeft = new GUIActionbar(dx11, "Sprites/Actionbar.png", 325.0f, 650.0f);
	actionbarRight = new GUIActionbar(dx11, "Sprites/Actionbar.png", 400.0f, 650.0f);

	//--------------------------------
	// Create GUI for Devscene
	GUI* gui = new GUI(dx11);
	
	// save the shaders somewhere, remember to clean it up
	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

	// object = mesh + material

	// Exit Wagon
	Object* wagon = AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, defaultShader);
	wagon->GetTransform().Scale(0.5f, 0.5f, 0.5f);
	wagon->GetTransform().Translate(50, 9.5, 50);
	wagon->GetTransform().Rotate(0.05f, -5, 0);
	AddObject(wagon);

	//Bush
	Object* bush = AssimpHandler::loadFbxObject("Models/Bush.fbx", dx11, defaultShader);
	bush->GetTransform().Scale(0.5f, 0.5f, 0.5f);
	bush->GetTransform().Translate(60, 9.5, 50);
	bush->GetTransform().Rotate(0.05f, -5, 0);
	AddObject(bush);

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

	Texture* grass_texture = Texture::CreateTexture("Textures/Grass_ColorTest.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	Texture* grass_normal = Texture::CreateTexture("Textures/Grass_Normal.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	//Texture* grass_texture = Texture::CreateTexture("Textures/Cartoon_Grass_2.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	//Texture* grass_normal = Texture::CreateTexture("Textures/Grass_Cartoon_Normal_2.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	
	Texture* sand_texture = Texture::CreateTexture("Textures/Sand_Color_2.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);	
	Texture* sand_normal = Texture::CreateTexture("Textures/Sand_Normal_2.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
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

	ground.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice(), false);
	Object* terrainObject = new Object(ground.GetMesh(), terrainMat);
	AddObject(terrainObject);
	

	// ------- water shader
	waterMesh.GenerateMesh("Textures/map_displacement_map_small.png", dx11.GetDevice(), true);
	Shader* waterShader = new Shader();
	waterShader->LoadPixelShader(L"Shaders/Water_ps.hlsl", "main", dx11.GetDevice());
	waterShader->LoadVertexShader(L"Shaders/Water_vs.hlsl", "main", dx11.GetDevice());
	Texture* heightMapTexture = Texture::CreateTexture("Textures/map_displacement_map_small.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	Material* waterMat = new Material(waterShader, dx11);
	
	waterMat->SetTexture(0, heightMapTexture, PIXEL_TYPE::PIXEL);

	//Mesh* waterPlane = ShittyOBJLoader::Load("Models/Water_Plane.obj", dx11.GetDevice());
	//Object* water =  AssimpHandler::loadFbxObject("Models/Water_Plane.fbx", dx11, waterShader);
	Object* water = new Object(waterMesh.GetMesh(), waterMat);
	water->SetMaterial(waterMat);
	water->GetTransform().Translate({ 0,6,0,});
	water->GetTransform().SetRotation({ 0,0,0 });
	//water->GetTransform().Scale(2*1.2, 2 * 1.2, 2 * 1.2);
	//water->GetTransform().Scale(2,2,2);
	AddObject(water);

	water->isWater = true;


	// ------ PLAYER
	Shader* toonShader = new Shader();
	toonShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
	toonShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

	this->player = new Player(dev_monkey_mesh, new Material(toonShader, dx11), controller, &ground, gui, dx11, static_cast<Scene*>(this));
	//player->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, monkey_texture, PIXEL_TYPE::PIXEL);
	//player->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, monkey_normal, PIXEL_TYPE::PIXEL);
	this->player->GetTransform().SetPosition({ 30, 7, 30 });
	this->controller->SetFollow(&this->player->GetTransform(), { 0, 10.0f, -10.0f });
	AddObject(this->player);

	
	this->spawnObjects = new SpawnObjects(dx11, static_cast<Scene*>(this), &ground, dev_monkey_mesh, new Material(defaultShader, dx11), this->player);
	this->spawnObjects->SetEnemy();
	AddObject(this->spawnObjects);
	
	for(int i = 0; i < 5; i++)
		this->coconuts[i] = new Projectile("Models/Coconut.fbx", &ground, dx11, defaultShader, { 0, 0,0 }, { 0, 0,0 } /* player->GetTransform().GetRotation()*/);
	
	coconuts[0]->GetTransform().Translate(35 + 22.5, 10, 25 + 22.5);
	coconuts[1]->GetTransform().Translate(40, 10, 25);
	coconuts[2]->GetTransform().Translate(45, 10, 25);
	coconuts[3]->GetTransform().Translate(50, 10, 25);
	coconuts[4]->GetTransform().Translate(55, 10, 25);
	for (int i = 0; i < 5; i++)
		AddObject(coconuts[i]);
	
			
	// Spoon
	for(int i = 0; i < 5; i++)
		this->spoons[i] = new Spoon("Models/Spoon.fbx", &ground, dx11, defaultShader, { 0, 0,0 }, { 0, 0,0 } /* player->GetTransform().GetRotation()*/);
	spoons[0]->GetTransform().Translate(35, 10, 30);
	spoons[1]->GetTransform().Translate(40, 10, 30);
	spoons[2]->GetTransform().Translate(45, 10, 30);
	spoons[3]->GetTransform().Translate(50, 10, 30);
	spoons[4]->GetTransform().Translate(55, 10, 30);
	for (int i = 0; i < 5; i++)
		AddObject(spoons[i]);
	
	
	// ------ Leveldesign
	CreateSceneObjects();	
	



	// - - - - - GUI OBJECTs sist, pga inget z-värde. 

	// Add objects
	gui->AddGUIObject(gametimerText, "gametimerText");
	gui->AddGUIObject(fpsText, "fpsText");
	gui->AddGUIObject(healthFrame, "healthFrame");
	gui->AddGUIObject(actionbarLeft, "actionbarLeft");
	gui->AddGUIObject(actionbarRight, "actionbarRight");

	// Set GUI
	renderer->SetGUI(gui);


	gametimer.Start();
	//// Testing animation
	//Object* animation = AssimpHandler::loadFbxObject("Models/animation.fbx", dx11, defaultShader);
	//AddObject(animation);


	// Music last thing to load
	this->levelMusic = new SoundHandler();
	this->levelMusic->LoadSound("Levelsound", "SoundEffects/Ben.wav");
	this->levelMusic->PlaySound("Levelsound", 0.05f);
}

void DevScene::Unload()
{
	// @TODO
	gametimer.Restart();
	gametimer.Stop();
		
	this->levelMusic->StopSound();
}

void DevScene::Update(const float& deltaTime)
{	
	Scene::Update(deltaTime);

	//FPS STUFF
	fpsTimer.Start();			
	
	// Change all weapons to vector
	for(auto i : coconuts)
		player->UpdateHands(i);
	for (auto i : spoons)
		player->UpdateHands(i);
	//
	
		
	gametimerText->SetString("Timer: " + std::to_string(static_cast<int>(std::floor(gametimer.GetMilisecondsElapsed() / 1000.0))));
	controller->Update(deltaTime);
	
	fpsTimer.Stop();
	fpsText->SetString("FPS: " + std::to_string((int)(1 / ((fpsTimer.GetMicrosecondsElapsed() / 1000000)))));
	fpsTimer.Restart();
	checkForNextScene();

	gametimerText->SetString("Time until extraction: " + std::to_string(static_cast<int>(gametimer.GetTimeUntilEnd(timeUntilEnd))));
	


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

	if (canWin && player->GetWorldBounds().Overlaps(allObjects[0]->GetWorldBounds()))
	{
		gametimerText->SetString("You won");
		gametimerText->SetPosition(window.GetWidth() / 2.0f - 75.0f, 0.0f);
		SetNextScene(true);
	}
}

Scene* DevScene::GetNextScene() const
{
	return nextScene;
}

void DevScene::CreateSceneObjects()
{
	if (true)
	{
		// save the shaders somewhere, remember to clean it up
		Shader* defaultShader = new Shader();
		defaultShader->LoadPixelShader(L"Shaders/ToonShader_ps.hlsl", "main", dx11.GetDevice());
		defaultShader->LoadVertexShader(L"Shaders/ToonShader_vs.hlsl", "main", dx11.GetDevice());

		// Left beach stand
		Object* beachstand = AssimpHandler::loadFbxObject("Models/Beachstand.fbx", dx11, defaultShader);
		beachstand->GetTransform().Translate(35, 8, 75);
		beachstand->GetTransform().SetRotation({ 0, 1.6, 0 });
		AddObject(beachstand);

		// Boat
		Object* boat = AssimpHandler::loadFbxObject("Models/Boat.fbx", dx11, defaultShader);
		boat->GetTransform().Translate(160, 8.0f, 30);
		boat->GetTransform().SetRotation({ 0, 1.6, 0 });
		AddObject(boat);

		// Rocks
		Object* rocks = AssimpHandler::loadFbxObject("Models/Rocks.fbx", dx11, defaultShader);
		rocks->GetTransform().Scale(1, 1, 1);
		rocks->GetTransform().Translate(145, 7.5, 65);
		AddObject(rocks);

		

		// Chairs Left beachside
		Object* chair = AssimpHandler::loadFbxObject("Models/Sunchair.fbx", dx11, defaultShader);
		Object* chairs[5];
		for (int i = 0; i < 5; i++)
			chairs[i] = new Object(*chair);
				
		chairs[0]->GetTransform().Translate(15, 7.5f, 55);
		chairs[0]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(chairs[0]);
		
		chairs[1]->GetTransform().Translate(14, 7.0f, 58);
		chairs[1]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(chairs[1]);
		
		chairs[2]->GetTransform().Translate(13, 6.9f, 61);
		chairs[2]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(chairs[2]);
		
		chairs[3]->GetTransform().Translate(14, 7.0f, 68);
		chairs[3]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(chairs[3]);
		
		chairs[4]->GetTransform().Translate(13, 6.9f, 71);
		chairs[4]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(chairs[4]);

		// Benches left beachside
		Object* bench = AssimpHandler::loadFbxObject("Models/Bench.fbx", dx11, defaultShader);
		Object* benches[2];
		for (int i = 0; i < 2; i++)
			benches[i] = new Object(*bench);
				
		benches[0]->GetTransform().Translate(29, 7, 50);
		benches[0]->GetTransform().SetRotation({ 0,2,0 });
		AddObject(benches[0]);
				
		benches[1]->GetTransform().Translate(25, 7, 95);
		benches[1]->GetTransform().SetRotation({ 0,0.9,0 });
		AddObject(benches[1]);

		// Parasoll left beachside
		Object* parasoll = AssimpHandler::loadFbxObject("Models/Umbrella_BlueRed.fbx", dx11, defaultShader);
		Object* parasolls[2];
		for (int i = 0; i < 2; i++)
			parasolls[i] = new Object(*parasoll);

		parasolls[0]->GetTransform().Translate(20, 5.4, 60);
		parasolls[0]->GetTransform().Rotate(0.1, 0, -0.1);
		AddObject(parasolls[0]);

		parasolls[1]->GetTransform().Translate(17, 3.7, 80);
		parasolls[1]->GetTransform().Rotate(0, 0, -0.2);
		AddObject(parasolls[1]);

		// Balls left beachside
		Object* redball = AssimpHandler::loadFbxObject("Models/Beachball_Red.fbx", dx11, defaultShader);
		Object* redballs[2];
		for (int i = 0; i < 2; i++)
			redballs[i] = new Object(*redball);
				
		redballs[0]->GetTransform().Translate(25, 5.8, 65);
		redballs[0]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(redballs[0]);
				
		redballs[1]->GetTransform().Translate(125, 6, 155);
		redballs[1]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(redballs[1]);

		Object* blueball = AssimpHandler::loadFbxObject("Models/Beachball_Blue.fbx", dx11, defaultShader);
		Object* blueballs[2];
		for (int i = 0; i < 2; i++)
			blueballs[i] = new Object(*blueball);
				
		blueballs[0]->GetTransform().Translate(15, 6.5, 90);
		blueballs[0]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(blueballs[0]);
				
		blueballs[1]->GetTransform().Translate(130, 6.5, 150);
		blueballs[1]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(blueballs[1]);

		// Palms left beach side 
		Object* palm = AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, defaultShader);
		Object* palms[3];
		for (int i = 0; i < 3; i++)
			palms[i] = new Object(*palm);

		palms[0]->GetTransform().Translate(40, 9.5, 55);
		AddObject(palms[0]);

		palms[1]->GetTransform().Translate(45, 8, 75);
		AddObject(palms[1]);

		palms[2]->GetTransform().Translate(45, 8, 90);
		AddObject(palms[2]);

		// Mountains
		Object* mountain = AssimpHandler::loadFbxObject("Models/Mountain.fbx", dx11, defaultShader);
		Object* mountains[2];
		for (int i = 0; i < 2; i++)
			mountains[i] = new Object(*mountain);

		// Middle mountain
		mountains[0]->GetTransform().Translate(100, 7, 110);
		AddObject(mountains[0]);

		// Top left mountain
		mountains[1]->GetTransform().Translate(30, 4, 170);
		mountains[1]->GetTransform().Rotate(0, 180, 0);
		AddObject(mountains[1]);


		// Middle palms
		Object* middlePalms[5];
		for (int i = 0; i < 5; i++)
			middlePalms[i] = new Object(*palm);
		// 2 stycken �ver mitten berget
		middlePalms[0]->GetTransform().Translate(65, 8, 140);
		AddObject(middlePalms[0]);

		middlePalms[1]->GetTransform().Translate(85, 8.2, 145);
		AddObject(middlePalms[1]);

		// 3 under mitten berget
		middlePalms[2]->GetTransform().Translate(110, 9, 55);
		AddObject(middlePalms[2]);

		middlePalms[3]->GetTransform().Translate(105, 8, 75);
		AddObject(middlePalms[3]);

		middlePalms[4]->GetTransform().Translate(115, 8, 90);
		AddObject(middlePalms[4]);

		// palmer h�gra sidan
		Object* rightPalms[3];
		for (int i = 0; i < 3; i++)
			rightPalms[i] = new Object(*palm);

		rightPalms[0]->GetTransform().Translate(150, 8, 160);
		AddObject(rightPalms[0]);

		rightPalms[1]->GetTransform().Translate(155, 8, 150);
		AddObject(rightPalms[1]);

		rightPalms[2]->GetTransform().Translate(157, 8, 130);
		AddObject(rightPalms[2]);

		// Grass
		Object* grass = AssimpHandler::loadFbxObject("Models/Grass.fbx", dx11, defaultShader);
		Object* grassgroup[7];
		for (int i = 0; i < 7; i++)
			grassgroup[i] = new Object(*grass);

		// By Icecream wagon, left beach side
		grassgroup[0]->GetTransform().Translate(48, 9.4, 43);
		grassgroup[0]->GetTransform().SetRotation({ 0, 0, -0.05f });
		grassgroup[0]->GetTransform().Scale(1, 1, 1);
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
		Object* SurfboardBlue = AssimpHandler::loadFbxObject("Models/SurfboardBlue.fbx", dx11, defaultShader);
		Object* SurfboardOrange = AssimpHandler::loadFbxObject("Models/SurfboardOrange.fbx", dx11, defaultShader);
		Object* SurfboardTrippy = AssimpHandler::loadFbxObject("Models/SurfboardTrippy.fbx", dx11, defaultShader);

		SurfboardBlue->GetTransform().Translate(150, 8.5, 30);
		SurfboardBlue->GetTransform().SetRotation({ 0, 0, 0 });
		AddObject(SurfboardBlue);

		SurfboardOrange->GetTransform().Translate(148, 8.5, 28);
		SurfboardOrange->GetTransform().SetRotation({ 0, 0, 0 });
		AddObject(SurfboardOrange);

		SurfboardTrippy->GetTransform().Translate(146, 8.5, 30);
		SurfboardTrippy->GetTransform().SetRotation({ 0, 0, 0 });
		AddObject(SurfboardTrippy);

		Object* bungalow = AssimpHandler::loadFbxObject("Models/Bungalow.fbx", dx11, defaultShader);
		bungalow->GetTransform().Translate(110, 7.5, 140);
		bungalow->GetTransform().SetRotation({ 0, -2, 0 });
		AddObject(bungalow);

		//this->controller->SetFollow(&boat->GetTransform(), { 0, 10.0f, -10.0f });	


	}

}

void DevScene::AddSceneObject(Object* obj)
{

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
	//LevelObjects.push_back(obj);
}

void DevScene::AddPickups(Object* obj)
{
	//pickups.push_back(obj);
}

void DevScene::RemovePickup(Object* obj)
{
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

