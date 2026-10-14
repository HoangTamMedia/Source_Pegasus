
#include "stdafx.h"
#include "NewUICharacterInfoWindow.h"
#include "NewUISystem.h"
#include "CharacterManager.h"
#include "CSItemOption.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "UIControls.h"
#include "ZzzInterface.h"
#include "ZzzScene.h"
#include "ZzzInventory.h"
#include "SkillManager.h"
#include "UIJewelHarmony.h"
#include "UIManager.h"
#include "wsclientinline.h"
#include "ServerListManager.h"
#include "CustomMessage.h"
#include "CBInterface.h"

extern int DisplayWin;
extern int DisplayHeight;
extern int MouseX;
extern int MouseY;

using namespace SEASON3B;

SEASON3B::CNewUICharacterInfoWindow::CNewUICharacterInfoWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_iNumCurOpenTab = TAB_POINT;
}

SEASON3B::CNewUICharacterInfoWindow::~CNewUICharacterInfoWindow()
{
	Release();
}

bool SEASON3B::CNewUICharacterInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHARACTER, this);

	LoadImages();

	// Built once - see the note in SetButtonInfo.
	m_TabBtn.CreateRadioGroup(PG_TAB_COUNT, IMAGE_CHAINFO_PG_TAB);

	std::list<unicode::t_string> TabText;
	TabText.push_back("Character");
	TabText.push_back("Advance");
	TabText.push_back("Fruit");
	m_TabBtn.ChangeRadioText(TabText);
	m_TabBtn.SetFont(g_hFont);

	SetPos(x, y);

	SetButtonInfo();

	Show(false);

	return true;
}

void SEASON3B::CNewUICharacterInfoWindow::SetButtonInfo()
{
	unicode::t_char strText[256];

	static const int s_anStatRowY[BTN_STAT_COUNT] =
	{
		HEIGHT_STRENGTH, HEIGHT_DEXTERITY, HEIGHT_VITALITY, HEIGHT_ENERGY, HEIGHT_CHARISMA
	};

	// pg_chainfo_plus is a 3-state vertical strip (blue / blue / grey).
	for (int i = 0; i < BTN_STAT_COUNT; ++i)
	{
		m_BtnStat[i].ChangeButtonImgState(true, IMAGE_CHAINFO_PG_PLUS, true);
		m_BtnStat[i].ChangeButtonInfo(m_Pos.x + PG_PLUS_X, m_Pos.y + s_anStatRowY[i] - 1,
			PG_PLUS_W, PG_PLUS_H);
	}

	// Only reposition here. CreateRadioGroup appends to the list instead of
	// resetting it, and SetButtonInfo runs on every SetPos (including every
	// frame of a drag), so creating tabs here breeds them across the screen.
	m_TabBtn.ChangeRadioButtonInfo(true, m_Pos.x + PG_TAB_X, m_Pos.y + PG_TAB_Y,
		PG_TAB_W, PG_TAB_H, PG_TAB_GAP);
	m_TabBtn.ChangeFrame(m_iNumCurOpenTab);

	unicode::_sprintf(strText, GlobalText[927], "C");

	m_BtnClose.ChangeButtonImgState(true, IMAGE_CHAINFO_PG_CLOSE, true);
	m_BtnClose.ChangeButtonInfo(m_Pos.x + PG_CLOSE_X, m_Pos.y + PG_CLOSE_Y, PG_CLOSE_W, PG_CLOSE_H);
	m_BtnClose.ChangeToolTipText(strText, true);

	// Bottom action row: exit, quest, pet, master level.
	const int iBottomStep = PG_BOTTOM_W + PG_BOTTOM_GAP;
	int iBottomX = m_Pos.x + PG_BOTTOM_X;

	m_BtnExit.ChangeButtonImgState(true, IMAGE_CHAINFO_PG_BTN_EXIT, false);
	m_BtnExit.ChangeButtonInfo(iBottomX, m_Pos.y + PG_BOTTOM_Y, PG_BOTTOM_W, PG_BOTTOM_H);
	m_BtnExit.ChangeToolTipText(strText, true);

	m_BtnQuest.ChangeButtonImgState(true, IMAGE_CHAINFO_PG_BTN_QUEST, false);
	m_BtnQuest.ChangeButtonInfo(iBottomX + iBottomStep, m_Pos.y + PG_BOTTOM_Y,
		PG_BOTTOM_W, PG_BOTTOM_H);
	unicode::_sprintf(strText, "%s(%s)", GlobalText[1140], "T");
	m_BtnQuest.ChangeToolTipText(strText, true);

	m_BtnPet.ChangeButtonImgState(true, IMAGE_CHAINFO_PG_BTN_PET, false);
	m_BtnPet.ChangeButtonInfo(iBottomX + iBottomStep * 2, m_Pos.y + PG_BOTTOM_Y,
		PG_BOTTOM_W, PG_BOTTOM_H);
	m_BtnPet.ChangeToolTipText(GlobalText[1217], true);
#if (OnOffMasterSkill)
	m_BtnMasterLevel.ChangeButtonImgState(true, IMAGE_CHAINFO_PG_BTN_MASTER, false);
	m_BtnMasterLevel.ChangeButtonInfo(iBottomX + iBottomStep * 3, m_Pos.y + PG_BOTTOM_Y,
		PG_BOTTOM_W, PG_BOTTOM_H);
	m_BtnMasterLevel.ChangeToolTipText(GlobalText[1749], true);
#endif
}

void SEASON3B::CNewUICharacterInfoWindow::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUICharacterInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	SetButtonInfo();
}

bool SEASON3B::CNewUICharacterInfoWindow::UpdateMouseEvent()
{

	if (IsVisible())
	{
		static bool isDraggingCharInfo = false;
		static int dragStartX = 0;
		static int dragStartY = 0;

		const int TITLE_BAR_HEIGHT = 64;

		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{

			if (SEASON3B::CheckMouseIn(m_Pos.x, m_Pos.y, CHAINFO_WINDOW_WIDTH, TITLE_BAR_HEIGHT))
			{

				bool canDrag = true;

				if (SEASON3B::CheckMouseIn(m_Pos.x + PG_CLOSE_X, m_Pos.y + PG_CLOSE_Y,
					PG_CLOSE_W, PG_CLOSE_H))
				{
					canDrag = false;
				}

				// The tab row sits inside the title band, so it must not drag.
				if (SEASON3B::CheckMouseIn(m_Pos.x + PG_TAB_X, m_Pos.y + PG_TAB_Y,
					PG_TAB_COUNT * (PG_TAB_W + PG_TAB_GAP), PG_TAB_H))
				{
					canDrag = false;
				}

				if (canDrag && !isDraggingCharInfo)
				{
					isDraggingCharInfo = true;

					dragStartX = MouseX - m_Pos.x;
					dragStartY = MouseY - m_Pos.y;
				}
			}

			if (isDraggingCharInfo)
			{

				int newX = MouseX - dragStartX;
				int newY = MouseY - dragStartY;

				extern int DisplayWin;
				extern int DisplayHeight;

				if (newX < 0) newX = 0;
				if (newY < 0) newY = 0;
				if (newX + CHAINFO_WINDOW_WIDTH > DisplayWin) newX = DisplayWin - CHAINFO_WINDOW_WIDTH;
				if (newY + CHAINFO_WINDOW_HEIGHT > DisplayHeight) newY = DisplayHeight - CHAINFO_WINDOW_HEIGHT;

				SetPos(newX, newY);

				return false;
			}
		}
		else
		{

			if (isDraggingCharInfo)
			{
				isDraggingCharInfo = false;
			}
		}
	}

	if (BtnProcess() == true)
	{
		return false;
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, CHAINFO_WINDOW_WIDTH, CHAINFO_WINDOW_HEIGHT))
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUICharacterInfoWindow::BtnProcess()
{
	int iTab = m_TabBtn.UpdateMouseEvent();
	if (iTab != RADIOGROUPEVENT_NONE)
	{
		m_iNumCurOpenTab = iTab;
		return true;
	}

	if (CharacterAttribute->PrintPlayer.ViewPoint > 0)
	{
		int iBaseClass = gCharacterManager.GetBaseClass(Hero->Class);
		int iCount = 0;
		if (iBaseClass == CLASS_DARK_LORD)
		{
			iCount = 5;
		}
		else
		{
			iCount = 4;
		}
		for (int i = 0; i < iCount; ++i)
		{
			if (m_BtnStat[i].UpdateMouseEvent() == true)
			{
				SendRequestAddPoint(i);
				return true;
			}
		}
	}

	if (m_BtnClose.UpdateMouseEvent() == true || m_BtnExit.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHARACTER);
		return true;
	}

	if (m_BtnQuest.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MYQUEST);
		return true;
	}

	if (m_BtnPet.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_PET);
		return true;
	}
