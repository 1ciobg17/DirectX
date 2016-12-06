//////////////////////////////////
// Filename: GameManager.h
//////////////////////////////////
#pragma once

#include <windows.h>
#include <string.h>
#include <iostream>
#include "InputClass.h"
#include "GraphicsClass.h"
using namespace std;

//////////////////////////////////
// Class Name: GameManager Class
//////////////////////////////////

class GameManager
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	LPCWSTR m_applicationName;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
public:
	GameManager();
	GameManager(const GameManager&);
	~GameManager();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static GameManager* gApplicationHandle = 0;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);