
#pragma once

#include "include.h"
#include <time.h>

class CShopPackage
{
public:
	CShopPackage();
	virtual ~CShopPackage();

	bool	SetPackage(std::string strdata);
	void	SetLeftCount(int nCount);

	int		GetProductCount();
	void	SetProductSeqFirst();
	bool	GetProductSeqFirst(int& ProductSeq);
	bool	GetProductSeqNext(int& ProductSeq);

	int		GetPriceCount();
	void	SetPriceSeqFirst();
	bool	GetPriceSeqFirst(int& PriceSeq);
	bool	GetPriceSeqNext(int& PriceSeq);

public:
	int		ProductDisplaySeq;
	int		ViewOrder;
	int		PackageProductSeq;
	char	PackageProductName[SHOPLIST_LENGTH_PACKAGENAME];
	int		PackageProductType;
	int		Price;
	char	Description[SHOPLIST_LENGTH_PACKAGEDESC];
	char	Caution[SHOPLIST_LENGTH_PACKAGECAUTION];
	int		SalesFlag;
	int		GiftFlag;
	tm		StartDate;
	tm		EndDate;
	int		CapsuleFlag;
	int		CapsuleCount;
	char	ProductCashName[SHOPLIST_LENGTH_PACKAGECASHNAME];
	char	PricUnitName[SHOPLIST_LENGTH_PACKAGEPRICEUNIT];
	int		DeleteFlag;
	int		EventFlag;
	int		ProductAmount;
	char	InGamePackageID[SHOPLIST_LENGTH_INGAMEPACKAGEID];
	int		ProductCashSeq;
	int		PriceCount;
	bool	DeductMileageFlag;
	int		CashType;
	int		CashTypeFlag;

	int		LeftCount;

private:
	void	SetProductSeqList(std::string strdata);
	void	SetPriceSeqList(std::string strdata);

	std::vector<int> ProductSeqList;
	std::vector<int>::iterator ProductSeqIter;

	std::vector<int> PriceSeqList;
	std::vector<int>::iterator PriceSeqIter;
};
