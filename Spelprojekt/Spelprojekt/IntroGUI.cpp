#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController) : dx11(dx11)
{
    this->gui = gui;
    this->input = cameraController->getInput();
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
    // DO SOME MOUSEOVER SHIT HERE
    if (true)
    {

    }

}

void IntroGUI::LoadStart()
{

    //LOAD ALL GUI OBJECTS FOR START, ONCE
    GUISprite* StartGameSprite = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 300.0f);
    gui->AddGUIObject(StartGameSprite, "test");
    StartGameSprite = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 500.0f);
    gui->AddGUIObject(StartGameSprite, "test2");
    StartGameSprite = new GUISprite(dx11, "Sprites/Frame.png", 10.0f, 700.0f);
    gui->AddGUIObject(StartGameSprite, "test3");
    first = false;
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

void IntroGUI::ClearGUI()
{
 
    gui->GetGUIList().clear();
}

