#include "stdafx.h"
#include "./Utilities/Log/ErrorReport.h"
#include "GlobalPortalSystem.h"

#ifdef LDK_ADD_GLOBAL_PORTAL_SYSTEM

GlobalPortalSystem& GlobalPortalSystem::Instance()
{
	static GlobalPortalSystem m_GlobalPortalSys;
	return m_GlobalPortalSys;
}

GlobalPortalSystem::~GlobalPortalSystem()
{

}

void GlobalPortalSystem::Initialize()
{
	g_ErrorReport.Write( "-- ۷ιŻ ʱȭ -- \r\n" );
	ZeroMemory(m_Id, sizeof(m_Id));

	m_isAuthSet = FALSE;
	m_isAuthSet = SetAuthInfo();
}

void GlobalPortalSystem::SetPathToRegistry(	HKEY* hKey )
{
	char szCurrentDir[256];
	ZeroMemory(szCurrentDir, sizeof(szCurrentDir));
	GetCurrentDirectory(256, szCurrentDir);
	strcat(szCurrentDir, "\\Mu.exe");
	RegSetValueEx( *hKey, "Exe", 0, REG_SZ, (const unsigned char*)szCurrentDir, sizeof(szCurrentDir) );

}

bool GlobalPortalSystem::SetAuthInfo()
{
	if( m_isAuthSet )
	{

		g_ErrorReport.Write( "-- Ű  Լ ߺ ȣ -- \r\n" );
		return FALSE;
	}

	char strGameName[128];
 	ZeroMemory(strGameName, sizeof(strGameName));
	strcat(strGameName, "MU");

 	WLAUTH_INFO AuthInfo = {0};

	RMSM sm(CreateShareMem());
	int result = (sm.get())->ReadFromShareMemory(strGameName, &AuthInfo);
 	if( 0 != result )
  	{
		g_ErrorReport.Write( "-- Ű   - %d -- \r\n", result );
  		return FALSE;
  	}

	strcpy( m_Id, AuthInfo.id );
	g_ErrorReport.Write("-- Ű id - %s -- ", AuthInfo.id);

	return TRUE;
}

bool GlobalPortalSystem::ResetAuthInfo()
{
	m_isAuthSet = FALSE;
	m_isAuthSet = SetAuthInfo();

	return m_isAuthSet;
}

#endif
