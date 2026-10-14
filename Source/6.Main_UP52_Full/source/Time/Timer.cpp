

#include "stdafx.h"
#include "Timer.h"

CTimer::CTimer()
{
	if (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency) == FALSE)
	{
		m_bUsePerformanceCounter = FALSE;

		TIMECAPS Caps;
		::timeGetDevCaps(&Caps, sizeof(Caps));

		if (::timeBeginPeriod(Caps.wPeriodMin) == TIMERR_NOCANDO)
		{
#ifdef __TIMER_DEBUG
			__TraceF(TEXT("timeBeginPeriod(...) Error\n"));

#endif
		}

		m_mmAbsTimerStart = m_mmTimerStart = ::timeGetTime();
	}
	else
	{
		m_bUsePerformanceCounter = TRUE;

		::QueryPerformanceCounter((LARGE_INTEGER*)&m_pcTimerStart);

		m_pcAbsTimerStart = m_pcTimerStart;
		m_resolution = (float)(1.0 / (double)m_frequency) * 1000.0f;
	}
}

CTimer::~CTimer()
{
	if (!m_bUsePerformanceCounter)
	{
		TIMECAPS Caps;
		::timeGetDevCaps(&Caps, sizeof(Caps));
		::timeEndPeriod(Caps.wPeriodMin);
	}
}

double CTimer::GetTimeElapsed()
{
	__int64 timeElapsed;

	if (m_bUsePerformanceCounter)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&timeElapsed);
		timeElapsed -= m_pcTimerStart;
		return (double)timeElapsed * (double)m_resolution;
	}
	else
	{
		timeElapsed = ::timeGetTime() - m_mmTimerStart;
		return (double)timeElapsed;
	}
}

double CTimer::GetAbsTime()
{
	__int64 absTime;

	if (m_bUsePerformanceCounter)
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&absTime);
		return (double)absTime * (double)m_resolution;
	}
	else
	{
		absTime = ::timeGetTime();
		return (double)absTime;
	}
}

void CTimer::ResetTimer()
{

	if (m_bUsePerformanceCounter)
		::QueryPerformanceCounter((LARGE_INTEGER*)&m_pcTimerStart);
	else
		m_mmTimerStart = ::timeGetTime();
}
