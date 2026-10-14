
#pragma once

#include "include.h"

class CShopCategory
{
public:
	CShopCategory();
	virtual ~CShopCategory();

	bool SetCategory(std::string strdata);

	void SetCategoryFirst();
	bool GetCategoryNext(int& CategorySeq);

	void SetPackagSeqFirst();
	bool GetPackagSeqNext(int& PackagSeq);

	void AddPackageSeq(int PackageSeq);
	void ClearPackageSeq();

public:
	int ProductDisplaySeq;
	char CategroyName[SHOPLIST_LENGTH_CATEGORYNAME];
	int EventFlag;
	int OpenFlag;
	int ParentProductDisplaySeq;
	int DisplayOrder;
	int Root;

	std::vector<int> CategoryList;
	std::vector<int>::iterator Categoryiter;

	std::vector<int> PackageList;
	std::vector<int>::iterator Packageiter;
};
