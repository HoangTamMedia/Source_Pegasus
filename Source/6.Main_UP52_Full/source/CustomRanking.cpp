#include "stdafx.h"
#include "CustomRanking.h"
#include "Protocol.h"
#include "CustomMessage.h"
#include "CBInterface.h"
#include "Util.h"
#include "Protect.h"
#include "Other.h"
#include "NewUISystem.h"
#include "UIControls.h"
#include "wsclientinline.h"
#include "CharacterManager.h"
#include "NewUIMyInventory.h"

int NextRanking = 0;
int PrevRanking = 0;

CustomRanking gCustomRanking;

CustomRanking::CustomRanking()
{
	this->ClearCustomRanking();

}

CustomRanking::~CustomRanking()
{

}
void CustomRanking::ClearCustomRanking()
{
	for (int n = 0; n < MAX_RANK; n++)
	{
		memset(gCustomRanking[n].Value1, 0, sizeof(gCustomRanking[n].Value1));
		memset(gCustomRanking[n].Value2, 0, sizeof(gCustomRanking[n].Value2));
		memset(gCustomRanking[n].Value3, 0, sizeof(gCustomRanking[n].Value3));
		memset(gCustomRanking[n].Value4, 0, sizeof(gCustomRanking[n].Value4));

	}
	this->RankEnable = 0;

}
void CustomRanking::GCReqRanking(CustomRanking::PMSG_CUSTOM_RANKING_RECV* lpMsg)
{
	memcpy(this->rankname, lpMsg->rankname, sizeof(this->rankname));
	memcpy(this->col1, lpMsg->col1, sizeof(this->col1));
	memcpy(this->col2, lpMsg->col2, sizeof(this->col2));
	memcpy(this->col3, lpMsg->col3, sizeof(this->col3));
	memcpy(this->col4, lpMsg->col4, sizeof(this->col4));

	this->RankEnable = 1;

	mListRanking.clear();

	this->count = lpMsg->count;

	for (int n = 0; n < lpMsg->count; n++)
	{
		CUSTOM_RANKING_DATA* lpInfo = (CUSTOM_RANKING_DATA*)(((BYTE*)lpMsg) + sizeof(PMSG_CUSTOM_RANKING_RECV) + (sizeof(CUSTOM_RANKING_DATA) * n));

		memcpy(gCustomRanking[n].Value1, lpInfo->Value1, sizeof(gCustomRanking[n].Value1));
		memcpy(gCustomRanking[n].Value2, lpInfo->Value2, sizeof(gCustomRanking[n].Value2));
		memcpy(gCustomRanking[n].Value3, lpInfo->Value3, sizeof(gCustomRanking[n].Value3));
		memcpy(gCustomRanking[n].Value4, lpInfo->Value4, sizeof(gCustomRanking[n].Value4));

	}
}

void CustomRanking::OpenWindow()
{
	if ((GetTickCount() - gInterface.Data[eRankPANEL_MAIN].EventTick) < 1500)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow = false;
		return;
	}

	gInterface.Data[eRankPANEL_MAIN].EventTick = GetTickCount();

	if (gInterface.Data[eRankPANEL_MAIN].OnShow == true)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow = false;

		return;
	}

	this->ClearCustomRanking();

	this->RankingCount = 0;

	this->SelectBarRankingNumber = 0;

	PMSG_CUSTOM_RANKING_COUNT_SEND pMsg1;

	pMsg1.header.set(0xF3, 0xE7, sizeof(pMsg1));

	DataSend((BYTE*)& pMsg1, pMsg1.header.size);

	PMSG_CUSTOM_RANKING_SEND pMsg;

	pMsg.header.set(0xF3, 0xE6, sizeof(pMsg));

	this->RankingType = 0;

	pMsg.type = 0;

	DataSend((BYTE*)& pMsg, pMsg.header.size);

	gInterface.Data[eRankPANEL_MAIN].OnShow = true;

}

void CustomRanking::RankNameRev(PMSG_RANK_INFO* lpMsg)
{
	this->ranknamecount = 0;
	memset(this->m_RankInfo, 0, sizeof(this->m_RankInfo));
	for (int i = 0; i < MAX_RANK; i++)
	{

		memcpy(this->m_RankInfo[i].NameRank, lpMsg->Info[i].NameRank, 32);
		if (strlen(this->m_RankInfo[i].NameRank) != 0)
		{
			this->ranknamecount++;
		}
	}
}

