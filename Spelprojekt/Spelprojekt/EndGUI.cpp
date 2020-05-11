#include "EndGUI.h"

EndGUI::EndGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, EndScene* scenes) : dx11(dx11)
{
    this->currentScene = scenes;
    this->gui = gui;
    this->input = cameraController->getInput();
}

EndGUI::~EndGUI()
{
}

void EndGUI::Update()
{
    switch (menu)
    {
    case Menu::restart:
        if (first)
            LoadStart();
        Restart();
        break;
    case Menu::quit:
        if (first)
            LoadQuit();
        Quit();
        break;
    }
}

void EndGUI::Restart()
{
    GUISprite* play = static_cast<GUISprite*>(gui->GetGUIList()->at("restart"));
    if (play->Clicked(input))
    {
        currentScene->SetNextScene("game");
    }
    GUISprite* quit = static_cast<GUISprite*>(gui->GetGUIList()->at("quit"));
    if (quit->Clicked(input))
    {
        menu = Menu::quit;
        first = true;
    }

    GUISprite* intro = static_cast<GUISprite*>(gui->GetGUIList()->at("menu"));
    if (intro->Clicked(input))
    {
        currentScene->SetNextScene("intro");
    }
}

void EndGUI::LoadStart()
{
    ClearGUI();
    GUISprite* winLose;
    
    if (currentScene->getWinOrLose() == true)
    {
       winLose = new GUISprite(dx11, "Sprites/youwon.png", 0.0f, 0.0f);
    }
    else
    {
        winLose = new GUISprite(dx11, "Sprites/youlose.png", 0.0f, 0.0f);
    }

    GUISprite* play = new GUISprite(dx11, "Sprites/restart.png", 0.0f, 0.0f);
    GUISprite* quit = new GUISprite(dx11, "Sprites/quit.png", 0.0f, 0.0f);
    GUISprite* menu = new GUISprite(dx11, "Sprites/backtointro.png", 0.0f, 0.0f);

    winLose->SetPosition((currentScene->getWindow().GetWidth() / 2.0f) - (winLose->GetTextureWidth() / 2.0f), 100.0f);
    play->SetPosition((currentScene->getWindow().GetWidth() / 2.0f) - (play->GetTextureWidth() / 2.0f), 250.0f);
    menu->SetPosition((currentScene->getWindow().GetWidth() / 2.0f) - (menu->GetTextureWidth() / 2.0f), 400.0f);
    quit->SetPosition((currentScene->getWindow().GetWidth() / 2.0f) - (quit->GetTextureWidth() / 2.0f), 550.0f);

    gui->AddGUIObject(winLose, "winorlose");
    gui->AddGUIObject(play, "restart");
    gui->AddGUIObject(quit, "quit");
    gui->AddGUIObject(menu, "menu");

    first = false;
}

void EndGUI::Quit()
{
    GUISprite* quitSprite = static_cast<GUISprite*>(gui->GetGUIList()->at("imsure"));
    if (quitSprite->Clicked(input))
    {
        currentScene->exitGame = true;
    }
    quitSprite = static_cast<GUISprite*>(gui->GetGUIList()->at("backtoendscreen"));
    if (quitSprite->Clicked(input))
    {
        first = true;
        menu = Menu::restart;
    }
}

void EndGUI::LoadQuit()
{
    ClearGUI();

    GUISprite* imsure = new GUISprite(dx11, "Sprites/imsure.png", 0.0f, 0.0f);
    GUISprite* backtoendscreen = new GUISprite(dx11, "Sprites/backtointro.png", 0.0f, 0.0f);

    imsure->SetPosition((currentScene->getWindow().GetWidth() / 2.0f) - (imsure->GetTextureWidth() / 2.0f), 200.0f);
    backtoendscreen->SetPosition((currentScene->getWindow().GetWidth() / 2.0f) - (backtoendscreen->GetTextureWidth() / 2.0f), 400.0f);

    gui->AddGUIObject(imsure, "imsure");
    gui->AddGUIObject(backtoendscreen, "backtoendscreen");

    first = false;
}

void EndGUI::ClearGUI()
{
    for (auto& it : *gui->GetGUIList())
    {
        // Do stuff
        GUIObject* test = it.second;
        delete test;
    }
    gui->GetGUIList()->clear();
}
