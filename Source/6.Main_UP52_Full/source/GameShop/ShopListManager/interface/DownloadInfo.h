
#pragma once

#include <Wininet.h>
#define DL_DEFAULT_BUFFER_SIZE			4096

typedef enum _DownloaderType
{
	FTP,
	HTTP,
} DownloaderType;

class DownloadFileInfo
{
public:

	DownloadFileInfo();
	~DownloadFileInfo();

	TCHAR *		GetFileName();
	TCHAR *		GetLocalFilePath();
	TCHAR *		GetRemoteFilePath();
	TCHAR *		GetTargetDirPath();
	ULONGLONG	GetFileLength();

	void	SetFilePath(TCHAR * szFileName,
						TCHAR * szLocalFilePath,
						TCHAR * szRemoteFilePath,
						TCHAR * szTargerDirPath);
	void	SetFileLength(ULONGLONG uFileLength);

private:

	TCHAR		m_szFileName[MAX_PATH];

	TCHAR		m_szLocalFilePath[MAX_PATH];

	TCHAR		m_szRemoteFilePath[INTERNET_MAX_URL_LENGTH];

	TCHAR		m_szTargerDirPath[MAX_PATH];

	ULONGLONG	m_uFileLength;
};

class DownloadServerInfo
{
public:

	DownloadServerInfo();
	~DownloadServerInfo();

	TCHAR *			GetServerURL();

	TCHAR *			GetUserID();

	TCHAR *			GetPassword();

	INTERNET_PORT	GetPort();

	DownloaderType	GetDownloaderType();

	DWORD			GetReadBufferSize();

	DWORD			GetConnectTimeout();

	BOOL			IsOverWrite();

	BOOL			IsPassive();

	void			SetServerInfo(TCHAR *			szServerURL,
								  INTERNET_PORT		nPort,
								  TCHAR *			szUserID,
								  TCHAR *			szPassword);

	void			SetDownloaderType(DownloaderType dwDownloaderType);

	void			SetReadBufferSize(DWORD dwReadBufferSize);

	void			SetOverWrite(BOOL bOverWrite);

	void			SetPassiveMode(BOOL bPassive);

	void			SetConnectTimeout(DWORD dwConnectTimeout);

private:

	TCHAR						m_szServerURL[INTERNET_MAX_URL_LENGTH];

	TCHAR						m_szUserID[INTERNET_MAX_USER_NAME_LENGTH];

	TCHAR						m_szPassword[INTERNET_MAX_PASSWORD_LENGTH];

	INTERNET_PORT				m_nPort;

	DownloaderType				m_DownloaderType;

	DWORD						m_dwReadBufferSize;

	BOOL						m_bOverWrite;

	BOOL						m_bPassive;

	DWORD						m_dwConnectTimeout;
};