#if (OnOffMasterSkill)
	if (m_BtnMasterLevel.UpdateMouseEvent() == true)
	{
		if (gCharacterManager.IsMasterLevel(Hero->Class) == true

#ifdef PBG_ADD_NEWCHAR_MONK
			|| gCharacterManager.GetCharacterClass(Hero->Class) == CLASS_TEMPLENIGHT
#endif
			)
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MASTER_LEVEL);
		return true;
	}
#endif
	return false;
}

bool SEASON3B::CNewUICharacterInfoWindow::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHARACTER);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUICharacterInfoWindow::Update()
{
	return true;
}

// SEASON3B::RenderImage treats the draw size as the SOURCE rect - drawing a
// 100x100 image at 39x39 crops its top-left corner instead of scaling it down.
// This maps the whole source into the destination rect, which is what every
// call in this window actually wants.
static void RenderImageScaled(GLuint uiImage, float x, float y, float fW, float fH,
	float fSrcW, float fSrcH)
{
	BITMAP_t* pImage = Bitmaps.FindTexture(uiImage);
	if (pImage == NULL || pImage->Width <= 0.f || pImage->Height <= 0.f)
		return;

	float u = 0.5f / pImage->Width;
	float v = 0.5f / pImage->Height;
	float uw = (fSrcW - 0.5f) / pImage->Width;
	float vh = (fSrcH - 0.5f) / pImage->Height;

	RenderBitmap(uiImage, x, y, fW, fH, u, v, uw - u, vh - v);
}

void SEASON3B::CNewUICharacterInfoWindow::RenderFrame()
{
	// One-piece Pegasus panel; nothing to tile.
	RenderImageScaled(IMAGE_CHAINFO_PG_PANEL, (float)m_Pos.x, (float)m_Pos.y,
		(float)CHAINFO_WINDOW_WIDTH, (float)CHAINFO_WINDOW_HEIGHT, 324.f, 638.f);

	// Black field box behind level / reset / relife reset.
	RenderImageScaled(IMAGE_CHAINFO_PG_BOX, (float)(m_Pos.x + PG_BOX_INFO_X),
		(float)(m_Pos.y + PG_BOX_INFO_Y), (float)PG_BOX_INFO_W, (float)PG_BOX_INFO_H, 110.f, 40.f);

	// Ring first, portrait into its opening. The opening in I10 is filled with
	// solid dark brown, not transparent, so the portrait has to go on top.
	RenderImageScaled(IMAGE_CHAINFO_PG_FRAME, (float)(m_Pos.x + PG_FRAME_X),
		(float)(m_Pos.y + PG_FRAME_Y), (float)PG_FRAME_W, (float)PG_FRAME_H, 359.f, 263.f);
	RenderImageScaled(IMAGE_CHAINFO_PG_PORTRAIT, (float)(m_Pos.x + PG_PORTRAIT_X),
		(float)(m_Pos.y + PG_PORTRAIT_Y), (float)PG_PORTRAIT_SIZE, (float)PG_PORTRAIT_SIZE,
		100.f, 100.f);

	// Three money cells: black box, icon on the left, value drawn in RenderTexts.
	static const struct { int iImage; float fSrcW, fSrcH; } s_aMoneyIcon[3] =
	{
		{ IMAGE_CHAINFO_PG_ICON_ZEN,   74.f, 78.f },
		{ IMAGE_CHAINFO_PG_ICON_POINT, 72.f, 73.f },
		{ IMAGE_CHAINFO_PG_ICON_MONTH, 75.f, 82.f },
	};

	for (int i = 0; i < 3; ++i)
	{
		int iBoxX = m_Pos.x + PG_BOX_MONEY_X + i * (PG_BOX_MONEY_W + PG_BOX_MONEY_GAP);

		RenderImageScaled(IMAGE_CHAINFO_PG_BOX, (float)iBoxX, (float)(m_Pos.y + PG_BOX_MONEY_Y),
			(float)PG_BOX_MONEY_W, (float)PG_BOX_MONEY_H, 110.f, 40.f);

		RenderImageScaled(s_aMoneyIcon[i].iImage, (float)(iBoxX + 5),
			(float)(m_Pos.y + PG_BOX_MONEY_Y + 6), (float)PG_ICON_SIZE, (float)PG_ICON_SIZE,
			s_aMoneyIcon[i].fSrcW, s_aMoneyIcon[i].fSrcH);
	}

	// Stat rows. TournamentUI_I7 already contains the blue label plate and the
	// dark value box side by side, so one draw covers both.
	static const int s_anStatRowY[BTN_STAT_COUNT] =
	{
		HEIGHT_STRENGTH, HEIGHT_DEXTERITY, HEIGHT_VITALITY, HEIGHT_ENERGY, HEIGHT_CHARISMA
	};

	int iRowCount = (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_DARK_LORD)
		? BTN_STAT_COUNT : BTN_STAT_COUNT - 1;

	for (int i = 0; i < iRowCount; ++i)
	{
		RenderImageScaled(IMAGE_CHAINFO_PG_STATROW, (float)(m_Pos.x + PG_ROW_X),
			(float)(m_Pos.y + s_anStatRowY[i]), (float)PG_ROW_W, (float)PG_ROW_H, 374.f, 46.f);
	}
}

bool SEASON3B::CNewUICharacterInfoWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderTexts();
	RenderButtons();
	DisableAlphaBlend();
	return true;
}

void SEASON3B::CNewUICharacterInfoWindow::RenderTexts()
{
	RenderSubjectTexts();
	RenderTableTexts();
	RenderAttribute();
}

void SEASON3B::CNewUICharacterInfoWindow::RenderSubjectTexts()
{
	char strID[256];
	sprintf(strID, "%s", CharacterAttribute->Name);
	unicode::t_char strClassName[256];
	unicode::_sprintf(strClassName, "(%s)", gCharacterManager.GetCharacterClassText(CharacterAttribute->Class));

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(20, 20, 20, 20);
	SetPlayerColor(Hero->PK);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 12, strID, CHAINFO_WINDOW_WIDTH, 0, RT3_SORT_CENTER);

	unicode::t_char strServerName[MAX_TEXT_LENGTH];

	const char* apszGlobalText[4]
		= { GlobalText[461], GlobalText[460], GlobalText[3130], GlobalText[3131] };
	sprintf(strServerName, apszGlobalText[g_ServerListManager->GetNonPVPInfo()],
		g_ServerListManager->GetSelectServerName(), g_ServerListManager->GetSelectServerIndex());

	float fAlpha = sinf(WorldTime * 0.001f) + 1.f;
	g_pRenderText->SetTextColor(255, 255, 255, 127 * (2.f - fAlpha));
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 27, strClassName, CHAINFO_WINDOW_WIDTH, 0, RT3_SORT_CENTER);
	g_pRenderText->SetTextColor(255, 255, 255, 127 * fAlpha);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 27, strServerName, CHAINFO_WINDOW_WIDTH, 0, RT3_SORT_CENTER);
}

