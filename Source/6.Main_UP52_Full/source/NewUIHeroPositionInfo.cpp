

#include "stdafx.h"
#include "PgFont.h"
#include "NewUIHeroPositionInfo.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "MapManager.h"
#include "ZzzAI.h"
#include "ZzzOpenglUtil.h"
#include "Protocol.h"
#include "CBInterface.h"
#include "MenuCustom.h"
#include "BAiFindPath.h"
#include "PartyManager.h"
using namespace SEASON3B;
char* ButtonText[6] = { "Menu", "Show Name", "Show HP Bar", "Camera 3D","Config Auto","Active Auto" };
double ScaleMap = 0.125;
DWORD CacheTimeClick = 0;
bool BlockClickCustom = false;
bool ShowMiniMapColtrol = true;
extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayWinMid;
extern int DisplayHeightExt;
extern int DisplayWinExt;
extern int DisplayWinReal;

// The bar the map line sits on: TournamentUI_I9, drawn at the art's own size.
// The glyph on its left is a button, and it opens the same menu F5 does - see
// the hit test in UpdateMouseEvent and the draw in Render.
// Every number below is measured off the reference shot, not guessed. The
// window is created at (0,0), so these are viewport coordinates directly: the
// bar spans x 61..201, y 78..97 with the viewport starting at (38,70).
const float PG_MAPBAR_W = 124.f;
const float PG_MAPBAR_H = 18.f;
const float PG_MAPBAR_BTN = 24.f;		// the glyph plus its margin
// The bar is inset from the very corner rather than jammed against it - flush
// against the window edge it reads as part of the title bar above it.
const float PG_MAPBAR_OX = 23.f;
const float PG_MAPBAR_OY = 8.f;
CNewUIHeroPositionInfo::CNewUIHeroPositionInfo()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_CurHeroPosition.x = m_CurHeroPosition.y = 0;
	IndexIMGMap = -1;

	this->DataMap.ModelID = -1;
	this->DataMap.XSW_Minimap_Frame = 0;
	this->DataMap.YSW_Minimap_Frame = 0;
	this->DataMap.XSW_Minimap = 15;
	this->DataMap.YSW_Minimap = 17;
	this->DataMap.XSW_Time_Frame = 0;
	this->DataMap.YSW_Time_Frame = 0;
	this->DataMap.This_92 = 0;
	this->DataMap.This_96 = 0;
	this->DataMap.DropMoveUserX = 0;
	this->DataMap.DropMoveUserY = 0;
	this->DataMap.This_108;
	this->DataMap.This_112;
	this->DataMap.CurrentMoveX = 0;
	this->DataMap.CurrentMoveY = 0;
	this->DataMap.Scale = 1;
	this->DataMap.TimerEventVK = 0;
	this->DataMap.Alpha = 1.0f;
}

CNewUIHeroPositionInfo::~CNewUIHeroPositionInfo()
{
	Release();
}

bool CNewUIHeroPositionInfo::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_HERO_POSITION_INFO, this);

	WidenX = HERO_POSITION_INFO_BASEB_WINDOW_WIDTH;
	if (WindowWidth == 800)
	{
		WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.4f));
	}
	else
		if (WindowWidth == 1024)
		{
			WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.2f));
		}

	SetPos(x, y);
	LoadImages();

	unicode::t_string tooltiptext1 = GlobalText[3561];
	unicode::t_string btname1 = "";

	SetButtonInfo(
		&m_BtnConfig,
		IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 3,
		x + (int)(PG_MAPBAR_OX + PG_MAPBAR_W) + 6,
		y,
		18,
		13,
		1,
		0,
		1,
		1u,
		btname1,
		tooltiptext1,
		0);

	unicode::t_string tooltiptext2 = GlobalText[3562];
	unicode::t_string btname2 = "";

	SetButtonInfo(
		&m_BtnStart,
		IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4,
		x + (int)(PG_MAPBAR_OX + PG_MAPBAR_W) + 24,
		y,
		18,
		13,
		1,
		0,
		1,
		1u,
		btname2,
		tooltiptext2,
		0);

	unicode::t_string tooltiptext3 = GlobalText[3563];
	unicode::t_string btname3 = "";

	SetButtonInfo(
		&m_BtnStop,
		IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 5,
		x + (int)(PG_MAPBAR_OX + PG_MAPBAR_W) + 24,
		y,
		18,
		13,
		1,
		0,
		1,
		1u,
		btname2,
		tooltiptext3,
		0);

	MoveTextTipPos(&m_BtnConfig, -20, 9);
	MoveTextTipPos(&m_BtnStart, -20, 9);
	MoveTextTipPos(&m_BtnStop, -20, 9);

	Show(true);
	TimeClickMouseL = 0;
	return true;
}

