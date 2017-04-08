// include the basic windows header files and the Direct3D header files
#include "stdafx.h"

#include "Globals.h"

#include "mainincludes.h"


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"Dong - The Expansion Runner - Damon Wareham 2015",
                          WS_OVERLAPPEDWINDOW,
                          0,
                          0,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

	//must call this for COM
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	
	//create the engine
	if( FAILED( XAudio2Create( &engine ) ) )
	{
		CoUninitialize();
		return -1;
	}

	//create the mastering voice
	if( FAILED( engine->CreateMasteringVoice( &g_master ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -2;
	}

	//load a music file
	if( !musicbuffer.load( "audio/music/madhatter.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a music file
	if( !music2buffer.load( "audio/music/teatime.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}

	//load a sound file
	if( !soundbuffer.load( "audio/sound/change.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !sound2buffer.load( "audio/sound/metalhit.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !sound3buffer.load( "audio/sound/laser.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !sound4buffer.load( "audio/sound/xplode.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}

	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &music, musicbuffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &music, music2buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

		//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx, soundbuffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx2, sound2buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx3, sound3buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx4, sound4buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

	//start consuming audio in the source voice
	music->Start();
	sfx->Start();
	sfx2->Start();
	sfx3->Start();
	sfx4->Start();

    //--Initialising Direct Input
	hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL); 

	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	Background *bg = new Background();
	objects.push_back(bg);
	TitleScreen *title = new TitleScreen();
	objects.push_back(title);
	Paddle *paddle1 = new Paddle(PLAYER);
	objects.push_back(paddle1);
	ScoreItem *score = new ScoreItem(paddle1->GetScore(), paddle1->GetID());

	// enter the main loop:
	bool needtoQuit = false;
    MSG msg;
	gameInfo* currentGame = new gameInfo;	// this creates our game instance, and calls it currentGame

    while(needtoQuit == false)
    {
		//static DWORD start = GetTickCount();
		currentGame->frameStartTime = GetTickCount();

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
           // if the message was a "quit" message...
			if( msg.message == WM_QUIT )
			{
				needtoQuit = true;	// we want to quit asap
			}
			
			// if it was any other type of message (i.e. one we don't care about), process it as normal
            else 
            {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
            }

        }
	//---Direct input
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

	if(currentGame->gamestate == PLAYING) {
		if(currentGame->musicplaying == false) {
			currentGame->musicplaying = true;
			music->Stop();
			music->FlushSourceBuffers();
				music->Start();
			music->SubmitSourceBuffer(musicbuffer.xaBuffer() ); // play music
		}
	}

	if(currentGame->sound)
	{
		music->SetVolume(1);
		sfx->SetVolume(1.0f);
		sfx2->SetVolume(1.0f);
		sfx3->SetVolume(1.0f);
		sfx4->SetVolume(1.0f);
	} 
	else
	{
		music->SetVolume(0);
		sfx->SetVolume(0);
		sfx2->SetVolume(0);
		sfx3->SetVolume(0);
		sfx4->SetVolume(0);
	}

	if(keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hWnd, WM_DESTROY, 0, 0);

	if(keyboardState[DIK_O] & 0x80)
	{
		if(!okeypress) {
			okeypress = true;
			if(currentGame->sound) {
				currentGame->sound = false;
			} else {
				currentGame->sound = true;
			}
		}
	} else {okeypress = false;}


	if(keyboardState[DIK_RETURN] & 0x80)
	{
		if(!enterpress) {
			enterpress = true;
		if(currentGame->gamestate == TITLE) {
			currentGame->gamestate = PLAYING;
			title->SetShow(false);
			paddle1->SetShow(true);
			bg->SetShow(true);
			Border *border = new Border(0.0f, -1.75f, 5);
			platforms.push_back(border);
		}
		else {
			GameWon(currentGame);
			}
		}
	} else {enterpress = false;}

	if(keyboardState[DIK_P] & 0x80)
	{
		if(!pausepress) {
			pausepress = true;
		if(currentGame->gamestate == PLAYING) {
			if(!currentGame->pause)
				currentGame->pause = true;
			else currentGame->pause = false;
		}
		}
	} else {pausepress = false;}

	if(!currentGame->pause) {

			if(keyboardState[DIK_A] & 0x80)
			{
				if(paddle1->CheckShow())
				paddle1->MoveLeft(platformspeed, currentGame->deltaTime);
			}	
			else if(keyboardState[DIK_D] & 0x80)
			{
				if(paddle1->CheckShow())
				paddle1->MoveRight(platformspeed, currentGame->deltaTime);
			}

			if(keyboardState[DIK_SPACE] & 0x80)
			{
				if(paddle1->CheckShow()) {					
					if(paddle1->onplatform) {
					//sfx3->SubmitSourceBuffer(sound3buffer.xaBuffer() );
					}
					paddle1->Jump(currentGame->deltaTime);

				}
			}

			if(keyboardState[DIK_LSHIFT] & 0x80)
			{
				if(paddle1->CheckShow())
				paddle1->Jumpincrease(currentGame->deltaTime);
			}else {paddle1->extraj = false;}

			if(keyboardState[DIK_R] & 0x80) //For debug purporses
			{
				if(paddle1->CheckShow())
					paddle1->SetY(0.0f);
			}
			
	}

	if(currentGame->gamestate == PLAYING) {
	currentGame->CameraX = paddle1->GetX()/10.0f;
	currentGame->CameraY = paddle1->GetX()/10.0f;
	currentGame->CameraLookX = paddle1->GetX()/10.0f;
	currentGame->CameraLookY = paddle1->GetY()/10.0f;
	}
	else {
		currentGame->CameraZ = 7.5f;
		currentGame->CameraX = 0.0f;
		currentGame->CameraY = 0.0f;
		currentGame->CameraLookZ = 0.0f;
		currentGame->CameraLookX = 0.0f;
		currentGame->CameraLookY = 0.0f;
	}

	if(currentGame->gamestate == TITLE)
		RunTitleScreen(title, currentGame);

	if(currentGame->gamestate == PLAYING) {
		if(!currentGame->pause) {
	if(paddle1->GetY() <= -5.0f)
		Unscored(paddle1, currentGame);
		}
	}


		UpdateProgram(paddle1, currentGame);
        RenderFrame(currentGame);

		currentGame->frameEndTime = GetTickCount();
		currentGame->frameDelta = (currentGame->frameEndTime - currentGame->frameStartTime);

    }

	delete currentGame;
	currentGame = NULL;
    // clean up DirectX and COM
    CleanD3D();

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);


    // create the depth buffer texture
    D3D11_TEXTURE2D_DESC texd;
    ZeroMemory(&texd, sizeof(texd));

    texd.Width = SCREEN_WIDTH;
    texd.Height = SCREEN_HEIGHT;
    texd.ArraySize = 1;
    texd.MipLevels = 1;
    texd.SampleDesc.Count = 4;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *pDepthBuffer;
    dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

    // create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
    pDepthBuffer->Release();

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;    // set the left to 0
    viewport.TopLeftY = 0;    // set the top to 0
    viewport.Width = SCREEN_WIDTH;    // set the width to the window's width
    viewport.Height = SCREEN_HEIGHT;    // set the height to the window's height
    viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
    viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(gameInfo *currentGame)
{
    D3DXMATRIX matTranslation, matView, matRotate, matProjection, matScale;
    D3DXMATRIX matFinal;

    // create a view matrix
    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3(currentGame->CameraX, currentGame->CameraY, currentGame->CameraZ),    // the camera position
                       &D3DXVECTOR3(currentGame->CameraLookX, currentGame->CameraLookY, currentGame->CameraLookZ),    // the look-at position
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

    // create a projection matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               (FLOAT)D3DXToRadian(45),                    // field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,                                       // near view-plane
                               100.0f);                                    // far view-plane

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    // clear the depth buffer
    devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
        devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

        // select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Calls for all objects to perform their render
	for(it01 = objects.begin(); it01 != objects.end(); ++it01) {
			(*it01)->Render(devcon, &textures, pCBuffer, matTranslation,
				            matRotate, matScale, matFinal, matView, matProjection, (*it01)->GetRed(), (*it01)->GetGreen(), (*it01)->GetBlue());
		}
	for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); ++sc01) {
			(*sc01)->Render(devcon, &textures, pCBuffer, matTranslation,
				            matRotate, matScale, matFinal, matView, matProjection, (*sc01)->GetRed(), (*sc01)->GetGreen(), (*sc01)->GetBlue());
		}
	for(pl01 = platforms.begin(); pl01 != platforms.end(); ++pl01) {
			(*pl01)->Render(devcon, &textures, pCBuffer, matTranslation,
				            matRotate, matScale, matFinal, matView, matProjection, (*pl01)->GetRed(), (*pl01)->GetGreen(), (*pl01)->GetBlue());
		}
	for(ba01 = balls.begin(); ba01 != balls.end(); ++ba01) {
			(*ba01)->Render(devcon, &textures, pCBuffer, matTranslation,
				            matRotate, matScale, matFinal, matView, matProjection, (*ba01)->GetRed(), (*ba01)->GetGreen(), (*ba01)->GetBlue());
		}

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    zbuffer->Release();
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    pIBuffer->Release();
    pCBuffer->Release();
	for(int killtex = 0; killtex < textures.size(); killtex++ )
	{
		textures.at(killtex)->Release();
	}
	textures.clear();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
	engine->Release(); //Relase audio engine
}


// this is the function that creates the shape to render
void InitGraphics()
{
    // create vertices to represent the corners of the cube
    VERTEX OurVertices[] =
    {
        {-1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},    // side 1
        {1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f},

        {-1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f},    // side 2
        {-1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f},
        {1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f},

        {-1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f},    // side 3
        {-1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f},
        {1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f},

        {-1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f},    // side 4
        {1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f},
        {-1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f},
        {1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f},

        {1.0f, -1.0f, -1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 5
        {1.0f, 1.0f, -1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
        {1.0f, -1.0f, 1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f},

        {-1.0f, -1.0f, -1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 6
        {-1.0f, -1.0f, 1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
        {-1.0f, 1.0f, -1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f},
    };

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&bd, NULL, &pVBuffer);

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);


    // create the index buffer out of DWORDs
    DWORD OurIndices[] = 
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 5, 6,    // side 2
        6, 5, 7,
        8, 9, 10,    // side 3
        10, 9, 11,
        12, 13, 14,    // side 4
        14, 13, 15,
        16, 17, 18,    // side 5
        18, 17, 19,
        20, 21, 22,    // side 6
        22, 21, 23,
    };

    // create the index buffer
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(DWORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;

    dev->CreateBuffer(&bd, NULL, &pIBuffer);

    devcon->Map(pIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurIndices, sizeof(OurIndices));                   // copy the data
    devcon->Unmap(pIBuffer, NULL);

	ID3D11ShaderResourceView *pTexture;    // the pointer to the texture

	
    D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/Player1.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading
	textures.push_back(pTexture);



	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/Player2.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading
	
	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/Metal.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/dongtitlescreen.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/dongtitlescreen2.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/space.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/iron.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/playcheat.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);
}


// this function loads and prepares the shaders
void InitPipeline()
{
    // compile the shaders
    ID3D10Blob *VS, *PS;
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // create the shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input element object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    // use the input element descriptions to create the input layout
    dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);

    // create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 176;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &pCBuffer);

    devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
}

void UpdateProgram(Paddle *player, gameInfo *currentGame)
{
	currentGame->deltaTime = (float)currentGame->frameDelta / IDEAL_DELTA;

	if(!currentGame->pause) {
	for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); ++sc01)
			(*sc01)->Update(currentGame->CameraLookY, currentGame->CameraLookX, player->GetScore());

	for(pl01 = platforms.begin(); pl01 != platforms.end(); ++pl01) 
			(*pl01)->Update(platformspeed, currentGame->deltaTime);

	for(ba01 = balls.begin(); ba01 != balls.end(); ++ba01) 
			(*ba01)->Update(platformspeed, currentGame->deltaTime);

	if(platforms.size() > 5) {
	for(pl01 = platforms.begin(); pl01 != platforms.end(); ++pl01) {
			if((*pl01)->GetX() >= 10) {
				delete (*pl01);
				pl01 = platforms.erase(pl01);
			}
	}
	}

	if(balls.size() > 5) {
	for(ba01 = balls.begin(); ba01 != balls.end(); ++ba01) {
			if((*ba01)->GetX() >= 10) {
				delete (*ba01);
				ba01 = balls.erase(ba01);
			}
	}
	}

	platformdelay--;

	if(platformdelay - platformspeed <= 0) {
			Border *border = new Border();
			platforms.push_back(border);
	if(rand() % 8 == 0)
	{
		Ball *ball = new Ball(BALLGOOD);
		balls.push_back(ball);
	}
	if(rand() % 8 == 0)
	{
		Ball *ball = new Ball(BALLBAD);
		balls.push_back(ball);
	}
	platformdelay = (PLATFORMDELAYMAX - ((int)platformspeed * 100));
	if(platformspeed < PLATFORMINCREASEMAX)
	platformspeed += PLATFORMINCREASESPEED;
	}

	for(it01 = objects.begin(); it01 != objects.end(); ++it01)
			(*it01)->Update(currentGame->deltaTime);

	//--Checking objects for collisions
	for(it01 = objects.begin(); it01 != objects.end(); ++it01)
		{
			if( ! (*it01)->Collidable() ) continue;
			if( ! (*it01)->CheckShow() ) continue;
			for(it02 = it01; it02 != objects.end(); ++it02)
			{
				if( !(*it02)->Collidable() ) continue;
				if( (*it01)->GetID() == (*it02)->GetID()) continue;

				if( (*it01)->CheckCollisions( (*it02)))
				{
					(*it01)->Collided( (*it02), currentGame->deltaTime);
					(*it02)->Collided( (*it01), currentGame->deltaTime);
				}
			}
			for(pl01 = platforms.begin(); pl01 != platforms.end(); ++pl01) {
				if( !(*pl01)->Collidable() ) continue;
			if( (*it01)->CheckCollisions( (*pl01)))
				{
					(*it01)->Collided( (*pl01), currentGame->deltaTime);
					(*pl01)->Collided( (*it01), currentGame->deltaTime);
				}
		}
		for(ba01 = balls.begin(); ba01 != balls.end(); ++ba01) {
				if( !(*ba01)->Collidable() ) continue;
			if( (*it01)->CheckCollisions( (*ba01)))
				{
					(*it01)->Collided( (*ba01), currentGame->deltaTime);
					(*ba01)->Collided( (*it01), currentGame->deltaTime);
					
					if((*ba01)->CheckObjectID(BALLBAD)) {
					Unscored(player, currentGame);
					sfx4->SubmitSourceBuffer(sound4buffer.xaBuffer() );
					}
					if((*ba01)->CheckObjectID(BALLGOOD))
						{
							sfx->SubmitSourceBuffer(soundbuffer.xaBuffer() );
							platformspeed -= PLATFORMDECREASESPEED;
							if(platformspeed < 0.0f)
								platformspeed = 0.0f;
						}
					}
				}
		}
		}
	for(ba01 = balls.begin(); ba01 != balls.end(); ++ba01) {
		for(pl01 = platforms.begin(); pl01 != platforms.end(); ++pl01) {
			if( !(*ba01)->Collidable() ) continue;
			if( !(*pl01)->Collidable() ) continue;
			if( (*ba01)->CheckCollisions( (*pl01))) {
				if((*ba01)->CheckObjectID(BALLGOOD)) continue;
				(*pl01)->Collided( (*ba01), currentGame->deltaTime);
				(*ba01)->Collided( (*pl01), currentGame->deltaTime);
			}
		}
	}
	//--End of collision checking

	}


