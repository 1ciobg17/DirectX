////////////////////
// Filename: TimerClass.h
//////////////////////////
#pragma once

#include <Windows.h>

class TimerClass
{
private:
	INT64 m_frequency;
	float m_ticksPerMS;
	INT64 m_startTime;
	float m_frameTime;
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();
};