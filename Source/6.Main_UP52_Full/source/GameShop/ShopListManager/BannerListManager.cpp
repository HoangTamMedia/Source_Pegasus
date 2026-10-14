
#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "BannerListManager.h"

CBannerListManager::CBannerListManager()
{
	this->m_BannerInfoList = new CBannerInfoList();

	this->m_vScriptFiles.push_back(BANNER_SCRIPT_FILENAME);
}

CBannerListManager::~CBannerListManager()
{
	SAFE_DELETE(m_BannerInfoList);
}

WZResult			CBannerListManager::LoadScript(bool bDonwLoad)
{
	std::string path = this->GetScriptPath();

	return this->m_BannerInfoList->LoadBanner(path,BANNER_SCRIPT_FILENAME,bDonwLoad);
}

#endif