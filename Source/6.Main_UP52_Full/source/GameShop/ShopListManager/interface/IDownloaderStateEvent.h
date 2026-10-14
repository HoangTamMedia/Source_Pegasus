
#pragma once

class IDownloaderStateEvent
{
public:

	IDownloaderStateEvent(){};
	virtual ~IDownloaderStateEvent(){};

	virtual void	OnStartedDownloadFile(TCHAR* szFileName, ULONGLONG uFileLength) = 0;

	virtual void	OnProgressDownloadFile(TCHAR* szFileName, ULONGLONG uDownloadFileLength) = 0;

	virtual void	OnCompletedDownloadFile(TCHAR* szFileName, WZResult wzResult) = 0;
};
