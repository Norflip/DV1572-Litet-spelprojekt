#include <SpriteFont.h>

#include "DX11Handler.h"

class HUD
{
	public:
		HUD(DX11Handler&);
		~HUD() {};
		void drawHUD(const wchar_t *display, float x, float y);
	private:
		DX11Handler& dx11;
		DirectX::SpriteBatch *spriteBatch;
		DirectX::SpriteFont *spriteFont;
};