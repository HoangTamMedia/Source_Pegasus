
#include "stdafx.h"

#include "muConsoleDebug.h"
#include <iostream>
#include "ZzzInterface.h"
#include "ZzzOpenglUtil.h"
#include "WindowsConsole.h"
#include "./Utilities/Log/ErrorReport.h"
#include "GlobalBitmap.h"
#include "ZzzTexture.h"
#ifdef CSK_DEBUG_MAP_PATHFINDING
#include "ZzzPath.h"
#endif
#include <ZzzAI.h>

CmuConsoleDebug::CmuConsoleDebug() : m_bInit(false)
{
#ifdef CSK_LH_DEBUG_CONSOLE
	if(leaf::OpenConsoleWindow("Mu Debug Console Window"))
	{
		leaf::ActivateCloseButton(false);

		leaf::ShowConsole(false);
		m_bInit = true;

		g_ErrorReport.Write("Mu Debug Console Window Init - completed(Handle:0x%00000008X)\r\n", leaf::GetConsoleWndHandle());
	}
#endif
}

CmuConsoleDebug::~CmuConsoleDebug()
{
#ifdef CSK_LH_DEBUG_CONSOLE
	leaf::CloseConsoleWindow();
#endif
}

CmuConsoleDebug* CmuConsoleDebug::GetInstance()
{
#ifdef CSK_LH_DEBUG_CONSOLE
	static CmuConsoleDebug sInstance;
	return &sInstance;
#else
	return 0;
#endif
}

void CmuConsoleDebug::UpdateMainScene()
{
#ifdef CSK_LH_DEBUG_CONSOLE
	if(m_bInit)
	{
		if(SEASON3B::IsPress(VK_SHIFT) == TRUE)
		{
			if(PressKey(VK_F7))
			{
				leaf::ShowConsole(!leaf::IsConsoleVisible());
			}
		}
	}
#endif
}

bool CmuConsoleDebug::CheckCommand(const std::string& strCommand)
{
#ifdef CSK_LH_DEBUG_CONSOLE
	if(!m_bInit)
		return false;

	if(strCommand.compare("$open") == NULL)
	{
		leaf::ShowConsole(true);
		return true;
	}
	else if(strCommand.compare("$close") == NULL)
	{
		leaf::ShowConsole(false);
		return true;
	}
	else if(strCommand.compare("$clear") == NULL)
	{
		leaf::SetConsoleTextColor();
		leaf::ClearConsoleScreen();
		return true;
	}
#ifdef CSK_DEBUG_MAP_ATTRIBUTE
	else if(strCommand.compare("$mapatt on") == NULL)
	{
		EditFlag = EDIT_WALL;
		return true;
	}
	else if(strCommand.compare("$mapatt off") == NULL)
	{
		EditFlag = EDIT_NONE;
		return true;
	}
#endif
#ifdef CSK_DEBUG_MAP_PATHFINDING
	else if(strCommand.compare("$path on") == NULL)
	{
		g_bShowPath = true;
	}
	else if(strCommand.compare("$path off") == NULL)
	{
		g_bShowPath = false;
	}
#endif
#ifdef CSK_DEBUG_RENDER_BOUNDINGBOX
	else if(strCommand.compare("$bb on") == NULL)
	{
		g_bRenderBoundingBox = true;
	}
	else if(strCommand.compare("$bb off") == NULL)
	{
		g_bRenderBoundingBox = false;
	}
#endif
	else if(strCommand.compare("$type_test") == NULL)
	{
		Write(MCD_SEND, "MCD_SEND");
		Write(MCD_RECEIVE, "MCD_RECEIVE");
		Write(MCD_ERROR, "MCD_ERROR");
		Write(MCD_NORMAL, "MCD_NORMAL");
		return true;
	}
	else if(strCommand.compare("$texture_info") == NULL)
	{
		Write(MCD_NORMAL, "Texture Number : %d", Bitmaps.GetNumberOfTexture());
		Write(MCD_NORMAL, "Texture Memory : %dKB", Bitmaps.GetUsedTextureMemory()/1024);
		return true;
	}
	else if(strCommand.compare("$color_test") == NULL)
	{
		leaf::SetConsoleTextColor(leaf::COLOR_DARKRED);
		std::cout << "color test: dark red" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_DARKGREEN);
		std::cout << "color test: dark green" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_DARKBLUE);
		std::cout << "color test: dark blue" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_RED);
		std::cout << "color test: red" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_GREEN);
		std::cout << "color test: green" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_BLUE);
		std::cout << "color test: blue" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_OLIVE);
		std::cout << "color test: olive" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_PURPLE);
		std::cout << "color test: purple" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_TEAL);
		std::cout << "color test: teal" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_GRAY);
		std::cout << "color test: gray" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_AQUA);
		std::cout << "color test: aqua" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_FUCHSIA);
		std::cout << "color test: fuchsia" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_YELLOW);
		std::cout << "color test: yellow" << std::endl;
		leaf::SetConsoleTextColor(leaf::COLOR_WHITE);
		std::cout << "color test: white" << std::endl;
		return true;
	}
