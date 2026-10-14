
#pragma once

#include "include.h"
#include "ListManager.h"
#include "ShopList.h"

class CShopListManager : public CListManager
{
public:
	CShopListManager();
	virtual ~CShopListManager();

	CShopList*		GetListPtr() {return m_ShopList;};

private:
	CShopList*		m_ShopList;

	WZResult		LoadScript(bool bDonwLoad);
};