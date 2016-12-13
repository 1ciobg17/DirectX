//////////////////////////
// Filename: InputClass.h
//////////////////////////
#pragma once
//the following need to be linked for Direct Input to work
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputClass
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	unsigned char m_keyboardButtons[256];
	DIMOUSESTATE m_mouseState;
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();
	bool IsEscapePressed();
	bool IsUpArrowPressed();
	bool IsDownArrowPressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	bool IsWPressed();
	bool IsSPressed();
	bool IsAPressed();
	bool IsDPressed();
	void GetMouseLocation(int&, int&);
};