#endif
	return false;
}
bool WriteInFile(LPCTSTR szBuffer)
{
	// This used to open the log twice and close it twice for every single line -
	// an fopen/fclose pair purely to test existence, then a CreateFile, a seek to
	// the end, the write, and a CloseHandle. On a folder that OneDrive is syncing
	// each of those trips the sync filter, so a line cost milliseconds. Combat
	// produces well over a hundred lines a second, all of it synchronous on the
	// render thread, which is what put a three hundred millisecond stall in the
	// middle of a fight.
	//
	// The handle is kept open instead, and reopened only when the date in the
	// file name rolls over. FILE_APPEND_DATA removes the seek as well.
	static HANDLE s_hFile = INVALID_HANDLE_VALUE;
	static WORD   s_wOpenDay = 0;

	SYSTEMTIME Time;
	GetLocalTime(&Time);

	if (s_hFile == INVALID_HANDLE_VALUE || s_wOpenDay != Time.wDay)
	{
		if (s_hFile != INVALID_HANDLE_VALUE)
			CloseHandle(s_hFile);

		char szPath[128] = { 0 };
		sprintf_s(szPath, "./Console_%02d-%02d-%02d.log",
			Time.wDay, Time.wMonth, Time.wYear);

		s_hFile = CreateFile(szPath, FILE_APPEND_DATA, FILE_SHARE_READ, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		s_wOpenDay = Time.wDay;

		if (s_hFile == INVALID_HANDLE_VALUE)
			return false;
	}

	char szMessageNewLine[560] = { 0 };
	sprintf_s(szMessageNewLine, "[%02d:%02d:%02d]%s\r\n",
		Time.wHour, Time.wMinute, Time.wSecond, szBuffer);

	DWORD dwWrite = 0;
	WriteFile(s_hFile, szMessageNewLine, (DWORD)strlen(szMessageNewLine), &dwWrite, NULL);

	return true;
}
void CmuConsoleDebug::Write(int iType, const char* pStr, ...)
{
#ifdef CONSOLE_DEBUG
	if(m_bInit)
	{
		// Setting the colour is a call into the console host, so it is only made
		// when the colour actually has to change. A run of packet lines is
		// nearly always the same type.
		static int s_iLastType = -1;
		if (iType != s_iLastType)
		{
			s_iLastType = iType;
			switch(iType)
			{
			case MCD_SEND:
				leaf::SetConsoleTextColor(leaf::COLOR_OLIVE);
				break;
			case MCD_RECEIVE:
				leaf::SetConsoleTextColor(leaf::COLOR_DARKGREEN);
				break;
			case MCD_ERROR:
				leaf::SetConsoleTextColor(leaf::COLOR_WHITE, leaf::COLOR_DARKRED);
				break;
			case MCD_NORMAL:
				leaf::SetConsoleTextColor(leaf::COLOR_GRAY);
				break;
			}
		}

		char szBuffer[256]="";
		va_list	pArguments;

		va_start(pArguments, pStr);
		vsprintf(szBuffer, pStr, pArguments);
		va_end(pArguments);

		// '\n' rather than std::endl: endl flushes, and a flush per line to a
		// Windows console is one of the slowest things this client does. The
		// stream still flushes on its own, and on exit.
		std::cout << szBuffer << '\n';

		WriteInFile(szBuffer);
	}
#endif
}

CmuSimpleLog::CmuSimpleLog(void)
{
#ifdef CSK_LH_DEBUG_CONSOLE
	m_bLogfirst = true;
	m_strFilename = "SimpleLog.txt";
	m_pFile = NULL;
#endif
}

CmuSimpleLog::~CmuSimpleLog(void)
{
}

void CmuSimpleLog::setFilename(const char* strFilename)
{
#ifdef CSK_LH_DEBUG_CONSOLE
	m_strFilename = strFilename;
#endif
}

void CmuSimpleLog::log(char *str, ...)
{
#ifdef CSK_LH_DEBUG_CONSOLE
	if(m_bLogfirst)
	{
		m_pFile = fopen(m_strFilename.c_str(), "a");
		fclose(m_pFile);
		m_bLogfirst = false;
	}

	m_pFile = fopen(m_strFilename.c_str(), "a");

	va_list ap;

	va_start (ap, str);
	vfprintf (m_pFile, str, ap);
	va_end (ap);

	fclose(m_pFile);
#endif
}