void CNewUIHeroPositionInfo::Release()
{
	UnloadImages();
	IndexIMGMap = -1;
	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIHeroPositionInfo::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool CNewUIHeroPositionInfo::BtnProcess()
{
	// Nothing left to process. The helper's config and start/stop buttons moved
	// to the HUD's Start Auto plate; they are no longer drawn here, and leaving
	// their hit tests live would have them swallowing clicks on a bar that no
	// longer shows them.
	return false;
}

bool CNewUIHeroPositionInfo::UpdateMouseEvent()
{
	// The glyph on the left of the bar opens the game menu, the same one F5
	// opens. It looks like a button in the art, so it behaves like one.
	if (CheckMouseIn(m_Pos.x + PG_MAPBAR_OX, m_Pos.y + PG_MAPBAR_OY, PG_MAPBAR_BTN, PG_MAPBAR_H))
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			gBCustomMenu.OpenCustomMenu();
			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	if (true == BtnProcess() || BlockClickCustom)
		return false;

	int Width = HERO_POSITION_INFO_BASEA_WINDOW_WIDTH + WidenX + HERO_POSITION_INFO_BASEC_WINDOW_WIDTH;
	if (CheckMouseIn(m_Pos.x, m_Pos.y, Width + 80, HERO_POSITION_INFO_BASE_WINDOW_HEIGHT))
		return false;

	return true;
}

bool CNewUIHeroPositionInfo::UpdateKeyEvent()
{
	return true;
}

bool CNewUIHeroPositionInfo::Update()
{
	if ((IsVisible() == true) && (Hero != NULL))
	{
		m_CurHeroPosition.x = (Hero->PositionX);
		m_CurHeroPosition.y = (Hero->PositionY);
	}

	return true;
}
bool MiniMapFileCheck1(int Map)
{
	if (Map - 1 == 30) { return 0; }

	char Path[64];

	wsprintf(Path, "Data\\Custom\\Maps\\World%d.ozt", Map);
	FILE* fp = fopen(Path, "rb");

	if (fp != NULL)
	{
		fclose(fp);
		return 1;
	}
	else
	{
		return 0;
	}
}
int CacheMapNumber = -1;
void CNewUIHeroPositionInfo::MiniMapLoad()
{
	if (gMapManager.WorldActive != CacheMapNumber && gMapManager.WorldActive != 30)
	{
		char buff[32];

		wsprintf(buff, "Custom\\Maps\\World%d.tga", (gMapManager.WorldActive + 1));

		if (MiniMapFileCheck1(gMapManager.WorldActive + 1) != 0)
		{
			IndexIMGMap = IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 6;
			LoadBitmap(buff, IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 6, GL_LINEAR);
		}
		else
		{
			IndexIMGMap = -1;
		}

	}
	CacheMapNumber = gMapManager.WorldActive;
}

void CNewUIHeroPositionInfo::DataViewPortMapLoad(float x, float y)
{
	float v53;
	float v48;
	float X;
	float Y;
	int v8;
	signed int i;
	float lpObj[2];
	float lpTarget[2];
	EnableAlphaTest(1);
	float BoxPrintW;
	float BoxCurrentX;
	float BoxCurrentY;

	lpObj[0] = (double)(Hero->PositionX) + 2.5;
	lpObj[1] = (double)(Hero->PositionY);

	BoxPrintW = (ScaleMap * 2);

	v53 = (double)(lpObj[0] + 2.5) / BoxPrintW;
	v48 = (double)(253.5 - lpObj[1]) / BoxPrintW;
	for (int i = 0; i < 400; ++i)
	{
		CHARACTER* c = &CharactersClient[i];
		OBJECT* o = &c->Object;
		if (!c || c == Hero || !o->Live) continue;

		lpTarget[0] = (double)(c->PositionX) + 2.5;
		lpTarget[1] = (double)(c->PositionY);

		if (ScaleMap == 0.125)
		{
			BoxCurrentX = 56.0 + ((lpObj[0] - lpTarget[0]) * ((lpObj[0] < lpTarget[0]) ? 1.1 : 1.7));
			BoxCurrentY = 46.0 - ((lpObj[1] - lpTarget[1]) * 1.4);
		}
		else if (ScaleMap == 0.25)
		{
			BoxCurrentX = 56.0 + ((lpObj[0] - lpTarget[0]) * 0.7);
			BoxCurrentY = 46.0 - ((lpObj[1] - lpTarget[1]) * 0.5);
		}
		else
		{
			BoxCurrentX = 51.0 + ((lpObj[0] - lpTarget[0]) * 0.4);
			BoxCurrentY = 46.0 - ((lpObj[1] - lpTarget[1]) * 0.4);
		}
		X = x + BoxCurrentX + (lpTarget[0] / BoxPrintW) - v53;
		Y = y + BoxCurrentY + ((253.5 - lpTarget[1]) / BoxPrintW) - v48;

		// Size carries as much as colour on this map. In a spot the dots are
		// mostly monsters, and picking a party member out of that crowd by hue
		// alone does not work on a 3px square - so the ones you actually need to
		// find are drawn bigger as well as differently coloured.
		float fDot = 3.f;

		if (o->Kind == KIND_NPC)
		{
			glColor3f(206 / 255.f, 209 / 255.f, 4 / 255.f);
		}
		else if (o->Kind == KIND_PLAYER)
		{
			glColor3f(0 / 255.f, 255 / 255.f, 72 / 255.f);
		}
		else if (o->Kind == KIND_MONSTER)
		{
			glColor3f(209 / 255.f, 144 / 255.f, 4 / 255.f);
		}

		// PK status, over the plain player colour. Red is the one hue none of the
		// four kinds already use, and this is the piece of information on the map
		// that actually costs you something to miss.
		if (o->Kind == KIND_PLAYER && c->PK >= PVP_CAUTION)
		{
			if (c->PK >= PVP_MURDERER1)
			{
				glColor3f(1.f, 0.16f, 0.16f);
				fDot = 4.f;
			}
			else
			{
				glColor3f(1.f, 0.52f, 0.32f);
			}
		}

		// Guild first, then party over the top of it. Someone who is both is far
		// more useful to spot as a party member, so party wins the tie.
		if (c->GuildMarkIndex == Hero->GuildMarkIndex && Hero->GuildMarkIndex >= 0)
		{
			glColor3f(0 / 255.f, 255 / 255.f, 234 / 255.f);
			fDot = 4.f;
		}

		// White is the one bright colour none of the four kinds already use, so a
		// party member reads instantly against yellow, green, orange and cyan.
		if (g_pPartyManager->IsPartyMemberChar(c) == true)
		{
			glColor3f(1.f, 1.f, 1.f);
			fDot = 5.f;
		}

		// Grown about its centre, so a bigger dot marks the same spot rather
		// than drifting down and right of it.
		if (x < 94 && y < 94)
		{
			const float fOff = (fDot - 3.f) * 0.5f;
			RenderColor((float)(X)-fOff, (float)(Y)-fOff, fDot, fDot);
		}
	}
	EndRenderColor();
}
void SetBlockCur(bool Set)
{
	g_pBCustomMenuInfo->SetBlockCur(Set);
}
void JShowButtonSlash(float X, float Y, float W, float DropCurrentY)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		SetBlockCur(true);
		RenderTipText(X + 30, Y + 3, ButtonText[0]);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
			gBCustomMenu.OpenCustomMenu();
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		if (gInterface.Data[eSauMenu].OnShow)
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.008999999613, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}

}

