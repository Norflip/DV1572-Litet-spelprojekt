#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scene, SoundHandler* sound, SoundHandler* soundeffect) : dx11(dx11)
{
    this->currentScene = scene;
    this->gui = gui;
    this->input = cameraController->getInput();
    this->mainSound = sound;   
    this->soundeffects = soundeffect;
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


    ///////////////////////////////////////////////////////////////////////////////////////////////////


    // Set current volume for music
    GUISprite* musicVolBar = static_cast<GUISprite*>(gui->GetGUIList()->at("MusicBar"));    
    currentMusicVolume = mainSound->GetGlobalVolume();
    musicVolBar->VolumeBar(maxVolume, currentMusicVolume);

    GUISprite* lowMusicVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("leftmusicvolume"));
    if (lowMusicVolume->Clicked(input))
    {
        if (mainSound->GetGlobalVolume() > 0.0f) {
            currentMusicVolume = mainSound->GetGlobalVolume();
            currentMusicVolume -= volumeScale;
            this->mainSound->SetGlobalVolume(currentMusicVolume);
            musicVolBar->VolumeBar(maxVolume, currentMusicVolume);
        }      
    }
    GUISprite* highMusicVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("rightmusicvolume"));
    if (highMusicVolume->Clicked(input))
    {
        if (mainSound->GetGlobalVolume() < 1.0f)
        {
            currentMusicVolume = mainSound->GetGlobalVolume();
            currentMusicVolume += volumeScale;
            this->mainSound->SetGlobalVolume(currentMusicVolume);
            musicVolBar->VolumeBar(maxVolume, currentMusicVolume);
        }           
    }

    //////////////////////////////////////////////////////////////

    GUISprite* backtointro = static_cast<GUISprite*>(gui->GetGUIList()->at("backtointro"));
    if (backtointro->Clicked(input))
    {
        first = true;
        menu = Menu::start;
    }
    GUISprite* vsync = static_cast<GUISprite*>(gui->GetGUIList()->at("vsync"));
    if (vsync->Clicked(input))
    {

        // THIS WORKS
        currentScene->getRenderer()->setVsync(false);
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
}

void IntroGUI::LoadOptions()
{
    ClearGUI();
    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/music.png", 100.0f, 50.0f), "musicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/sounds.png", 100.0f, 200.0f), "soundsvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/vsync.png", 100.0f, 350.0f), "vsync");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtointro");

    // frame and bar music
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 650.0f, 60.0f), "MusicBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower4.png", 555.0f, 60.0f), "leftmusicvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher4.png", 1045.0f, 60.0f), "rightmusicvolume");    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 650.0f, 60.0f), "MusicFrame");

    // frame and bar sounds
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 650.0f, 210.0f), "SoundsBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower4.png", 555.0f, 210.0f), "leftsoundvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher4.png", 1045.0f, 210.0f), "rightsoundvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 650.0f, 210.0f), "SoundFrame");
   
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
        first = true;
        menu = Menu::start;
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

