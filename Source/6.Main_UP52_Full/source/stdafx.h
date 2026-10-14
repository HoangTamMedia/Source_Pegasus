
#pragma once

#pragma warning( disable : 4067 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4237 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4091 )
#pragma warning( disable : 4819 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4838 )

#pragma warning( disable : 5208 4018 4477 4789 4101 4700)

#pragma warning( disable : 28159 )
#pragma warning( disable : 26812 )

#define WIN32_LEAN_AND_MEAN

#ifndef _USE_32BIT_TIME_T
	#define _USE_32BIT_TIME_T
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#pragma warning( push, 3 )

#include <windows.h>
#include <random>

#include <WinSock2.h>
#include <mmsystem.h>
#include <shellapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <mbstring.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <conio.h>

#include <string>
#include <list>
#include <map>
#include <deque>
#include <algorithm>
#include <vector>
#include <queue>

#pragma warning( pop )

#include <gl/glew.h>
#include <gl/gl.h>

#include "Winmain.h"
#include "Defined_Global.h"

#include "_define.h"
#include "_enum.h"
#include "_types.h"
#include "_struct.h"
#include "w_WindowMessageHandler.h"
#include "_GlobalFunctions.h"
#include "_TextureIndex.h"
#include "InfoHelperFunctions.h"
#include "UIDefaultBase.h"
#include "NewUICommon.h"
#include "./Math/ZzzMathLib.h"
#include "ZzzOpenglUtil.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#define CURL_STATICLIB
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"Normaliz.lib")
#ifdef _DEBUG
#pragma comment(lib,"..\\ExternalObject\\curl\\libcurl_a_debug.lib")
#else
#pragma comment(lib,"..\\ExternalObject\\curl\\libcurl_a.lib")

#endif
#include "ExternalObject\\FreeImage.h"
#pragma comment(lib, "..\\ExternalObject\\FreeImage.lib")

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
#define GetWindowsX()								(WindowWidth  / g_fScreenRate_x)
#define GetWindowsY()								(WindowHeight / g_fScreenRate_y)
#define setPosCenterX(x)							((GetWindowsX() / 2.f) - (x / 2.f))
#define setPosCenterY(x)							((GetWindowsY() / 2.f) - (x / 2.f) - 51.f)
#define setPosRight(x)								(GetWindowsX() - 640.f + x )
#define setPosDown(x)								(GetWindowsY() - 480.f + x )
#define GetCenterX(x)							((GetWindowsX() - x) / 2.f)
#define GetCenterY(x)							(((GetWindowsY() - x) /  2.f) - 51.f)
#define GetSubCenterY(x)							(((GetWindowsY - x) /  2.f) - 51.f)
#define setPosMidRight(x)						((GetWindowsX - 640.f) / 2.f + x )
#define setPosMidDown(x)						((GetWindowsY - 480.f) / 2.f + x )
#define SubWindowsY(x)							(GetWindowsY() - x )
#define SubWindowsX(x)							(GetWindowsX() - x )
#define JEWELBANKVER2				1