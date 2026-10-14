
#if !defined(AFX_GLOBALPORTALSYSTEM_H__CF0EA9D0_8E05_4094_9284_E3BAEC47BE40__INCLUDED_)
#define AFX_GLOBALPORTALSYSTEM_H__CF0EA9D0_8E05_4094_9284_E3BAEC47BE40__INCLUDED_

#pragma once

#ifdef KJH_MOD_LIBRARY_LINK_EACH_NATION
#include <ISharemem.h>
#else
#include "ISharemem.h"
#endif

#ifdef LDK_ADD_GLOBAL_PORTAL_SYSTEM

class GlobalPortalSystem
{
private:
	bool m_isAuthSet;

	char m_Id[WL_ID_LENGTH];

public:
	static GlobalPortalSystem& Instance();
	virtual ~GlobalPortalSystem();

	void Initialize();

	void SetPathToRegistry(HKEY* hKey);

	bool SetAuthInfo();
	bool ResetAuthInfo();

	const char* GetAuthID() { return m_Id; }

	bool IsAuthSet() { return m_isAuthSet; }

private:
	GlobalPortalSystem() { Initialize(); }

};

#endif
#endif
