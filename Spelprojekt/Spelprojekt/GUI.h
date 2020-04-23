#pragma once
#include <vector>
#include <SpriteFont.h>

// New
#include "GUISprite.h"
#include "GUIText.h"
#include "GUIActionbar.h"

#include "DX11Handler.h"
#include "Timer.h"

class GUI
{
public:
	GUI(DX11Handler&);
	~GUI() {};
	void AddGUIObject(GUIObject *addObj);
	void RemoveGUIObject(GUIObject* removeObj);
	DX11Handler& GetDXHandler() { return dx11; }

	void DrawAll();
private:
	std::vector<GUIObject*> GUIObjects;

	DirectX::SpriteBatch* spriteBatch;
	DX11Handler& dx11;
};