#include "stdafx.h"
#include "CustomRankUser.h"
#include "NewUISystem.h"
#include "CBInterface.h"
#include "CUIController.h"
#include "CharacterManager.h"
#include "Util.h"
#include "Protocol.h"
#include "NewUIBase.h"
#include "Other.h"
#include "CBInterfaceVIPChar.h"

CCustomRankUser gCustomRankUser;

namespace
{
	// The VIP rank label used to build its font and destroy it again on every
	// ranked player, every frame. CreateFontA runs through the system font
	// mapper and is far too expensive to sit in a per-frame loop. The size comes
	// from the rank table so a single static will not do, but there are only a
	// handful of distinct sizes - one font each, kept for the process.
	HFONT GetRankFont(int iSize)
	{
		static std::map<int, HFONT> s_mapFont;

		std::map<int, HFONT>::iterator it = s_mapFont.find(iSize);
		if (it != s_mapFont.end())
			return it->second;

		HFONT hFont = CreateFontA(iSize, 0, 0, 0, 400, 0, 0, 0, 0x1, 0, 0, 3u, 0, "Arial");
		s_mapFont[iSize] = hFont;
		return hFont;
	}
}

CCustomRankUser::CCustomRankUser()
{
	this->m_DataRank.clear();
}

CCustomRankUser::~CCustomRankUser()
{

}
int CCustomRankUser::GetNumberData(int KeyIndex)
{
	if (KeyIndex < 0 || this->m_DataRank.empty()) return -1;
	for (int i = 0; i < this->m_DataRank.size(); i++)
	{
		if (this->m_DataRank[i].m_Index == KeyIndex)
		{
			return i;
		}
	}
	return -1;
}

void CCustomRankUser::GCReqRankLevelUser(BYTE* lpMsg)
{
	if (lpMsg == NULL)
	{
		return;
	}
	PMSG_CUSTOM_RANKUSER* aRecv = (PMSG_CUSTOM_RANKUSER*)lpMsg;

	CUSTOM_RANKUSER_DATA info = { 0 };

	info.m_Enabled = true;
	info.mLvVIPChar = aRecv->mLvVIPChar;

	info.m_Index = aRecv->iIndex;
	int NumberData = this->GetNumberData(info.m_Index);
	if (NumberData == -1)
	{
		this->m_DataRank.push_back(info);
	}
	else
	{
		this->m_DataRank[NumberData] = info;
	}
	auto it = std::find_if(m_DataRank.begin(), m_DataRank.end(),[&](const CUSTOM_RANKUSER_DATA & data) {return data.m_Index == info.m_Index;});
	if (it != m_DataRank.end())
	{
		*it = info;
	}
	else
	{
		this->m_DataRank.push_back(info);
	}

}
void CCustomRankUser::DrawInfo(int aIndex, float X, float Y, float W, float H)
{
	if (SceneFlag == 5)
	{

		int NumberData = this->GetNumberData(aIndex);
		if (NumberData == -1) return;

		int v1 = this->m_DataRank[NumberData].m_rDanhHieu % 10;
		int NumberDH = (this->m_DataRank[NumberData].m_rDanhHieu - (v1)) / 10;

		float RankStartX = X + (W / 2) - 50;
		float RankStartY = Y - 40;

		if (this->m_DataRank[NumberData].mLvVIPChar > 0 )
		{
			int LevelVipChar = this->m_DataRank[NumberData].mLvVIPChar;
			std::map<int, CUSTOMVIPCHAR_DATA>::iterator it = gBInterfaceVIPChar.m_VipCharRank.find(LevelVipChar);
			if (it != gBInterfaceVIPChar.m_VipCharRank.end())
			{

				if (it->second.TypeRank == 1)
				{
					int SizeTextFont = it->second.SizeW;
					HFONT CHFontVIPRank = GetRankFont(SizeTextFont);

					TextDraw(CHFontVIPRank, (float)RankStartX - 50.0f - it->second.X, (float)Y - 18.0f - it->second.Y, it->second.ColorText, 0x0, 200, 0, 3, it->second.TextDir);
				}
				else if (it->second.TypeRank == 2)
				{

					RenderBitmap(150000 + LevelVipChar, RankStartX - it->second.X, RankStartY - it->second.Y, it->second.SizeW - 60.0f, it->second.H - 40.0f, 0, 0, 0.7, 1.0, 1, 1, 0.0);
				}
			}

		}

  }
}