void JGetEventZoom(float X, float Y, float W)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && ScaleMap != 0.125 && GetTickCount() >= CacheTimeClick + 300)
		{
			PlayBuffer(25, 0, 0);
			ScaleMap = (ScaleMap == 0.375) ? 0.25 : 0.125;
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, 0.3775999844, 0.1209000051, 0.05900005624, 1, 1, 0.0);
			CacheTimeClick = GetTickCount();
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, (ScaleMap == 0.125) ? 0.3834999801 : 0.1341000199, 0.3775999844, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, (ScaleMap == 0.125) ? 0.3834999801 : 0.008999999613, 0.3775999844, 0.1209000051, 0.05900005624, 1, 1, 0.0);
	}

}

void JGetEventinZoom(float X, float Y, float W)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && ScaleMap != 0.375 && GetTickCount() >= CacheTimeClick + 300)
		{
			PlayBuffer(25, 0, 0);

			ScaleMap = (ScaleMap == 0.125) ? 0.25 : 0.375;

			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, 0.4396000206, 0.1209000051, 0.05900005624, 1, 1, 0.0);
			CacheTimeClick = GetTickCount();
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, (ScaleMap == 0.375) ? 0.3834999801 : 0.1341000199, 0.4396000206, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, (ScaleMap == 0.375) ? 0.3834999801 : 0.008999999613, 0.4396000206, 0.1209000051, 0.05900005624, 1, 1, 0.0);
	}

}
void JShowButton3DCamera(float X, float Y, float W, float DropCurrentY)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		SetBlockCur(true);
		RenderTipText(X + 30, Y + 3, ButtonText[3]);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
			Camera3DEnable ^= 1;
			CameraAngle[2] = -45.f;
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
			if (Camera3DEnable)
			{
				CreateNotice("On Camera 3D", 1);
			}
			else
			{
				CreateNotice("Off Camera 3D", 1);
			}
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		if (Camera3DEnable)
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.008999999613, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}

	}

}
void JShowButtonHPBar(float X, float Y, float W, float DropCurrentY)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		RenderTipText(X + 30, Y + 3, ButtonText[2]);
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
			mShowHPBar ^= 1;
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		if (mShowHPBar)
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.008999999613, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}

}
void JShowButtonShowName(float X, float Y, float W, float DropCurrentY)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		RenderTipText(X + 30, Y + 3, ButtonText[1]);
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
			if (gProtect.m_MainInfo.PlayerShowName)
			{
				mShowName ^= 1;
				g_bGMObservation = mShowName;
			}

			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		if (mShowName)
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.008999999613, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}

}

