//////////////////////////
// Filename: CPUClass.cpp
///////////////////////////
#include "CPUClass.h"

CPUClass::CPUClass()
{

}

CPUClass::CPUClass(const CPUClass& other)
{

}

CPUClass::~CPUClass()
{

}

void CPUClass::Initialize()
{
	PDH_STATUS status;

	//initialize the flag indicating whether this object can read the system cpu usage or not
	m_canReadCPU = true;

	//create a query object to poll cpu usage
	status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCPU = false;
	}

	//set query object to poll all cpus in the system
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_TOTAL)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_canReadCPU = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;

	return;
}

void CPUClass::Shutdown()
{
	if (m_canReadCPU)
	{
		PdhCloseQuery(m_queryHandle);
	}

	return;
}

void CPUClass::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCPU)
	{
		if ((m_lastSampleTime + 1000) < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}

	return;
}

int CPUClass::GetCPUPercentage()
{
	int usage;

	if (m_canReadCPU)
	{
		usage = (int)m_cpuUsage;
	}
	else
	{
		usage = 0;
	}

	return usage;
}