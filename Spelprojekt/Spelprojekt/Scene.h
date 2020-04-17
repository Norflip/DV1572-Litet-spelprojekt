#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Player.h"

class Scene
{
public:
	Scene(Renderer* renderer, DX11Handler& dx11, Window& window);
	virtual ~Scene();

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(const float& deltaTime) = 0;
	virtual void FixedUpdate(const float& fixedDeltaTime) = 0;
	virtual Scene* GetNextScene() const = 0;

protected:
	Renderer* renderer;
	Window& window;
	DX11Handler& dx11;
};