void CustomRanking::GCReqRankingCount(PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg)
{

	this->RankingCount = (lpMsg->count > 0) ? lpMsg->count : 0;
}

int PageBank = 0;
int MaxPerPageBank = 10;

static void BPagePrevAction(LPVOID pClass)
{
	if (GetTickCount() < gInterface.Data[eRankPANEL_MAIN].EventTick + 300) return;
	gInterface.Data[eRankPANEL_MAIN].EventTick = GetTickCount();

	if (PageBank > 0)
	{
		PageBank--;
	}
}
static void BPageNextAction(LPVOID pClass)
{
	if (GetTickCount() < gInterface.Data[eRankPANEL_MAIN].EventTick + 300) return;
	gInterface.Data[eRankPANEL_MAIN].EventTick = GetTickCount();

	if (PageBank < (gCustomRanking.count / MaxPerPageBank) - 1)
	{
		PageBank++;
	}
}

bool top1RequestSent = true;
void CustomRanking::DrawRankPanelWindow()
{
	if (!gInterface.Data[eRankPANEL_MAIN].OnShow)
	{
		return;
	}

	if (gInterface.CheckWindow(Interface::Inventory)
		|| gInterface.CheckWindow(Interface::Character)
		|| gInterface.CheckWindow(Interface::Warehouse)
		|| gInterface.CheckWindow(Interface::Shop)
		|| gInterface.CheckWindow(Interface::ChaosBox)
		|| gInterface.CheckWindow(Interface::Trade)
		|| gInterface.CheckWindow(Interface::Store)
		|| gInterface.CheckWindow(Interface::OtherStore)
		|| gInterface.CheckWindow(Interface::LuckyCoin1)
		|| gInterface.CheckWindow(Interface::NPC_ChaosMix)
		|| gInterface.CheckWindow(Interface::MoveList)

		)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow = false;
		return;
	}

	if (!gInterface.Data[eRankPANEL_MAIN].OnShow)
	{
		top1RequestSent = true;
		return;
	}

	// Was 620, but the table only ever fills about 320 - the rest was dead
	// frame hanging off to the right. StartX is derived from this, and the
	// content is anchored to StartX, so narrowing it pulls the right edge in
	// toward the table instead of moving the table.
	float MainWidth = 400.0;
	float MainHeight = 255.0;
	float StartBody = ((MAX_WIN_HEIGHT - 51) / 2) - (MainHeight / 2);
	float StartY = ((MAX_WIN_HEIGHT - 51) / 2) - (MainHeight / 2) - 30.0f;
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 147.0f;
	float MainCenter = StartX + (MainWidth / 3);
	float ButtonX = MainCenter - (float)(29.0 / 2);
	float X = -20;
	float y = - 22;

	// Empty caption on purpose - see CBInterfaceVIPChar. The frame is wanted,
	// the second title bar and close button it draws over the window's own are
	// not.
	if (g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth + 50, MainHeight + 10, eRankPANEL_MAIN, ""))

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);
	gInterface.DrawFormat(eYellow, StartX + 37.0f, StartY + 13.0f, 100, 1, "Ranking [F3]");

	int CloseX = StartX + 315.0f;

	int CloseY = StartY + 3.0f;

	RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30.0f, 30.0f);

	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 30, 30) == 1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eRankPANEL_MAIN].EventTick > 300)
		{

			gInterface.Data[eRankPANEL_MAIN].Close();
		}
		RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30, 30, 0, 0, RGBA(255, 255, 255, 255));
	}

	DisableAlphaBlend();
	if (SEASON3B::IsPress(VK_ESCAPE) == true)
	{
		gInterface.Data[eRankPANEL_MAIN].OnShow ^= 1;
		PlayBuffer(SOUND_CLICK01);
	}

	{
		DWORD Color = eGray100;

		float SizeButtonW = 80.0;

		if (this->RankEnable == 1)
		{
			EnableAlphaTest();
			glColor3f(1.0, 1.0, 1.0);
			gInterface.DrawFormat(eWhite, StartX + 45 + X, StartY + 40, 110, 1, "Top");

			gInterface.DrawFormat(eWhite, StartX + 80 + X, StartY + 40, 110, 1, "%s", this->col1);

			gInterface.DrawFormat(eWhite, StartX + 160 + X, StartY + 40, 110, 1, "%s", this->col2);

			gInterface.DrawFormat(eWhite, StartX + 230 + X, StartY + 40, 110, 1, "%s", this->col3);

			gInterface.DrawFormat(eWhite, StartX + 300 + X, StartY + 40, 110, 1, "%s - Reset", this->col4);
			DisableAlphaBlend();
			char* name = (char*)(CharacterAttribute->Name);

			for (int n = 0, i = (PageBank * MaxPerPageBank); n < MaxPerPageBank && i < this->count; i++)

			{
				Color = eWhite;

				if (i >= 0)
				{
					EnableAlphaTest();
					glColor3f(1.0, 1.0, 1.0);

					if (i < 4)
					{
						Color = eYellow;
					}
					else
					{
						Color = eWhite;
					}

					gInterface.DrawFormat(Color, StartX + 48 + X, StartY + 80 + (20 * n) + y, 52, 2, "%d", (i + 1));
					DisableAlphaBlend();
				}

				if (strcmp(name, this->gCustomRanking[i].Value1) == 0)
				{
					Color = eWhite;
				}

				g_pBCustomMenuInfo->RenderRanking(StartX + 36 + X, StartY + 75 + (20 * n) + y, 330, 22, 0x00000096, 0, 0);

				if (SEASON3B::CheckMouseIn(StartX + 36 + X, StartY + 75 + (20 * n) + y, 330, 22))
				{
					FrameTarget(StartX + 37 + X, StartY + 75 + (20 * n) + y, 326, 17, RGBA(31, 144, 255, 0xff));

				}

				if (i > 2)
				{

					EnableAlphaTest();
					glColor3f(1.0, 1.0, 1.0);
					gInterface.DrawFormat(eWhite, StartX + 160 + X, StartY + 80 + (20 * n) + y, 110, 1, "%s" ,this->gCustomRanking[i].Value1);
					int value = atoi(this->gCustomRanking[i].Value2);
					gInterface.DrawFormat(eWhite, StartX + 230 + X, StartY + 80 + (20 * n) + y, 110, 1, "%s", gInterface.NumberFormat(value));
					gInterface.DrawFormat(eWhite, StartX + 295 + X, StartY + 80 + (20 * n) + y, 110, 1, "%s", this->gCustomRanking[i].Value4);
					DisableAlphaBlend();

				}
				else
				{
					EnableAlphaTest();
					glColor3f(1.0, 1.0, 1.0);
					gInterface.DrawFormat(eWhite, StartX + 160 + X, StartY + 80 + (20 * n) + y, 110, 1, "%s", this->gCustomRanking[i].Value1);
					int value = atoi(this->gCustomRanking[i].Value2);
					gInterface.DrawFormat(eWhite, StartX + 230 + X, StartY + 80 + (20 * n) + y, 110, 1, "%s", gInterface.NumberFormat(value));
					gInterface.DrawFormat(eWhite, StartX + 295 + X, StartY + 80 + (20 * n) + y, 110, 1, "%s", this->gCustomRanking[i].Value4);
					DisableAlphaBlend();

				}

				if (atoi(this->gCustomRanking[i].Value3) > 0)
				{
					EnableAlphaTest();
					glColor3f(1.0, 1.0, 1.0);
					RenderBitmap(150000 + atoi(this->gCustomRanking[i].Value3), StartX + 30 + X, StartY + 48 + (20 * n) + y, 170.0f, 120.0f, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
					DisableAlphaBlend();
				}

				n++;

			}
			float CenterX = StartX + (400 / 2) + 145;
			float CenterY = (StartY + 390) - 43;
			const BYTE state[3] = { 0, 1, 2 };
			EnableAlphaTest();
			glColor3f(1.0, 1.0, 1.0);
			TextDraw(g_hFontBold, CenterX - (380 / 2), CenterY - 115 + 5, 0xffffffff, 0x0, 60, 0, 3, "%d / %d", PageBank + 1, (this->count / MaxPerPageBank));
			DisableAlphaBlend();
			if (g_pBCustomMenuInfo->DrawButtonGUI(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::ButtonPageL, CenterX - (360 / 2) - 26, CenterY - 115, 20, 22, 3))
			{
				BPagePrevAction(NULL);
			}

			if (g_pBCustomMenuInfo->DrawButtonGUI(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::ButtonPageR, CenterX - (290 / 2) + 8, CenterY - 115, 20, 22, 3))
			{
				BPagePrevAction(NULL);
			}
		}
	}
}