#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scene, SoundHandler* sound, SoundHandler* soundeffect) : dx11(dx11)
{
    this->currentScene = scene;
    this->gui = gui;
    this->input = cameraController->getInput();
    this->mainSound = sound;   
    this->soundeffects = soundeffect;
    this->lastOff = "Sprites/off_isON.png";
    this->lastOn = "Sprites/on_isOFF.png";
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
    case Menu::soundtracks: // sound crap
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

        // SOUNDTRACK
        GUISprite* soundtrack = static_cast<GUISprite*>(gui->GetGUIList()->at("soundtracks"));

        if (soundtrack->Clicked(input))
        {
            first = true;
            menu = Menu::soundtracks;
        }

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
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/play.png", 100.0f, 50.0f), "play");    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/options.png", 100.0f, 200.0f), "options");
    // SOUNDTRACK
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/soundtracks.png", 100.0f, 350.0f), "soundtracks");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/quit.png", 100.0f, 500.0f), "quit");
    first = false;
}

void IntroGUI::Options()
{   
    // Set current volume for music
    GUISprite* musicVolBar = static_cast<GUISprite*>(gui->GetGUIList()->at("MusicBar"));    
    currentMusicVolume = mainSound->GetGlobalVolume();
    musicVolBar->VolumeBar(maxVolume, currentMusicVolume);

    GUISprite* lowMusicVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("leftmusicvolume"));
    if (lowMusicVolume->Clicked(input))
    {
        if (mainSound->GetGlobalVolume() > minVolume) {
            currentMusicVolume = mainSound->GetGlobalVolume();
            currentMusicVolume -= volumeScale;
            this->mainSound->SetGlobalVolume(currentMusicVolume);
            musicVolBar->VolumeBar(maxVolume, currentMusicVolume);
        }      
    }

    GUISprite* highMusicVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("rightmusicvolume"));
    
    if (highMusicVolume->Clicked(input))
    {
        if (mainSound->GetGlobalVolume() < maxVolume)
        {
            currentMusicVolume = mainSound->GetGlobalVolume();
            currentMusicVolume += volumeScale;
            this->mainSound->SetGlobalVolume(currentMusicVolume);
            musicVolBar->VolumeBar(maxVolume, currentMusicVolume);
        }           
    }

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
    currentSoundVolume = soundeffects->GetGlobalVolume();
    soundsVolBar->VolumeBar(maxVolume, currentSoundVolume);

    GUISprite* lowSoundVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("leftsoundvolume"));
    if (lowSoundVolume->Clicked(input))
    {
        if (soundeffects->GetGlobalVolume() > 0.0f) {
            currentSoundVolume = soundeffects->GetGlobalVolume();
            currentSoundVolume -= volumeScale;
            this->soundeffects->SetGlobalVolume(currentSoundVolume);
            soundsVolBar->VolumeBar(maxVolume, currentSoundVolume);
        }
    }

    GUISprite* highSoundVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("rightsoundvolume"));
    if (highSoundVolume->Clicked(input))
    {
        if (soundeffects->GetGlobalVolume() < 1.0f)
        {
            currentSoundVolume = soundeffects->GetGlobalVolume();
            currentSoundVolume += volumeScale;
            this->soundeffects->SetGlobalVolume(currentSoundVolume);
            soundsVolBar->VolumeBar(maxVolume, currentSoundVolume);
        }
    }

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


    GUISprite* backtointro = static_cast<GUISprite*>(gui->GetGUIList()->at("backtointro"));

    if (backtointro->Clicked(input))
    {
        first = true;
        menu = Menu::start;
    }
       
    // mouseover
    if (backtointro->MouseOver(input)) 
    {
        backtointro->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        backtointro->SetWICSprite(dx11, "Sprites/backtointro.png");
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
}

void IntroGUI::LoadOptions()
{
    ClearGUI();
    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/music.png", 100.0f, 50.0f), "musicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/sounds.png", 100.0f, 200.0f), "soundsvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/vsync.png", 100.0f, 350.0f), "vsync");

    // VSYNC ON/OFF   
    GUISprite* vsyncONOFF_on = new GUISprite(dx11, this->lastOn, 560.0f, 350.0f);
    GUISprite* vsyncONOFF_off = new GUISprite(dx11, this->lastOff, 790.0f, 350.0f);
    gui->AddGUIObject(vsyncONOFF_on, "vsyncON");
    gui->AddGUIObject(vsyncONOFF_off, "vsyncOFF");
    
    ///////////////////////////////
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtointro");
        
    // frame and bar music
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 650.0f, 60.0f), "MusicBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower.png", 555.0f, 60.0f), "leftmusicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher.png", 1045.0f, 60.0f), "rightmusicvolume");    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 650.0f, 60.0f), "MusicFrame");

    // frame and bar sounds
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 650.0f, 210.0f), "SoundsBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower.png", 555.0f, 210.0f), "leftsoundvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher.png", 1045.0f, 210.0f), "rightsoundvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 650.0f, 210.0f), "SoundFrame");
   
    first = false;
}

void IntroGUI::Soundtracks()
{
    GUISprite* backToMain = static_cast<GUISprite*>(gui->GetGUIList()->at("backtooptions"));
    if (backToMain->Clicked(input)) {
        first = true;
        menu = Menu::start;
    }

    // Click on musictrack, change string, update


    if (backToMain->MouseOver(input)) {
        backToMain->SetWICSprite(dx11, "Sprites/backtointro_mouseover.png");
    }
    else {
        backToMain->SetWICSprite(dx11, "Sprites/backtointro.png");
    }


    // Mouseover functions 
    GUISprite* monstersinc = static_cast<GUISprite*>(gui->GetGUIList()->at("monster"));
    if (monstersinc->MouseOver(input)) {
        monstersinc->SetWICSprite(dx11, "Sprites/monstersinc_mouseover.png");
    }
    else {
        monstersinc->SetWICSprite(dx11, "Sprites/monstersinc.png");
    }


}

void IntroGUI::LoadSoundtracks()
{
    ClearGUI();
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/tracks.png", 100.0f, 50.0f), "tracks");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/tracksFrame.png", 95.0f, 140.0f), "tracksFrame");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/monstersinc.png", 105.0f, 220.0f), "monster");
    // Different musictrack sprites

    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtooptions");
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
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtointro");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/imsure.png", 100.0f, 350.0f), "imsure");
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