void JShowButtonConfigAuto(float X, float Y, float W, float DropCurrentY)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		RenderTipText(X + 30, Y + 3, ButtonText[4]);
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);

			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MuHelper);
			PlayBuffer(SOUND_CLICK01);
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{

		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.008999999613, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}

}
void JShowButtonPlayAuto(float X, float Y, float W, float DropCurrentY)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		RenderTipText(X + 30, Y + 3, ButtonText[5]);
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			if (!g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Started)
			{

				StartMuHelper();
			}
			else
			{
				StartMuHelper(1);
			}
			PlayBuffer(SOUND_CLICK01);

			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.2588, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Started)
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.1341000199, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.008999999613, DropCurrentY, 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}

}
void JGetButtonClosed(float X, float Y, float W)
{
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
			PlayBuffer(25, 0, 0);
			ShowMiniMapColtrol ^= 1;
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.7661997676, 0.7609999776 + (!ShowMiniMapColtrol ? 0.06260011024 : 0), 0.08950000256, 0.04200005904, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.6410998702, 0.7609999776 + (!ShowMiniMapColtrol ? 0.06260011024 : 0), 0.08950000256, 0.04200005904, 1, 1, 0.0);
		}
	}
	else
	{
		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.5159999728, 0.7609999776 + (!ShowMiniMapColtrol ? 0.06260011024 : 0), 0.08950000256, 0.04200005904, 1, 1, 0.0);
	}
}

void JGetButtonAutoReset(float X, float Y, float W)
{
	EnableAlphaTest(1);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	if (SEASON3B::CheckMouseIn((int)X, (int)Y, (int)W, (int)W) && !g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
	{
		SetBlockCur(true);
		if (SEASON3B::IsPress(VK_LBUTTON) && (GetTickCount() - CacheTimeClick) > 300)
		{
			CacheTimeClick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
			PlayBuffer(25, 0, 0);

			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.7661997676, 0.3775999844 + (0.06260011024 * 9), 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
		else
		{
			RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.6410998702, 0.3775999844 + (0.06260011024 * 9), 0.1209000051, 0.05900005624, 1, 1, 0.0);
		}
	}
	else
	{
		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, W, 0.5159999728, 0.3775999844 + (0.06260011024 * 9), 0.1209000051, 0.05900005624, 1, 1, 0.0);
	}
	DisableAlphaBlend();
	EnableAlphaTest(0);
}
void JGetTimeBar(float X, float Y, float W, float H)
{
	time_t TimeLocal;
	struct tm* LocalT;
	time(&TimeLocal);

	LocalT = localtime(&TimeLocal);
	RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, X, Y, W, H, 0.7919999957, 0.4439999759, 0.1950000077, 0.05000000075, 1, 1, 0.0);

	char LocalTime[30];
	sprintf(LocalTime, "%2d:%02d", LocalT->tm_hour, LocalT->tm_min);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText((int)X, (int)Y + 4, LocalTime, (int)W, 0, RT3_SORT_CENTER);

}
// A look per arrival, so the banner is not the same trick every time. Each one
// pairs an effect set with a colour that suits it - FIRE wants embers under it,
// ICE wants pale blue - and RAINBOW writes its own hue, so those rows sit on
// white and let it work.
struct PG_BANNER_LOOK
{
	DWORD dwFx;
	float r, g, b;
};

static const PG_BANNER_LOOK PG_BANNER_LOOKS[] =
{
	{ PG_TFX_POPIN  | PG_TFX_SHEEN | PG_TFX_GRADIENT, 1.00f, 0.84f, 0.35f },
	{ PG_TFX_FIRE   | PG_TFX_SWELL,                   1.00f, 0.55f, 0.20f },
	{ PG_TFX_ICE    | PG_TFX_WAVE,                    0.65f, 0.85f, 1.00f },
	{ PG_TFX_RAINBOW| PG_TFX_BOUNCE,                  1.00f, 1.00f, 1.00f },
	{ PG_TFX_GLITCH | PG_TFX_TREMBLE,                 0.70f, 1.00f, 0.70f },
	{ PG_TFX_TYPE   | PG_TFX_SHEEN,                   0.90f, 0.90f, 1.00f },
	{ PG_TFX_WAVE   | PG_TFX_PULSE | PG_TFX_GRADIENT, 1.00f, 0.72f, 0.85f },
	{ PG_TFX_POPIN  | PG_TFX_BOUNCE | PG_TFX_RAINBOW, 1.00f, 1.00f, 1.00f },
};

static const int PG_BANNER_LOOK_COUNT =
	sizeof(PG_BANNER_LOOKS) / sizeof(PG_BANNER_LOOKS[0]);

