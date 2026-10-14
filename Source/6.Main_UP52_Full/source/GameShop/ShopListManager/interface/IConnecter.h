
#pragma once

#include "GameShop\ShopListManager\interface\WZResult\WZResult.h"
#include "GameShop\ShopListManager\interface\DownloadInfo.h"

class IConnecter
{
public:

	IConnecter(DownloadServerInfo *	pServerInfo,
			   DownloadFileInfo *	pFileInfo)
			   :	m_pServerInfo(pServerInfo),
					m_pFileInfo(pFileInfo)
	{};
	~IConnecter(){};

	virtual WZResult		CreateSession(HINTERNET& hSession) = 0;

	virtual WZResult		CreateConnection(HINTERNET& hSession,
											 HINTERNET& hConnection) = 0;

	virtual WZResult		OpenRemoteFile(HINTERNET& hConnection,
										   HINTERNET& hRemoteFile,
										   ULONGLONG& nFileLength) = 0;

	virtual WZResult		ReadRemoteFile(HINTERNET& hRemoteFile,
										   BYTE* byReadBuffer,
										   DWORD* dwBytesRead) = 0;

protected:

	WZResult 				m_Result;

	DownloadServerInfo *	m_pServerInfo;

	DownloadFileInfo *		m_pFileInfo;
};
