//////////////////////////////////
// Filename: GameManager.h
//////////////////////////////////
#pragma once

#include <windows.h>
#include <string.h>
#include <iostream>
#include "InputClass.h"
#include "GraphicsClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TimerClass.h"
#include "MovementClass.h"
#include "Camera.h"
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

	FPSClass* m_FPS;
	CPUClass* m_CPU;
	TimerClass* m_Timer;

	MovementClass* m_cameraPosition;
	Camera* m_Camera;

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