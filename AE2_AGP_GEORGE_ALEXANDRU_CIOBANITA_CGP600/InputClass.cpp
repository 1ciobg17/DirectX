///////////////////////////////////
// Filename: InputClass.cpp
//////////////////////////////////

#include "InputClass.h"

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	//store the screen size which will be used for positioning the mouse cursor
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//initialize the mouse location on screen
	m_mouseX = 0;
	m_mouseY = 0;

	//initialize the direct input object
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//initialize the direct input object for the keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//set the data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//set the cooperative level of the keyboard, DISCL_EXCLUSIVE makes it so the keyboard is used only by this program, DISCL_NONEXCLUSIVE is the opposite and the keyboard input is not exclusive to the program
	result = m_keyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//now acquire the keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//initialize the mouse object
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//set the data format
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	//the cooperative level
	result = m_mouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//acquire the mouse
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	//release the mouse
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	//release the keyboard
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	//release the main direct input object
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;

	//read keyboard state
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	//read mouse state
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//process the changes in the states
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	//read the keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardButtons), (LPVOID)&m_keyboardButtons);
	if (FAILED(result))
	{
		//if the keyboard was not acquired or something else is getting in the way
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	//read the mouse device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//if the mouse was not acquired or something else is getting in the way
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{
	//update the location of the mouse cursor based on the change of the mouse location during the frame
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	//ensure the mouse location doesn't exceed the screen width of height
	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;

	if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;

	return;
}

bool InputClass::IsEscapePressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_ESCAPE] && 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsUpArrowPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_UP] && 0xC8)
	{
		return true;
	}

	return false;
}

bool InputClass::IsDownArrowPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_DOWN] && 0xD0)
	{
		return true;
	}

	return false;
}

bool InputClass::IsRightArrowPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_RIGHT] && 0xCD)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftArrowPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_LEFT] && 0xCB)
	{
		return true;
	}

	return false;
}

bool InputClass::IsWPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_W] && 0x11)
	{
		return true;
	}

	return false;
}

bool InputClass::IsSPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_S] && 0x1F)
	{
		return true;
	}

	return false;
}

bool InputClass::IsAPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_A] && 0x1E)
	{
		return true;
	}

	return false;
}

bool InputClass::IsDPressed()
{
	//is escape key being pressed?, bitwise & keyboard check
	if (m_keyboardButtons[DIK_D] && 0x20)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

