#pragma once
#include <vector>
#include <SpriteFont.h>
#include <unordered_map>
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
	void AddGUIObject(GUIObject *addObj, std::string name);
	void AddGUIButtonObject(GUIObject* addButton, std::string name);
	void RemoveGUIObject(std::string name);
	DX11Handler& GetDXHandler() { return dx11; }
	std::unordered_map<std::string, GUIObject*>*  GetGUIList() { return &this->GUIObjects; };
	std::unordered_map<std::string, GUIObject*>* GetGUIButtonList() { return &this->GUIButtons; };
	void DrawAll();

private:
	std::unordered_map<std::string, GUIObject*> GUIObjects;
	std::unordered_map<std::string, GUIObject*> GUIButtons;
	DirectX::SpriteBatch* spriteBatch;
	DX11Handler& dx11;
};