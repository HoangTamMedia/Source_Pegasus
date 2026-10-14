
#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "Include.h"

DownloadFileInfo::DownloadFileInfo()
{
	this->m_uFileLength = 0;
	RtlSecureZeroMemory(this->m_szFileName,sizeof(this->m_szFileName));
	RtlSecureZeroMemory(this->m_szLocalFilePath,sizeof(this->m_szLocalFilePath));
	RtlSecureZeroMemory(this->m_szRemoteFilePath,sizeof(this->m_szRemoteFilePath));
	RtlSecureZeroMemory(this->m_szTargerDirPath,sizeof(this->m_szTargerDirPath));
}

DownloadFileInfo::~DownloadFileInfo()
{

}

TCHAR*DownloadFileInfo::GetFileName()
{
	return m_szFileName;
}

TCHAR*DownloadFileInfo::GetLocalFilePath()
{
	return m_szLocalFilePath;
}

TCHAR*DownloadFileInfo::GetRemoteFilePath()
{
	return m_szRemoteFilePath;
}

TCHAR*DownloadFileInfo::GetTargetDirPath()
{
	return m_szTargerDirPath;
}

ULONGLONG DownloadFileInfo::GetFileLength()
{
	return this->m_uFileLength;
}

void DownloadFileInfo::SetFilePath(TCHAR* szFileName,TCHAR* szLocalFilePath,TCHAR* szRemoteFilePath,TCHAR* szTargerDirPath)
{
	StringCchCopy(this->m_szFileName,sizeof(this->m_szFileName),szFileName);
	StringCchCopy(this->m_szLocalFilePath,sizeof(this->m_szLocalFilePath),szLocalFilePath);
	StringCchCopy(this->m_szRemoteFilePath,sizeof(this->m_szRemoteFilePath),szRemoteFilePath);
	if(szTargerDirPath)
		StringCchCopy(this->m_szTargerDirPath,sizeof(this->m_szTargerDirPath),szTargerDirPath);
}

void  DownloadFileInfo::SetFileLength(ULONGLONG uFileLength)
{
	this->m_uFileLength = uFileLength;
}

DownloadServerInfo::DownloadServerInfo()
{
	this->m_nPort = 21;
	this->m_DownloaderType = FTP;
	this->m_dwReadBufferSize = DL_DEFAULT_BUFFER_SIZE;
	this->m_bOverWrite = 1;
	this->m_bPassive = 0;
	this->m_dwConnectTimeout = 0;
	RtlSecureZeroMemory(this->m_szServerURL,sizeof(this->m_szServerURL));
	RtlSecureZeroMemory(this->m_szUserID,sizeof(this->m_szUserID));
	RtlSecureZeroMemory(this->m_szPassword,sizeof(this->m_szPassword));
}

DownloadServerInfo::~DownloadServerInfo()
{

}

DWORD DownloadServerInfo::GetConnectTimeout()
{
	return this->m_dwConnectTimeout;
}

DownloaderType DownloadServerInfo::GetDownloaderType()
{
	return this->m_DownloaderType;
}

TCHAR*  DownloadServerInfo::GetPassword()
{
	return this->m_szPassword;
}

INTERNET_PORT DownloadServerInfo::GetPort()
{
	return this->m_nPort;
}

DWORD DownloadServerInfo::GetReadBufferSize()
{
	return this->m_dwReadBufferSize;
}

TCHAR*  DownloadServerInfo::GetServerURL()
{
	return this->m_szServerURL;
}

TCHAR* DownloadServerInfo::GetUserID()
{
	return this->m_szUserID;
}

BOOL DownloadServerInfo::IsOverWrite()
{
	return this->m_bOverWrite;
}

BOOL DownloadServerInfo::IsPassive()
{
	return this->m_bPassive;
}

void DownloadServerInfo::SetServerInfo(TCHAR* szServerURL,INTERNET_PORT nPort,TCHAR* szUserID,TCHAR* szPassword)
{
	char* search = _tcschr(szServerURL,':');
	if(search&&search[1]=='/'&&search[2]=='/')
		StringCchCopy(this->m_szServerURL,sizeof(this->m_szServerURL),search+3);
	else
		StringCchCopy(this->m_szServerURL,sizeof(this->m_szServerURL),szServerURL);
	this->m_nPort = nPort;
	StringCchCopy(this->m_szUserID,sizeof(this->m_szUserID),szUserID);
	StringCchCopy(this->m_szPassword,sizeof(this->m_szPassword),szPassword);
}

void DownloadServerInfo::SetDownloaderType(DownloaderType dwDownloaderType)
{
	this->m_DownloaderType = dwDownloaderType;
}

void DownloadServerInfo::SetReadBufferSize(DWORD dwReadBufferSize)
{
	this->m_dwReadBufferSize = dwReadBufferSize;
}

void DownloadServerInfo::SetOverWrite(BOOL bOverWrite)
{
	this->m_bOverWrite = bOverWrite;
}

void DownloadServerInfo::SetPassiveMode(BOOL bPassive)
{
	this->m_bPassive = bPassive;
}

void DownloadServerInfo::SetConnectTimeout(DWORD dwConnectTimeout)
{
	this->m_dwConnectTimeout = dwConnectTimeout;
}
#endif