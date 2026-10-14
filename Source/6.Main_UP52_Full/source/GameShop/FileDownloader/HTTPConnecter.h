
#pragma once

#include "GameShop\ShopListManager\interface\IConnecter.h"

class HTTPConnecter : public IConnecter
{
public:

	HTTPConnecter(DownloadServerInfo *	pServerInfo,
				   DownloadFileInfo *	pFileInfo);
	~HTTPConnecter();

	virtual WZResult		CreateSession(HINTERNET& hSession);

	virtual WZResult		CreateConnection(HINTERNET& hSession,
											 HINTERNET& hConnection);

	virtual WZResult		OpenRemoteFile(HINTERNET& hConnection,
										   HINTERNET& hRemoteFile,
										   ULONGLONG& nFileLength);

	virtual WZResult		ReadRemoteFile(HINTERNET& hRemoteFile,
										   BYTE* byReadBuffer,
										   DWORD* dwBytesRead);
};
