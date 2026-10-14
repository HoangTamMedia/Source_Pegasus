
#pragma once

#include "GameShop\ShopListManager\interface\WZResult\ErrorCodeDefine.h"
#define	MAX_ERROR_MESSAGE	1024

class WZResult
{
public:

	WZResult();
	~WZResult();

	WZResult &	operator=(const WZResult& result);
	BOOL		IsSuccess();
	TCHAR *		GetErrorMessage();
	DWORD		GetErrorCode();
	DWORD		GetWindowErrorCode();

	void SetSuccessResult();
	void SetResult(DWORD dwErrorCode,
				   DWORD dwWindowErrorCode,
				   TCHAR* szFormat,
				   ...);

	static WZResult BuildSuccessResult();
	static WZResult BuildResult(DWORD dwErrorCode,
								DWORD dwWindowErrorCode,
								TCHAR* szFormat,
								...);

private:

	DWORD	m_dwErrorCode;
	DWORD	m_dwWindowErrorCode;
	TCHAR	m_szErrorMessage[MAX_ERROR_MESSAGE];
};
