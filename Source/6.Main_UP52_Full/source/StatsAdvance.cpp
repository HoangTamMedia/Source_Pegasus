#include "stdafx.h"
#include "protocol.h"
#include "Interface.h"
#include "StatsAdvance.h"
#include <CBInterface.h>
#include <NewUISystem.h>
#include <Util.h>
#include "CharacterManager.h"
#include "CustomLucChien.h"

CStatsAdvance g_StatsAdvance;

CStatsAdvance::CStatsAdvance()
{
	this->m_Enable = false;
	this->m_Loaded = false;

	memset(&this->m_Data, 0, sizeof(this->m_Data));
}

CStatsAdvance::~CStatsAdvance()
{
}

void CStatsAdvance::Load()
{
	this->m_Enable = true;
}

CNewUIScrollBar* AdvanceScroll = nullptr;

void CStatsAdvance::DrawStart()
{

	if (!gInterface.Data[eStatsAdvance].OnShow)
	{
		return;
	}

	float MainWidth = 350.0;

	float MainHeight = 700.0;

	float StartY = -3;

	float StartX = -8;

	if (GetKeyState(VK_ESCAPE) < 0)
	{
		gInterface.Data[eStatsAdvance].OnShow = 0;

}

	g_pBCustomMenuInfo->gDrawPegasus(&StartX, &StartY, MainWidth, MainHeight, eStatsAdvance, "Thuộc Tính Nhân Vật");

	g_pBCustomMenuInfo->gInfoBox(StartX + 22, StartY + 50, 180, 90, 0x00000060, 0, 0);

	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);

	gInterface.DrawFormat(eWhite, StartX + 68, StartY + 15, 100, 1, "Thuộc Tính Nhân Vật");

	gInterface.DrawFormat(eWhite, StartX + 30, StartY + 60, 100, 1, "Chủng Tộc: %s ", gCharacterManager.GetCharacterClassText(CharacterAttribute->Class));

	gInterface.DrawFormat(eWhite, StartX + 30, StartY + 75, 100, 1, "Pegasus Coin : %s ", gInterface.NumberFormat(CharacterAttribute->PrintPlayer.Coin1));

	gInterface.DrawFormat(eWhite, StartX + 30, StartY + 90, 100, 1, "Pegasus Pear : %s ", gInterface.NumberFormat(CharacterAttribute->PrintPlayer.Coin2));

	gInterface.DrawFormat(eWhite, StartX + 30, StartY + 105, 100, 1, "Pegasus Point : %s ", gInterface.NumberFormat(CharacterAttribute->PrintPlayer.Coin3));

	StartY += 130;

	StartX += 10;

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Sát Thương Chí Mạng:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eRed, "%d", this->m_Data.CriticalDamageRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Sát Thương Hoàn Hảo:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eRed, "%d", this->m_Data.ExellentDamageRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Sát Thương Nhân Đôi:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eRed, "%d", this->m_Data.DoubleDamageRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Phản Sát Hồi Thương:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eRed, "%d", this->m_Data.TotalDamageReflect);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Giảm Sát Thương:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eRed, "%d", this->m_Data.DamageReductionRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Bỏ Qua Phòng Thủ:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eGold, "%d", this->m_Data.IgnoreDefenceRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Sức Tấn Công:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eWhite, "%d", this->m_Data.IncreaseDamagePvP);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Khả Năng Phòng Thủ:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eWhite, "%d", this->m_Data.IncreaseDefencePvP);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Kháng Sát Thương Nhân Đôi:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eRed, "%d", this->m_Data.ResistDoubleDamageRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Kháng Bỏ Qua Phòng Thủ:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eWhite, "%d", this->m_Data.ResistIgnoreDefenceRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Kháng Bỏ Qua Tỷ Lệ SD:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eWhite, "%d", this->m_Data.ResistIgnoreSDRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Kháng Sát Thương Chí Mạng:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eWhite, "%d", this->m_Data.ResistCriticalDamageRate);

	this->DrawLine(StartX + 5, StartY + 5, MainWidth, "Kháng Sát Thương Hoàn Hảo:");
	StartY = this->DrawLineR(StartX + 15, StartY + 5, MainWidth, eWhite, "%d", this->m_Data.ResistExellentDamageRate);
	DisableAlphaBlend();

}

void CStatsAdvance::DrawLine(float PosX, float PosY, int Width, LPCSTR Text, ...)
{
	float flResult = 0.0;

	char szBuff[256];
	int iBuffLen = sizeof(szBuff) - 1;
	ZeroMemory(szBuff, iBuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(szBuff, iBuffLen, Text, args);
	va_end(args);

	gInterface.DrawFormat(eWhite, PosX + 10, PosY, 190, 0, szBuff);

}

float CStatsAdvance::DrawLineR(float PosX, float PosY, int Width, DWORD dwColor, LPCSTR Text, ...)
{
	float flResult = 0.0;

	char szBuff[256];
	int iBuffLen = sizeof(szBuff) - 1;
	ZeroMemory(szBuff, iBuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(szBuff, iBuffLen, Text, args);
	va_end(args);

	TextDraw((HFONT)g_hFontBold, PosX + 150, PosY, dwColor, 0x0, 60, 0,1, szBuff);

	flResult = PosY + 12;

	return flResult;
}

void CStatsAdvance::Recv(PMSG_STATS_ADVANCE* lpMsg)
{
	if (!lpMsg->Result)
	{
		return;
	}

	memcpy(&this->m_Data, &lpMsg->StatInfo, sizeof(this->m_Data));

	this->m_Loaded = true;
}