void SEASON3B::CNewUICharacterInfoWindow::RenderTableTexts()
{
	unicode::t_char strLevel[128];
	unicode::t_char strExtra[32];
	unicode::t_char strExp[128];
	unicode::t_char strPoint[128];

	{

		unicode::_sprintf(strLevel, gCustomMessage.GetMessageA(0), CharacterAttribute->Level, CharacterAttribute->PrintPlayer.ViewReset, CharacterAttribute->PrintPlayer.ViewMasterReset);

		unicode::_sprintf(strExp, GlobalText[201], CharacterAttribute->Experience, CharacterAttribute->NextExperince);
	}
	if (g_pBCustomMenuInfo->DrawButtonGUI(BITMAP_INTERFACE_EX + 20, m_Pos.x + PG_HELP_X, m_Pos.y + PG_HELP_Y, 15, 15, 1))
	{
		if (GetForegroundWindow() == g_hWnd && !gInterface.CheckWindow(Interface::ObjWindow::ChatWindow) && !gInterface.CheckWindow(Interface::ObjWindow::Guild))
		{

			if (GetTickCount() < gInterface.Data[eWindowCongDiem].EventTick + 300) return;
			gInterface.Data[eWindowCongDiem].EventTick = GetTickCount();

			gInterface.Data[eWindowCongDiem].OnShow ^= 1;
		}
	}

	if (CharacterAttribute->Level > 9)
	{
		int iMinus, iMaxMinus;
		if (CharacterAttribute->wMinusPoint == 0)
		{
			iMinus = 0;
		}
		else
		{
			iMinus = -CharacterAttribute->wMinusPoint;
		}

		iMaxMinus = -CharacterAttribute->wMaxMinusPoint;

		unicode::_sprintf(strPoint, "%s %d/%d | %s %d/%d",
			GlobalText[1412], CharacterAttribute->AddPoint, CharacterAttribute->MaxAddPoint,
			GlobalText[1903], iMinus, iMaxMinus);
	}
	else
	{
		wsprintf(strPoint, "%s %d/%d | %s %d/%d", GlobalText[1412], 0, 0, GlobalText[1903], 0, 0);
	}

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(230, 230, 0, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->RenderText(m_Pos.x + PG_INFO_X, m_Pos.y + PG_INFO_LEVEL_Y, strLevel);

	if (CharacterAttribute->PrintPlayer.ViewPoint > 0)
	{
		unicode::t_char strLevelUpPoint[128];

		if (gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == false || CharacterAttribute->PrintPlayer.ViewPoint > 0)
		{
			unicode::_sprintf(strLevelUpPoint, GlobalText[217], CharacterAttribute->PrintPlayer.ViewPoint);
		}
		else
			unicode::_sprintf(strLevelUpPoint, "");
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetTextColor(255, 138, 0, 255);
		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->RenderText(m_Pos.x + PG_FREEPOINT_X, m_Pos.y + PG_FREEPOINT_Y, strLevelUpPoint);
	}

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->RenderText(m_Pos.x + PG_INFO_X, m_Pos.y + PG_INFO_RESET_Y, strExp);

	int iAddPoint, iMinusPoint;

	if (CharacterAttribute->AddPoint <= 10)
	{
		iAddPoint = 100;
	}
	else
	{
		int iTemp = 0;
		if (CharacterAttribute->MaxAddPoint != 0)
		{
			iTemp = (CharacterAttribute->AddPoint * 100) / CharacterAttribute->MaxAddPoint;
		}
		if (iTemp <= 10)
		{
			iAddPoint = 70;
		}
		else if (iTemp > 10 && iTemp <= 30)
		{
			iAddPoint = 60;
		}
		else if (iTemp > 30 && iTemp <= 50)
		{
			iAddPoint = 50;
		}
		else if (iTemp > 50)
		{
			iAddPoint = 40;
		}
	}

	if (CharacterAttribute->wMinusPoint <= 10)
	{
		iMinusPoint = 100;
	}
	else
	{
		int iTemp = 0;
		if (CharacterAttribute->wMaxMinusPoint != 0)
		{
			iTemp = (CharacterAttribute->wMinusPoint * 100) / CharacterAttribute->wMaxMinusPoint;
		}

		if (iTemp <= 10)
		{
			iMinusPoint = 70;
		}
		else if (iTemp > 10 && iTemp <= 30)
		{
			iMinusPoint = 60;
		}
		else if (iTemp > 30 && iTemp <= 50)
		{
			iMinusPoint = 50;
		}
		else if (iTemp > 50)
		{
			iMinusPoint = 40;
		}
	}

	unicode::t_char strPointProbability[128];
	unicode::_sprintf(strPointProbability, GlobalText[1907], iAddPoint, iMinusPoint);
	// The three point figures live inside the money cells, to the right of each
	// icon. Drawn here rather than as their own lines - the band between the
	// cells and the first stat plate only has room for the free-point line.
	unicode::t_char strCell[3][64];

	unicode::_sprintf(strCell[0], "%d/%d",
		CharacterAttribute->AddPoint, CharacterAttribute->MaxAddPoint);
	unicode::_sprintf(strCell[1], "%d/%d",
		(CharacterAttribute->wMinusPoint == 0) ? 0 : -CharacterAttribute->wMinusPoint,
		-CharacterAttribute->wMaxMinusPoint);
	unicode::_sprintf(strCell[2], "%d%%/%d%%", iAddPoint, iMinusPoint);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(230, 190, 90, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	for (int i = 0; i < 3; ++i)
	{
		int iTextX = m_Pos.x + PG_BOX_MONEY_X + i * (PG_BOX_MONEY_W + PG_BOX_MONEY_GAP)
			+ PG_ICON_SIZE + 8;

		g_pRenderText->RenderText(iTextX, m_Pos.y + PG_BOX_MONEY_Y + 14, strCell[i],
			PG_BOX_MONEY_W - PG_ICON_SIZE - 12, 0, RT3_SORT_CENTER);
	}
}

void SEASON3B::CNewUICharacterInfoWindow::RenderAttribute()
{
	g_pRenderText->SetFont(g_hFontBold);

	DWORD wStrength;

	wStrength = CharacterAttribute->PrintPlayer.ViewStrength + CharacterAttribute->AddStrength;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion1))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
		if (CharacterAttribute->AddStrength)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(230, 230, 0, 255);
		}

	unicode::t_char strStrength[32];
	unicode::_sprintf(strStrength, "%d", wStrength);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X, m_Pos.y + HEIGHT_STRENGTH + 7, GlobalText[166], PG_ROW_LABEL_W, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X + PG_ROW_VALUE_X, m_Pos.y + HEIGHT_STRENGTH + 7, strStrength, PG_ROW_VALUE_W, 0, RT3_SORT_CENTER);

	unicode::t_char strAttakMamage[256];
	int iAttackDamageMin = 0;
	int iAttackDamageMax = 0;

	bool bAttackDamage = GetAttackDamage(&iAttackDamageMin, &iAttackDamageMax);

	int Add_Dex = 0;
	int Add_Rat = 0;
	int Add_Dfe = 0;
	int Add_Att_Max = 0;
	int Add_Att_Min = 0;
	int Add_Dfe_Rat = 0;
	int Add_Ch_Dfe = 0;
	int Add_Mana_Max = 0;
	int Add_Mana_Min = 0;

	ITEM* pWeaponRight = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT];
	ITEM* pWeaponLeft = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];

	int iAttackRating = CharacterAttribute->AttackRating + Add_Rat;
	int iAttackRatingPK = CharacterAttribute->AttackRatingPK + Add_Dex;
	iAttackDamageMax += Add_Att_Max;
	iAttackDamageMin += Add_Att_Min;

	if (g_isCharacterBuff((&Hero->Object), eBuff_AddAG))
	{
		DWORD wDexterity = CharacterAttribute->PrintPlayer.ViewDexterity + CharacterAttribute->AddDexterity;
		iAttackRating += wDexterity;
		iAttackRatingPK += wDexterity;
		if (PartyNumber >= 3)
		{
			int iPlusRating = (wDexterity * ((PartyNumber - 2) * 0.01f));
			iAttackRating += iPlusRating;
			iAttackRatingPK = iPlusRating;
		}
	}
	if (g_isCharacterBuff((&Hero->Object), eBuff_HelpNpc))
	{
		int iTemp = 0;
		if (CharacterAttribute->Level > 180)
		{
			iTemp = (180 / 3) + 45;
		}
		else
		{
			iTemp = (CharacterAttribute->Level / 3) + 45;
		}

		iAttackDamageMin += iTemp;
		iAttackDamageMax += iTemp;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
	{
		int nTemp = CharacterAttribute->PrintPlayer.ViewStrength + CharacterAttribute->AddStrength
			+ CharacterAttribute->PrintPlayer.ViewDexterity + CharacterAttribute->AddDexterity;
		float fTemp = int(CharacterAttribute->PrintPlayer.ViewEnergy / 30) / 100.f;
		iAttackDamageMin += nTemp / 7 * fTemp;
		iAttackDamageMax += nTemp / 4 * fTemp;
	}

	int iMinIndex = 0, iMaxIndex = 0, iMagicalIndex = 0;

	StrengthenCapability SC_r, SC_l;

	int rlevel = (pWeaponRight->Level >> 3) & 15;

	if (rlevel >= pWeaponRight->Jewel_Of_Harmony_OptionLevel)
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability(&SC_r, pWeaponRight, 1);

		if (SC_r.SI_isSP)
		{
			iMinIndex = SC_r.SI_SP.SI_minattackpower;
			iMaxIndex = SC_r.SI_SP.SI_maxattackpower;
			iMagicalIndex = SC_r.SI_SP.SI_magicalpower;
		}
	}

	int llevel = (pWeaponLeft->Level >> 3) & 15;

	if (llevel >= pWeaponLeft->Jewel_Of_Harmony_OptionLevel)
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability(&SC_l, pWeaponLeft, 1);

		if (SC_l.SI_isSP)
		{
			iMinIndex += SC_l.SI_SP.SI_minattackpower;
			iMaxIndex += SC_l.SI_SP.SI_maxattackpower;
			iMagicalIndex += SC_l.SI_SP.SI_magicalpower;
		}
	}

	int iDefenseRate = 0, iAttackPowerRate = 0;

	StrengthenCapability rightinfo, leftinfo;

	int iRightLevel = (pWeaponRight->Level >> 3) & 15;

	if (iRightLevel >= pWeaponRight->Jewel_Of_Harmony_OptionLevel)
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability(&rightinfo, pWeaponRight, 1);
	}

	int iLeftLevel = (pWeaponLeft->Level >> 3) & 15;

	if (iLeftLevel >= pWeaponLeft->Jewel_Of_Harmony_OptionLevel)
	{
		g_pUIJewelHarmonyinfo->GetStrengthenCapability(&leftinfo, pWeaponLeft, 1);
	}

	if (rightinfo.SI_isSP)
	{
		iAttackPowerRate += rightinfo.SI_SP.SI_attackpowerRate;
	}
	if (leftinfo.SI_isSP)
	{
		iAttackPowerRate += leftinfo.SI_SP.SI_attackpowerRate;
	}

	for (int k = EQUIPMENT_WEAPON_LEFT; k < MAX_EQUIPMENT; ++k)
	{
		StrengthenCapability defenseinfo;

		ITEM* pItem = &CharacterMachine->Equipment[k];

		int eqlevel = (pItem->Level >> 3) & 15;

		if (eqlevel >= pItem->Jewel_Of_Harmony_OptionLevel)
		{
			g_pUIJewelHarmonyinfo->GetStrengthenCapability(&defenseinfo, pItem, 2);
		}

		if (defenseinfo.SI_isSD)
		{
			iDefenseRate += defenseinfo.SI_SD.SI_defenseRate;
		}
	}

	int itemoption380Attack = 0;
	int itemoption380Defense = 0;

	for (int j = 0; j < MAX_EQUIPMENT; ++j)
	{
		bool is380item = CharacterMachine->Equipment[j].option_380;
		int i380type = CharacterMachine->Equipment[j].Type;

		if (is380item && i380type > -1 && i380type < MAX_ITEM)
		{
			ITEM_ADD_OPTION item380option;

			item380option = g_pItemAddOptioninfo->GetItemAddOtioninfo(CharacterMachine->Equipment[j].Type);

			if (item380option.m_byOption1 == 1)
			{
				itemoption380Attack += item380option.m_byValue1;
			}

			if (item380option.m_byOption2 == 1)
			{
				itemoption380Attack += item380option.m_byValue2;
			}

			if (item380option.m_byOption1 == 3)
			{
				itemoption380Defense += item380option.m_byValue1;
			}

			if (item380option.m_byOption2 == 3)
			{
				itemoption380Defense += item380option.m_byValue2;
			}
		}
	}

	ITEM* pItemRingLeft, * pItemRingRight;

	pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];
	if (pItemRingLeft && pItemRingRight)
	{
		int iNonExpiredLRingType = -1;
		int iNonExpiredRRingType = -1;
		if (!pItemRingLeft->bPeriodItem || !pItemRingLeft->bExpiredPeriod)
		{
			iNonExpiredLRingType = pItemRingLeft->Type;
		}
		if (!pItemRingRight->bPeriodItem || !pItemRingRight->bExpiredPeriod)
		{
			iNonExpiredRRingType = pItemRingRight->Type;
		}

		int maxIAttackDamageMin = 0;
		int maxIAttackDamageMax = 0;
		if (iNonExpiredLRingType == ITEM_HELPER + 41 || iNonExpiredRRingType == ITEM_HELPER + 41)
		{
			maxIAttackDamageMin = max(maxIAttackDamageMin, 20);
			maxIAttackDamageMax = max(maxIAttackDamageMax, 20);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxIAttackDamageMin = max(maxIAttackDamageMin, 30);
			maxIAttackDamageMax = max(maxIAttackDamageMax, 30);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxIAttackDamageMin = max(maxIAttackDamageMin, 40);
			maxIAttackDamageMax = max(maxIAttackDamageMax, 40);
		}

		iAttackDamageMin += maxIAttackDamageMin;
		iAttackDamageMax += maxIAttackDamageMax;
	}

	ITEM* pItemHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
	if (pItemHelper)
	{
		if (pItemHelper->Type == ITEM_HELPER + 37 && pItemHelper->Option1 == 0x04)
		{
			WORD wLevel = CharacterAttribute->Level;
			iAttackDamageMin += (wLevel / 12);
			iAttackDamageMax += (wLevel / 12);
		}
		if (pItemHelper->Type == ITEM_HELPER + 64)
		{
			if (false == pItemHelper->bExpiredPeriod)
			{
				iAttackDamageMin += int(float(iAttackDamageMin) * 0.4f);
				iAttackDamageMax += int(float(iAttackDamageMax) * 0.4f);
			}
		}
		if (pItemHelper->Type == ITEM_HELPER + 123)
		{
			if (false == pItemHelper->bExpiredPeriod)
			{
				iAttackDamageMin += int(float(iAttackDamageMin) * 0.2f);
				iAttackDamageMax += int(float(iAttackDamageMax) * 0.2f);
			}
		}
		if (pItemHelper->Type == ITEM_HELPER + 1)
		{
			iAttackDamageMin += int(float(iAttackDamageMin) * 0.3f);
			iAttackDamageMax += int(float(iAttackDamageMax) * 0.3f);
		}
	}

	if (iAttackRating > 0)
	{
		if (iAttackDamageMin + iMinIndex >= iAttackDamageMax + iMaxIndex)
		{
			unicode::_sprintf(strAttakMamage, GlobalText[203], iAttackDamageMax + iMaxIndex, iAttackDamageMax + iMaxIndex, iAttackRating);
		}
		else
		{
			unicode::_sprintf(strAttakMamage, GlobalText[203], iAttackDamageMin + iMinIndex, iAttackDamageMax + iMaxIndex, iAttackRating);
		}
	}
	else
	{
		if (iAttackDamageMin + iMinIndex >= iAttackDamageMax + iMaxIndex)
		{
			unicode::_sprintf(strAttakMamage, GlobalText[204], iAttackDamageMax + iMaxIndex, iAttackDamageMax + iMaxIndex);
		}
		else
		{
			unicode::_sprintf(strAttakMamage, GlobalText[204], iAttackDamageMin + iMinIndex, iAttackDamageMax + iMaxIndex);
		}
	}

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	if (bAttackDamage)
	{
		g_pRenderText->SetTextColor(100, 150, 255, 255);
	}
	else
	{
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll3))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	int iY = HEIGHT_STRENGTH + 25;
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + HEIGHT_STRENGTH + 25, strAttakMamage);

	if (iAttackRatingPK > 0)
	{
		if (itemoption380Attack != 0 || iAttackPowerRate != 0)
		{
			unicode::_sprintf(strAttakMamage, GlobalText[2109], iAttackRatingPK, itemoption380Attack + iAttackPowerRate);
		}
		else
		{
			unicode::_sprintf(strAttakMamage, GlobalText[2044], iAttackRatingPK);
		}

		iY += 13;
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strAttakMamage);
	}

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion2))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
		if (CharacterAttribute->AddDexterity)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(230, 230, 0, 255);
		}

	unicode::t_char strDexterity[32];
	DWORD wDexterity = CharacterAttribute->PrintPlayer.ViewDexterity + CharacterAttribute->AddDexterity;
	unicode::_sprintf(strDexterity, "%d", wDexterity);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X, m_Pos.y + HEIGHT_DEXTERITY + 7, GlobalText[1702], PG_ROW_LABEL_W, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X + PG_ROW_VALUE_X, m_Pos.y + HEIGHT_DEXTERITY + 7, strDexterity, PG_ROW_VALUE_W, 0, RT3_SORT_CENTER);

	bool bDexSuccess = true;
	int iBaseClass = gCharacterManager.GetBaseClass(Hero->Class);

	for (int i = EQUIPMENT_HELM; i <= EQUIPMENT_BOOTS; ++i)
	{
		if (iBaseClass == CLASS_DARK)
		{
			if ((CharacterMachine->Equipment[i].Type == -1 && (i != EQUIPMENT_HELM && iBaseClass == CLASS_DARK))
				|| (CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability <= 0))
			{
				bDexSuccess = false;
				break;
			}
		}
#ifdef PBG_ADD_NEWCHAR_MONK
		else if (iBaseClass == CLASS_RAGEFIGHTER)
		{
			if ((CharacterMachine->Equipment[i].Type == -1 && (i != EQUIPMENT_GLOVES && iBaseClass == CLASS_RAGEFIGHTER))
				|| (CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability <= 0))
			{
				bDexSuccess = false;
				break;
			}
		}
#endif
		else
		{
			if ((CharacterMachine->Equipment[i].Type == -1) ||
				(CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability <= 0))
			{
				bDexSuccess = false;
				break;
			}
		}
	}

	if (bDexSuccess)
	{
		int iType;
		if (iBaseClass == CLASS_DARK)
		{

			iType = CharacterMachine->Equipment[EQUIPMENT_ARMOR].Type;

			if (
				(iType != ITEM_ARMOR + 15)
				&& (iType != ITEM_ARMOR + 20)
				&& (iType != ITEM_ARMOR + 23)
				&& (iType != ITEM_ARMOR + 32)
				&& (iType != ITEM_ARMOR + 37)
				&& (iType != ITEM_ARMOR + 47)
				&& (iType != ITEM_ARMOR + 48)
				)
			{
				bDexSuccess = false;
			}

			iType = iType % MAX_ITEM_INDEX;
		}
		else
		{
			iType = CharacterMachine->Equipment[EQUIPMENT_HELM].Type % MAX_ITEM_INDEX;
		}

		if (bDexSuccess)
		{
			for (int i = EQUIPMENT_ARMOR; i <= EQUIPMENT_BOOTS; ++i)
			{
#ifdef PBG_ADD_NEWCHAR_MONK
				if (iBaseClass == CLASS_RAGEFIGHTER && i == EQUIPMENT_GLOVES)
					continue;
#endif
				if (iType != CharacterMachine->Equipment[i].Type % MAX_ITEM_INDEX)
				{
					bDexSuccess = false;
					break;
				}
			}
		}
	}

	int t_adjdef = CharacterAttribute->Defense + Add_Ch_Dfe;

	if (g_isCharacterBuff((&Hero->Object), eBuff_HelpNpc))
	{
		if (CharacterAttribute->Level > 180)
		{
			t_adjdef += 180 / 5 + 50;
		}
		else
		{
			t_adjdef += (CharacterAttribute->Level / 5 + 50);
		}
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
	{
		int nTemp = CharacterAttribute->PrintPlayer.ViewDexterity + CharacterAttribute->AddDexterity;
		float fTemp = (40 - int(CharacterAttribute->PrintPlayer.ViewEnergy / 60)) / 100.f;
		fTemp = MAX(fTemp, 0.1f);
		t_adjdef -= nTemp / 3 * fTemp;
	}

	int maxdefense = 0;

	for (int j = 0; j < MAX_EQUIPMENT; ++j)
	{
		int TempLevel = (CharacterMachine->Equipment[j].Level >> 3) & 15;
		if (TempLevel >= CharacterMachine->Equipment[j].Jewel_Of_Harmony_OptionLevel)
		{
			StrengthenCapability SC;

			g_pUIJewelHarmonyinfo->GetStrengthenCapability(&SC, &CharacterMachine->Equipment[j], 2);

			if (SC.SI_isSD)
			{
				maxdefense += SC.SI_SD.SI_defense;
			}
		}
	}

	int iChangeRingAddDefense = 0;

	pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];
	if (pItemRingLeft->Type == ITEM_HELPER + 39 || pItemRingRight->Type == ITEM_HELPER + 39)
	{
		iChangeRingAddDefense = (t_adjdef + maxdefense) / 10;
	}

	if (Hero->Helper.Type == MODEL_HELPER + 80)
		iChangeRingAddDefense += 50;

	if (Hero->Helper.Type == MODEL_HELPER + 106)
		iChangeRingAddDefense += 50;

	unicode::t_char strBlocking[256];

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	int nAdd_FulBlocking = 0;
	if (
		g_isCharacterBuff((&Hero->Object), eBuff_Def_up_Ourforces)
		|| g_isCharacterBuff((&Hero->Object), EFFECT_GREATER_DEFENSE_SUCCESS_RATE_IMPROVED)
		|| g_isCharacterBuff((&Hero->Object), EFFECT_GREATER_DEFENSE_SUCCESS_RATE_ENHANCED)

		)
	{
#ifdef PBG_MOD_RAGEFIGHTERSOUND
		int _AddStat = (10 + (float)(CharacterAttribute->PrintPlayer.ViewEnergy - 80) / 10);
#else
		int _AddStat = (10 + (float)(CharacterAttribute->PrintPlayer.ViewEnergy - 160) / 50);
#endif

		if (_AddStat > 100)
			_AddStat = 100;

		_AddStat = CharacterAttribute->SuccessfulBlocking * _AddStat / 100;
		nAdd_FulBlocking += _AddStat;
	}
