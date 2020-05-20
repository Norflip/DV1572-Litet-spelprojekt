#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scene, Gamemanager* gamemanager) : dx11(dx11)    // tabrot soundeffect
{
    this->currentScene = scene;
    this->gui = gui;
    this->input = cameraController->getInput();

    this->lastOff = "Sprites/off_isON.png";
    this->lastOn = "Sprites/on_isOFF.png";

    this->oneChecked = true;
    this->twoChecked = false;
    this->threeChecked = false;
      
    this->gamemanager = gamemanager;
}

IntroGUI::~IntroGUI()
{    
}

void IntroGUI::Update()
{
    ///////////////////////////////////////

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
    case Menu::soundtracks: 
        if (first)
            LoadSoundtracks();
        Soundtracks();
        break;
    case Menu::quit:
        if (first)
            LoadQuit();
        Quit();
        break;
    }

    ///////////////////////////////////////
       
    
        /*if (gamemanager->GetCurrentTrack() == 1) {
            soundtrack = Soundtrack::Track1;
        }
        if (gamemanager->GetCurrentTrack() == 2) {
            soundtrack = Soundtrack::Track2;
        }
        if (gamemanager->GetCurrentTrack() == 3) {
            soundtrack = Soundtrack::Track3;
        }


        switch (soundtrack) {
        case Soundtrack::Track1:
            oneChecked = true;
            twoChecked = false;
            threeChecked = false;
            gamemanager->SetMusictrack("SoundEffects/Ben.wav");
            switchTrack = false;
            break;
        case Soundtrack::Track2:
            oneChecked = false;
            twoChecked = true;
            threeChecked = false;
            gamemanager->SetMusictrack("SoundEffects/FluffingDuck.wav");
            switchTrack = false;
            break;
        case Soundtrack::Track3:
            oneChecked = false;
            twoChecked = false;
            threeChecked = true;
            gamemanager->SetMusictrack("SoundEffects/Cuphead.wav");
            switchTrack = false;
            break;
        }*/

        switch (gamemanager->GetCurrentTrack()) {
        case 1:
            oneChecked = true;
            twoChecked = false;
            threeChecked = false;
            gamemanager->SetMusictrack("SoundEffects/Ben.wav");
            switchTrack = false;
            break;
        case 2:
            oneChecked = false;
            twoChecked = true;
            threeChecked = false;
            gamemanager->SetMusictrack("SoundEffects/FluffingDuck.wav");
            switchTrack = false;
            break;
        case 3:
            oneChecked = false;
            twoChecked = false;
            threeChecked = true;
            gamemanager->SetMusictrack("SoundEffects/Cuphead.wav");
            switchTrack = false;
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

    GUISprite* options = static_cast<GUISprite*>(gui->GetGUIList()->at("options"));
    if (options->Clicked(input))
    {
        //ClearGUI();
        menu = Menu::options;
        first = true;
    }

    // SOUNDTRACK
    GUISprite* soundtrack = static_cast<GUISprite*>(gui->GetGUIList()->at("soundtracks"));
    if (soundtrack->Clicked(input))
    {
        //ClearGUI();
        menu = Menu::soundtracks;
        first = true;
    }

    GUISprite* quit = static_cast<GUISprite*>(gui->GetGUIList()->at("quit"));
    if (quit->Clicked(input))
    {
        //ClearGUI();
        menu = Menu::quit;
        first = true;
    }
                


    // MOUSEOVER
    // To options
    if (options->MouseOver(input)) {
        options->SetWICSprite(dx11, "Sprites/options_mouseover.png");
    }
    else {
        options->SetWICSprite(dx11, "Sprites/options.png");
    }

    // play
    if (play->MouseOver(input)) {
        play->SetWICSprite(dx11, "Sprites/play_mouseover.png");
    }
    else {
        play->SetWICSprite(dx11, "Sprites/play.png");
    }

    //quit
    if (quit->MouseOver(input)) {
        quit->SetWICSprite(dx11, "Sprites/quit_mouseover.png");
    }
    else {
        quit->SetWICSprite(dx11, "Sprites/quit.png");
    }
                
    // Soundtrack
    if (soundtrack->MouseOver(input)) {
        soundtrack->SetWICSprite(dx11, "Sprites/soundtracks_mouseover.png");
    }
    else {
        soundtrack->SetWICSprite(dx11, "Sprites/soundtracks.png");
    }

}

