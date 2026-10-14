
#pragma once

#include "ShopCategory.h"
#include <map>

class CShopCategoryList
{
public:
	CShopCategoryList(void);
	~CShopCategoryList(void);

	void Clear();

	int GetSize();
	virtual void Append(CShopCategory category);

	void SetFirst();
	bool GetNext(CShopCategory& category);

	bool GetValueByKey(int nKey, CShopCategory& category);
	bool GetValueByIndex(int nIndex, CShopCategory& category);

	bool InsertPackage(int Category, int Package);
	bool RefreshPackageSeq(int Category, int PackageSeqs[], int PackageCount);

protected:
	std::map<int, CShopCategory> m_Categroys;
	std::map<int, CShopCategory>::iterator m_Categoryiter;
	std::vector<int> m_CategoryIndex;

};
