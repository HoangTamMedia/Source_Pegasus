
#ifndef _LAUNCHERHELPER_H_
#define _LAUNCHERHELPER_H_

#pragma once

#pragma warning(disable : 4786)
#include <string>

#pragma pack(push, 1)
typedef struct __LAUNCHINFO {
	std::string ip;
	unsigned long port;
} WZLAUNCHINFO, *LPWZLAUNCHINFO;
#pragma pack(pop)

bool wzRegisterConnectionKey();
void wzUnregisterConnectionKey();
unsigned long wzGetConnectionKey();

bool wzPushLaunchInfo(const WZLAUNCHINFO& LaunchInfo);

bool wzPopLaunchInfo(WZLAUNCHINFO& LaunchInfo);

#endif