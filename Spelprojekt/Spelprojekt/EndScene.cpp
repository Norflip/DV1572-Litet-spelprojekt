#include "EndScene.h"

EndScene::EndScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, std::string sceneName) : Scene(sceneName, renderer, dx11, window), scenes(scenes)
{
	this->nextScene = nullptr;
	this->controller = new CameraController(GetSceneCamera(), window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	if (sceneName == "WinScene")
	{
		winText = new GUIText(dx11, "You Win ", window.GetWidth() / 2.0f - 200.0f, 200.0f);
		winText->SetFontColor(DirectX::Colors::Green);
	}
	else if (sceneName == "GameOverScene")
	{
		winText = new GUIText(dx11, "You Lose ", window.GetWidth() / 2.0f - 200.0f, 200.0f);
		winText->SetFontColor(DirectX::Colors::Red);
	}
	winText->SetFontSize(DirectX::XMFLOAT2(5.0f, 5.0f));


	menuText = new GUIText(dx11, "Menu", window.GetWidth() / 2.0f - 150.0f, 300.0f);
	menuText->SetFontSize(DirectX::XMFLOAT2(4.0f, 4.0f));

	restartText = new GUIText(dx11, "Restart", window.GetWidth() / 2.0f - 150.0f, 400.0f);
	restartText->SetFontSize(DirectX::XMFLOAT2(4.0f, 4.0f));

	infoText = new GUIText(dx11, "Press h to go to gameScene \nPress n to go to introScene", window.GetWidth() / 2.0f - 150.0f, 500.0f);
	infoText->SetFontSize(DirectX::XMFLOAT2(1.0f, 1.0f));
	infoText->SetFontColor(DirectX::Colors::Black);
}

EndScene::~EndScene()
{
	delete controller;
	delete winText;
}

void EndScene::Load()
{
	GUI* gui = new GUI(dx11);

	gui->AddGUIObject(winText, "Win");
	gui->AddGUIObject(menuText, "Menu");
	gui->AddGUIObject(restartText, "Restart");
	gui->AddGUIObject(infoText, "Info");
	renderer->SetGUI(gui);

	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

}

void EndScene::Unload()
{
}

void EndScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	controller->Update(deltaTime);
	CheckForNextScene();
}

Scene* EndScene::GetNextScene() const
{
	return nextScene;
}

void EndScene::CheckForNextScene()
{
	Input* input = this->window.GetInput();

	// Change scene logic
	if (input->GetKeyDown('h'))
	{
		for (int i = 0; i < scenes.size(); i++)
		{
			if (scenes[i]->GetName() == "DevScene")
				nextScene = scenes[i];
		}
	}

	if (input->GetKeyDown('n'))
	{
		for (int i = 0; i < scenes.size(); i++)
		{
			if (scenes[i]->GetName() == "IntroScene")
			{
				nextScene = scenes[i];
			}
		}
	}
}

