#pragma once
#include "GUIObject.h"

class GUITextObject : public GUIObject
{
public:
	GUITextObject();
	GUITextObject(DX11Handler&, const std::string& display, float x, float y);
	~GUITextObject();
	void Draw(DirectX::SpriteBatch*) override;	
	void SetPosition(float x, float y) override;

	void SetDXHandler(DX11Handler&);
	void SetFont(const wchar_t* font);
	void SetString(const std::string& text);

	std::string GetString() const;

private:
	DX11Handler* dxhandler;
	DirectX::SpriteFont* spriteFont;
	std::string text;
	std::wstring txtDisplay;
	float x, y;
};