
#include "stdafx.h"
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#include "ShopCategory.h"
#include "StringToken.h"

CShopCategory::CShopCategory()
{
	this->CategoryList.clear();
	this->PackageList.clear();
}

CShopCategory::~CShopCategory()
{

}

bool CShopCategory::SetCategory(std::string strdata)
{
	if(strdata.empty())
		return 0;

	CStringToken token(strdata,"@");

	if(token.hasMoreTokens()==0)
		return 0;

	this->ProductDisplaySeq = atoi(token.nextToken().c_str());
	StringCchCopy(this->CategroyName,sizeof(this->CategroyName),token.nextToken().c_str());
	this->EventFlag = atoi(token.nextToken().c_str());
	this->OpenFlag = atoi(token.nextToken().c_str());
	this->ParentProductDisplaySeq = atoi(token.nextToken().c_str());
	this->DisplayOrder = atoi(token.nextToken().c_str());
	this->Root = atoi(token.nextToken().c_str());

	return 1;
}

void CShopCategory::SetCategoryFirst()
{
	this->Categoryiter = this->CategoryList.begin();
}

bool CShopCategory::GetCategoryNext(int& CategorySeq)
{
	if(this->Categoryiter==this->CategoryList.end())
		return 0;

	CategorySeq = (*this->Categoryiter);
	this->Categoryiter++;
	return 1;
}

void CShopCategory::SetPackagSeqFirst()
{
	this->Packageiter = this->PackageList.begin();
}

bool CShopCategory::GetPackagSeqNext(int& PackagSeq)
{
	if(this->Packageiter==this->PackageList.end())
		return 0;

	PackagSeq = (*this->Packageiter);
	this->Packageiter++;
	return 1;
}

void CShopCategory::AddPackageSeq(int PackageSeq)
{
	this->PackageList.push_back(PackageSeq);
}

void CShopCategory::ClearPackageSeq()
{
	this->PackageList.clear();
}
#endif