void Scored(Paddle *player)
{
	player->Score();	
	ScoreItem *score = new ScoreItem(player->GetScore(), player->GetID());
	scoreobjects.push_back(score);
}

void Unscored(Paddle *player, gameInfo *currentGame)
{
	sfx4->SubmitSourceBuffer(sound4buffer.xaBuffer() );
	if(player->GetScore() == 0) {
		GameWon(currentGame);
	} else {
	player->Die();
	}
}

void GameWon(gameInfo *currentGame)
{
	currentGame->CameraX = 0.0f;
	currentGame->CameraY = 0.0f;
	currentGame->CameraZ = 0.0f;
	currentGame->CameraLookX = 0.0f;
	currentGame->CameraLookY = 0.0f;
	currentGame->CameraLookZ = 0.0f;

	currentGame->gamestate = TITLE;
}

void RunTitleScreen(TitleScreen *title, gameInfo *currentGame)
{
	for(pl01 = platforms.begin(); pl01 != platforms.end(); )
	{
		delete (*pl01);
		pl01 = platforms.erase(pl01);
	}
	for(ba01 = balls.begin(); ba01 != balls.end(); )
	{
		delete (*ba01);
		ba01 = balls.erase(ba01);
	}
	title->SetShow(true);
	title->SetTexNum(5);
	for(it01 = objects.begin(); it01 != objects.end(); ++it01)
	{
		(*it01)->Reset();
	}
	if(currentGame->musicplaying) {
		music->Stop();
		music->FlushSourceBuffers();
		music->Start();
		music->SubmitSourceBuffer(music2buffer.xaBuffer() );
		currentGame->musicplaying = false;
	}
	currentGame->pause = false;
	platformdelay = 0;
	platformspeed = 0.0f;
}