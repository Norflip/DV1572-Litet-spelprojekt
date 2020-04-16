#include "GUI.h"

GUI::GUI(DX11Handler& dx11) : dx11(dx11)
{
	spriteBatch = new DirectX::SpriteBatch(dx11.GetContext());
}

void GUI::AddGUIObject(GUIObject *addObj)
{
	GUIObjects.push_back(addObj);
}

void GUI::DrawAll()
{
	spriteBatch->Begin();
	for (auto i : GUIObjects)
	{	
		i->Draw(spriteBatch);
	}
	spriteBatch->End();
}