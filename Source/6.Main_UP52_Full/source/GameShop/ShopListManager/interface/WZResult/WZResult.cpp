
#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "WZResult.h"
#include <crtdbg.h>
#include <strsafe.h>

WZResult::WZResult()
{
	this->SetSuccessResult();
}

WZResult::~WZResult()
{

}

BOOL WZResult::IsSuccess()
{
	return !this->m_dwErrorCode&&!this->m_dwWindowErrorCode;
}

DWORD  WZResult::GetErrorCode()
{
	return this->m_dwErrorCode;
}

char*  WZResult::GetErrorMessage()
{
	return this->m_szErrorMessage;
}

DWORD  WZResult::GetWindowErrorCode()
{
	return this->m_dwWindowErrorCode;
}

WZResult& WZResult::operator=(const WZResult& a2)
{
	this->m_dwErrorCode = a2.m_dwErrorCode;
	this->m_dwWindowErrorCode = a2.m_dwWindowErrorCode;
	StringCchCopy(this->m_szErrorMessage,sizeof(this->m_szErrorMessage),a2.m_szErrorMessage);
	return *this;
}

void WZResult::SetResult(DWORD dwErrorCode,DWORD dwWindowErrorCode,TCHAR* szFormat,...)
{
	va_list va;

	va_start(va,szFormat);
	this->m_dwErrorCode = dwErrorCode;
	this->m_dwWindowErrorCode = dwWindowErrorCode;
	StringCchVPrintf(this->m_szErrorMessage,sizeof(this->m_szErrorMessage),szFormat,va);
}

void WZResult::SetSuccessResult()
{
	this->m_dwErrorCode = WZ_SUCCESS;
	this->m_dwWindowErrorCode = ERROR_SUCCESS;
	StringCchCopy(this->m_szErrorMessage,sizeof(this->m_szErrorMessage),"Success");
}

WZResult WZResult::BuildSuccessResult()
{
	WZResult result;

	result.SetSuccessResult();

	return result;
}

WZResult WZResult::BuildResult(DWORD dwErrorCode,DWORD dwWindowErrorCode,TCHAR* szFormat,...)
{
	WZResult result;
	char Buffer[MAX_ERROR_MESSAGE];
	va_list args;

	va_start(args,szFormat);

	memset(Buffer,0,sizeof(Buffer));
	StringCchVPrintf(Buffer,sizeof(Buffer),szFormat,args);

	result.SetResult(dwErrorCode,dwWindowErrorCode,Buffer);

	return result;
}

#endif