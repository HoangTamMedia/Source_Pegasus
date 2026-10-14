
#include "stdafx.h"
#include "Protect.h"
#include ".\\Utilities\\CCRC32.H"
#include "Util.h"

CProtect gProtect;

CProtect::CProtect()
{
	ZeroMemory(&this->m_MainInfo, sizeof(this->m_MainInfo));
	ZeroMemory(&this->m_TextInfo, sizeof(this->m_TextInfo));
}

CProtect::~CProtect()
{

}

BYTE EncDecKey1 = 0;
BYTE EncDecKey2 = 0;
bool CProtect::CheckSocketPort(SOCKET s)
{
	SOCKADDR_IN addr;
	int addr_len = sizeof(addr);

	if (getpeername(s, (SOCKADDR*)& addr, &addr_len) == SOCKET_ERROR)
	{
		return 0;
	}

	if (PORT_RANGE(ntohs(addr.sin_port)) == 0)
	{
		return 0;
	}

	return 1;
}
void CProtect::DecryptData(BYTE* lpMsg, int size)
{
	if (lpMsg == NULL || size <= 0)
	{
		return;
	}

	for (int n = 0; n < size; n++)
	{
		lpMsg[n] ^= EncDecKey1;
		int temp = (int)lpMsg[n] - (int)EncDecKey2;
		lpMsg[n] = (BYTE)(temp & 0xFF);
	}
}

void CProtect::EncryptData(BYTE* lpMsg, int size)
{
	if (lpMsg == NULL || size <= 0)
	{
		return;
	}

	for (int n = 0; n < size; n++)
	{
		int temp = (int)lpMsg[n] + (int)EncDecKey2;
		lpMsg[n] = (BYTE)(temp & 0xFF);
		lpMsg[n] ^= EncDecKey1;
	}
}
void CProtect::LoadEncDec()
{
#if(ENCRYPT_STATE==1)

	WORD EncDecKey = 0;

	for (int n = 0; n < sizeof(gProtect.m_MainInfo.CustomerName); n++)
	{
		EncDecKey += (WORD)((gProtect.m_MainInfo.CustomerName[n] ^ gProtect.m_MainInfo.ClientSerial[(n % sizeof(gProtect.m_MainInfo.ClientSerial))]) & 0xFF);
	}

	EncDecKey1 = BEncDecKey1;
	EncDecKey2 = BEncDecKey2;

	EncDecKey1 = (BYTE)(((int)EncDecKey1 + (int)LOBYTE(EncDecKey)) & 0xFF);
	EncDecKey2 = (BYTE)(((int)EncDecKey2 + (int)HIBYTE(EncDecKey)) & 0xFF);
#endif
}
bool CProtect::ReadDllFile(char* name)
{
	HMODULE hDll = LoadLibraryA(name);

	if (hDll == NULL)
	{
		return false;
	}

	return true;
}

bool CProtect::ReadMainFile(char* name)
{
	CCRC32 CRC32;

	if (CRC32.FileCRC(name, &this->m_ClientFileCRC, 1024) == 0)
	{
		return 0;
	}

	HANDLE file = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);

	if (file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if (GetFileSize(file, 0) != sizeof(MAIN_FILE_INFO))
	{
		CloseHandle(file);
		return 0;
	}

	DWORD OutSize = 0;

	if (ReadFile(file, &this->m_MainInfo, sizeof(MAIN_FILE_INFO), &OutSize, 0) == 0)
	{
		CloseHandle(file);
		return 0;
	}

	for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)& this->m_MainInfo)[n] -= (BYTE)(0x95 ^ HIBYTE(n));
		((BYTE*)& this->m_MainInfo)[n] ^= (BYTE)(0xCA ^ LOBYTE(n));
	}

	CloseHandle(file);

	return 1;
}

bool CProtect::ReadTextFile(char* name)
{
	HANDLE file = CreateFile(name,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,0);

	if(file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if(GetFileSize(file,0) != sizeof(TEXT_FILE_INFO))
	{
		CloseHandle(file);
		MessageBox(0, "ReadTextFile 1", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	DWORD OutSize = 0;

	if(ReadFile(file,&this->m_TextInfo,sizeof(TEXT_FILE_INFO),&OutSize,0) == 0)
	{
		CloseHandle(file);
		MessageBox(0, "ReadTextFile 2", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	for(int n=0;n < sizeof(TEXT_FILE_INFO);n++)
	{
		((BYTE*)&this->m_TextInfo)[n] -= (BYTE)(0x95^HIBYTE(n));
		((BYTE*)&this->m_TextInfo)[n] ^= (BYTE)(0xCA^LOBYTE(n));
	}

	CloseHandle(file);

	return 1;
}

void CProtect::CheckLauncher()
{
	if((this->m_MainInfo.LauncherType & 1) == 0)
	{
		return;
	}

	if(OpenMutex(MUTEX_ALL_ACCESS,0,"XTLauncherExecutable") == 0)
	{
		ExitProcess(0);
	}
}

void CProtect::CheckInstance()
{
	if((this->m_MainInfo.LauncherType & 2) == 0)
	{
		return;
	}

	char buff[256];

	wsprintf(buff,"XTEAM_MAIN_10405_%s",this->m_MainInfo.IpAddress);

	if(OpenMutex(MUTEX_ALL_ACCESS,0,buff) == 0)
	{
		CreateMutex(0,0,buff);
	}
	else
	{
		ExitProcess(0);
	}
}

void CProtect::CheckClientFile()
{
	if(this->m_MainInfo.ClientCRC32 == 0)
	{
		return;
	}

	char name[MAX_PATH] = {0};

	if(GetModuleFileName(0,name,sizeof(name)) == 0)
	{
		ExitProcess(0);
	}

	if(_stricmp(ConvertModuleFileName(name),this->m_MainInfo.ClientName) != 0)
	{
		ExitProcess(0);
	}

	CCRC32 CRC32;

	DWORD ClientCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.ClientName,&ClientCRC32,1024) == 0)
	{
		ExitProcess(0);
	}

	if(this->m_MainInfo.ClientCRC32 != ClientCRC32)
	{
		ExitProcess(0);
	}
}

void CProtect::CheckPluginFile()
{
	if(this->m_MainInfo.PluginCRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD PluginCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.PluginName,&PluginCRC32,1024) == 0)
	{
		ExitProcess(0);
	}

	if(this->m_MainInfo.PluginCRC32 != PluginCRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName);

	if(module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module,"EntryProc");

	if(EntryProc != 0)
	{
		EntryProc();
	}
}

void CProtect::CheckCameraFile()
{
	if(this->m_MainInfo.CameraCRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD CameraCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.CameraName,&CameraCRC32,1024) == 0)
	{
		ExitProcess(0);
	}

	if(this->m_MainInfo.CameraCRC32 != CameraCRC32)
	{
		ExitProcess(0);
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.CameraName);

	if(module == 0)
	{
		ExitProcess(0);
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module,"EntryProc");

	if(EntryProc != 0)
	{
		EntryProc();
	}
}
