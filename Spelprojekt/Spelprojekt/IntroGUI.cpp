#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scene) : dx11(dx11)
{
    this->currentScene = scene;
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
            LoadQuit();
        Quit();
        break;
    }

}

void IntroGUI::Start()
{
    // DO SOME MOUSEOVER SHIT HERE

        GUISprite* temp = static_cast<GUISprite*>(gui->GetGUIList().at("test"));
        if (temp->Clicked(input))
        {
            currentScene->setNextScene();
        }
        GUISprite* temp2 = static_cast<GUISprite*>(gui->GetGUIList().at("test2"));
        if (temp2->Clicked(input))
        {
            ClearGUI();
            menu = Menu::quit;
            first = true;
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

    GUISprite* temp2 = static_cast<GUISprite*>(gui->GetGUIList().at("quitButton"));
    if (temp2->Clicked(input))
    {
        currentScene->exitGame = true;
    }
}

void IntroGUI::LoadQuit()
{
    GUISprite* StartGameSprite = new GUISprite(dx11, "Sprites/Frame.png", 400.0f, 300.0f);
    gui->AddGUIObject(StartGameSprite, "quitButton");
}

void IntroGUI::ClearGUI()
{

    gui->GetGUIList().clear();

}