void IntroGUI::LoadStart()
{
    ClearGUI();
    
    //LOAD ALL GUI OBJECTS FOR START, ONCE
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/play.png", 100.0f, 200.0f), "play");    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/options.png", 100.0f, 300.0f), "options");
    // SOUNDTRACK
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/soundtracks.png", 100.0f, 400.0f), "soundtracks");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/quit.png", 100.0f, 500.0f), "quit");
    first = false;
}

void IntroGUI::Options()
{   
    // Set current volume for music
    GUISprite* musicVolBar = static_cast<GUISprite*>(gui->GetGUIList()->at("MusicBar"));    

    GUISprite* lowMusicVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("leftmusicvolume"));
    if (lowMusicVolume->Clicked(input) && gamemanager->GetCurrentMusicVolume() != 0)
    {
        if (gamemanager->GetCurrentMusicVolume() > 0) {
            currentMusicVolume = gamemanager->GetCurrentMusicVolume();
            currentMusicVolume -= volumeScale;
            gamemanager->SetCurrentMusicVolume(currentMusicVolume);
            gamemanager->GetMusicHandler()->SetGlobalVolume(gamemanager->GetCurrentMusicVolume());
        }      
    }

    GUISprite* highMusicVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("rightmusicvolume"));    
    if (highMusicVolume->Clicked(input) && gamemanager->GetCurrentMusicVolume() != 1)
    {
        if (gamemanager->GetCurrentMusicVolume() < 1 )
        {
            currentMusicVolume = gamemanager->GetCurrentMusicVolume();
            currentMusicVolume += volumeScale;
            gamemanager->SetCurrentMusicVolume(currentMusicVolume);
            gamemanager->GetMusicHandler()->SetGlobalVolume(gamemanager->GetCurrentMusicVolume());
        }           
    }

    musicVolBar->VolumeBar(maxVolume, gamemanager->GetCurrentMusicVolume());

    // mouseover
    if (lowMusicVolume->MouseOver(input)) {
        lowMusicVolume->SetWICSprite(dx11, "Sprites/VolLower_mouseover.png");
    }
    else {
        lowMusicVolume->SetWICSprite(dx11, "Sprites/VolLower.png");
    }

    if (highMusicVolume->MouseOver(input)) {
        highMusicVolume->SetWICSprite(dx11, "Sprites/VolHigher_mouseover.png");
    }
    else {
        highMusicVolume->SetWICSprite(dx11, "Sprites/VolHigher.png");
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////


    // Set current volume for sounds
    GUISprite* soundsVolBar = static_cast<GUISprite*>(gui->GetGUIList()->at("SoundsBar"));    

    GUISprite* lowSoundVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("leftsoundvolume"));
    if (lowSoundVolume->Clicked(input) && gamemanager->GetCurrentSoundVolume() != 0)
    {
        if (gamemanager->GetCurrentSoundVolume() > 0.0f) {
            currentSoundVolume = gamemanager->GetCurrentSoundVolume();
            currentSoundVolume -= volumeScale;
            gamemanager->SetCurrentSoundVolume(currentSoundVolume);
            gamemanager->GetSoundeffectHandler()->SetGlobalVolume(gamemanager->GetCurrentSoundVolume());
        }
    }

    GUISprite* highSoundVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("rightsoundvolume"));
    if (highSoundVolume->Clicked(input) && gamemanager->GetCurrentSoundVolume() != 1)
    {
        if (gamemanager->GetCurrentSoundVolume() < 1.0f)
        {
            currentSoundVolume = gamemanager->GetCurrentSoundVolume();
            currentSoundVolume += volumeScale;
            gamemanager->SetCurrentSoundVolume(currentSoundVolume);
            gamemanager->GetSoundeffectHandler()->SetGlobalVolume(gamemanager->GetCurrentSoundVolume());
        }
    }

    soundsVolBar->VolumeBar(maxVolume, gamemanager->GetCurrentSoundVolume());

    // mouseover
    if (lowSoundVolume->MouseOver(input)) {
        lowSoundVolume->SetWICSprite(dx11, "Sprites/VolLower_mouseover.png");
    }
    else {
        lowSoundVolume->SetWICSprite(dx11, "Sprites/VolLower.png");
    }

    if (highSoundVolume->MouseOver(input)) {
        highSoundVolume->SetWICSprite(dx11, "Sprites/Volhigher_mouseover.png");
    }
    else {
        highSoundVolume->SetWICSprite(dx11, "Sprites/VolHigher.png");
    }

           

    ///////////////////////////////////////////////////////////////////////////////////////////////////


    // VSYNC
    GUISprite* vsyncON = static_cast<GUISprite*>(gui->GetGUIList()->at("vsyncON"));
    GUISprite* vsyncOFF = static_cast<GUISprite*>(gui->GetGUIList()->at("vsyncOFF"));

    if (vsyncON->Clicked(input))
    {
        if (!vsyncOn) {
            vsyncON->SetWICSprite(dx11, "Sprites/on_isON.png");
            lastOn = "Sprites/on_isON.png";
            vsyncOFF->SetWICSprite(dx11, "Sprites/off_isOFF.png");        
            lastOff = "Sprites/off_isOFF.png";

            currentScene->getRenderer()->setVsync(true);
            vsyncOn = true;
            std::cout << "VSYNC ON!" << std::endl;
        }    

       /*
        // THIS WORKS
        
        //dx11.GetSwapChain()->SetFullscreenState(true, NULL);

       // HRESULT hr;

       // dx11.GetContext()->OMSetRenderTargets(0, 0, 0);

       // dx11.GetRTV()->Release();
       // // resize hwnd here?


       // HWND testHWND = currentScene->getWindow().GetHWND();
       //// SetWindowPos(testHWND, NULL, 0, 0, 1920, 1080, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

       // // STENCIL NEED TO BE RECREATED

       // hr = dx11.GetSwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
       // assert(SUCCEEDED(hr));
       // ID3D11Texture2D* pBuffer;
       // hr = dx11.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D),
       //     (void**)&pBuffer);
       // assert(SUCCEEDED(hr));
       // ID3D11RenderTargetView* test = dx11.GetRTV();
       // hr = dx11.GetDevice()->CreateRenderTargetView(pBuffer, NULL, &test);
       // assert(SUCCEEDED(hr));
       // pBuffer->Release();
       // dx11.GetContext()->OMSetRenderTargets(1, &test, NULL);

       // //dx11.SetRTV(test);

       // //dx11.GetDevice()->CreateRenderTargetView(dx11.GetBackBufferPtr(), nullptr, &test);
       // //dx11.GetBackBufferPtr()->Release();

       // ID3D11DepthStencilView* dsv;
       // 

       // // VIEWPORT
       // D3D11_VIEWPORT viewport;
       // ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
       // viewport.TopLeftX = 0.0f;
       // viewport.TopLeftY = 0.0f;
       // viewport.Width = static_cast<float>(1280);
       // viewport.Height = static_cast<float>(720);
       // viewport.MinDepth = 0.0f;
       // viewport.MaxDepth = 1.0f;

       // // DEPTH TEXTURE
       // D3D11_TEXTURE2D_DESC depthTexDesc;
       // depthTexDesc.Width = 1280;
       // depthTexDesc.Height = 720;
       // depthTexDesc.MipLevels = 1;
       // depthTexDesc.ArraySize = 1;
       // depthTexDesc.SampleDesc.Count = 1;
       // depthTexDesc.SampleDesc.Quality = 0;
       // depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
       // depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
       // depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
       // depthTexDesc.CPUAccessFlags = 0;
       // depthTexDesc.MiscFlags = 0;

       //
       // std::cout << "HERE";
       // ID3D11Texture2D* depthTexture;
       // hr = dx11.GetDevice()->CreateTexture2D(&depthTexDesc, 0, &depthTexture);
       // assert(SUCCEEDED(hr));

       // D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
       // dsvDesc.Format = depthTexDesc.Format;
       // dsvDesc.Flags = 0;
       // dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
       // dsvDesc.Texture2D.MipSlice = 0;

       // hr = dx11.GetDevice()->CreateDepthStencilView(depthTexture, &dsvDesc, &dsv);
       // assert(SUCCEEDED(hr));

       // /*depthTexture->Release();
       // depthTexture = nullptr;*/
       // RenderTarget* target = new RenderTarget(test, nullptr, dsv, viewport);
       // dx11.SetRenderTarget(target);
       //// this->backbuffer = new RenderTarget(backbufferRTV, nullptr, dsv, viewport);                
    }

    if (vsyncOFF->Clicked(input)) {
        if (vsyncOn) {
            vsyncON->SetWICSprite(dx11, "Sprites/on_isOFF.png");
            lastOn = "Sprites/on_isOFF.png";
            vsyncOFF->SetWICSprite(dx11, "Sprites/off_isON.png");
            lastOff = "Sprites/off_isON.png";

            currentScene->getRenderer()->setVsync(false);
            vsyncOn = false;
            std::cout << "VSYNC OFF!" << std::endl;
        }
    }

    // Mouseover
    if (vsyncON->MouseOver(input) && !vsyncOn) {        
        vsyncON->SetWICSprite(dx11, "Sprites/on_isOFF_Mouseover.png");
    }
    else {
        vsyncON->SetWICSprite(dx11, this->lastOn);
    }

    if (vsyncOFF->MouseOver(input) && vsyncOn) {        
        vsyncOFF->SetWICSprite(dx11, "Sprites/off_isOFF_Mouseover.png");       
    }
    else {
        vsyncOFF->SetWICSprite(dx11, this->lastOff);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////


    GUISprite* backtointroNEW = static_cast<GUISprite*>(gui->GetGUIList()->at("backtointro"));

    if (backtointroNEW->Clicked(input))
    {
        first = true;
        menu = Menu::start;
    }

    // mouseover
    if (backtointroNEW->MouseOver(input))
    {
        backtointroNEW->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        backtointroNEW->SetWICSprite(dx11, "Sprites/backtointro.png");
    }

}

