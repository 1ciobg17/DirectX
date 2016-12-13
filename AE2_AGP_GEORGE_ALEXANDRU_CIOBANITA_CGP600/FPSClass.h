////////////////////////////
// Filename: FPSClass.h
////////////////////////////
#pragma once

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class FPSClass
{
private:
	int m_fps, m_count;
	unsigned long m_startTime;
public:
	FPSClass();
	FPSClass(const FPSClass&);
	~FPSClass();
	void Initialize();
	void Frame();
	int GetFPS();
};