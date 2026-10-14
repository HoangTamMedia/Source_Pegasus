#pragma once
#include "Protocol.h"
class CCustomRankUser
{
	struct PMSG_CUSTOM_RANKUSER
	{
		PSBMSG_HEAD h;
		int iIndex;
		WORD mLvVIPChar;
	};
	struct CUSTOM_RANKUSER_DATA
	{
		WORD m_Index;

		BOOL m_Enabled;
		int m_rDanhHieu;

		WORD mLvVIPChar;
	};
public:
	CCustomRankUser();
	virtual ~CCustomRankUser();
	void CCustomRankUser::GCReqRankLevelUser(BYTE* aRecv);
	std::vector< CUSTOM_RANKUSER_DATA> m_DataRank;
	int CCustomRankUser::GetNumberData(int KeyIndex);
	void CCustomRankUser::DrawInfo(int aIndex,float X, float Y, float W, float H);
};

extern CCustomRankUser gCustomRankUser;
