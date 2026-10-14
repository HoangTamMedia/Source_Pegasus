#include "stdafx.h"
#include "ItemTooltip.h"
#include "Util.h"
#include "ZzzCharacter.h"
#include "NewUISystem.h"

nCInfo GInfo;

nCInfo::nCInfo()
{
	this->m_CustomInfo.clear();
	this->m_CustomDescripcionInfo.clear();
}

nCInfo::~nCInfo()
{
}

void nCInfo::loadnInformation(nInformation* info)
{
	for (int n = 0; n < MaxLineItemToolTip; n++)
	{
		if (info[n].ItemIndexMin < -1 || info[n].ItemIndexMax > 7680)
		{
			return;
		}
		this->m_CustomInfo.insert(std::pair<int, nInformation>(info[n].Index, info[n]));
	}
}

void nCInfo::loadnText(nText* info)
{
	for (int n = 0; n < MaxLineItemToolTip; n++)
	{
		if (info[n].Index < 0 || info[n].Index > 7680)
		{
			return;
		}
		this->m_CustomDescripcionInfo.insert(std::pair<int, nText>(info[n].Index, info[n]));
	}
}

char* nGetTextLine(int TextIndex)
{
	std::map<int, nText>::iterator it = GInfo.m_CustomDescripcionInfo.find(TextIndex);

	if (it != GInfo.m_CustomDescripcionInfo.end())
	{
		return it->second.Text;
	}

	return "no found";
}
extern	char    TextList[60][512];
extern	int     TextListColor[60];
extern	int     TextBold[60];

bool CheckItemSlotOpt(int IndexItem, int Level, int Exl)
{
	int CountSlot = 0;
	BYTE byFirstClass = Hero->Class;

	int GetIndexSlot[6] =
	{
	   Hero->BodyPart[1].Type - 1171,
	   Hero->BodyPart[2].Type - 1171,
	   Hero->BodyPart[3].Type - 1171,
	   Hero->BodyPart[4].Type - 1171,
	   Hero->BodyPart[5].Type - 1171,
	};
	int GetLevelSlot[6] =
	{
	   Hero->BodyPart[1].Level,
	   Hero->BodyPart[2].Level,
	   Hero->BodyPart[3].Level,
	   Hero->BodyPart[4].Level,
	   Hero->BodyPart[5].Level,

	};
	int GetExlSlot[6] =
	{
	  Hero->BodyPart[1].Option1,
	  Hero->BodyPart[2].Option1,
	  Hero->BodyPart[3].Option1,
	  Hero->BodyPart[4].Option1,
	  Hero->BodyPart[5].Option1,
	};
	for (int Slot = 0; Slot < 6; Slot++)
	{
		if ((int)GetIndexSlot[Slot] == (int)ITEM((7 + Slot), IndexItem)
			&& Level <= GetLevelSlot[Slot] && Exl <= GetExlSlot[Slot])
		{
			CountSlot++;
		}
	}
	if (CountSlot >= 5 || (CountSlot >= 4 && (byFirstClass == 3 || byFirstClass == 6)))
	{
		return 1;
	}
	return 0;
}
#include "CBInterface.h"
int nCInfo::nInformationOP(int TextNum, ITEM* ItemSearch)
{

	int CountSlot = 0;

	BYTE byFirstClass = Hero->Class;

	for (std::map<int, nInformation>::iterator it = GInfo.m_CustomInfo.begin(); it != GInfo.m_CustomInfo.end(); it++)
	{
		if (it == GInfo.m_CustomInfo.end())
		{
			return 0;
		}

		if (it->second.ItemIndexMin == -1)
		{

			int GetIndexItemSet = (it->second.ItemIndexMax % 512);

			if (ItemSearch->Type == ITEM(7, GetIndexItemSet) ||
				ItemSearch->Type == ITEM(8, GetIndexItemSet) ||
				ItemSearch->Type == ITEM(9, GetIndexItemSet) ||
				ItemSearch->Type == ITEM(10, GetIndexItemSet) ||
				ItemSearch->Type == ITEM(11, GetIndexItemSet))
			{
				bool SetColorGray = true;

				wsprintf(TextList[TextNum], it->second.OptionName);
				TextListColor[TextNum] = it->second.OptionColor;
				TextBold[TextNum++] = 1;

				for (int i = 0; i < 15; i++)
				{

					if (CheckItemSlotOpt(GetIndexItemSet, it->second.LevelItem[i], it->second.Exc))
					{
						SetColorGray = false;
					}

					else
					{
						SetColorGray = true;
					}

					if (it->second.TextIndex[i] == -1)
					{
						continue;
					}
					wsprintf(TextList[TextNum], "%s", nGetTextLine(it->second.TextIndex[i]));
					if (SetColorGray)
					{
						TextListColor[TextNum] = 10;
					}
					else
					{
						TextListColor[TextNum] = it->second.TextColor[i];
					}
					TextBold[TextNum++] = 0;
				}

			}

		}
		else if (ItemSearch->Type >= it->second.ItemIndexMin && ItemSearch->Type <= it->second.ItemIndexMax && it->second.ItemIndexMin != -1)
		{

			bool SetColorGray = false;

			wsprintf(TextList[TextNum], it->second.OptionName);
			TextListColor[TextNum] = it->second.OptionColor;
			TextBold[TextNum++] = 1;

			for (int i = 0; i < 15; i++)
			{
				if ((it->second.LevelItem[i] > (int)((ItemSearch->Level >> 3) & 15) && it->second.LevelItem[i] != -1) || ((int)ItemSearch->Option1 < it->second.Exc && it->second.Exc != -1))
				{
					SetColorGray = true;
				}

				if (it->second.TextIndex[i] == -1)
				{
					continue;
				}
				wsprintf(TextList[TextNum], "%s", nGetTextLine(it->second.TextIndex[i]));

				if (SetColorGray)
				{
					TextListColor[TextNum] = 10;
				}
				else
				{
					TextListColor[TextNum] = it->second.TextColor[i];
				}
				TextBold[TextNum++] = 0;
			}

		}

	}

	DWORD TinhToanLucChien = gCustomLucChien.TinhToanLucChien(ItemSearch);

	if (TinhToanLucChien != 0)
	{

		wsprintf(TextList[TextNum], "Score Gear :  %s ", gInterface.NumberFormat(TinhToanLucChien));

		TextListColor[TextNum] = 12;

		TextBold[TextNum++] = 10;

	}

	return TextNum;

}
