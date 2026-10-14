#include "stdafx.h"
#include "CustomEventTime.h"
#include "CustomMessage.h"
#include "CBInterface.h"
#include "Util.h"
#include "Protect.h"
#include "Other.h"
#include "NewUISystem.h"
#include "UIControls.h"
#include "wsclientinline.h"

SauEventTime gSauEventTime;

SauEventTime::SauEventTime()
{
	this->Init();
}

SauEventTime::~SauEventTime()
{

}

void SauEventTime::Init()
{
	this->m_ListEventTime.clear();
	this->m_ListItemBag.clear();
}

CNewUIScrollBar* ListEventTime = nullptr;
int MaxListEvenTimeInPage = 12;
bool UpdateMaxPosEvent = false;

const char* FormatTime(int time)
{
	static char text2[30];
	int totalseconds = time;
	int hours = totalseconds / 3600;
	int minutes = (totalseconds / 60) % 60;
	int seconds = totalseconds % 60;
	int days;

	if (time == 0)
	{
		wsprintf(text2, "Đã Xuất Hiện");
	}
	else if (time < 0)
	{
		wsprintf(text2, "Tắt");
	}
	else
	{
		if (hours > 23) {
			days = hours / 24;
			wsprintf(text2, "%d day(s)+", days);
		}
		else {
			wsprintf(text2, "%02d:%02d:%02d", hours, minutes, seconds);
		}
	}

	return text2;
}

void SauEventTime::OpenSauEvent()
{
	if ((GetTickCount() - gInterface.Data[eSauNewEvent].EventTick) > 300)
	{
		gInterface.Data[eSauNewEvent].EventTick = GetTickCount();

		XULY_CGPACKET pMsg;

		pMsg.header.set(0xD7, 0x41, sizeof(pMsg));

		pMsg.ThaoTac = 111;

		DataSend((LPBYTE)&pMsg, pMsg.header.size);

	}
}

DWORD Tick;

void SauEventTime::UpdateTime()
{

	if ((GetTickCount() - Tick) > 1000)
	{

		for (int i = 0; i < m_ListEventTime.size(); ++i)
		{
			if (m_ListEventTime[i].TimeBoss > 0)
			{
				--m_ListEventTime[i].TimeBoss;
			}
		}

		Tick = GetTickCount();
	}
}
bool ShowList = false;
int PageList = 0;
int PageItem = 0;
int MaxItemBag = 10;

bool CacheForm2 = false;
int IndexBoss = 0;
int DrawBoss = 0;
int IndexSelect = -1;
bool SetViTri = false;