// The map's name, once, when you arrive. The atlas and its effect flags already
// do the work, so this is just the timing around a single call.
static void PgMapBanner()
{
	static int   s_iLastMap = -1;
	static DWORD s_dwShown  = 0;
	static int   s_iLook    = -1;

	const int iMap = gMapManager.WorldActive;

	if (iMap != s_iLastMap)
	{
		// The first map of a session is an arrival too, but the world is still
		// loading at that point and the banner would play to a black screen, so
		// the first change only arms it.
		if (s_iLastMap != -1)
			s_dwShown = GetTickCount();

		// Stepped rather than drawn at random: cycling is what actually
		// guarantees "something different each time", where random happily
		// repeats. The starting point comes off the clock so the order is not
		// identical every session.
		if (s_iLook < 0)
			s_iLook = (int)(GetTickCount() % PG_BANNER_LOOK_COUNT);
		else
			s_iLook = (s_iLook + 1) % PG_BANNER_LOOK_COUNT;

		s_iLastMap = iMap;
	}

	if (s_dwShown == 0)
		return;

	const float PG_BANNER_MS = 3200.f;
	const DWORD dwLife = GetTickCount() - s_dwShown;

	if (dwLife > (DWORD)PG_BANNER_MS)
	{
		s_dwShown = 0;
		return;
	}

	const char* szMap = gMapManager.GetMapName(iMap);

	if (szMap == NULL || szMap[0] == 0)
		return;

	// In quickly, hold, then out slowly. An arrival should register at once and
	// leave without being noticed going.
	const float t = dwLife / PG_BANNER_MS;
	float fAlpha = 1.f;

	if (t < 0.12f)
		fAlpha = t / 0.12f;
	else if (t > 0.75f)
		fAlpha = 1.f - (t - 0.75f) / 0.25f;

	const PG_BANNER_LOOK& look =
		PG_BANNER_LOOKS[s_iLook < 0 ? 0 : s_iLook];

	// The phase is taken from the arrival, so effects that travel along the
	// string start at their beginning rather than wherever the global clock
	// happened to be.
	PgScreenTextFx(MAX_WIN_WIDTH * 0.5f, DisplayHeight * 0.30f, szMap,
		look.r, look.g, look.b, 34.f, fAlpha, look.dwFx, (float)s_dwShown);
}

void CNewUIHeroPositionInfo::DrawMiniMap()
{
	if (IndexIMGMap == -1 || !mShowMiniMap) return;

	float v67;
	unsigned __int8 v87;
	int v92;
	char Cord[256];
	float y;
	float x;
	int JGetHelperBar;
	double RectangleRotate[2][8];

	float usercurrenty;
	float usercurrentx;

	RectangleRotate[0][0] = 50.0;
	RectangleRotate[1][0] = 70.0;

	RectangleRotate[0][1] = 51.5;
	RectangleRotate[1][1] = 69.5;

	RectangleRotate[0][2] = 52.5;
	RectangleRotate[1][2] = 68.5;

	RectangleRotate[0][3] = 51.5;
	RectangleRotate[1][3] = 67.0;

	RectangleRotate[0][4] = 51.0;
	RectangleRotate[1][4] = 66.0;

	RectangleRotate[0][5] = 49.0;
	RectangleRotate[1][5] = 67.0;

	RectangleRotate[0][6] = 48.5;
	RectangleRotate[1][6] = 68.0;

	RectangleRotate[0][7] = 49.0;
	RectangleRotate[1][7] = 69.5;

	x = 2;
	y = 2;
	EnableAlphaTest(1);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	JGetButtonClosed(x, y, 15.0);

	if (ShowMiniMapColtrol)
	{

		float ViTriXMap = x;

		usercurrentx = (double)(Hero->PositionX) / 256.0;
		usercurrenty = (double)(256 - Hero->PositionY) / 256.0;
		GetDrawCircle(g_pHeroPositionInfo->IndexIMGMap, ViTriXMap + 9.0, y + 20.0, 84.0, usercurrentx, usercurrenty, ScaleMap, 1, 1, 0.0);

		DataViewPortMapLoad(x + 3, y + 19.0);

		v87 = (unsigned __int8)(signed int)(((Hero->Object.Angle[2]) + 22.5) / 360.0 * 8.0 + 1.0) % 8;
		v67 = (float)(180 + (Hero->Object.Angle[2]));

		RenderBitmapRotate(BITMAP_INTERFACE_CUSTOM + 4, ViTriXMap + RectangleRotate[0][(int)v87], y + RectangleRotate[1][(int)v87] - 5, 12.0, 12.0, v67, 0.0, 0.0, 0.1030000001, 0.05200000107);

		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 4, ViTriXMap + 5.0, y + 18.0, 92.0, 92.0, 0.3700000048, 0.06200000271, 0.5769999027, 0.2899999917, 1, 1, 0.0);

		float SacleY = 0.06260011024;
		float SacleX = 0.1239999682;

		JShowButtonSlash(x + 1, y + 23.0, 18.0, 0.8768191338);

		JGetEventZoom(x, y + 77.0, 18.0);
		JGetEventinZoom(x + 14.0, y + 93.0, 18.0);

		if (!g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Started)
		{
			JShowButtonPlayAuto(x + 75.0, y + 19, 18.0, 0.3775999844 + (SacleY * 2));
		}
		else
		{
			JShowButtonPlayAuto(x + 75.0, y + 19, 18.0, 0.3775999844 + (SacleY * 3));
		}
		JShowButtonConfigAuto(x + 88.0, y + 37.0, 18.0, 0.3775999844 + (SacleY * 5));

		JShowButton3DCamera(x + 90.0, y + 56.0, 18.0, 0.3775999844 + (SacleY * 4));
		JShowButtonHPBar(x + 85.0, y + 75.0, 18.0, 0.3775999844 + (SacleY * 9));
		JShowButtonShowName(x + 71.0, y + 93.0, 18.0, 0.3775999844 + (SacleY * 7));

		DisableAlphaBlend();
		EnableAlphaTest(0);

		EnableAlphaBlend();

		RenderBitmap(BITMAP_INTERFACE_CUSTOM + 2, ViTriXMap + 9.5, y + 22.5, 82.0, 82.0, 0.3729999959, 0.03899973631, 0.2620999813, 0.2600000203, 1, 1, 0.0);
	}
	DisableAlphaBlend();
	EnableAlphaTest(0);
}

