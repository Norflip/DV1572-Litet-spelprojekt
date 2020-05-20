#include "EndGUI.h"

EndGUI::EndGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, EndScene* scenes, SoundHandler* soundeffect, Gamemanager* gamemanager) : dx11(dx11)
{
    this->currentScene = scenes;
    this->gui = gui;
    this->input = cameraController->getInput();
    this->soundeffects = soundeffect;

    this->gamemanager = gamemanager;
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
    GUISprite* restart = static_cast<GUISprite*>(gui->GetGUIList()->at("restart"));
    if (restart->Clicked(input))
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


    // MOUSEOVER
    // play again
    if (restart->MouseOver(input)) {
        restart->SetWICSprite(dx11, "Sprites/restart_mouseover.png");
    }
    else {
        restart->SetWICSprite(dx11, "Sprites/restart.png");
    }

    // back to main
    if (intro->MouseOver(input)) {
        intro->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        intro->SetWICSprite(dx11, "Sprites/backtointro.png");
    }

    if (quit->MouseOver(input)) {
        quit->SetWICSprite(dx11, "Sprites/quit_mouseover.png");
    }
    else {
        quit->SetWICSprite(dx11, "Sprites/quit.png");
    }

}

void EndGUI::LoadStart()
{
    ClearGUI();
    GUISprite* winLose;
    
    if (currentScene->getWinOrLose() == true)
    {
       winLose = new GUISprite(dx11, "Sprites/Glassbokal_Win.png", 0.0f, 0.0f);
    }
    else
    {
        winLose = new GUISprite(dx11, "Sprites/Glassbokal_lose.png", 0.0f, 0.0f);

        // Lose sound
        this->gamemanager->GetMusicHandler()->StopSound();
        if (!playedOnce) {
            this->soundeffects->LoadSound("Lose", "SoundEffects/Fail.wav");
            this->soundeffects->PlaySound("Lose", this->soundeffects->GetGlobalVolume());
            playedOnce = true;
        }
        
    }

    GUISprite* restart = new GUISprite(dx11, "Sprites/restart.png", 0.0f, 0.0f);       
    GUISprite* quit = new GUISprite(dx11, "Sprites/quit.png", 0.0f, 0.0f);          
    GUISprite* menu = new GUISprite(dx11, "Sprites/backtointro.png", 0.0f, 0.0f);   

    winLose->SetPosition((currentScene->GetWindow().GetWidth() / 2.0f) - (winLose->GetTextureWidth() / 2.0f), 0);
    restart->SetPosition((currentScene->GetWindow().GetWidth() / 2.0f) - (restart->GetTextureWidth() / 2.0f) + 400.0f, 300.0f);
    menu->SetPosition((currentScene->GetWindow().GetWidth() / 2.0f) - (menu->GetTextureWidth() / 2.0f) + 400.0f, 400.0f);
    quit->SetPosition((currentScene->GetWindow().GetWidth() / 2.0f) - (quit->GetTextureWidth() / 2.0f) + 400.0f, 500.0f);

    gui->AddGUIObject(winLose, "winorlose");
    gui->AddGUIObject(restart, "restart");
    gui->AddGUIObject(quit, "quit");
    gui->AddGUIObject(menu, "menu");

    first = false;
}

void EndGUI::Quit()
{
    GUISprite* quitimsure = static_cast<GUISprite*>(gui->GetGUIList()->at("imsure"));
    if (quitimsure->Clicked(input))
    {
        currentScene->exitGame = true;
    }
    GUISprite* backtoscreen = static_cast<GUISprite*>(gui->GetGUIList()->at("backtoendscreen"));
    if (backtoscreen->Clicked(input))
    {
        first = true;
        menu = Menu::restart;
    }

    if (quitimsure->MouseOver(input)) {
        quitimsure->SetWICSprite(dx11, "Sprites/imsure_mouseover.png");
    }
    else {
        quitimsure->SetWICSprite(dx11, "Sprites/imsure.png");
    }

    if (backtoscreen->MouseOver(input)) {
        backtoscreen->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        backtoscreen->SetWICSprite(dx11, "Sprites/backtointro.png");
    }



}

void EndGUI::LoadQuit()
{
    ClearGUI();

    GUISprite* imsure = new GUISprite(dx11, "Sprites/imsure.png", 0.0f, 0.0f);
    GUISprite* backtoendscreen = new GUISprite(dx11, "Sprites/backtointro.png", 0.0f, 0.0f);

    imsure->SetPosition((currentScene->GetWindow().GetWidth() / 2.0f) - (imsure->GetTextureWidth() / 2.0f), 300.0f);
    backtoendscreen->SetPosition((currentScene->GetWindow().GetWidth() / 2.0f) - (backtoendscreen->GetTextureWidth() / 2.0f), 400.0f);

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
