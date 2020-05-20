#include "EndScene.h"

EndScene::EndScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, std::string sceneName, bool& exitGame, SoundHandler* soundeffect, Gamemanager* gamemanager) : Scene(sceneName, renderer, dx11, window), scenes(scenes), exitGame(exitGame)
{
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->nextScene = nullptr;
	this->controller = new CameraController(GetSceneCamera(), window.GetInput(), CameraController::State::Follow);
	window.GetInput()->LockCursor(false);

	this->didWin = false;
	this->soundeffects = soundeffect;

	this->gamemanager = gamemanager;
}

EndScene::~EndScene()
{
	delete controller;
}

void EndScene::Load()
{
	GUI* gui = new GUI(dx11);
	
	endGUI = new EndGUI(gui, dx11, controller, this, soundeffects, gamemanager);
	renderer->SetGUI(gui);

	Shader* defaultShader = new Shader();
	defaultShader->LoadPixelShader(L"Shaders/Default_ps.hlsl", "main", dx11.GetDevice());
	defaultShader->LoadVertexShader(L"Shaders/Default_vs.hlsl", "main", dx11.GetDevice());

}

void EndScene::Unload()
{
	this->gamemanager->GetMusicHandler()->StopSound();

	this->soundeffects->StopSound();
}

void EndScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	controller->Update(deltaTime);
	endGUI->Update();
}

Scene* EndScene::GetNextScene() const
{
	return nextScene;
}

void EndScene::SetNextScene(std::string whichScene)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (whichScene == "game")
		{
			if (scenes[i]->GetName() == "DevScene")
				nextScene = scenes[i];
		}
		
		if (whichScene == "intro")
		{
			if (scenes[i]->GetName() == "IntroScene")
			{
				nextScene = scenes[i];
			}
		}
	}
}

bool EndScene::getWinOrLose()
{
	return this->didWin;
}