#endif
	if (bDexSuccess)
	{

		if (CharacterAttribute->SuccessfulBlocking > 0)
		{
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			if (nAdd_FulBlocking)
			{
				unicode::_sprintf(strBlocking, GlobalText[206], t_adjdef + maxdefense + iChangeRingAddDefense,
					CharacterAttribute->SuccessfulBlocking, (CharacterAttribute->SuccessfulBlocking) / 10 + nAdd_FulBlocking);
			}
			else
			{
				unicode::_sprintf(strBlocking, GlobalText[206], t_adjdef + maxdefense + iChangeRingAddDefense,
					CharacterAttribute->SuccessfulBlocking, (CharacterAttribute->SuccessfulBlocking) / 10);
			}
#else
			unicode::_sprintf(strBlocking, GlobalText[206], t_adjdef + maxdefense + iChangeRingAddDefense, CharacterAttribute->SuccessfulBlocking, (CharacterAttribute->SuccessfulBlocking) / 10);
#endif
		}
		else
		{
			unicode::_sprintf(strBlocking, GlobalText[207], t_adjdef + maxdefense + iChangeRingAddDefense, (t_adjdef + iChangeRingAddDefense) / 10);
		}
	}
	else
	{
		if (CharacterAttribute->SuccessfulBlocking > 0)
		{
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			if (nAdd_FulBlocking)
			{
				unicode::_sprintf(strBlocking, GlobalText[206], t_adjdef + maxdefense + iChangeRingAddDefense, CharacterAttribute->SuccessfulBlocking, nAdd_FulBlocking);
			}
			else
			{
				unicode::_sprintf(strBlocking, GlobalText[208], t_adjdef + maxdefense + iChangeRingAddDefense, CharacterAttribute->SuccessfulBlocking);
			}
#else

			unicode::_sprintf(strBlocking, GlobalText[208],
				t_adjdef + maxdefense + iChangeRingAddDefense,
				CharacterAttribute->SuccessfulBlocking
			);
#endif
		}
		else
		{

			unicode::_sprintf(strBlocking, GlobalText[209],
				t_adjdef + maxdefense + iChangeRingAddDefense
			);
		}
	}

	iY = HEIGHT_DEXTERITY + 24;
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin3))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}
	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll2))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if (g_isCharacterBuff((&Hero->Object), eBuff_Def_up_Ourforces)
		|| g_isCharacterBuff((&Hero->Object), EFFECT_GREATER_DEFENSE_SUCCESS_RATE_IMPROVED)
		|| g_isCharacterBuff((&Hero->Object), EFFECT_GREATER_DEFENSE_SUCCESS_RATE_ENHANCED))
	{
		g_pRenderText->SetTextColor(100, 150, 255, 255);
	}
