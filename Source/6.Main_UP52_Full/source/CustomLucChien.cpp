#include "StdAfx.h"
#include "CustomLucChien.h"
#include "ItemManager.h"
#include "Interface.h"

CustomLucChien gCustomLucChien;

CustomLucChien::CustomLucChien()
{
	this->Init();
}

CustomLucChien::~CustomLucChien()
{
}

void CustomLucChien::Init()
{
	for (int n = 0; n < MAX_CUSTOM_LUCCHIEN; n++)
	{
		this->mLucChienDataItem[n].Index = -1;
	}
}

void CustomLucChien::Load(ITEMDATA_LUCCHIEN* info)
{
	for (int n = 0; n < MAX_CUSTOM_LUCCHIEN; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CustomLucChien::SetInfo(ITEMDATA_LUCCHIEN info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_LUCCHIEN)
	{
		return;
	}

	this->mLucChienDataItem[info.Index] = info;
}

ITEMDATA_LUCCHIEN* CustomLucChien::GetInfoLucChien(int IndexItem)
{
	for (int n = 0; n < MAX_CUSTOM_LUCCHIEN; n++)
	{
		if (this->mLucChienDataItem[n].IndexMin >= IndexItem && IndexItem <= this->mLucChienDataItem[n].IndexMax)
		{
			return &this->mLucChienDataItem[n];
		}
	}
	return 0;
}
bool CustomLucChien::CheckItemIsSkill(ITEM* ItemSearch)
{
	for (int i = 0; i < ItemSearch->SpecialNum; i++)
	{
		if (ItemSearch->Special[i] >= 18 && ItemSearch->Special[i] <=24||
			ItemSearch->Special[i] == 54 ||
			ItemSearch->Special[i] == 56 ||
			ItemSearch->Special[i] == 509 ||
			ItemSearch->Special[i] == 66 ||
			ItemSearch->Special[i] == 62 ||
			ItemSearch->Special[i] == 76 ||
			ItemSearch->Special[i] == 260  ||
			ItemSearch->Special[i] == 261  ||
			ItemSearch->Special[i] == 270)
		{
			return 1;
		}
	}
	return 0;
}
bool CustomLucChien::CheckItemIsLuck(ITEM* ItemSearch)
{
	for (int i = 0; i < ItemSearch->SpecialNum; i++)
	{
		if (ItemSearch->Special[i] == 85)
		{
			return 1;
		}
	}
	return 0;
}
int CustomLucChien::CheckItemIsOptLife(ITEM* ItemSearch)
{
	for (int i = 0; i < ItemSearch->SpecialNum; i++)
	{
		if (ItemSearch->Special[i] == 80||ItemSearch->Special[i] == 81)
		{
			return ItemSearch->SpecialValue[i];
		}
	}
	return 0;
}
BYTE CustomLucChien::GetNewOptionCount(BYTE NewOption)
{
	BYTE count = 0;

	for (int n = 0; n < 6; n++)
	{
		if ((NewOption & (1 << n)) != 0)
		{
			count++;
		}
	}

	return count;
}
BYTE CustomLucChien::GetSocketOptionCount(BYTE SocketOption[5])
{
	BYTE count = 0;

	for (int n = 0; n < 5; n++)
	{
		if (SocketOption[n] != 0xFF)
		{
			count++;
		}
	}

	return count;
}

int CustomLucChien::TinhToanLucChien(ITEM* ItemSearch)
{
	DWORD TinhToanLucChien = 0;
	for (int n = 0; n < MAX_CUSTOM_LUCCHIEN; n++)
	{
		if ((ItemSearch->Type) >= this->mLucChienDataItem[n].IndexMin && (ItemSearch->Type) <= this->mLucChienDataItem[n].IndexMax)
		{
			TinhToanLucChien += this->mLucChienDataItem[n].LCGoc;
			TinhToanLucChien += this->mLucChienDataItem[n].LCLevel * ((ItemSearch->Level >> 3) & 15);

			const ITEM_ATTRIBUTE* is = &ItemAttribute[ItemSearch->Type];

			TinhToanLucChien += is->Level *this->mLucChienDataItem[n].LCCap ;

			if (gCustomLucChien.CheckItemIsSkill(ItemSearch))
			{
				TinhToanLucChien += this->mLucChienDataItem[n].LCOptSkill;
			}
			if (gCustomLucChien.CheckItemIsLuck(ItemSearch))
			{
				TinhToanLucChien += this->mLucChienDataItem[n].LCOptLuck;
			}
			if (gCustomLucChien.CheckItemIsOptLife(ItemSearch))
			{
				TinhToanLucChien += (this->mLucChienDataItem[n].LCOptLife * gCustomLucChien.CheckItemIsOptLife(ItemSearch));
			}
			if (ItemSearch->Option1)
			{
				TinhToanLucChien += (this->mLucChienDataItem[n].LCOptExl * gCustomLucChien.GetNewOptionCount(ItemSearch->Option1));
			}
			if (ItemSearch->Jewel_Of_Harmony_Option)
			{
				TinhToanLucChien += this->mLucChienDataItem[n].LCOptHarmony;
			}
			if (ItemSearch->option_380)
			{
				TinhToanLucChien += this->mLucChienDataItem[n].LCOpt380;
			}
			if (ItemSearch->ExtOption)
			{
				TinhToanLucChien += this->mLucChienDataItem[n].LCOptAnc;
			}
			if (ItemSearch->SocketCount)
			{
				TinhToanLucChien += (this->mLucChienDataItem[n].LCOptSocket * gCustomLucChien.GetSocketOptionCount((BYTE*)ItemSearch->bySocketOption));
			}

		}
	}

	return TinhToanLucChien;
}
