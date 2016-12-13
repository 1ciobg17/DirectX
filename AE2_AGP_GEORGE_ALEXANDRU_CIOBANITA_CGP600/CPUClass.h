/////////////////////////
// Filename:CPUClass.h
////////////////////////
#pragma once

#pragma comment(lib, "pdh.lib")

#include <pdh.h>

class CPUClass
{
private:
	bool m_canReadCPU;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
public:
	CPUClass();
	CPUClass(const CPUClass&);
	~CPUClass();
	void Initialize();
	void Shutdown();
	void Frame();
	int GetCPUPercentage();
};