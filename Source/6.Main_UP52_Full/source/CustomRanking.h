#pragma once

#define MAX_RANK 50
#include "Protocol.h"

class CustomRanking
{
public:

	struct PMSG_CUSTOM_RANKING_RECV
	{
		PSWMSG_HEAD h;
		int index;
		char rankname[60];
		char col1[20];
		char col2[20];
		char col3[20];
		char col4[20];
		int count;
	};

	struct PMSG_CUSTOM_RANKING_COUNT_RECV
	{
		PSBMSG_HEAD header;
		int count;
	};

	struct PMSG_CUSTOM_RANKING_COUNT_SEND
	{
		PSBMSG_HEAD header;
	};

	struct PMSG_CUSTOM_RANKING_SEND
	{
		PSBMSG_HEAD header;
		BYTE type;
	};

	struct CUSTOM_RANKING_DATA
	{
		char Value1[20];
		char Value2[30];
		char Value3[30];
		char Value4[30];

	};

	struct RANK_INFO_SEND
	{
		char NameRank[128];
	};

	struct PMSG_RANK_INFO
	{
		PWMSG_HEAD2 h;
		RANK_INFO_SEND Info[MAX_RANK];
	};
	CustomRanking();
	virtual ~CustomRanking();
	void OpenWindow();
	void ClearCustomRanking();
	void GCReqRanking(PMSG_CUSTOM_RANKING_RECV* lpMsg);
	void GCReqRankingCount(PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg);
	void DrawRankPanelWindow();
	int RankingType;
	int RankEnable;
	int RankingCount;
	int count;
	std::vector<std::string> mListRanking;
	void RankNameRev(PMSG_RANK_INFO* lpMsg);
	RANK_INFO_SEND m_RankInfo[MAX_RANK];
	int SelectBarRankingNumber;
private:
	char rankname[60];
	char col1[20];
	char col2[20];
	char col3[20];
	char col4[20];
	CUSTOM_RANKING_DATA gCustomRanking[MAX_RANK];
	int RankLoad;
	int ranknamecount;
};
extern CustomRanking gCustomRanking;