void IntroGUI::LoadOptions()
{
    ClearGUI();
    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/music.png", 100.0f, 100.0), "musicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/soundeffects.png", 100.0f, 200.0f), "soundsvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/vsync.png", 100.0f, 300.0), "vsync");

    // VSYNC ON/OFF   
    GUISprite* vsyncONOFF_on = new GUISprite(dx11, this->lastOn, 500.0f, 312.0f);
    GUISprite* vsyncONOFF_off = new GUISprite(dx11, this->lastOff, 700.0, 312.0f);
    gui->AddGUIObject(vsyncONOFF_on, "vsyncON");
    gui->AddGUIObject(vsyncONOFF_off, "vsyncOFF");
    
    ///////////////////////////////
    // difficultys
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/difficulty.png", 100.0f, 400.0f), "difficulty");
        
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/easy_active.png", 400, 415.0f), "easydif");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/medium.png", 600, 415.0f), "mediumdif");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/hard.png", 800, 415.0f), "harddif");

    // back to intro
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtointro");
        
    // frame and bar music    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower.png", 480.0f, 107.0f), "leftmusicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher.png", 845.0f, 107.0f), "rightmusicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 550.0f, 107.0f), "MusicBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 550.0f, 107.0f), "MusicFrame");

    // frame and bar sounds
    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower.png", 480.0f, 210.0f), "leftsoundvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher.png", 845.0f, 210.0f), "rightsoundvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 550.0f, 210.0f), "SoundsBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 550.0f, 210.0f), "SoundFrame");
   
    first = false;
}

