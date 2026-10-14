
#pragma once

#include "GameShop\ShopListManager\interface\IConnecter.h"
#include "GameShop\ShopListManager\interface\IDownloaderStateEvent.h"

class FileDownloader
{
public:

	FileDownloader(IDownloaderStateEvent *	pStateEvent,
				   DownloadServerInfo *		pServerInfo,
				   DownloadFileInfo *		pFileInfo);
	~FileDownloader();

	void				Break();

	WZResult			DownloadFile();

private:

	BOOL				CanBeContinue();

	void				Release();

	IConnecter *		CreateConnecter();

	WZResult 			CreateConnection();
	static unsigned int __stdcall RunConnectThread(LPVOID pParam);
	WZResult 			Connection();

	WZResult 			TransferRemoteFile();

	WZResult 			CreateLocalFile();

	WZResult 			ReadRemoteFile(BYTE* byReadBuffer, DWORD* dwBytesRead);

	WZResult 			WriteLocalFile(BYTE* byReadBuffer, DWORD dwBytesRead);

	void				SendStartedDownloadFileEvent(ULONGLONG nFileLength);

	void				SendCompletedDownloadFileEvent(WZResult wzResult);

	void				SendProgressDownloadFileEvent(ULONGLONG nTotalBytesRead);

	volatile BOOL				m_bBreak;

	WZResult 					m_Result;

	IDownloaderStateEvent *		m_pStateEvent;

	DownloadServerInfo *		m_pServerInfo;

	DownloadFileInfo *			m_pFileInfo;

	IConnecter *				m_pConnecter;

	HINTERNET					m_hSession;

	HINTERNET					m_hConnection;

	HINTERNET					m_hRemoteFile;

	HANDLE						m_hLocalFile;

	ULONGLONG					m_nFileLength;
};
