

#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "ShopCategoryList.h"

CShopCategoryList::CShopCategoryList()
{
	this->Clear();
}

CShopCategoryList::~CShopCategoryList()
{

}

void CShopCategoryList::Clear()
{
	this->m_Categroys.clear();
	this->m_CategoryIndex.clear();
}

int CShopCategoryList::GetSize()
{
	return static_cast<int>(this->m_Categroys.size());
}

void CShopCategoryList::Append(CShopCategory category)
{
	if(this->m_Categroys.find(category.ProductDisplaySeq)!=this->m_Categroys.end())
		return;

	this->m_Categroys.insert(std::make_pair(category.ProductDisplaySeq,category));

	this->m_CategoryIndex.push_back(category.ProductDisplaySeq);

	if(category.Root!=1)
	{
		std::map<int,CShopCategory>::iterator it = this->m_Categroys.find(category.ParentProductDisplaySeq);

		if(it==this->m_Categroys.end())
		{
			return;
		}

		it->second.CategoryList.push_back(category.ProductDisplaySeq);
	}
}

void CShopCategoryList::SetFirst()
{
	this->m_Categoryiter = this->m_Categroys.begin();
}

bool CShopCategoryList::GetNext(CShopCategory& category)
{
	if(this->m_Categoryiter==this->m_Categroys.end())
		return 0;

	category = this->m_Categoryiter->second;
	this->m_Categoryiter++;
	return 1;
}

bool CShopCategoryList::GetValueByKey(int nKey,CShopCategory& category)
{
	std::map<int,CShopCategory>::iterator it = this->m_Categroys.find(nKey);

	if(it==this->m_Categroys.end())
	{
		return 0;
	}

	category = it->second;
	return 1;
}

bool CShopCategoryList::GetValueByIndex(int nIndex,CShopCategory& category)
{
	if(nIndex < 0 || nIndex>=static_cast<int>(this->m_CategoryIndex.size()))
	{
		return 0;
	}

	return this->GetValueByKey(this->m_CategoryIndex[nIndex],category);
}

bool CShopCategoryList::InsertPackage(int Category,int Package)
{
	std::map<int,CShopCategory>::iterator it = this->m_Categroys.find(Category);

	if(it==this->m_Categroys.end())
	{
		return 0;
	}

	it->second.AddPackageSeq(Package);
	return 1;
}

bool CShopCategoryList::RefreshPackageSeq(int Category,int PackageSeqs[],int PackageCount)
{
	std::map<int,CShopCategory>::iterator it = this->m_Categroys.find(Category);

	if(it==this->m_Categroys.end())
	{
		return 0;
	}

	it->second.ClearPackageSeq();

	for(int n = 0;n<PackageCount;n++)
	{
		it->second.AddPackageSeq(PackageSeqs[n]);
	}

	return 1;
}
#endif