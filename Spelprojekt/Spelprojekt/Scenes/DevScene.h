#pragma once
#include "../Scene.h"
#include "../Logger.h"

class DevScene : public Scene
{
public:
	DevScene(Application*, Renderer*);
	virtual ~DevScene();

	void Update(const float& deltaTime) override;
};