void IntroGUI::Soundtracks()
{

    GUISprite* checkboxone = static_cast<GUISprite*>(gui->GetGUIList()->at("checkone"));

    if (checkboxone->Clicked(input)) {
        if (oneChecked == false) {
            gamemanager->SetCurrentTrack(1);
          //  soundtrack = Soundtrack::Track1;
        }
    }

    GUISprite* checkboxtwo = static_cast<GUISprite*>(gui->GetGUIList()->at("checktwo"));
    if (checkboxtwo->Clicked(input)) {
        if (twoChecked == false) {
            gamemanager->SetCurrentTrack(2);
          //  soundtrack = Soundtrack::Track2;
        }
    }

    GUISprite* checkboxthree = static_cast<GUISprite*>(gui->GetGUIList()->at("checkthree"));
    if (checkboxthree->Clicked(input)) {
        if (threeChecked == false) {
            gamemanager->SetCurrentTrack(3);
           // soundtrack = Soundtrack::Track3;
        }
    }


    GUISprite* ben = static_cast<GUISprite*>(gui->GetGUIList()->at("bensong"));   
    if (checkboxone->MouseOver(input) && !oneChecked) {
        checkboxone->SetWICSprite(dx11, "Sprites/checkbox_mouseover.png");
        ben->SetWICSprite(dx11, "Sprites/bensong.png");
    }
    else{  
        if (oneChecked) {
            checkboxone->SetWICSprite(dx11, "Sprites/checkbox_checked.png");
            ben->SetWICSprite(dx11, "Sprites/bensong_active.png");
        }        
        else {
            checkboxone->SetWICSprite(dx11, "Sprites/checkbox.png");
            ben->SetWICSprite(dx11, "Sprites/bensong.png");
        }
            
    }

    GUISprite* duck = static_cast<GUISprite*>(gui->GetGUIList()->at("ducksong"));    
    if (checkboxtwo->MouseOver(input) && !twoChecked) {
        duck->SetWICSprite(dx11, "Sprites/ducksong.png");
        checkboxtwo->SetWICSprite(dx11, "Sprites/checkbox_mouseover.png");
    }
    else {
        if (twoChecked) {
            checkboxtwo->SetWICSprite(dx11, "Sprites/checkbox_checked.png");
            duck->SetWICSprite(dx11, "Sprites/ducksong_active.png");
        }
            
        else {
            checkboxtwo->SetWICSprite(dx11, "Sprites/checkbox.png");
            duck->SetWICSprite(dx11, "Sprites/ducksong.png");
        }
            
    }

    GUISprite* cup = static_cast<GUISprite*>(gui->GetGUIList()->at("cupsong"));
    if (checkboxthree->MouseOver(input) && !threeChecked) {
        cup->SetWICSprite(dx11, "Sprites/cupsong.png");
        checkboxthree->SetWICSprite(dx11, "Sprites/checkbox_mouseover.png");
        
    }
    else {
        if (threeChecked) {
            checkboxthree->SetWICSprite(dx11, "Sprites/checkbox_checked.png");
            cup->SetWICSprite(dx11, "Sprites/cupsong_active.png");
        }
            
        else {
            checkboxthree->SetWICSprite(dx11, "Sprites/checkbox.png");
            cup->SetWICSprite(dx11, "Sprites/cupsong.png");
        }
            
    }

    
    ///////////////////////////////////////////////////////////////////////////////////


    GUISprite* backMenu = static_cast<GUISprite*>(gui->GetGUIList()->at("goback"));
    if (backMenu->MouseOver(input)) {
        backMenu->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        backMenu->SetWICSprite(dx11, "Sprites/backtointro.png");
    }

    if (backMenu->Clicked(input)) {
        first = true;
        menu = Menu::start;        
    }

}

