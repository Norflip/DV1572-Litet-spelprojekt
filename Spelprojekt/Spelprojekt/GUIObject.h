#pragma once
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include "DX11Handler.h"

class GUIObject
{
public:
	GUIObject();
	virtual ~GUIObject() {};
	virtual void Draw(DirectX::SpriteBatch*) = 0;
	virtual void SetPosition(float x, float y) = 0;

private:
};

