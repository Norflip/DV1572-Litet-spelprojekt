#include "HUD.h"

HUD::HUD(DX11Handler& dx11) : dx11(dx11)
{
	spriteBatch = new DirectX::SpriteBatch(dx11.GetContext());
	spriteFont = new DirectX::SpriteFont(dx11.GetDevice(), L"SpriteFonts/comic_sans_ms_16.spritefont");
}

void HUD::drawHUD(const wchar_t *display, float x, float y)
{
	spriteBatch->Begin();
	spriteFont->DrawString(
		spriteBatch,
		display,
		DirectX::XMFLOAT2(x, y),
		DirectX::Colors::White,
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f)
	);
	spriteBatch->End();
}