void IntroGUI::LoadSoundtracks()
{
    ClearGUI();
    
    GUISprite* soundtrackIcon = new GUISprite(dx11, "Sprites/levelmusic.png", 100.0f, 75.0f);
    gui->AddGUIObject(soundtrackIcon, "soundIcon");
    GUISprite* musicFrame = new GUISprite(dx11, "Sprites/tracksFrame.png", 80.0f, 180.0f);
    gui->AddGUIObject(musicFrame, "alltracksframe");
    GUISprite* backtomenu = new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f);    
    gui->AddGUIObject(backtomenu, "goback");  


    GUISprite* bensong = new GUISprite(dx11, "Sprites/bensong.png", 200.0f, 200.0f);
    GUISprite* ducksong = new GUISprite(dx11, "Sprites/ducksong.png", 200.0f, 270.0f);
    GUISprite* cupsong = new GUISprite(dx11, "Sprites/cupsong.png", 200.0f, 340.0f);
    gui->AddGUIObject(bensong, "bensong");
    gui->AddGUIObject(ducksong, "ducksong");
    gui->AddGUIObject(cupsong, "cupsong");


    // checkboxes
    GUISprite* checkfirst = new GUISprite(dx11, "Sprites/checkbox.png", 100.0f, 200.0f);
    GUISprite* checksecond = new GUISprite(dx11, "Sprites/checkbox.png", 100.0f, 270.0f);
    GUISprite* checkthird = new GUISprite(dx11, "Sprites/checkbox.png", 100.0f, 340.0f);
    gui->AddGUIObject(checkfirst, "checkone");
    gui->AddGUIObject(checksecond, "checktwo");
    gui->AddGUIObject(checkthird, "checkthree");

    first = false;
}

void IntroGUI::Quit()
{
    GUISprite* imsure = static_cast<GUISprite*>(gui->GetGUIList()->at("imsure"));
    if (imsure->Clicked(input))
    {
        currentScene->exitGame = true;
    }

    GUISprite* backagain = static_cast<GUISprite*>(gui->GetGUIList()->at("backtointro"));
    if (backagain->Clicked(input))
    {
        first = true;
        menu = Menu::start;
    }

    // MOUSEOVER
    if (imsure->MouseOver(input)) {
        imsure->SetWICSprite(dx11, "Sprites/imsure_mouseover.png");
    }
    else {
        imsure->SetWICSprite(dx11, "Sprites/imsure.png");
    }

    if (backagain->MouseOver(input)) {
        backagain->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        backagain->SetWICSprite(dx11, "Sprites/backtointro.png");
    }
}

void IntroGUI::LoadQuit()
{
    ClearGUI();

    gui->AddGUIObject(new GUISprite(dx11, "Sprites/imsure.png", 100.0f, 400.0f), "imsure");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtointro");
    
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

