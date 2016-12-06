////////////////////////////
// Filename: GameManager.cpp
////////////////////////////

#include "GameManager.h"

GameManager::GameManager()
{
	m_Input = 0;
	m_Graphics = 0;
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

	m_Input->Initialize();

	//Create the Graphics object. This object will handle rendering all the graphics for this application
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	//Initialize the graphics object
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hWnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void GameManager::Shutdown()
{
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
				done = true;
			}
		}
	}

	return;
}

bool GameManager::Frame()
{
	bool result;

	//Check if the user pressed escape and wants to exit the application
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//Do the frame processing for the graphics object
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK GameManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//Check if a key has been pressed on the keyboard
		case WM_KEYDOWN:
		{
			//if a key is pressed send it to the input object so it can record that state
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		//check if a key has been released on the keyboard
		case WM_KEYUP:
		{
			//if a key is released then send it to the input object so it can unset the state for that key
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		//any other message send to the default message handler as our application won't make use of them
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
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