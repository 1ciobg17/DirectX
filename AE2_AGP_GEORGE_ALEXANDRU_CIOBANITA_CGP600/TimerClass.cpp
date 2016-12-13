/////////////////////////
// Filename: TimerClass.cpp
///////////////////////////
#include "TimerClass.h"

TimerClass::TimerClass()
{

}

TimerClass::TimerClass(const TimerClass& other)
{

}

TimerClass::~TimerClass()
{

}

bool TimerClass::Initialize()
{
	//Check to see if this system supports high performance timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}
	
	//find out how many times the frequency counter ticks every millisecond
	m_ticksPerMS = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMS;

	m_startTime = currentTime;

	return;
}

float TimerClass::GetTime()
{
	return m_frameTime;
}