struct AUTOMOVE_REQ
{
	PSBMSG_HEAD h;
	int	TYPE;
};

void CGAutoMove(int Type)
{
	AUTOMOVE_REQ pRequest;
	pRequest.h.set(0xF3, 0x37, sizeof(pRequest));
	pRequest.TYPE = Type;
	DataSend((LPBYTE)&pRequest, pRequest.h.size);
}

int MoveAutoLoad = 0;
void AutoMove()
{
	if (!g_pNewUIMiniMap->Movement)
	{
		return;
	}
	char MessText[255] = { 0 };

	if (SEASON3B::IsPress(VK_ESCAPE))
	{
		Hero->Movement = false;
		g_pNewUIMiniMap->Movement = false;
		SetPlayerStop(Hero);
		gBAiFindPath.Clear();
		return;
	}

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pRenderText->SetShadowText(1);
	g_pRenderText->SetTextColor(255, 179, 0, 255);

	unicode::_sprintf(MessText, gCustomMessage.GetMessageA(93), gBAiFindPath.ViTriCanDenX, gBAiFindPath.ViTriCanDenY);
	g_pRenderText->RenderText((int)(DisplayWin / 2) - (300 / 2), (int)30, MessText, 300, 25, RT3_SORT_CENTER);

	if (MoveAutoLoad == 1)
	{
		g_pRenderText->RenderText((int)(DisplayWin / 2) - (50 / 2), (int)42, "AUTO RUNNING ..", 0, 0, RT3_SORT_CENTER);
		MoveAutoLoad = 2;
	}
	else if (MoveAutoLoad == 2)
	{
		g_pRenderText->RenderText((int)(DisplayWin / 2) - (50 / 2), (int)42, "AUTO RUNNING ...", 0, 0, RT3_SORT_CENTER);
		MoveAutoLoad = 3;
	}
	else if (MoveAutoLoad == 3)
	{
		g_pRenderText->RenderText((int)(DisplayWin / 2) - (50 / 2), (int)42, "AUTO RUNNING ....", 0, 0, RT3_SORT_CENTER);
		MoveAutoLoad = 4;
	}
	else if (MoveAutoLoad == 4)
	{
		g_pRenderText->RenderText((int)(DisplayWin / 2) - (50 / 2), (int)42, "AUTO RUNNING .....", 0, 0, RT3_SORT_CENTER);
		MoveAutoLoad = 0;
	}
	else
	{
		g_pRenderText->RenderText((int)(DisplayWin / 2) - (50 / 2), (int)42, "AUTO RUNNING ......", 0, 0, RT3_SORT_CENTER);
		MoveAutoLoad = 1;
	}
	g_pRenderText->SetShadowText(0);
	if (Hero->Movement)
	{
		return;
	}

	if (gBAiFindPath.ViTriCanDenX != -1 && gBAiFindPath.ViTriCanDenY != -1)
	{
		if (Hero->PositionX == gBAiFindPath.ViTriCanDenX && Hero->PositionY == gBAiFindPath.ViTriCanDenY)
		{
			gBAiFindPath.Clear();
			g_pNewUIMiniMap->Movement = false;
		}
		else
		{
			gBAiFindPath.TimDuongDi(Hero->PositionX, Hero->PositionY, gBAiFindPath.ViTriCanDenX, gBAiFindPath.ViTriCanDenY);
		}
	}
	if (Hero->Dead == 0)
	{
		if (Hero->Appear == 0)
		{
			if (PathFinding2(Hero->PositionX, Hero->PositionY, g_pNewUIMiniMap->ViTriDiChuyen.x, g_pNewUIMiniMap->ViTriDiChuyen.y, &Hero->Path, 0.0f))
			{
				Hero->MovementType = MOVEMENT_MOVE;

				Hero->Movement = true;
				SendMove(Hero, &Hero->Object);

			}
			else
			{
				Hero->Movement = false;
			}
		}
	}
	else
	{
		Hero->Movement = false;
	}

}