bool MovePage = false;
void SauEventTime::DrawSauEventTime()
{
	float WindowW = 220;
	float WindowH = 370;
	float StartX = (MAX_WIN_WIDTH / 2) - (WindowW / 2);
	float StartY = ((MAX_WIN_HEIGHT - 51) / 2) - (WindowH / 2);

	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, WindowW, WindowH, eSauNewEvent, "Sự Kiên [H]");
	if (!gInterface.Data[eSauNewEvent].OnShow)
	{
		if (ListEventTime) ListEventTime = nullptr;
		ShowList = false;
		CacheForm2 = false;
		IndexSelect = -1;
		SetViTri = false;
		gInterface.Data[eSauNewEvent].X = StartX;
		return;
	}

	int Show = 150;
	if (ShowList == true)
	{
		WindowW += Show;
		WindowW += 150;
	}
	if (SetViTri == true)
	{
		gInterface.Data[eSauNewEvent].X = gInterface.Data[eSauNewEvent].X - 80 ;

		SetViTri = false;
	}

	::EnableAlphaTest(true);

	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	::DisableAlphaBlend();
	{

		float InfoMocNapX = (StartX + 10) + 3;

		float InfoMocNapY = (StartY + 25);

		float WButton = 88;

		float HInfo = WindowH - 40 - 30 - 20;

		int DataListVQ = this->m_ListEventTime.size();

		UpdateTime();

		float KhoangCachYMocNap = 25;

		int MaxList = 0;

		float InfoEventX = StartX + 25;
		float InfoEventY = StartY + 70;
		int Y = 0;
		float BarW = 330;
		EnableAlphaBlend();
		glColor3f(1.0, 1.0, 1.0);

		gInterface.DrawFormat(eYellow, InfoEventX + 5.0f, InfoEventY - 43.0f, BarW, 1, "Sự Kiên [H]");

		gInterface.DrawFormat(eWhite, InfoEventX + 5, InfoEventY - 15, BarW, 1, "Event");

		gInterface.DrawFormat(eWhite, InfoEventX + 125, InfoEventY - 15, BarW, 1, "Time Event");

		DisableAlphaBlend();
		for (int n = (PageList * MaxListEvenTimeInPage); n < DataListVQ; n++)
		{

			const char* text2 = FormatTime(this->m_ListEventTime[n].TimeBoss);
			::EnableAlphaTest(true);

			::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			::DisableAlphaBlend();

			if (this->m_ListEventTime[n].TimeBoss < 300)
			{
				EnableAlphaTest(true);

				glColor3f(1.0, 1.0, 1.0);

				gInterface.DrawFormat(eYellow, InfoEventX , InfoEventY + 7, BarW, 1, "-[%d] %s" , n + 1, this->m_ListEventTime[n].EventName);

				gInterface.DrawFormat(eExcellent, InfoEventX + 125, InfoEventY + 7, BarW, 1, "%s", text2);

				DisableAlphaBlend();
			}
			else
			{
				EnableAlphaTest(true);

				glColor3f(1.0, 1.0, 1.0);

				gInterface.DrawFormat(eWhite, InfoEventX , InfoEventY + 7, BarW, 1, "-[%d] %s", n + 1, this->m_ListEventTime[n].EventName);

				gInterface.DrawFormat(eWhite, InfoEventX + 125, InfoEventY + 7, BarW, 1, "%s", text2);

				DisableAlphaBlend();

			}

			bool bMouseIn = SEASON3B::CheckMouseIn(InfoEventX - 6, InfoEventY + 2, 184, 18);

			bool bLeftClick = bMouseIn && SEASON3B::IsPress(VK_LBUTTON);

			if (bLeftClick && this->m_ListEventTime[n].TimeBoss < 300)
			{

				XULY_CGPACKET pMsg;
				pMsg.header.set(0xD3, 0x41, sizeof(pMsg));
				pMsg.ThaoTac = this->m_ListEventTime[n].MoveGate[rand() % 3];
				DataSend((LPBYTE)&pMsg, pMsg.header.size);
				gInterface.Data[eSauNewEvent].OnShow ^= 1;
				PlayBuffer(25, 0, 0);
			}

			if (bMouseIn)
			{
				if (this->m_ListEventTime[n].TimeBoss < 300)
				{

					FrameTarget(InfoEventX - 6, InfoEventY + 2, 184, 18, RGBA(144, 238, 144, 0xFF));
				}
				else
				{

					FrameTarget(InfoEventX - 6, InfoEventY + 2, 184, 18, RGBA(255, 0, 0, 0xFF));
				}
			}

			InfoEventY = InfoEventY + (KhoangCachYMocNap);
			MaxList++;
			if (MaxList >= MaxListEvenTimeInPage) break;
		}

		int sotrang = DataListVQ / MaxListEvenTimeInPage;

		if (DataListVQ % MaxListEvenTimeInPage > 0)
		{
			sotrang += 1;
		}

		float CenterY = (StartY + WindowH) ;

		const BYTE state[3] = { 0, 1, 2 };

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		EnableAlphaTest(true);

		glColor3f(1.0, 1.0, 1.0);

		float PosXBoxItem = StartX + BarW - 10;
		float PosYBoxItem = StartY + 55;
		int KhoangCach = 18;
		int CountDoc = 0;

		if (ShowList == true)
		{
			gInterface.DrawFormat(eYellow, PosXBoxItem + 20, PosYBoxItem - 15, BarW, 1, "Vật Phẩm Trong Sự Kiện");

			g_pBCustomMenuInfo->RenderToolTipItem(PosXBoxItem - 3, PosYBoxItem - 5, Show, WindowH - 60 - 40, 0x00000060, 0, 0);

			int Count = 0;

			float XKeo = PosXBoxItem + 5 + Show;

			float YKeo = StartY + 40;

			StartX = StartX + 10;

			int DataItemBag = this->m_ListItemBag.size();

			if (MovePage == true)
			{

				MovePage = false;
			}

			int sotrang2 = DataItemBag / MaxItemBag;

			if (DataItemBag % MaxItemBag > 0)
			{
				sotrang2 += 1;
			}

			for (int x = (PageItem * MaxItemBag); x < DataItemBag; x++)
			{
				EnableAlphaTest(true);

				glColor3f(1.0, 1.0, 1.0);
				gInterface.DrawFormat(eYellow, PosXBoxItem + 5, PosYBoxItem , Show, 1, "[%d]- %s ", x + 1, BGetItemName(this->m_ListItemBag[x].Index, this->m_ListItemBag[x].RateDrop));

				PosYBoxItem = PosYBoxItem + KhoangCach;
				CountDoc++;
				if (CountDoc >= MaxItemBag) break;
				DisableAlphaBlend();
			}
			EnableAlphaTest(true);

			glColor3f(1.0, 1.0, 1.0);
			gInterface.DrawFormat(eYellow, PosXBoxItem + 65, StartY + WindowH - 70 - (Count * 13), Show, 1, "%d / %d", PageItem + 1, sotrang2);
			DisableAlphaBlend();

		}

	}
}

void SauEventTime::RecvListEventTime(BYTE* Recv)
{
	if (!Recv) return;
	this->m_ListEventTime.clear();

	PMSG_SAUEVENT_SEND* mRecv = (PMSG_SAUEVENT_SEND*)Recv;

	for (int i = 0; i < mRecv->count; i++)
	{
		SendEventTime lpInfo = *(SendEventTime*)(((BYTE*)Recv) + sizeof(PMSG_SAUEVENT_SEND) + (sizeof(SendEventTime) * i));

		this->m_ListEventTime.push_back(lpInfo);

	}
	gInterface.Data[eSauNewEvent].OnShow ^= 1;
}

void SauEventTime::RecvListItem(BYTE* Recv)
{
	if (!Recv) return;

	PMSG_SAUEVENT_SEND* mRecv = (PMSG_SAUEVENT_SEND*)Recv;

	for (int i = 0; i < mRecv->count; i++)
	{
		ITEM_BAG_INFO lpInfo = *(ITEM_BAG_INFO*)(((BYTE*)Recv) + sizeof(PMSG_SAUEVENT_SEND) + (sizeof(ITEM_BAG_INFO) * i));

		this->m_ListItemBag.push_back(lpInfo);

	}
}