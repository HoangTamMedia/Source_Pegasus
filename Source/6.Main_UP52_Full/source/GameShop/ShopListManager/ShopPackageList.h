
#pragma once

#include "ShopPackage.h"
#include <map>

class CShopPackageList
{
public:
	CShopPackageList(void);
	~CShopPackageList(void);

	int GetSize();
	void Clear();

	virtual void Append(CShopPackage package);

	void SetFirst();
	bool GetNext(CShopPackage& package);

	bool GetValueByKey(int nKey, CShopPackage& package);
	bool GetValueByIndex(int nIndex, CShopPackage& package);

	bool SetPacketLeftCount(int PackageSeq, int nCount);

protected:
	std::map<int, CShopPackage> m_Packages;
	std::map<int, CShopPackage>::iterator m_iter;
	std::vector<int> m_PackageIndex;
};
