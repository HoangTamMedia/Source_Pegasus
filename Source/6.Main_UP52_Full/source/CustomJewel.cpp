
#include "stdafx.h"
#include "CustomJewel.h"
#include "CustomWing.h"
#include "NewUISystem.h"

CCustomJewel gCustomJewel;

CCustomJewel::CCustomJewel()
{
	this->Init();
}

CCustomJewel::~CCustomJewel()
{

}

void CCustomJewel::Init()
{
	for (int n = 0; n < MAX_CUSTOM_JEWEL; n++)
	{
		this->m_CustomJewelInfo[n].Index = -1;
	}
}

void CCustomJewel::Load(CUSTOM_JEWEL_INFO* info)
{
	for (int n = 0; n < MAX_CUSTOM_JEWEL; n++)
	{
		this->SetInfo(info[n]);

	}
}

void CCustomJewel::SetInfo(CUSTOM_JEWEL_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_JEWEL)
	{
		return;
	}

	this->m_CustomJewelInfo[info.Index] = info;
}

CUSTOM_JEWEL_INFO* CCustomJewel::GetInfo(int index)
{
	if (index < 0 || index >= MAX_CUSTOM_JEWEL)
	{
		return 0;
	}

	if (this->m_CustomJewelInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomJewelInfo[index];
}

CUSTOM_JEWEL_INFO* CCustomJewel::GetInfoByItem(int ItemIndex)
{
	for (int n = 0; n < MAX_CUSTOM_JEWEL; n++)
	{
		CUSTOM_JEWEL_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

BOOL CCustomJewel::CheckCustomJewel(int index)
{
	if (this->GetInfo(index) != 0)
	{
		return 1;
	}

	return 0;
}
extern bool IsRingPen(int index);
bool isBS(int Index)
{
	if (Index == 7181 || Index == 7182)
	{
		return true;
	}
	return false;
}

BOOL CCustomJewel::CheckCustomJewelByItem(int ItemIndex)
{

	if (this->GetInfoByItem(ItemIndex) != 0)
	{
		return 1;
	}

	return 0;
}

BOOL CCustomJewel::CheckCustomJewelApplyItem(int ItemIndex, int TargetItemIndex)
{

	if (isBS(ItemIndex) == 1 && IsRingPen(TargetItemIndex) == 1)
	{
		printf("SU DUNG NGOC \n");
		return 1;
	}

	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	if ((TargetItemIndex / MAX_ITEM_TYPE) >= 7 && (TargetItemIndex / MAX_ITEM_TYPE) <= 11 && (TargetItemIndex % MAX_ITEM_TYPE) >= 62 && (TargetItemIndex % MAX_ITEM_TYPE) <= 72)
	{
		return 0;
	}

	if (TargetItemIndex > BConverITEM(12, 6) && (TargetItemIndex < BConverITEM(12, 36) || TargetItemIndex > BConverITEM(12, 43)) && TargetItemIndex != BConverITEM(12, 49) && TargetItemIndex != BConverITEM(12, 50) && (TargetItemIndex < BConverITEM(12, 262) || TargetItemIndex > BConverITEM(12, 265)) && TargetItemIndex != BConverITEM(13, 30) && gCustomWing.CheckCustomWingByItem(TargetItemIndex) == 0)
	{
		return 0;
	}

	if (lpInfo->EnableSlotWeapon == 0 && (TargetItemIndex >= BConverITEM(0, 0) && TargetItemIndex < BConverITEM(6, 0)))
	{
		return 0;
	}

	if (lpInfo->EnableSlotArmor == 0 && (TargetItemIndex >= BConverITEM(6, 0) && TargetItemIndex < BConverITEM(12, 0)))
	{
		return 0;
	}

	if (lpInfo->EnableSlotWing == 0 && ((TargetItemIndex >= BConverITEM(12, 0) && TargetItemIndex <= BConverITEM(12, 6)) || (TargetItemIndex >= BConverITEM(12, 36) && TargetItemIndex <= BConverITEM(12, 43)) || TargetItemIndex == BConverITEM(12, 49) || TargetItemIndex == BConverITEM(12, 50) || (TargetItemIndex >= BConverITEM(12, 262) && TargetItemIndex <= BConverITEM(12, 265)) || TargetItemIndex == BConverITEM(13, 30) || gCustomWing.CheckCustomWingByItem(TargetItemIndex) != 0))
	{
		return 0;
	}

	return 1;
}

int CCustomJewel::GetCustomJewelSuccessRate(int ItemIndex, int AccountLevel)
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->SuccessRate[AccountLevel];
}

int CCustomJewel::GetCustomJewelSalePrice(int ItemIndex)
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->SalePrice;
}
