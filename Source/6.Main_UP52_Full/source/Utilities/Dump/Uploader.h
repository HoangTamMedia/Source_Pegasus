#pragma once

#include <wininet.h>

typedef struct
{
	HINTERNET* pInternetSession;
	HINTERNET* pFtpConnection;
	LPCTSTR szUrl;
	int nPort;
	LPCTSTR szId;
	LPCTSTR szPassword;
} FTP_PARM;

class CUploader
{
public:
	const static int BUFFER_SIZE = 256;
	const static DWORD TIME_OUT = 3000;

	CUploader(LPCTSTR szUrl, int nPort, LPCTSTR szId, LPCTSTR szPassword, int nUploadCount);
	virtual ~CUploader();

	virtual int		UploadFTP(LPCTSTR szLocalFile, LPCTSTR szServerFile);

protected:
	TCHAR		m_szUrl[BUFFER_SIZE];
	int			m_nPort;
	TCHAR		m_szId[BUFFER_SIZE];
	TCHAR		m_szPassword[BUFFER_SIZE];
	int			m_nUploadCount;
};
