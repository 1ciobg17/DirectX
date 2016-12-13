////////////////////////////
// Filename: GameManager.cpp
////////////////////////////

#include "GameManager.h"

GameManager::GameManager()
{
	m_Input = 0;
	m_Graphics = 0;
	m_FPS = 0;
	m_CPU = 0;
	m_Timer = 0;
	m_cameraPosition = 0;
	m_Camera = 0;
}

GameManager::GameManager(const GameManager& other)
{
}

GameManager::~GameManager()
{
}

bool GameManager::Initialize()
{
	int screenWidth, screenHeight;
	bool result;
	D3DXMATRIX baseViewMatrix;

	//Initialize the width and height of the screen to zero before sending the variables into the function
	screenWidth = 0;
	screenHeight = 0;

	//Initialize the windows api
	InitializeWindows(screenWidth, screenHeight);

	//Create the Input object. This object will be used to handle reading the keyboard input from the user
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	//initielize the input
	result = m_Input->Initialize(m_hInstance, m_hWnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hWnd, L"Could not initialize the input object", L"Error", MB_OK);
		return false;
	}

	//Create the camera
	m_Camera = new Camera();
	if (!m_Camera)
	{
		return false;
	}

	//initialize a base view matrix with the camera for 2d user isntance rendering
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	//Create the Graphics object. This object will handle rendering all the graphics for this application
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	//Initialize the graphics object
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hWnd, baseViewMatrix);
	if (!result)
	{
		return false;
	}

	//create the fps object
	m_FPS = new FPSClass;
	if (!m_FPS)
	{
		return false;
	}

	//Initialize the FPS object
	m_FPS->Initialize();

	//create the cpu object
	m_CPU = new CPUClass;
	if (!m_CPU)
	{
		return false;
	}

	//initialize the cpu object
	m_CPU->Initialize();

	//create the timer object
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	result=m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hWnd, L"Could not initialize the Timer object", L"Error", MB_OK);
		return false;
	}

	//create the camera position object
	m_cameraPosition = new MovementClass;
	if (!m_cameraPosition)
	{
		return false;
	}

	return true;
}

void GameManager::Shutdown()
{
	//release the camera position object
	if (m_cameraPosition)
	{
		delete m_cameraPosition;
		m_cameraPosition = 0;
	}

	//release the camera
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//release the timer object
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	//release the cpu object
	if (m_CPU)
	{
		m_CPU->Shutdown();
		delete m_CPU;
		m_CPU = 0;
	}

	//relase the fps object
	if (m_FPS)
	{
		delete m_FPS;
		m_FPS = 0;
	}

	//Release the graphics object
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	//Release the input object
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input;
	}

	//Shutdown the window
	ShutdownWindows();

	return;
}

void GameManager::Run()
{
	MSG msg;
	bool done, result;
	
	//Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	//Loop until there is a quit message from the window or the user
	done = false;
	while (!done)
	{
		//Handle the windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If windows signlas to end the application then exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Otherwise do the frame processing
			result = Frame();
			if (!result)
			{
				MessageBox(m_hWnd, L"Frame processing failed", L"Error", MB_OK);
				done = true;
			}
		}

		//check if the user pressed escape and wants to quit
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}

	return;
}

bool GameManager::Frame()
{
	bool result, keyDown;
	int mouseX, mouseY;
	float posX, posY, posZ, rotX, rotY, rotZ;
	D3DXMATRIX viewMatrix;

	//update the system starts
	m_Timer->Frame();
	m_FPS->Frame();
	m_CPU->Frame();

	//input frame processing happens here
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	m_cameraPosition->SetFrameTime(m_Timer->GetTime());

	keyDown = m_Input->IsLeftArrowPressed();
	m_cameraPosition->TurnLeft(keyDown);

	keyDown = m_Input->IsRightArrowPressed();
	m_cameraPosition->TurnRight(keyDown);

	keyDown = m_Input->IsUpArrowPressed();
	m_cameraPosition->LookUp(keyDown);

	keyDown = m_Input->IsDownArrowPressed();
	m_cameraPosition->LookDown(keyDown);

	keyDown = m_Input->IsWPressed();
	m_cameraPosition->Forward(keyDown);

	keyDown = m_Input->IsSPressed();
	m_cameraPosition->Backward(keyDown);

	m_cameraPosition->GetPosition(posX, posY, posZ);
	m_cameraPosition->GetRotation(rotX, rotY, rotZ);

	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	//get the mouse cursor location from the input object
	m_Input->GetMouseLocation(mouseX, mouseY);

	//Generate the view matrix based on the camera's position
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);

	//Do the frame processing for the graphics object
	result = m_Graphics->Frame(m_FPS->GetFPS(), m_CPU->GetCPUPercentage(), m_Timer->GetTime(), mouseX, mouseY, viewMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK GameManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);	
}

void GameManager::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer to this object
	gApplicationHandle = this;

	//Get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	//Give the application a name
	m_applicationName = L"AE2_AGP_GEORGE_ALEXANDRU_CIOBANITA_CGP600\0";

	//setup the windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the client desktop screen
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth = GetSystemMetrics(SM_CXSCREEN);

	//setup the screen settings depending on where it is running in full screen or in windowed mode
	if (FULL_SCREEN)
	{
		//if full screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Change the display settings to full screen 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//Set the position of the window to the top left corner 
		posX = posY = 0;
	}
	else
	{
		//If windowed then set it to 800x600 resolution
		screenWidth = 800;
		screenHeight = 600;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//Create the window with the screen settings and get the handle to it
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	//Bring the window up on the screen and set it as a main focus
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	//Hide the mouse cursor
	//ShowCursor(false);

	return;
}

void GameManager::ShutdownWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//remove the window
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	//Remove the application instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	//Release the pointer to this class
	gApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		//check if the window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//Check if the window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		//All other messages pass to the message handler in the system class
		default:
		{
			return gApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}