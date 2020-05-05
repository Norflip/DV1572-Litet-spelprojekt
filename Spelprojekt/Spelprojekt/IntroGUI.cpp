#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11) : dx11(dx11)
{
    this->gui = gui;
    
    GUISprite* StartGameSprite = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 700.0f);
    gui->AddGUIObject(StartGameSprite);
}

IntroGUI::~IntroGUI()
{
}

void IntroGUI::Update()
{
    switch (menu)
    {
    case Menu::start:
        if (first)
            LoadStart();
        Start();
        break;
    case Menu::options:
        if (first)
            LoadStart();
        Options();
        break;
    case Menu::quit:
        if (first)
            LoadStart();
        Quit();
        break;
    }

}

void IntroGUI::Start()
{
}

void IntroGUI::LoadStart()
{
}

void IntroGUI::Options()
{
}

void IntroGUI::LoadOptions()
{
}

void IntroGUI::Quit()
{
}

void IntroGUI::LoadQuit()
{
}

