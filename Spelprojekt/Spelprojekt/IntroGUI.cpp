#include "IntroGUI.h"

IntroGUI::IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scene, SoundHandler* sound) : dx11(dx11)
{
    this->currentScene = scene;
    this->gui = gui;
    this->input = cameraController->getInput();
    this->mainSound = sound;   
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
    GUISprite* lowVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("leftvolume"));
    GUISprite* volumeBar = static_cast<GUISprite*>(gui->GetGUIList()->at("VolumeBar"));

    // Set current volume
    currentVolume = mainSound->GetGlobalVolume();
    volumeBar->HealthBar(maxVolume, currentVolume);

    if (lowVolume->Clicked(input))
    {
        if (mainSound->GetGlobalVolume() > 0.0f) {
            currentVolume = mainSound->GetGlobalVolume();
            currentVolume -= volumeScale;
            this->mainSound->SetGlobalVolume(currentVolume);
            volumeBar->HealthBar(maxVolume, currentVolume);
        }      
    }
    GUISprite* highVolume = static_cast<GUISprite*>(gui->GetGUIList()->at("rightvolume"));
    if (highVolume->Clicked(input))
    {
        if (mainSound->GetGlobalVolume() < 1.0f)
        {
            currentVolume = mainSound->GetGlobalVolume();
            currentVolume += volumeScale;
            this->mainSound->SetGlobalVolume(currentVolume);
            volumeBar->HealthBar(maxVolume, currentVolume);
        }           
    }

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
    
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/vol.png", 100.0f, 100.0f), "volume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/vsync.png", 100.0f, 300.0f), "vsync");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/backtointro.png", 100.0f, 500.0f), "backtointro");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolLower4.png", 555.0f, 110.0f), "leftvolume");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolHigher4.png", 1045.0f, 110.0f), "rightvolume");

    // frame and bar
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolBar.png", 650.0f, 110.0f), "VolumeBar");
    gui->AddGUIObject(new GUISprite(dx11, "Sprites/VolFrame.png", 650.0f, 110.0f), "SoundFrame");
   
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

