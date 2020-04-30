#include "IntroScene.h"



IntroScene::IntroScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*> scenes) : Scene(renderer, dx11, window)
{
	sceneName = "IntroScene";
	this->scenes = scenes;

}

IntroScene::~IntroScene()
{
}

void IntroScene::Load()
{
	input = window.GetInput();
}

void IntroScene::Unload()
{
}

void IntroScene::Update(const float& deltaTime)
{

	checkForNextScene();
}

void IntroScene::FixedUpdate(const float& fixedDeltaTime)
{

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
