#pragma once
#include <SpriteBatch.h>
#include "DX11Handler.h"


class GUIObject
{
public:
	GUIObject();
	virtual ~GUIObject() {};
	virtual void Draw(DirectX::SpriteBatch*) = 0;
	//void drawTexture();
private:
};