#endif
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strBlocking);

	WORD wAttackSpeed = CLASS_WIZARD == iBaseClass || CLASS_SUMMONER == iBaseClass
		? CharacterAttribute->MagicSpeed : CharacterAttribute->AttackSpeed;

	unicode::_sprintf(strBlocking, GlobalText[64], wAttackSpeed);
	iY += 13;

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin1))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll1))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	ITEM* phelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
	if (phelper->Durability != 0 &&
		(phelper->Type == ITEM_HELPER + 64 || phelper->Type == ITEM_HELPER + 123))
	{
		if (IsRequireEquipItem(phelper))
		{
			if (false == pItemHelper->bExpiredPeriod)
			{
				g_pRenderText->SetTextColor(255, 0, 240, 255);
			}
		}
	}
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strBlocking);

	if (itemoption380Defense != 0 || iDefenseRate != 0)
	{
		unicode::_sprintf(strBlocking, GlobalText[2110], CharacterAttribute->SuccessfulBlockingPK + Add_Dfe, itemoption380Defense + iDefenseRate);
	}
	else
	{
		unicode::_sprintf(strBlocking, GlobalText[2045], CharacterAttribute->SuccessfulBlockingPK + Add_Dfe);
	}

	iY += 13;
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strBlocking);

	g_pRenderText->SetFont(g_hFontBold);

	DWORD wVitality = CharacterAttribute->PrintPlayer.ViewVitality + CharacterAttribute->AddVitality;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion3))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		if (
			g_isCharacterBuff((&Hero->Object), eBuff_Hp_up_Ourforces)
			|| g_isCharacterBuff((&Hero->Object), EFFECT_FITNESS_IMPROVED)
			)
		{
#ifdef PBG_MOD_RAGEFIGHTERSOUND
			CharacterMachine->CalculateAll();
			wVitality = CharacterAttribute->PrintPlayer.ViewVitality + CharacterAttribute->AddVitality;
#else
			WORD _AddStat = (WORD)(30 + (WORD)((CharacterAttribute->PrintPlayer.ViewEnergy - 380) / 10));
			if (_AddStat > 200)
			{
				_AddStat = 200;
			}
			wVitality = CharacterAttribute->PrintPlayer.ViewVitality + _AddStat;
#endif
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
#endif
			if (CharacterAttribute->AddVitality)
			{
				g_pRenderText->SetTextColor(100, 150, 255, 255);
			}
			else
			{
				g_pRenderText->SetTextColor(230, 230, 0, 255);
			}

	unicode::t_char strVitality[256];
	unicode::_sprintf(strVitality, "%d", wVitality);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X, m_Pos.y + HEIGHT_VITALITY + 7, GlobalText[169], PG_ROW_LABEL_W, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X + PG_ROW_VALUE_X, m_Pos.y + HEIGHT_VITALITY + 7, strVitality, PG_ROW_VALUE_W, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);

	{
		unicode::_sprintf(strVitality, GlobalText[211], CharacterAttribute->PrintPlayer.ViewCurHP, CharacterAttribute->PrintPlayer.ViewMaxHP);
	}

	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin4))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll5))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_RiceCake))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (phelper->Durability != 0 && phelper->Type == ITEM_HELPER + 65)
	{
		if (IsRequireEquipItem(phelper))
		{
			if (false == pItemHelper->bExpiredPeriod)
			{
				g_pRenderText->SetTextColor(255, 0, 240, 255);
			}
		}
	}
	g_pRenderText->SetBgColor(0);
	iY = HEIGHT_VITALITY + 24;
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strVitality);

