#include "DevScene.h"

DevScene::DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, SoundHandler* sound) : Scene("DevScene", renderer, dx11, window), scenes(scenes)
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

	// Soundhandler
	this->levelMusic = sound;	
}

DevScene::~DevScene()
{
	delete controller;
}

void DevScene::Load()
{		

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
	wagon->GetTransform().Translate(99, 9.5, 50);
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

	
	Texture* sand_texture = Texture::CreateTexture("Textures/Sand_Color_Test.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);	
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
	water->GetTransform().Translate({ 0,5,0,});
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
	this->player->GetTransform().SetPosition({ 55, 7, 55 });
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

	

	// Play scenemusic
	this->levelMusic->StopSound();
	this->levelMusic->LoadSound("Levelsound", "SoundEffects/Ben.wav");
	//levelMusic->PlaySound("Levelsound", levelMusic->GetGlobalVolume());
}

void DevScene::Unload()
{
	// @TODO
	gametimer.Restart();
	gametimer.Stop();
		
	//this->levelMusic->StopSound();
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

		
		////////////////////////// MOUNTAINS /////////////////////////////
		Object* mountain = AssimpHandler::loadFbxObject("Models/Mountain.fbx", dx11, defaultShader);
		Object* mountains[2];
		for (int i = 0; i < 2; i++)
			mountains[i] = new Object(*mountain);

		// Middle mountain
		mountains[0]->GetTransform().Translate(100, 6, 110);
		AddObject(mountains[0]);

		// Top left mountain
		mountains[1]->GetTransform().Translate(30, 4, 170);
		mountains[1]->GetTransform().Rotate(0, 180, 0);
		AddObject(mountains[1]);

		////////////////////////// ROCKS /////////////////////////////
		Object* rocks = AssimpHandler::loadFbxObject("Models/Rocks.fbx", dx11, defaultShader);
		rocks->GetTransform().Rotate(0, 180, 0);
		rocks->GetTransform().Translate(195, 5, 145);
		AddObject(rocks);

		////////////////////////// BOATS /////////////////////////////
		Object* boat = AssimpHandler::loadFbxObject("Models/Boat.fbx", dx11, defaultShader);
		boat->GetTransform().Translate(205, 6.2f, 115);
		boat->GetTransform().SetRotation({ 0.1, 1.7, -0.15});
		AddObject(boat);

		////////////////////////// STANDS /////////////////////////////
		// Left beach stand
		Object* beachstand = AssimpHandler::loadFbxObject("Models/Beachstand.fbx", dx11, defaultShader);
		Object* beachstands[2]; 
		for(int i = 0; i < 2; i++)
			beachstands[i] = new Object(*beachstand);

		beachstands[0]->GetTransform().Translate(135, 7, 48);
		AddObject(beachstands[0]);

		beachstands[1]->GetTransform().Translate(125, 7, 48);
		beachstands[1]->GetTransform().SetRotation({ -0.1, -0.1, 0 });
		AddObject(beachstands[1]);
		

		////////////////////////// SUNCHAIR /////////////////////////////
		
		// Chairs Left beachside
		Object* chair = AssimpHandler::loadFbxObject("Models/Sunchair.fbx", dx11, defaultShader);
		Object* sunChairs[18];
		for (int i = 0; i < 18; i++)
			sunChairs[i] = new Object(*chair);
				
		sunChairs[0]->GetTransform().Translate(200, 6.5, 40);
		sunChairs[0]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[0]);
				
		sunChairs[1]->GetTransform().Translate(195, 6.5, 39.8);
		sunChairs[1]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[1]);
		
		sunChairs[2]->GetTransform().Translate(190, 6.5, 39.6);
		sunChairs[2]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[2]);
				
		
		sunChairs[3]->GetTransform().Translate(175, 6.5, 39.6);
		sunChairs[3]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[3]);
		
		sunChairs[4]->GetTransform().Translate(170, 6.5, 39.4);
		sunChairs[4]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[4]);

		sunChairs[5]->GetTransform().Translate(165, 6.5, 39.2);
		sunChairs[5]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[5]);

		
		sunChairs[6]->GetTransform().Translate(150, 6.5, 39.2);
		sunChairs[6]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[6]);

		sunChairs[7]->GetTransform().Translate(145, 6.5, 39.2);
		sunChairs[7]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[7]);

		sunChairs[8]->GetTransform().Translate(140, 6.5, 39.2);
		sunChairs[8]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[8]);


		sunChairs[9]->GetTransform().Translate(120, 6.5, 38);
		sunChairs[9]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[9]);

		sunChairs[10]->GetTransform().Translate(115, 6.5, 37.6);
		sunChairs[10]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[10]);

		sunChairs[11]->GetTransform().Translate(110, 6.5, 37.4);
		sunChairs[11]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[11]);


		sunChairs[12]->GetTransform().Translate(90, 6.5, 34);
		sunChairs[12]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[12]);

		sunChairs[13]->GetTransform().Translate(85, 6.5, 34);
		sunChairs[13]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[13]);

		sunChairs[14]->GetTransform().Translate(80, 6.5, 34);
		sunChairs[14]->GetTransform().Rotate(-0.1f, -6.3, 0);
		AddObject(sunChairs[14]);


		sunChairs[15]->GetTransform().Translate(35, 6.5, 57);
		sunChairs[15]->GetTransform().Rotate(-0.1f, -5.7, 0);
		AddObject(sunChairs[15]);

		sunChairs[16]->GetTransform().Translate(40, 6.5, 55);
		sunChairs[16]->GetTransform().Rotate(-0.1f, -5.5, 0);
		AddObject(sunChairs[16]);

		sunChairs[17]->GetTransform().Translate(42, 6.5, 51);
		sunChairs[17]->GetTransform().Rotate(-0.1f, -5.3, 0);
		AddObject(sunChairs[17]);

		//this->controller->SetFollow(&sunChairs[0]->GetTransform(), { 0, 10.0f, -10.0f });


		////////////////////////// PARASOLLS /////////////////////////////

		Object* parasoll = AssimpHandler::loadFbxObject("Models/Umbrella_BlueRed.fbx", dx11, defaultShader);
		Object* parasolls[6];
		for (int i = 0; i < 6; i++)
			parasolls[i] = new Object(*parasoll);

		parasolls[0]->GetTransform().Translate(192.5, 5.3, 40);
		AddObject(parasolls[0]);

		parasolls[1]->GetTransform().Translate(172.5, 5.4, 40);
		AddObject(parasolls[1]);

		parasolls[2]->GetTransform().Translate(147.5, 5.5, 40);
		AddObject(parasolls[2]);

		parasolls[3]->GetTransform().Translate(117.5, 5.6, 38);
		parasolls[3]->GetTransform().Rotate(-0.05, 0, 0);
		AddObject(parasolls[3]);

		parasolls[4]->GetTransform().Translate(87.5, 5.6, 35);
		parasolls[4]->GetTransform().Rotate(-0.05, 0, 0);
		AddObject(parasolls[4]);

		parasolls[5]->GetTransform().Translate(45, 5.6, 55);
		parasolls[5]->GetTransform().Rotate(-0.05, 0, 0);
		AddObject(parasolls[5]);


		////////////////////////// SURFBOARDS /////////////////////////////

		Object* SurfboardBlue = AssimpHandler::loadFbxObject("Models/SurfboardBlue.fbx", dx11, defaultShader);
		Object* SurfboardOrange = AssimpHandler::loadFbxObject("Models/SurfboardOrange.fbx", dx11, defaultShader);
		Object* SurfboardTrippy = AssimpHandler::loadFbxObject("Models/SurfboardTrippy.fbx", dx11, defaultShader);

		SurfboardBlue->GetTransform().Translate(130, 7, 35);
		SurfboardBlue->GetTransform().SetRotation({ 0, 0, 0 });
		AddObject(SurfboardBlue);

		SurfboardOrange->GetTransform().Translate(128, 7, 33);
		SurfboardOrange->GetTransform().SetRotation({ 0, 0, 0 });
		AddObject(SurfboardOrange);

		SurfboardTrippy->GetTransform().Translate(130, 7, 37);
		SurfboardTrippy->GetTransform().SetRotation({ 0, 0, 0 });
		AddObject(SurfboardTrippy);

		////////////////////////// BEACHBALLS /////////////////////////////

		// Balls left beachside
		Object* redball = AssimpHandler::loadFbxObject("Models/Beachball_Red.fbx", dx11, defaultShader);
		Object* redballs[2];
		for (int i = 0; i < 2; i++)
			redballs[i] = new Object(*redball);
				
		redballs[0]->GetTransform().Translate(100, 7, 38);
		redballs[0]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(redballs[0]);
				
		redballs[1]->GetTransform().Translate(160, 7, 190);
		redballs[1]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(redballs[1]);

		Object* blueball = AssimpHandler::loadFbxObject("Models/Beachball_Blue.fbx", dx11, defaultShader);
		Object* blueballs[2];
		for (int i = 0; i < 2; i++)
			blueballs[i] = new Object(*blueball);
				
		blueballs[0]->GetTransform().Translate(105,6.5, 35);
		blueballs[0]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(blueballs[0]);
				
		blueballs[1]->GetTransform().Translate(155, 6.5, 195);
		blueballs[1]->GetTransform().Rotate(0.2f, -5, 0);
		AddObject(blueballs[1]);

		//// Benches left beachside
		//Object* bench = AssimpHandler::loadFbxObject("Models/Bench.fbx", dx11, defaultShader);
		//Object* benches[2];
		//for (int i = 0; i < 2; i++)
		//	benches[i] = new Object(*bench);
		//		
		//benches[0]->GetTransform().Translate(29, 7, 50);
		//benches[0]->GetTransform().SetRotation({ 0,2,0 });
		//AddObject(benches[0]);
		//		
		//benches[1]->GetTransform().Translate(25, 7, 95);
		//benches[1]->GetTransform().SetRotation({ 0,0.9,0 });
		//AddObject(benches[1]);

		

		

		//// Palms left beach side 
		//Object* palm = AssimpHandler::loadFbxObject("Models/Palm.fbx", dx11, defaultShader);
		//Object* palms[3];
		//for (int i = 0; i < 3; i++)
		//	palms[i] = new Object(*palm);

		//palms[0]->GetTransform().Translate(40, 9.5, 55);
		//AddObject(palms[0]);

		//palms[1]->GetTransform().Translate(45, 8, 75);
		//AddObject(palms[1]);

		//palms[2]->GetTransform().Translate(45, 8, 90);
		//AddObject(palms[2]);

		


		//// Middle palms
		//Object* middlePalms[5];
		//for (int i = 0; i < 5; i++)
		//	middlePalms[i] = new Object(*palm);
		//// 2 stycken �ver mitten berget
		//middlePalms[0]->GetTransform().Translate(65, 8, 140);
		//AddObject(middlePalms[0]);

		//middlePalms[1]->GetTransform().Translate(85, 8.2, 145);
		//AddObject(middlePalms[1]);

		//// 3 under mitten berget
		//middlePalms[2]->GetTransform().Translate(110, 9, 55);
		//AddObject(middlePalms[2]);

		//middlePalms[3]->GetTransform().Translate(105, 8, 75);
		//AddObject(middlePalms[3]);

		//middlePalms[4]->GetTransform().Translate(115, 8, 90);
		//AddObject(middlePalms[4]);

		//// palmer h�gra sidan
		//Object* rightPalms[3];
		//for (int i = 0; i < 3; i++)
		//	rightPalms[i] = new Object(*palm);

		//rightPalms[0]->GetTransform().Translate(150, 8, 160);
		//AddObject(rightPalms[0]);

		//rightPalms[1]->GetTransform().Translate(155, 8, 150);
		//AddObject(rightPalms[1]);

		//rightPalms[2]->GetTransform().Translate(157, 8, 130);
		//AddObject(rightPalms[2]);

		

		Object* bungalow = AssimpHandler::loadFbxObject("Models/Bungalow.fbx", dx11, defaultShader);
		bungalow->GetTransform().Translate(110, 7.5, 140);
		bungalow->GetTransform().SetRotation({ 0, -2, 0 });
		AddObject(bungalow);

		//// Grass
		//Object* grass = AssimpHandler::loadFbxObject("Models/Grass.fbx", dx11, defaultShader);
		//Object* grassgroup[7];
		//for (int i = 0; i < 7; i++)
		//	grassgroup[i] = new Object(*grass);

		//// By Icecream wagon, left beach side
		//grassgroup[0]->GetTransform().Translate(48, 9.4, 43);
		//grassgroup[0]->GetTransform().SetRotation({ 0, 0, -0.05f });
		//grassgroup[0]->GetTransform().Scale(1, 1, 1);
		//AddObject(grassgroup[0]);

		//grassgroup[1]->GetTransform().Translate(45, 9, 77);
		//grassgroup[1]->GetTransform().Scale(1, 1, 1);
		//AddObject(grassgroup[1]);
		////

		//// Right Beach side, under mid mountain
		//grassgroup[2]->GetTransform().Translate(115, 10.5, 65);
		//grassgroup[2]->GetTransform().Scale(1, 1, 1);
		//grassgroup[2]->GetTransform().SetRotation({ 0.05, 0, 0 });
		//AddObject(grassgroup[2]);

		//grassgroup[3]->GetTransform().Translate(135, 10.2, 60);
		//grassgroup[3]->GetTransform().Scale(1, 1, 1);
		//grassgroup[3]->GetTransform().SetRotation({ 0.05, 0, -0.05f });
		//AddObject(grassgroup[3]);

		//grassgroup[4]->GetTransform().Translate(155, 10, 55);
		//grassgroup[4]->GetTransform().Scale(1, 1, 1);
		//grassgroup[4]->GetTransform().SetRotation({ 0.05, 0, 0.05f });
		//AddObject(grassgroup[4]);

		//// Palms right side top
		//grassgroup[5]->GetTransform().Translate(158, 10, 135);
		//grassgroup[5]->GetTransform().Scale(1, 1, 1);
		//grassgroup[5]->GetTransform().SetRotation({ 0.05, 0, -0.05f });
		//AddObject(grassgroup[5]);

		//grassgroup[6]->GetTransform().Translate(158, 10, 145);
		//grassgroup[6]->GetTransform().Scale(1, 1, 1);
		//grassgroup[6]->GetTransform().SetRotation({ 0.05, 0, 0.05f });
		//AddObject(grassgroup[6]);
		////

		

		


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

