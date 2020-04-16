#include "GUITextObject.h"

GUITextObject::GUITextObject(DX11Handler& dx11, const std::string& display, float x, float y)
{
	this->x = x;
	this->y = y;
	this->SetString(display);
	spriteFont = new DirectX::SpriteFont(dx11.GetDevice(), L"SpriteFonts/comic_sans_ms_16.spritefont");
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

void GUITextObject::SetString(const std::string& text)
{
	txtDisplay = std::wstring(text.begin(), text.end());
	this->text = text;
}