#ifdef PBG_ADD_NEWCHAR_MONK
	if (iBaseClass == CLASS_RAGEFIGHTER)
	{
		iY += 13;

		unicode::_sprintf(strVitality, GlobalText[3155], 50 + (wVitality / 10));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strVitality);
	}
#endif

	g_pRenderText->SetFont(g_hFontBold);

	DWORD wEnergy = CharacterAttribute->PrintPlayer.ViewEnergy + CharacterAttribute->AddEnergy;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion4))
	{
		g_pRenderText->SetTextColor(255, 120, 0, 255);
	}
	else
		if (CharacterAttribute->AddEnergy)
		{
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else
		{
			g_pRenderText->SetTextColor(230, 230, 0, 255);
		}

	unicode::t_char strEnergy[256];
	unicode::_sprintf(strEnergy, "%d", wEnergy);

	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X, m_Pos.y + HEIGHT_ENERGY + 7, GlobalText[168], PG_ROW_LABEL_W, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(m_Pos.x + PG_ROW_X + PG_ROW_VALUE_X, m_Pos.y + HEIGHT_ENERGY + 7, strEnergy, PG_ROW_VALUE_W, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);

	if (gCharacterManager.IsMasterLevel(Hero->Class) == true)
	{
		unicode::_sprintf(strEnergy, GlobalText[213], CharacterAttribute->PrintPlayer.ViewCurMP, CharacterAttribute->PrintPlayer.ViewMaxMP);
	}
	else
	{
		unicode::_sprintf(strEnergy, GlobalText[213], CharacterAttribute->PrintPlayer.ViewCurMP, CharacterAttribute->PrintPlayer.ViewMaxMP);
	}

	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin5))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll6))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Liguor))
	{
		g_pRenderText->SetTextColor(255, 0, 240, 255);
	}

	iY = HEIGHT_ENERGY + 24;

	g_pRenderText->RenderText(m_Pos.x + 18, m_Pos.y + iY, strEnergy);

	if (iBaseClass == CLASS_WIZARD || iBaseClass == CLASS_DARK || iBaseClass == CLASS_SUMMONER)
	{
		int Level = (pWeaponRight->Level >> 3) & 15;
		int iMagicDamageMin;
		int iMagicDamageMax;

		gCharacterManager.GetMagicSkillDamage(CharacterAttribute->Skill[Hero->CurrentSkill], &iMagicDamageMin, &iMagicDamageMax);

		int iMagicDamageMinInitial = iMagicDamageMin;
		int iMagicDamageMaxInitial = iMagicDamageMax;

		iMagicDamageMin += Add_Mana_Min;
		iMagicDamageMax += Add_Mana_Max;

		if (CharacterAttribute->Ability & ABILITY_PLUS_DAMAGE)
		{
			iMagicDamageMin += 10;
			iMagicDamageMax += 10;
		}

		int maxMg = 0;

		for (int j = 0; j < MAX_EQUIPMENT; ++j)
		{
			int TempLevel = (CharacterMachine->Equipment[j].Level >> 3) & 15;

			if (TempLevel >= CharacterMachine->Equipment[j].Jewel_Of_Harmony_OptionLevel)
			{
				StrengthenCapability SC;
				g_pUIJewelHarmonyinfo->GetStrengthenCapability(&SC, &CharacterMachine->Equipment[j], 1);

				if (SC.SI_isSP)
				{
					maxMg += SC.SI_SP.SI_magicalpower;
				}
			}
		}

		pItemRingLeft = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
		pItemRingRight = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];

		int iNonExpiredLRingType = -1;
		int iNonExpiredRRingType = -1;

		if (!pItemRingLeft->bPeriodItem || !pItemRingLeft->bExpiredPeriod)
		{
			iNonExpiredLRingType = pItemRingLeft->Type;
		}
		if (!pItemRingRight->bPeriodItem || !pItemRingRight->bExpiredPeriod)
		{
			iNonExpiredRRingType = pItemRingRight->Type;
		}

		int maxIMagicDamageMin = 0;
		int maxIMagicDamageMax = 0;

		if (iNonExpiredLRingType == ITEM_HELPER + 41 || iNonExpiredRRingType == ITEM_HELPER + 41)
		{
			maxIMagicDamageMin = max(maxIMagicDamageMin, 20);
			maxIMagicDamageMax = max(maxIMagicDamageMax, 20);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxIMagicDamageMin = max(maxIMagicDamageMin, 30);
			maxIMagicDamageMax = max(maxIMagicDamageMax, 30);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxIMagicDamageMin = max(maxIMagicDamageMin, 40);
			maxIMagicDamageMax = max(maxIMagicDamageMax, 40);
		}

		iMagicDamageMin += maxIMagicDamageMin;
		iMagicDamageMax += maxIMagicDamageMax;

		pItemHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
		if (pItemHelper)
		{
			if (pItemHelper->Type == ITEM_HELPER + 37 && pItemHelper->Option1 == 0x04)
			{
				WORD wLevel = CharacterAttribute->Level;
				iMagicDamageMin += (wLevel / 25);
				iMagicDamageMax += (wLevel / 25);
			}

			if (pItemHelper->Type == ITEM_HELPER + 64)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iMagicDamageMin += int(float(iMagicDamageMin) * 0.4f);
					iMagicDamageMax += int(float(iMagicDamageMax) * 0.4f);
				}

			}
			if (pItemHelper->Type == ITEM_HELPER + 123)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iMagicDamageMin += int(float(iMagicDamageMin) * 0.2f);
					iMagicDamageMax += int(float(iMagicDamageMax) * 0.2f);
				}
			}
			if (pItemHelper->Type == ITEM_HELPER + 1)
			{
				iMagicDamageMin += int(float(iMagicDamageMin) * 0.3f);
				iMagicDamageMax += int(float(iMagicDamageMax) * 0.3f);
			}
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
		{
			int nTemp = CharacterAttribute->PrintPlayer.ViewEnergy + CharacterAttribute->AddEnergy;
			float fTemp = int(CharacterAttribute->PrintPlayer.ViewEnergy / 30) / 100.f;
			iMagicDamageMin += nTemp / 9 * fTemp;
			iMagicDamageMax += nTemp / 4 * fTemp;
		}

		if ((pWeaponRight->Type >= MODEL_STAFF - MODEL_ITEM
			&& pWeaponRight->Type < (MODEL_STAFF + MAX_ITEM_INDEX - MODEL_ITEM))
			|| pWeaponRight->Type == (MODEL_SWORD + 31 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 23 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 25 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 21 - MODEL_ITEM)
			|| pWeaponRight->Type == (MODEL_SWORD + 28 - MODEL_ITEM)
			)
		{
			float magicPercent = (float)(pWeaponRight->MagicPower) / 100;

			ITEM_ATTRIBUTE* p = &ItemAttribute[pWeaponRight->Type];
			float   percent = CalcDurabilityPercent(pWeaponRight->Durability, p->MagicDur, pWeaponRight->Level, pWeaponRight->Option1, pWeaponRight->ExtOption);

			magicPercent = magicPercent - magicPercent * percent;
			unicode::_sprintf(strEnergy, GlobalText[215], iMagicDamageMin + maxMg, iMagicDamageMax + maxMg, (int)((iMagicDamageMaxInitial + maxMg) * magicPercent));
		}
		else
		{
			unicode::_sprintf(strEnergy, GlobalText[216], iMagicDamageMin + maxMg, iMagicDamageMax + maxMg);
		}

		iY += 13;
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(255, 255, 255, 255);

		if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
		{
			g_pRenderText->SetTextColor(255, 0, 240, 255);
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll4))
		{
			g_pRenderText->SetTextColor(255, 0, 240, 255);
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
		{
			g_pRenderText->SetTextColor(255, 0, 240, 255);
		}
		g_pRenderText->RenderText(m_Pos.x + 18, m_Pos.y + iY, strEnergy);
	}

	if (iBaseClass == CLASS_SUMMONER)
	{
		int iCurseDamageMin = 0;
		int iCurseDamageMax = 0;

		gCharacterManager.GetCurseSkillDamage(CharacterAttribute->Skill[Hero->CurrentSkill], &iCurseDamageMin, &iCurseDamageMax);

		if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
		{
			int nTemp = CharacterAttribute->PrintPlayer.ViewEnergy + CharacterAttribute->AddEnergy;
			float fTemp = int(CharacterAttribute->PrintPlayer.ViewEnergy / 30) / 100.f;
			iCurseDamageMin += nTemp / 9 * fTemp;
			iCurseDamageMax += nTemp / 4 * fTemp;
		}

		int iNonExpiredLRingType = -1;
		int iNonExpiredRRingType = -1;

		if (!pItemRingLeft->bPeriodItem || !pItemRingLeft->bExpiredPeriod)
		{
			iNonExpiredLRingType = pItemRingLeft->Type;
		}
		if (!pItemRingRight->bPeriodItem || !pItemRingRight->bExpiredPeriod)
		{
			iNonExpiredRRingType = pItemRingRight->Type;
		}

		int maxICurseDamageMin = 0;
		int maxICurseDamageMax = 0;

		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxICurseDamageMin = max(maxICurseDamageMin, 30);
			maxICurseDamageMax = max(maxICurseDamageMax, 30);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxICurseDamageMin = max(maxICurseDamageMin, 40);
			maxICurseDamageMax = max(maxICurseDamageMax, 40);
		}

		iCurseDamageMin += maxICurseDamageMin;
		iCurseDamageMax += maxICurseDamageMax;

		pItemHelper = &CharacterMachine->Equipment[EQUIPMENT_HELPER];
		if (pItemHelper)
		{
			if (pItemHelper->Type == ITEM_HELPER + 64)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iCurseDamageMin += int(float(iCurseDamageMin) * 0.4f);
					iCurseDamageMax += int(float(iCurseDamageMax) * 0.4f);
				}
			}
			if (pItemHelper->Type == ITEM_HELPER + 123)
			{
				if (false == pItemHelper->bExpiredPeriod)
				{
					iCurseDamageMin += int(float(iCurseDamageMin) * 0.2f);
					iCurseDamageMax += int(float(iCurseDamageMax) * 0.2f);
				}
			}
		}

		if (ITEM_STAFF + 21 <= pWeaponLeft->Type && pWeaponLeft->Type <= ITEM_STAFF + 29)
		{
			float fCursePercent = (float)(pWeaponLeft->MagicPower) / 100;

			ITEM_ATTRIBUTE* p = &ItemAttribute[pWeaponLeft->Type];
			float fPercent = ::CalcDurabilityPercent(pWeaponLeft->Durability,
				p->MagicDur, pWeaponLeft->Level, pWeaponLeft->Option1,
				pWeaponLeft->ExtOption);

			fCursePercent -= fCursePercent * fPercent;
			unicode::_sprintf(strEnergy, GlobalText[1693],
				iCurseDamageMin, iCurseDamageMax,
				(int)((iCurseDamageMax)*fCursePercent));
		}
		else
		{
			unicode::_sprintf(strEnergy, GlobalText[1694],
				iCurseDamageMin, iCurseDamageMax);
		}

		iY += 13;
		g_pRenderText->RenderText(m_Pos.x + 18, m_Pos.y + iY, strEnergy);
	}

	iY += 13;
	if (iBaseClass == CLASS_KNIGHT)
	{
		unicode::_sprintf(strEnergy, GlobalText[582], 200 + (wEnergy / 10));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy);
	}
	if (iBaseClass == CLASS_DARK)
	{
		unicode::_sprintf(strEnergy, GlobalText[582], 200);
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy);
	}
	if (iBaseClass == CLASS_DARK_LORD)
	{
		unicode::_sprintf(strEnergy, GlobalText[582], 200 + (wEnergy / 20));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy);
	}
