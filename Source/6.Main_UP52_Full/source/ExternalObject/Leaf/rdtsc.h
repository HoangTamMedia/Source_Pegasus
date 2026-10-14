
#ifndef _RDTSC_H_
#define _RDTSC_H_

#include <windows.h>

namespace leaf {

	inline bool IsSupportRdtsc()
	{

		DWORD reg;
		__asm
		{
			mov eax, 1
			cpuid
			mov reg, edx
		}

		if (reg & ( 1 << 4)) {
			return true;
		}
		return false;
	}

	inline __int64 GetClockCount()
	{
		__int64 i64StartClock;
		__asm
		{
			rdtsc
			mov dword ptr [i64StartClock+4], edx
			mov dword ptr [i64StartClock], eax
		}
		return i64StartClock;
	}

	inline __int64 GetCPUFrequency(UINT uiMeasureMSecs)
	{

		if(false == IsSupportRdtsc())
			return ( 0);

		__int64 llFreq;
		if ( !QueryPerformanceFrequency( ( LARGE_INTEGER*) &llFreq))
		{
			return ( 0);
		}

		HANDLE hProcess = GetCurrentProcess();
		HANDLE hThread = GetCurrentThread();
		DWORD dwCurPriorityClass = GetPriorityClass(hProcess);
		int iCurThreadPriority = GetThreadPriority(hThread);
		DWORD dwProcessMask, dwSystemMask, dwNewMask = 1;
		GetProcessAffinityMask(hProcess, &dwProcessMask, &dwSystemMask);

		SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
		SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
		SetProcessAffinityMask(hProcess, dwNewMask);

		__asm { cpuid }

		__int64 llStartTime, llEndTime;
		__int64 llStart, llEnd;

		QueryPerformanceCounter((LARGE_INTEGER *) &llStartTime);

		__asm
		{
			rdtsc
			mov dword ptr [llStart+4], edx
			mov dword ptr [llStart], eax
		}

		Sleep(uiMeasureMSecs);

		QueryPerformanceCounter((LARGE_INTEGER *) &llEndTime);

		__asm
		{
			rdtsc
			mov dword ptr [llEnd+4], edx
			mov dword ptr [llEnd], eax
		}

		SetProcessAffinityMask(hProcess, dwProcessMask);
		SetThreadPriority(hThread, iCurThreadPriority);
		SetPriorityClass(hProcess, dwCurPriorityClass);

		__int64 llDif = llEnd - llStart;
		__int64 llTimeDif = llEndTime - llStartTime;

		__int64 llFrequency = (__int64)((( double)llDif) / ((( double)llTimeDif) / llFreq));

		return llFrequency;
	}
}

#endif