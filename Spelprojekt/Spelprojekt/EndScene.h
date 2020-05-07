#pragma once
#include "Scene.h"
#include "ShittyOBJLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "assimpHandler.h"

class EndScene : public Scene
{
public:
	EndScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, std::string sceneName);
	virtual ~EndScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	Scene* GetNextScene() const override;

private:
	void CheckForNextScene();
private:

	CameraController* controller;
	GUIText* winText;
	GUIText* menuText;
	GUIText* restartText;
	GUIText* infoText;

	std::vector<Scene*>& scenes;

	bool didWin;
};