#ifdef PBG_ADD_NEWCHAR_MONK
	if (iBaseClass == CLASS_RAGEFIGHTER)
	{

		unicode::_sprintf(strEnergy, GlobalText[3156], 50 + (wEnergy / 10));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy);
		iY += 13;

		unicode::_sprintf(strEnergy, GlobalText[3157], 100 + (wDexterity / 8 + wEnergy / 10));
		g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + iY, strEnergy);
	}
#endif

	if (iBaseClass == CLASS_DARK_LORD)
	{
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetBgColor(0);

		DWORD wCharisma;

		wCharisma = CharacterAttribute->PrintPlayer.ViewLeadership + CharacterAttribute->AddCharisma;

		if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion5))
		{
			g_pRenderText->SetTextColor(255, 120, 0, 255);
		}
		else

			if (CharacterAttribute->AddCharisma)
			{
				g_pRenderText->SetTextColor(100, 150, 255, 255);
			}
			else
			{
				g_pRenderText->SetTextColor(230, 230, 0, 255);
			}

		unicode::t_char strCharisma[256];
		unicode::_sprintf(strCharisma, "%d", wCharisma);
		g_pRenderText->RenderText(m_Pos.x + PG_ROW_X, m_Pos.y + HEIGHT_CHARISMA + 7, GlobalText[1900], PG_ROW_LABEL_W, 0, RT3_SORT_CENTER);
		g_pRenderText->RenderText(m_Pos.x + PG_ROW_X + PG_ROW_VALUE_X, m_Pos.y + HEIGHT_CHARISMA + 7, strCharisma, PG_ROW_VALUE_W, 0, RT3_SORT_CENTER);
	}
}

void SEASON3B::CNewUICharacterInfoWindow::RenderButtons()
{
	int iBaseClass = gCharacterManager.GetBaseClass(Hero->Class);
	int iCount = 0;
	if (iBaseClass == CLASS_DARK_LORD)
	{
		iCount = 5;
	}
	else
	{
		iCount = 4;
	}

	if (CharacterAttribute->PrintPlayer.ViewPoint > 0)
	{
		for (int i = 0; i < iCount; ++i)
		{
			m_BtnStat[i].Render();
		}
	}

	m_TabBtn.Render();

	m_BtnClose.Render();
	m_BtnExit.Render();
	m_BtnQuest.Render();
	m_BtnPet.Render();
#if (OnOffMasterSkill)
	m_BtnMasterLevel.Render();
#endif
}

float SEASON3B::CNewUICharacterInfoWindow::GetLayerDepth()
{
	return 5.1f;
}

const POINT& SEASON3B::CNewUICharacterInfoWindow::GetPos() const
{
	return m_Pos;
}

