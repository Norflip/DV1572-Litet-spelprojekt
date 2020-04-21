#include "GUITextObject.h"

GUITextObject::GUITextObject()
{
	this->dxhandler = nullptr;
	this->spriteFont = 0;
	this->text = "";
	this->txtDisplay = L"";	
	this->x = 0.0f;
	this->y = 0.0f;
}

GUITextObject::GUITextObject(DX11Handler& dx11, const std::string& display, float x, float y)
{
	this->x = x;
	this->y = y;
	this->SetString(display);
	this->spriteFont = new DirectX::SpriteFont(dx11.GetDevice(), L"SpriteFonts/comic_sans_ms_16.spritefont");
}

GUITextObject::~GUITextObject()
{	
	if (dxhandler != nullptr) {
		dxhandler = nullptr;
	}

	if (spriteFont != nullptr) {
		spriteFont = nullptr;
	}
}

void GUITextObject::Draw(DirectX::SpriteBatch* spriteBatch)
{
	spriteFont->DrawString(
		spriteBatch,
		txtDisplay.c_str(),
		DirectX::XMFLOAT2(x, y),
		DirectX::Colors::White,
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f)
	);
}

std::string GUITextObject::GetString() const
{
	return text;
}

void GUITextObject::SetDXHandler(DX11Handler& dx11)
{
	this->dxhandler = &dx11;
}

void GUITextObject::SetFont(const wchar_t* font)
{
	this->spriteFont = new DirectX::SpriteFont(this->dxhandler->GetDevice(), font);
}

void GUITextObject::SetString(const std::string& text)
{
	txtDisplay = std::wstring(text.begin(), text.end());
	this->text = text;
}

void GUITextObject::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
