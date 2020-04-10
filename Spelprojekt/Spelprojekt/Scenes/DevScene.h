#pragma once
#include "../Scene.h"
#include "../Logger.h"

class DevScene : public Scene
{
public:
	DevScene(Application*, Renderer*);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;

	Scene* GetNextScene() const override;
};