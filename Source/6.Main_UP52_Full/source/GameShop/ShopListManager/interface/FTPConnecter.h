
#pragma once

#include "GameShop\ShopListManager\interface\IConnecter.h"

class FTPConnecter : public IConnecter
{
public:

	FTPConnecter(DownloadServerInfo *	pServerInfo,
				 DownloadFileInfo *		pFileInfo);
	~FTPConnecter();

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
