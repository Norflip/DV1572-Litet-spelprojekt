#pragma once
#include <SpriteFont.h>
#include "GUIObject.h"

class GUITextObject : public GUIObject
{
public:
	GUITextObject(DX11Handler&, const std::string& display, float x, float y);
	void Draw(DirectX::SpriteBatch*) override;
	std::string GetString() const;
	void SetString(const std::string& text);
private:
	DirectX::SpriteFont* spriteFont;
	std::string text;
	std::wstring txtDisplay;
	float x, y;

};