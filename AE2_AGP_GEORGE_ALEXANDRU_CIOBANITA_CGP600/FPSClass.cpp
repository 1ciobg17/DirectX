////////////////////
// Filename: FPSClass.cpp
/////////////////////////////
#include "FPSClass.h"

FPSClass::FPSClass()
{

}

FPSClass::FPSClass(const FPSClass& other)
{

}

FPSClass::~FPSClass()
{

}

void FPSClass::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}

void FPSClass::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		m_startTime = timeGetTime();
	}
}
int FPSClass::GetFPS()
{
	return m_fps;
}