void SEASON3B::CNewUICharacterInfoWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_CHAINFO_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back04.tga", IMAGE_CHAINFO_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_CHAINFO_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_CHAINFO_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_CHAINFO_BOTTOM, GL_LINEAR);

	LoadBitmap("Interface\\newui_item_table01(L).tga", IMAGE_CHAINFO_TABLE_TOP_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table01(R).tga", IMAGE_CHAINFO_TABLE_TOP_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table02(L).tga", IMAGE_CHAINFO_TABLE_BOTTOM_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table02(R).tga", IMAGE_CHAINFO_TABLE_BOTTOM_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table03(Up).tga", IMAGE_CHAINFO_TABLE_TOP_PIXEL, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table03(Dw).tga", IMAGE_CHAINFO_TABLE_BOTTOM_PIXEL, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table03(L).tga", IMAGE_CHAINFO_TABLE_LEFT_PIXEL, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_table03(R).tga", IMAGE_CHAINFO_TABLE_RIGHT_PIXEL, GL_LINEAR);

	LoadBitmap("Interface\\newui_exit_00.tga", IMAGE_CHAINFO_BTN_EXIT, GL_LINEAR);

	LoadBitmap("Interface\\newui_cha_textbox02.tga", IMAGE_CHAINFO_TEXTBOX, GL_LINEAR);
	LoadBitmap("Interface\\newui_chainfo_btn_level.tga", IMAGE_CHAINFO_BTN_STAT, GL_LINEAR);
	LoadBitmap("Interface\\newui_chainfo_btn_quest.tga", IMAGE_CHAINFO_BTN_QUEST, GL_LINEAR);
	LoadBitmap("Interface\\newui_chainfo_btn_pet.tga", IMAGE_CHAINFO_BTN_PET, GL_LINEAR);
	LoadBitmap("Interface\\newui_chainfo_btn_master.tga", IMAGE_CHAINFO_BTN_MASTERLEVEL, GL_LINEAR);

	// Pegasus skin. The .tga/.jpg names below are what the loader expects to be
	// given - CGlobalBitmap swaps them for the .OZT/.OZJ actually on disk.
	// I1 is the blue stone panel with the ornate corners. I33 is the same size
	// but plain black - easy to grab by mistake.
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\TournamentUI_I1.tga", IMAGE_CHAINFO_PG_PANEL, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\TournamentUI_I7.tga", IMAGE_CHAINFO_PG_STATROW, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_tab.tga", IMAGE_CHAINFO_PG_TAB, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_plus.tga", IMAGE_CHAINFO_PG_PLUS, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_close.tga", IMAGE_CHAINFO_PG_CLOSE, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_Character\\TournamentCharacter_I8.tga", IMAGE_CHAINFO_PG_ICON_ZEN, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_Character\\TournamentCharacter_I9.tga", IMAGE_CHAINFO_PG_ICON_POINT, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_Character\\TournamentCharacter_I11.tga", IMAGE_CHAINFO_PG_ICON_MONTH, GL_LINEAR);

	// Bottom action row. Pegasus has no art for these four, so they are the
	// stock icons rebuilt as strips at the size this window draws them.
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_btn_exit.tga", IMAGE_CHAINFO_PG_BTN_EXIT, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_btn_quest.tga", IMAGE_CHAINFO_PG_BTN_QUEST, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_btn_pet.tga", IMAGE_CHAINFO_PG_BTN_PET, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_UI\\pg_chainfo_btn_master.tga", IMAGE_CHAINFO_PG_BTN_MASTER, GL_LINEAR);

	LoadBitmap("Interface\\Pegasus\\Pegasus_Character\\TournamentCharacter_I10.tga", IMAGE_CHAINFO_PG_FRAME, GL_LINEAR);
	LoadBitmap("Interface\\Pegasus\\Pegasus_Character\\TournamentCharacter_I30.tga", IMAGE_CHAINFO_PG_BOX, GL_LINEAR);

	LoadPortraitImage();
}

// The round class portrait is per base class; reloaded whenever the window opens
// so a class change (or a different character) picks up the right art.
void SEASON3B::CNewUICharacterInfoWindow::LoadPortraitImage()
{
	// Pegasus_Character\TournamentCharacter_I1..I7, in CLASS_* order.
	// Sized by the initialiser, not MAX_CLASS - that macro is 6 or 7 depending
	// on PBG_ADD_NEWCHAR_MONK, and the table must not depend on which.
	static const int s_anPortraitByClass[] = { 1, 3, 7, 5, 6, 2, 4 };
	const int iPortraitCount = sizeof(s_anPortraitByClass) / sizeof(s_anPortraitByClass[0]);

	int iBaseClass = gCharacterManager.GetBaseClass(Hero->Class);
	if (iBaseClass < 0 || iBaseClass >= iPortraitCount)
		iBaseClass = 0;

	unicode::t_char szPath[256];
	unicode::_sprintf(szPath, "Interface\\Pegasus\\Pegasus_Character\\TournamentCharacter_I%d.tga",
		s_anPortraitByClass[iBaseClass]);

	DeleteBitmap(IMAGE_CHAINFO_PG_PORTRAIT);
	LoadBitmap(szPath, IMAGE_CHAINFO_PG_PORTRAIT, GL_LINEAR);
}

void SEASON3B::CNewUICharacterInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_CHAINFO_PG_BOX);
	DeleteBitmap(IMAGE_CHAINFO_PG_FRAME);
	DeleteBitmap(IMAGE_CHAINFO_PG_BTN_MASTER);
	DeleteBitmap(IMAGE_CHAINFO_PG_BTN_PET);
	DeleteBitmap(IMAGE_CHAINFO_PG_BTN_QUEST);
	DeleteBitmap(IMAGE_CHAINFO_PG_BTN_EXIT);
	DeleteBitmap(IMAGE_CHAINFO_PG_ICON_MONTH);
	DeleteBitmap(IMAGE_CHAINFO_PG_ICON_POINT);
	DeleteBitmap(IMAGE_CHAINFO_PG_ICON_ZEN);
	DeleteBitmap(IMAGE_CHAINFO_PG_PORTRAIT);
	DeleteBitmap(IMAGE_CHAINFO_PG_CLOSE);
	DeleteBitmap(IMAGE_CHAINFO_PG_PLUS);
	DeleteBitmap(IMAGE_CHAINFO_PG_TAB);
	DeleteBitmap(IMAGE_CHAINFO_PG_STATROW);
	DeleteBitmap(IMAGE_CHAINFO_PG_PANEL);

	DeleteBitmap(IMAGE_CHAINFO_BTN_MASTERLEVEL);
	DeleteBitmap(IMAGE_CHAINFO_BTN_PET);
	DeleteBitmap(IMAGE_CHAINFO_BTN_QUEST);
	DeleteBitmap(IMAGE_CHAINFO_BTN_STAT);
	DeleteBitmap(IMAGE_CHAINFO_TEXTBOX);

	DeleteBitmap(IMAGE_CHAINFO_BTN_EXIT);

	DeleteBitmap(IMAGE_CHAINFO_BOTTOM);
	DeleteBitmap(IMAGE_CHAINFO_RIGHT);
	DeleteBitmap(IMAGE_CHAINFO_LEFT);
	DeleteBitmap(IMAGE_CHAINFO_TOP);
	DeleteBitmap(IMAGE_CHAINFO_BACK);

	DeleteBitmap(IMAGE_CHAINFO_TABLE_RIGHT_PIXEL);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_LEFT_PIXEL);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_BOTTOM_PIXEL);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_TOP_PIXEL);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_BOTTOM_RIGHT);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_BOTTOM_LEFT);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_TOP_RIGHT);
	DeleteBitmap(IMAGE_CHAINFO_TABLE_TOP_LEFT);
}

void SEASON3B::CNewUICharacterInfoWindow::OpenningProcess()
{
	LoadPortraitImage();
	ResetEquipmentLevel();
#if (OnOffMasterSkill)
	if (gCharacterManager.IsMasterLevel(Hero->Class) == true
#ifdef PBG_ADD_NEWCHAR_MONK
		|| gCharacterManager.GetCharacterClass(Hero->Class) == CLASS_TEMPLENIGHT
#endif
		)
	{
		m_BtnMasterLevel.UnLock();
		m_BtnMasterLevel.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
		m_BtnMasterLevel.ChangeTextColor(RGBA(255, 255, 255, 255));
	}
	else
	{
		m_BtnMasterLevel.Lock();
		m_BtnMasterLevel.ChangeImgColor(BUTTON_STATE_UP, RGBA(100, 100, 100, 255));
		m_BtnMasterLevel.ChangeTextColor(RGBA(100, 100, 100, 255));
	}
#endif
	g_csItemOption.init();

	if (CharacterMachine->IsZeroDurability())
	{
		CharacterMachine->CalculateAll();
	}

	if (g_QuestMng.IsIndexInCurQuestIndexList(0x10009))
	{
		if (g_QuestMng.IsEPRequestRewardState(0x10009))
		{
			g_pMyQuestInfoWindow->UnselectQuestList();
			SendSatisfyQuestRequestFromClient(0x10009);
			g_QuestMng.SetEPRequestRewardState(0x10009, false);
		}
	}
}

void SEASON3B::CNewUICharacterInfoWindow::ResetEquipmentLevel()
{
	ITEM* pItem = CharacterMachine->Equipment;
	Hero->Weapon[0].Level = (pItem[EQUIPMENT_WEAPON_RIGHT].Level >> 3) & 15;
	Hero->Weapon[1].Level = (pItem[EQUIPMENT_WEAPON_LEFT].Level >> 3) & 15;
	Hero->BodyPart[BODYPART_HELM].Level = (pItem[EQUIPMENT_HELM].Level >> 3) & 15;
	Hero->BodyPart[BODYPART_ARMOR].Level = (pItem[EQUIPMENT_ARMOR].Level >> 3) & 15;
	Hero->BodyPart[BODYPART_PANTS].Level = (pItem[EQUIPMENT_PANTS].Level >> 3) & 15;
	Hero->BodyPart[BODYPART_GLOVES].Level = (pItem[EQUIPMENT_GLOVES].Level >> 3) & 15;
	Hero->BodyPart[BODYPART_BOOTS].Level = (pItem[EQUIPMENT_BOOTS].Level >> 3) & 15;

	CheckFullSet(Hero);
}