void ShowInfoTitleWindow()
{

	bool condition = true;
	std::string windowName = gProtect.m_MainInfo.WindowName;
	if (Hero)
	{
		windowName += " [Char: " + std::string(CharacterAttribute->Name) + "]";
		windowName += " (Lv " + std::to_string(CharacterAttribute->Level);
		windowName += " || Rs " + std::to_string(CharacterAttribute->PrintPlayer.ViewReset);
		windowName += " || RL " + std::to_string(CharacterAttribute->PrintPlayer.ViewMasterReset) + ")";
	}

	char fpsPingInfo[128];
	if (FPS_AVG > 0)
	{
		sprintf_s(fpsPingInfo, sizeof(fpsPingInfo), " | FPS: %.0f | Ping: %dms", FPS_AVG, g_Ping);
		windowName += fpsPingInfo;
	}
	else if (FPS > 0)
	{
		sprintf_s(fpsPingInfo, sizeof(fpsPingInfo), " | FPS: %.0f | Ping: %dms", FPS, g_Ping);
		windowName += fpsPingInfo;
	}

	SetWindowText(g_hWnd, windowName.c_str());
}

#if(UseNewMacroUI == 0)
bool CNewUIHeroPositionInfo::Render()
{
	ShowInfoTitleWindow();
	MiniMapLoad();
	AutoMove();
	unicode::t_char szText[255] = { NULL, };

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	// The old three-part Minimap_position bar is gone; this is TournamentUI_I9,
	// the plate the game-menu button wears, with the three-line glyph on its
	// left that opens that same menu.
	//
	// I9 is a white alpha mask, not coloured art: its border carries alpha 139
	// and its interior only 18, so the caller's glColor is what gives it a
	// colour and the world shows through the middle. That transparency is the
	// whole look - an opaque plate of the same colour reads as a sticker, and
	// the bar visibly warms up when you stand somewhere red. So it is tinted
	// here rather than repainted in a file.
	//
	// Only the glyph is taken from that art now; the bar itself is drawn, for
	// the reasons in the block below.
	{
		const float bx = m_Pos.x + PG_MAPBAR_OX;
		const float by = m_Pos.y + PG_MAPBAR_OY;

		// Sampled off the reference: over a black background the bar reads a flat
		// neutral 38, and where a lit pillar passes behind it the same pixels rise
		// to ~123. Solving those two together gives alpha 0.37 over a 0.40 grey.
		// It is translucent on purpose - the world is meant to show through, and
		// an opaque fill is what made this read as a sticker before.
		//
		// There is no frame. All four edges step 0 -> 38 with no lighter line, so
		// the gold border that used to be drawn here was invented.
		gInterface.DrawBarForm(bx, by, PG_MAPBAR_W, PG_MAPBAR_H,
			0.40f, 0.40f, 0.40f, 0.37f);

		// The glyph, drawn rather than lifted out of TournamentUI_I9: the three
		// bars measure 17 wide on a 6px pitch starting 2 down, which no crop of
		// that file lands on. Body ~123 with a brighter top edge near 150.
		for (int i = 0; i < 3; ++i)
		{
			const float gy = by + 2.f + i * 5.f;
			gInterface.DrawBarForm(bx + 4.f, gy, 15.f, 4.f, 0.48f, 0.48f, 0.48f, 1.f);
			gInterface.DrawBarForm(bx + 4.f, gy, 15.f, 1.f, 0.60f, 0.60f, 0.60f, 1.f);
		}
	}

	// The helper's config and start/stop buttons used to sit here, wedged
	// against the right end of the bar. They live on the HUD's Start Auto
	// plate now - see NewUIMainFrameWindow.cpp - which leaves this a bar and
	// nothing else.

	unicode::_sprintf(szText, "%s (%d , %d)", gMapManager.GetMapName(gMapManager.WorldActive), m_CurHeroPosition.x, m_CurHeroPosition.y);

	// Two lines, the way the bar is meant to read: the plate carries its own
	// caption, and where you are sits underneath it as bare gold text with no
	// plate of its own. Putting the map name inside the bar was the mistake -
	// it crowded the caption out and left the line fighting the glyph for room.
	//
	// Both are drawn through the same call "Menu Game [F5]" uses, with the same
	// eYellow, because that is the bar this one is copying. The sprite atlas is
	// the right typeface for names over heads and the wrong one here.
	{
		// Centred on what is left of the plate beside the glyph, so it clears
		// the button.
		// Two colours, not one. The caption samples as eGold (255,189,25) and the
		// map line as pure eYellow (255,255,0).
		//
		// Both are RT3_SORT_LEFT. _define.h has only LEFT=1, LEFT_CLIP=2 and
		// CENTER=3 - there is no right-align to ask for, and 0 is not a value at
		// all. Passing 2 here is what threw the caption off to the right, and
		// "Menu Game [F5]" itself passes 1, so left is also what it does.
		// The caption is placed so it ends near the bar's right edge instead.
		gInterface.DrawFormat(eGold, (int)(m_Pos.x + PG_MAPBAR_OX + 38.f),
			(int)(m_Pos.y + PG_MAPBAR_OY + 4.f), 0, RT3_SORT_LEFT,
			"Menu Game [F5]");

		// The map line starts flush with the bar's left edge and sits just under
		// it, so a long name grows rightwards.
		gInterface.DrawFormat(eYellow, (int)(m_Pos.x + PG_MAPBAR_OX + 1.f),
			(int)(m_Pos.y + PG_MAPBAR_OY + PG_MAPBAR_H + 3.f), 0, RT3_SORT_LEFT,
			"%s", szText);
	}

	DisableAlphaBlend();

	DrawMiniMap();

	// Last, so it lands over the minimap and the rest of the 2D pass.
	PgMapBanner();

	return true;
}
#else
bool CNewUIHeroPositionInfo::Render()
{
	ShowInfoTitleWindow();
	MiniMapLoad();
	unicode::t_char szText[256] = { NULL, };
	unicode::t_char szLevel[55] = { NULL, };
	unicode::t_char szReset[55] = { NULL, };
	unicode::t_char szAutoReset[55] = { NULL, };
	SetBlockCur(false);
	float TableX = (IndexIMGMap == -1) ? 20 : 110;
	float TableY = 25;
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	AutoMove();

	if (ShowMiniMapColtrol)
	{
		BDrawOutLine(TableX, TableY, 140, 30);
		DisableAlphaBlend();
		BRenderTabLine(TableX + 5, TableY + 17, 131.2 / 2, 18, 2, -1);
		EndRenderColor();

		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, 1.f);
		g_pRenderText->SetFont(g_hFontBold);

		g_pRenderText->SetBgColor(0);

		g_pRenderText->SetShadowText(2);

		float CaclcX = 131.2 / 2;

		g_pRenderText->SetTextColor(255, 255, 255, 255);
		time_t TimeServer;
		struct tm* ServerT;
		time(&TimeServer);

		ServerT = gmtime(&TimeServer);

		char ServerTime[30];

		unicode::_sprintf(szText, "Time : %2d:%02d:%02d", (ServerT->tm_hour + GMT) % 24, ServerT->tm_min, ServerT->tm_sec);
		g_pRenderText->RenderText(TableX, TableY + 7, szText, 140, 13 - 4, RT3_SORT_CENTER);

		g_pRenderText->SetTextColor(255, 174, 0, 255);
		unicode::_sprintf(szLevel, "Lv %d", CharacterAttribute->Level);
		g_pRenderText->RenderText(TableX + 5, TableY + 21, szLevel, CaclcX, 13 - 4, RT3_SORT_CENTER);

		g_pRenderText->SetTextColor(0, 255, 238, 255);
		unicode::_sprintf(szReset, "Rs %d", CharacterAttribute->PrintPlayer.ViewReset);
		g_pRenderText->RenderText(TableX + 5 + CaclcX, TableY + 21, szReset, CaclcX, 13 - 4, RT3_SORT_CENTER);

		JGetButtonAutoReset(TableX + 5, TableY + 38, 18.0);
		g_pRenderText->SetTextColor(55, 55, 55, 255);
		unicode::_sprintf(szAutoReset, "[Auto RS Not Active]");
		g_pRenderText->RenderText(TableX + 5, TableY + 40, szAutoReset, 132, 13 - 4, RT3_SORT_CENTER);

		g_pRenderText->SetShadowText(0);
		DisableAlphaBlend();
	}

	DrawMiniMap();
	return true;
}
#endif
float CNewUIHeroPositionInfo::GetLayerDepth()
{
	return 0.1f;
}

