#ifdef LEM_ADD_GAMECHU

#pragma once
#include "time.h"
#include "./Utilities/Log/ErrorReport.h"

#define MAX_GAMECHU_AUTHINFO 511
#define MAX_GAMECHU_USERINFO 8
#define __time64_t	__int64

struct gcBaseUserInfo
{
	wchar_t		szUserSerialNumber[64];
	__time64_t	ttTimeStamp;
	int			nPCBang;
	int			nSex;
	int			nAge;
    int         nPCBangServiceType;
	int			nLocation;
	int			nBirthYear;
	wchar_t		szNickName[64];
};

typedef int (*TF_CheckCertification)( const wchar_t*,
									  const wchar_t*,
									  const wchar_t*,
									  gcBaseUserInfo* ,
									  DWORD,
									  BOOL);

enum eGAMECHURETURN {	eDATA_SUCCESS		=0,
						eDATA_HASHVALUE		=1,
						eDATA_TIMEOUT		=10,
						eDATA_PASSINGFAIL	=100,
						eDATA_USERINFOERROR	=101,
						eDATA_PARAMERROR	=102,
						eDATA_UNKNOWNERROR  =1000
					};

class GCCertificaltionHelper
{
public:
	static GCCertificaltionHelper* Instance( void );
	virtual ~GCCertificaltionHelper();

protected:
	gcBaseUserInfo			m_UserData;
	wstring					m_wParam;
	wstring					m_wStatIndex;
	char					m_szParam[MAX_GAMECHU_AUTHINFO];
	char					m_szStat[MAX_GAMECHU_USERINFO+1];
	bool					m_bGameChu;
	string					m_szError;

	HINSTANCE				m_hInstGCLauncherDLL;
	TF_CheckCertification	m_pfCheckCertification;

public:
	void	Init( void );
	void	Release( void );

	void	Set_GameChuMyData			( PSTR _szCmdLine );
	void	Set_UserData				( gcBaseUserInfo _sInfo );
	void	Set_Error					( int _nVal );

	gcBaseUserInfo	Get_UserData( void )			{ return m_UserData; }
	wstring			Get_AuthInfo( void )			{ return m_wParam; }
	wstring			Get_StatInfo( void )			{ return m_wStatIndex; }
	char*			Get_AuthInfo_ToChar( void )		{ return m_szParam; }
	char*			Get_StatInfo_ToChar( void )		{ return m_szStat; }
	string			Get_Error( void )				{ return m_szError; }
	bool			Get_GameChuLogin( void )		{ return m_bGameChu; }

	#ifdef _DEBUG
		void Debug_GameChuMyDataSetBuffer( void );
	#endif

	BOOL Load_GamechuDLL(LPTSTR szFileName)
	{
		m_hInstGCLauncherDLL = LoadLibrary(szFileName);
		if( m_hInstGCLauncherDLL == NULL )
			return FALSE;

		m_pfCheckCertification = (TF_CheckCertification)GetProcAddress(m_hInstGCLauncherDLL, "CheckCertification" );

		return TRUE;
	}

	int  CheckCertification( const wchar_t*  wzString,
							 const wchar_t*  wzStatIndex,
							 const wchar_t*  wzServerKey,
							 gcBaseUserInfo* pUserInfo,
							 DWORD dTimeoutInterval)
	{
		if ( m_pfCheckCertification != NULL )
			return m_pfCheckCertification(wzString, wzStatIndex, wzServerKey, pUserInfo, dTimeoutInterval, FALSE);

		return 1000;
	}

private:
	GCCertificaltionHelper();

};

#define GAMECHU GCCertificaltionHelper::Instance()

#endif