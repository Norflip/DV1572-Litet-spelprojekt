#pragma once
#include "../Scene.h"
#include "../Logger.h"
#include "../Camera.h"
#include <vector>
#include "../Object.h"
#include "../ShittyOBJLoader.h"
#include "../Input.h"
#include "../HeightMapGenerator.h"
class DevScene : public Scene
{
public:
	
	DevScene(Renderer* renderer, DX11Handler& dx11, Window& window);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;

	Scene* GetNextScene() const override;

private:
	std::vector<Object*> objects;
	Camera* camera;
};