void CNewUIHeroPositionInfo::OpenningProcess()
{

}

void CNewUIHeroPositionInfo::ClosingProcess()
{

}

void CNewUIHeroPositionInfo::SetCurHeroPosition(int x, int y)
{
	m_CurHeroPosition.x = x;
	m_CurHeroPosition.y = y;
}

void CNewUIHeroPositionInfo::LoadImages()
{
#if(!UseNewMacroUI)

	LoadBitmap("Interface\\Minimap_positionA.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW, GL_LINEAR);
	// The plate the map line sits on - see the draw in Render.
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\TournamentUI_I9.tga", BITMAP_PG_MAPBAR, GL_LINEAR);
	LoadBitmap("Interface\\Minimap_positionB.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 1, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\Minimap_positionC.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 2, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Setup.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 3, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Start.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4, GL_LINEAR);
	LoadBitmap("Interface\\MacroUI\\MacroUI_Stop.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 5, GL_LINEAR);
#else
	LoadBitmap("Custom\\Maps\\CRadar.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 2, GL_LINEAR);
	LoadBitmap("Custom\\Maps\\map_radar.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4, GL_LINEAR);
	LoadBitmap("Custom\\Maps\\background-tcp.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 5, GL_LINEAR);
#endif

}

void CNewUIHeroPositionInfo::UnloadImages()
{
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 1);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 2);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 3);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 4);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW + 5);
}

void CNewUIHeroPositionInfo::SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos)
{
	m_Btn->ChangeButtonImgState(1, imgindex, overflg, isimgwidth, bClickEffect);
	m_Btn->ChangeButtonInfo(x, y, sx, sy);

	m_Btn->ChangeText(btname);
	m_Btn->ChangeToolTipText(tooltiptext, istoppos);

	if (MoveTxt)
	{
		m_Btn->MoveTextPos(0, -1);
	}
}

void CNewUIHeroPositionInfo::MoveTextTipPos(CNewUIButton* m_Btn, int iX, int iY)
{
	m_Btn->MoveTextTipPos(iX, iY);
}