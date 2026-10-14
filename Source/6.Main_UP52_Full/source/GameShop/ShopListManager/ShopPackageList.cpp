
#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "ShopPackageList.h"

CShopPackageList::CShopPackageList()
{
	this->m_Packages.clear();
}

CShopPackageList::~CShopPackageList()
{

}

int CShopPackageList::GetSize()
{
	return static_cast<int>(this->m_Packages.size());
}

void CShopPackageList::Clear()
{
	this->m_Packages.clear();
}

void CShopPackageList::Append(CShopPackage package)
{
	this->m_Packages.insert(std::make_pair(package.PackageProductSeq,package));
}

void CShopPackageList::SetFirst()
{
	this->m_iter = this->m_Packages.begin();
}

bool CShopPackageList::GetNext(CShopPackage& package)
{
	if(this->m_iter==this->m_Packages.end())
		return 0;

	package = this->m_iter->second;
	this->m_iter++;
	return 1;
}

bool CShopPackageList::GetValueByKey(int nKey,CShopPackage& package)
{
	std::map<int,CShopPackage>::iterator it = this->m_Packages.find(nKey);

	if(it==this->m_Packages.end())
		return 0;

	package = it->second;

	return 1;
}

bool CShopPackageList::GetValueByIndex(int nIndex,CShopPackage& package)
{
	if(nIndex<0||nIndex>=static_cast<int>(this->m_PackageIndex.size()))
	{
		return 0;
	}

	return this->GetValueByKey(this->m_PackageIndex[nIndex],package);
}

bool CShopPackageList::SetPacketLeftCount(int PackageSeq,int nCount)
{
	std::map<int,CShopPackage>::iterator it = this->m_Packages.find(PackageSeq);

	if(it==this->m_Packages.end())
		return 0;

	it->second.LeftCount = nCount;

	return 1;
}
#endif