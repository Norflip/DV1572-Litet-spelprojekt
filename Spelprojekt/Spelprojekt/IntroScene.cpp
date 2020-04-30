#include "IntroScene.h"



IntroScene::IntroScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*> scenes) : Scene(renderer, dx11, window)
{
	sceneName = "IntroScene";
	this->scenes = scenes;
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->controller = new CameraController(camera, window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	//Lights& lights = renderer->GetLights();
	//lights.SetSunDirection({ 1, -1, 0 });
	//lights.SetSunColor({ 0.98f, 0.96f, 0.73f, 1 });
	//lights.SetSunIntensity(0.6f);
}

IntroScene::~IntroScene()
{
	delete controller;
	delete camera;
}

void IntroScene::Load()
{
	input = window.GetInput();
	//healthFrame = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 700.0f);
	GUI* gui = new GUI(dx11);
	//gui->AddGUIObject(healthFrame);
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

	sphere->GetTransform().Translate(1, 1, 3);

	Object* glasse = AssimpHandler::loadFbxObject("Models/GlasseSmall.fbx", dx11, defaultShader);
	AddObject(glasse);
	Object* wagon = AssimpHandler::loadFbxObject("Models/Wagon.fbx", dx11, defaultShader);
	wagon->GetTransform().Translate(5, 5, 30);
	AddObject(wagon);


	controller->SetFollow(&wagon->GetTransform(), { 0, 10.0f, -10.0f });
	AddObject(sphere);

}

void IntroScene::Unload()
{
}

void IntroScene::Update(const float& deltaTime)
{



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

	size_t lastShaderID = -1;
	size_t lastMaterialID = -1;
	Material* currentMaterial = nullptr;

	for (auto shaderKey : sortedObjects)
	{
		for (auto materialKey : shaderKey.second)
		{
			for (auto object : materialKey.second)
			{
				if (object->IsEnabled() && camera->IsBoundsInView(object->GetWorldBounds()))
				{
					Material* material = object->GetMaterial();
					size_t shaderID = material->GetShader()->GetID();

					if (lastShaderID != shaderID)
					{
						material->GetShader()->Bind(dx11.GetContext());
						lastShaderID = shaderID;
					}

					if (lastMaterialID != material->GetID())
					{
						if (currentMaterial != nullptr)
							currentMaterial->Unbind(dx11.GetContext());

						currentMaterial = material;
						material->Bind(dx11.GetContext());
						lastMaterialID = material->GetID();
					}

					object->Render(renderer, view, projection);
				}
			}

			//material.first->Unbind(dx11.GetContext());
		}

		// shader unbind can become relevant if we add more then vs and ps shaders
	}

	UpdateAddRemoveSceneQueues();
	renderer->DisplayFrame(camera->GetTransform().GetPosition());



	checkForNextScene();
}

void IntroScene::FixedUpdate(const float& fixedDeltaTime)
{
	for (auto i : allObjects)
	{
		i->FixedUpdate(fixedDeltaTime);
	}
}

Scene* IntroScene::GetNextScene() const
{
	return nextScene;
}

void IntroScene::checkForNextScene()
{

	// Change scene logic
	if (input->GetKeyDown('h'))
	{
		for (int i = 0; i < scenes.size(); i++)
		{
			if (scenes[i]->getName() == "DevScene")
				nextScene = scenes[i];
		}
	}
}
