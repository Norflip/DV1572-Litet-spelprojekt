#pragma once
#include <vector>
#include <SpriteFont.h>
#include "GUIObject.h"
#include "DX11Handler.h"
#include "Timer.h"

class GUI
{
public:
	GUI(DX11Handler&);
	~GUI() {};
	void AddGUIObject(GUIObject *addObj);
	void DrawAll();
private:
	std::vector<GUIObject*> GUIObjects;
	DirectX::SpriteBatch* spriteBatch;
	DX11Handler& dx11;
};