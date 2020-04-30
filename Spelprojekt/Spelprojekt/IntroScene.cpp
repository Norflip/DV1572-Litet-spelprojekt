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
	if (input->GetKeyDown('h'))
	{

	}
}

void IntroScene::FixedUpdate(const float& fixedDeltaTime)
{

}

Scene* IntroScene::GetNextScene() const
{
	return nullptr;
}
