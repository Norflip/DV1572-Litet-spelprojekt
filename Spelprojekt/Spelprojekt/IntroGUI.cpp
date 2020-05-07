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
            LoadOptions();
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
 
    // gotta fix if static cast fails
        GUISprite* play = static_cast<GUISprite*>(gui->GetGUIList()->at("play"));
        if (play->Clicked(input))
        {
            //ClearGUI();
            currentScene->setNextScene();
        }
        GUISprite* quit = static_cast<GUISprite*>(gui->GetGUIList()->at("quit"));
        if (quit->Clicked(input))
        {
            //ClearGUI();
            menu = Menu::quit;
            first = true;
        }
        GUISprite* options = static_cast<GUISprite*>(gui->GetGUIList()->at("options"));
        if (options->Clicked(input))
        {
            //ClearGUI();
            menu = Menu::options;
            first = true;
        }

}

void IntroGUI::LoadStart()
{
    ClearGUI();
    //LOAD ALL GUI OBJECTS FOR START, ONCE
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/play.png", 100.0f, 100.0f), "play");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/quit.png", 100.0f, 300.0f), "quit");  
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/options.png", 100.0f, 500.0f), "options");
    first = false;
}

void IntroGUI::Options()
{
    GUISprite* temp2 = static_cast<GUISprite*>(gui->GetGUIList()->at("backtointro"));
    if (temp2->Clicked(input))
    {
        ClearGUI();
        menu = Menu::start;
        LoadStart();

    }
}

void IntroGUI::LoadOptions()
{
    ClearGUI();
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/vsync.png", 100.0f, 100.0f), "vsync");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 300.0f), "backtointro");
    first = false;
}

void IntroGUI::Quit()
{

    GUISprite* temp2 = static_cast<GUISprite*>(gui->GetGUIList()->at("imsure"));
    if (temp2->Clicked(input))
    {
        currentScene->exitGame = true;
    }
    temp2 = static_cast<GUISprite*>(gui->GetGUIList()->at("backtointro"));
    if (temp2->Clicked(input))
    {
        ClearGUI();
        menu = Menu::start;
        LoadStart();

    }
}

void IntroGUI::LoadQuit()
{
    ClearGUI();
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/imsure.png", 400.0f, 300.0f), "imsure");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 10.0f, 300.0f), "backtointro");
    first = false;
}

void IntroGUI::ClearGUI()
{


    for (auto& it : *gui->GetGUIList()) 
    {
        // Do stuff
        GUIObject* test = it.second;
        delete test;
    }
    gui->GetGUIList()->clear();
}

