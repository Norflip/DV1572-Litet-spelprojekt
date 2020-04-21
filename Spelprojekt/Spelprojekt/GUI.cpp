#include "GUI.h"

GUI::GUI(DX11Handler& dx11) : dx11(dx11)
{
	spriteBatch = new DirectX::SpriteBatch(dx11.GetContext());
}

void GUI::AddGUIObject(GUIObject *addObj)
{
	GUIObjects.push_back(addObj);
}

void GUI::RemoveGUIObject(GUIObject* removeObj)
{
	auto found = std::find(GUIObjects.begin(), GUIObjects.end(), removeObj);
	if(found != GUIObjects.end())
	{
		GUIObjects.erase(found);
	}
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