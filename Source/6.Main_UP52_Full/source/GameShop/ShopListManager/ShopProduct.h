
#pragma once

#include "include.h"
#include <map>

class CShopProduct
{
public:
	CShopProduct();
	virtual ~CShopProduct();

	bool SetProduct(std::string strdata);

public:
	int		ProductSeq;
	char	ProductName[SHOPLIST_LENGTH_PRODUCTNAME];
	char	PropertyName[SHOPLIST_LENGTH_PRODUCTPROPERTYNAME];
	char	Value[SHOPLIST_LENGTH_PRODUCTVALUE];
	char	UnitName[SHOPLIST_LENGTH_PRODUCTUNITNAME];
	int		Price;
	int		PriceSeq;
	int		PropertyType;
	int		MustFlag;
	int		vOrder;
	int		DeleteFlag;
	int		StorageGroup;
	int		ShareFlag;
	char	InGamePackageID[SHOPLIST_LENGTH_INGAMEPACKAGEID];
	int		PropertySeq;
	int		ProductType;
	int		UnitType;
};
