

#include "stdafx.h"

#include "NewUIMainFrameWindow.h"
#include "NewUIRenderNumber.h"
#include "MenuCustom.h"
#include "PegasusBattlePass.h"
#include "PegasusDungeon.h"
#include "PegasusCollection.h"
#include "PegasusDaily.h"
#include "PegasusNameFx.h"
#include "NewUIOptionWindow.h"
#include "Protocol.h"
#include "NewUISystem.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "ZzzInfomation.h"
#include "ZzzBMD.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "CSItemOption.h"
#include "CSChaosCastle.h"
#include "MapManager.h"
#include "CharacterManager.h"
#include "SkillManager.h"
#include "GMDoppelGanger1.h"
#include "GMDoppelGanger2.h"
#include "GMDoppelGanger3.h"
#include "GMDoppelGanger4.h"
#include "./Time/CTimCheck.h"
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
#include "MonkSystem.h"
#endif
#include "CBInterface.h"
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
#include "GameShop/InGameShopSystem.h"

// Master switch for the Pegasus HUD. Set to 0 to fall back to the stock MU
// bar. Must sit above every function it gates.
#define PG_USE_PEGASUS_HUD	1
#endif

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int  MouseUpdateTime;
extern int  MouseUpdateTimeMax;
extern int SelectedCharacter;
extern int Attacking;
extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayWinMid;
extern int DisplayHeightExt;
extern int DisplayWinExt;
extern int DisplayWinReal;

SEASON3B::CNewUIMainFrameWindow::CNewUIMainFrameWindow()
{
	m_bExpEffect = false;
	m_dwExpEffectTime = 0;
	m_dwPreExp = 0;
	m_dwGetExp = 0;
	m_bButtonBlink = false;
	ButtonSS2.x = 0;
	ButtonSS2.y = 0;
	ButtonSS2W = 0;
	ButtonSS2H = 0;
}

SEASON3B::CNewUIMainFrameWindow::~CNewUIMainFrameWindow()
{
	Release();
}

void SEASON3B::CNewUIMainFrameWindow::LoadImages()
{
	if (gProtect.m_MainInfo.IsVersion == 1)
	{
		LoadBitmap("Custom\\Interface\\MenuS2_Main.tga", IMAGE_MENU_1, GL_LINEAR);

		LoadBitmap("Custom\\Interface\\MenuS2_middle.jpg", IMAGE_MENU_2, GL_LINEAR);
		LoadBitmap("Custom\\Interface\\MenuS2_right.jpg", IMAGE_MENU_3, GL_LINEAR);

		LoadBitmap("Custom\\Interface\\MenuS2_Blue.tga", IMAGE_GAUGE_BLUE, GL_LINEAR);
		LoadBitmap("Custom\\Interface\\MenuS2_Green.tga", IMAGE_GAUGE_GREEN, GL_LINEAR);
		LoadBitmap("Custom\\Interface\\MenuS2_Red.tga", IMAGE_GAUGE_RED, GL_LINEAR);
		LoadBitmap("Custom\\Interface\\MenuS2_AG.jpg", IMAGE_GAUGE_AG, GL_LINEAR);
		LoadBitmap("Custom\\Interface\\MenuS2_SD.jpg", IMAGE_GAUGE_SD, GL_LINEAR);
		LoadBitmap("Custom\\Interface\\MenuS2_Button.jpg", IMAGE_MENUS2_BUTTON, GL_LINEAR);
	}
	else
	{
		LoadBitmap("Interface\\newui_menu_blue.jpg", IMAGE_GAUGE_BLUE, GL_LINEAR);
		LoadBitmap("Interface\\newui_menu_green.jpg", IMAGE_GAUGE_GREEN, GL_LINEAR);
		LoadBitmap("Interface\\newui_menu_red.jpg", IMAGE_GAUGE_RED, GL_LINEAR);
		LoadBitmap("Interface\\newui_menu01.jpg", IMAGE_MENU_1, GL_LINEAR);
		LoadBitmap("Interface\\newui_menu02.jpg", IMAGE_MENU_2, GL_LINEAR);
		LoadBitmap("Interface\\partCharge1\\newui_menu03.jpg", IMAGE_MENU_3, GL_LINEAR);
		LoadBitmap("Interface\\newui_menu_ag.jpg", IMAGE_GAUGE_AG, GL_LINEAR);
		LoadBitmap("Interface\\newui_menu_sd.jpg", IMAGE_GAUGE_SD, GL_LINEAR);
	}

	LoadBitmap("Interface\\newui_menu02-03.jpg", IMAGE_MENU_2_1, GL_LINEAR);

	LoadBitmap("Interface\\newui_exbar.jpg", IMAGE_GAUGE_EXBAR, GL_LINEAR);
	LoadBitmap("Interface\\Exbar_Master.jpg", IMAGE_MASTER_GAUGE_BAR, GL_LINEAR);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt05.jpg", IMAGE_MENU_BTN_CSHOP, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt01.jpg", IMAGE_MENU_BTN_CHAINFO, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt02.jpg", IMAGE_MENU_BTN_MYINVEN, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt03.jpg", IMAGE_MENU_BTN_FRIEND, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt04.jpg", IMAGE_MENU_BTN_WINDOW, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\Decor.tga", IMAGE_DECOR_WIDE, GL_LINEAR, GL_CLAMP_TO_EDGE);

}

void SEASON3B::CNewUIMainFrameWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_MENU_1);
	DeleteBitmap(IMAGE_MENU_2);
	DeleteBitmap(IMAGE_MENU_3);
	DeleteBitmap(IMAGE_MENU_2_1);
	DeleteBitmap(IMAGE_GAUGE_BLUE);
	DeleteBitmap(IMAGE_GAUGE_GREEN);
	DeleteBitmap(IMAGE_GAUGE_RED);
	DeleteBitmap(IMAGE_GAUGE_AG);
	DeleteBitmap(IMAGE_GAUGE_SD);
	DeleteBitmap(IMAGE_GAUGE_EXBAR);
	DeleteBitmap(IMAGE_MENU_BTN_CHAINFO);
	DeleteBitmap(IMAGE_MENU_BTN_MYINVEN);
	DeleteBitmap(IMAGE_MENU_BTN_FRIEND);
	DeleteBitmap(IMAGE_MENU_BTN_WINDOW);
	DeleteBitmap(IMAGE_DECOR_WIDE);
	DeleteBitmap(IMAGE_MENUS2_BUTTON);
}

bool SEASON3B::CNewUIMainFrameWindow::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if (NULL == pNewUIMng || NULL == pNewUI3DRenderMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MAINFRAME, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;
	m_pNewUI3DRenderMng->Add3DRenderObj(this, ITEMHOTKEYNUMBER_CAMERA_Z_ORDER);

	LoadImages();

	SetButtonInfo();

	Show(true);

	return true;
}

// Defined with the rest of the Pegasus HUD further down; forward-declared here
// so SetButtonInfo can place the stock buttons on real sockets instead of
// re-deriving the slot geometry from copied numbers.
namespace
{
	// Where the experience bar goes. The stock one is a 629-wide strip pinned
	// inside the centred 640 block, which leaves it floating short of both ends
	// of the Pegasus plate. These put it hard against the bottom of the screen,
	// spanning the plate, and RenderExperience keeps doing all the arithmetic.
	float PgExpX();
	float PgExpY();
	float PgExpW();
	float PgExpH();

	// The gold fill RenderExperience paints, and its UV extent. Both branches
	// of that function - master level and normal - go through these, so the
	// bar looks the same either way.
	int   PgExpFill();
	float PgExpFillU();
	float PgExpFillV();
}

void SEASON3B::CNewUIMainFrameWindow::SetButtonInfo()
{
	int x_Next = 489 + DisplayWinExt;
	int y_Next = (DisplayHeight)-51;
	int x_Add = 30;
	int y_Add = 41;
	int x_Step = x_Add;		// gap between buttons, kept apart from their width

	// Under the Pegasus HUD these five are neither drawn nor clickable - the
	// Q/W/E/R/T sockets belong to the potion hotkeys and the custom menu opens
	// everything they used to. Their geometry is still set up so the
	// non-Pegasus build is untouched.
	m_BtnCShop.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnCShop.ChangeButtonImgState(true, IMAGE_MENU_BTN_CSHOP, true);
	m_BtnCShop.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Step;
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeToolTipText(GlobalText[2277], true);

	m_BtnChaInfo.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnChaInfo.ChangeButtonImgState(true, IMAGE_MENU_BTN_CHAINFO, true);
	m_BtnChaInfo.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Step;
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeToolTipText(GlobalText[362], true);

	m_BtnMyInven.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnMyInven.ChangeButtonImgState(true, IMAGE_MENU_BTN_MYINVEN, true);
	m_BtnMyInven.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Step;
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeToolTipText(GlobalText[363], true);

	m_BtnFriend.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnFriend.ChangeButtonImgState(true, IMAGE_MENU_BTN_FRIEND, true);
	m_BtnFriend.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Step;
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeToolTipText(GlobalText[1043], true);

	m_BtnWindow.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnWindow.ChangeButtonImgState(true, IMAGE_MENU_BTN_WINDOW, true);
	m_BtnWindow.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	m_BtnWindow.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnWindow.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnWindow.ChangeToolTipText(GlobalText[1744], true);

	ButtonSS2.x = 87 + DisplayWinExt;
	ButtonSS2.y = DisplayHeight - 28;
	ButtonSS2W = 25;
	ButtonSS2H = 25;
}

void SEASON3B::CNewUIMainFrameWindow::Release()
{
	UnloadImages();

	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->Remove3DRenderObj(this);
		m_pNewUI3DRenderMng = NULL;
	}

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderLifeManaSS2()
{
#if PG_USE_PEGASUS_HUD
	// Part of the stock bar - Pegasus draws its own.
	return;
#endif

	DWORD wLifeMax, wLife, wManaMax, wMana;

	wLifeMax = CharacterAttribute->PrintPlayer.ViewMaxHP;
	wLife = min(max(0, CharacterAttribute->PrintPlayer.ViewCurHP), wLifeMax);
	wManaMax = CharacterAttribute->PrintPlayer.ViewMaxMP;
	wMana = min(max(0, CharacterAttribute->PrintPlayer.ViewCurMP), wManaMax);

	if (wLifeMax > 0)
	{
		if (wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			PlayBuffer(SOUND_HEART);
		}
	}

	float fLife = 0.f;
	float fMana = 0.f;

	if (wLifeMax > 0)
	{
		fLife = (wLifeMax - wLife) / (float)wLifeMax;
	}
	if (wManaMax > 0)
	{
		fMana = (wManaMax - wMana) / (float)wManaMax;
	}

	float width, height;
	float x, y;
	float fY, fH, fV;

	width = 52;
	height = 51;
	x = 98.0f + 43.f + DisplayWinExt;
	y = DisplayHeight - 50.f;

	fY = y + (fLife * height);
	fH = height - (fLife * height);
	fV = fLife;
	if (g_isCharacterBuff((&Hero->Object), eDeBuff_Poison))
	{
		RenderBitmap(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}
	else
	{
		RenderBitmap(IMAGE_GAUGE_RED, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}

	SEASON3B::RenderNumber(x + 25, (470 + DisplayHeightExt) - 18, wLife);

	char strTipText[256];
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[358], wLife, wLifeMax);
		RenderTipText((int)x, (int)(413 + DisplayHeightExt), strTipText);
	}

	width = 52;
	height = 51;
	x = 488.7f - 41.f + DisplayWinExt;
	y = DisplayHeight - 50.5f;

	fY = y + (fMana * height);
	fH = height - (fMana * height);
	fV = fMana;
	RenderBitmap(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);

	SEASON3B::RenderNumber(x + 30, (470 + DisplayHeightExt) - 18, wMana);

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[359], wMana, wManaMax);
		RenderTipText((int)x, (int)(413 + DisplayHeightExt), strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageAGSS2()
{
#if PG_USE_PEGASUS_HUD
	// Part of the stock bar - Pegasus draws its own.
	return;
#endif

	float x, y, width, height;
	float fY, fH, fV;

	DWORD dwMaxSkillMana, dwSkillMana;

	{
		dwMaxSkillMana = max(1, CharacterAttribute->PrintPlayer.ViewMaxBP);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->PrintPlayer.ViewCurBP);
	}

	float fSkillMana = 0.0f;

	if (dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

	width = 17;
	height = 40;
	x = 551.5f - 45 + DisplayWinExt;
	y = DisplayHeight - 41.f;

	fY = y + (fSkillMana * height);
	fH = height - (fSkillMana * height);
	fV = fSkillMana;

	RenderBitmap(IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV * height / 64.f, width / 32.f, (1.0f - fV) * height / 64.f);
	SEASON3B::RenderNumber(x + 10, (480 + DisplayHeightExt) - 18, (int)dwSkillMana);

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[214], dwSkillMana, dwMaxSkillMana);
		RenderTipText((int)x - 20, (int)(418 + DisplayHeightExt), strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageSDSS2()
{
#if PG_USE_PEGASUS_HUD
	// Part of the stock bar - Pegasus draws its own.
	return;
#endif

	float x, y, width, height;
	float fY, fH, fV;
	DWORD wMaxShield, wShield;

	{
		wMaxShield = max(1, CharacterAttribute->PrintPlayer.ViewMaxSD);
		wShield = min(wMaxShield, CharacterAttribute->PrintPlayer.ViewCurSD);
	}

	float fShield = 0.0f;

	if (wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}

	width = 17;
	height = 40;
	x = 73.0f + 44 + DisplayWinExt;
	y = DisplayHeight - 41.f;
	fY = y + (fShield * height);
	fH = height - (fShield * height);
	fV = fShield;

	RenderBitmap(IMAGE_GAUGE_SD, x, fY, width, fH, 0.f, fV * height / 64.f, width / 32.f, (1.0f - fV) * height / 64.f);
	SEASON3B::RenderNumber(x + 15, (480 + DisplayHeightExt) - 18, (int)wShield);

	height = 39.f;
	y = (480.f + DisplayHeightExt) - 10.f - 39.f;
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[2037], wShield, wMaxShield);
		RenderTipText((int)x - 20, (int)(418 + DisplayHeightExt), strTipText);
	}
}
void SEASON3B::CNewUIMainFrameWindow::RenderExperienceSS2()
{
#if PG_USE_PEGASUS_HUD
	// Part of the stock bar - Pegasus draws its own.
	return;
#endif


	__int64 wLevel;
	__int64 dwNexExperience;
	__int64 dwExperience;
	double x, y, width, height;

	{
		wLevel = CharacterAttribute->Level;
		dwNexExperience = CharacterAttribute->NextExperince;
		dwExperience = CharacterAttribute->Experience;
	}

	{
		x = 0 + DisplayWinExt;
		y = 470 + DisplayHeightExt;
		width = 6; height = 4;

		WORD wPriorLevel = wLevel - 1;
		DWORD dwPriorExperience = 0;

		if (wPriorLevel > 0)
		{
			dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

			if (wPriorLevel > 255)
			{
				int iLevelOverN = wPriorLevel - 255;
				dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
			}
		}

		float fNeedExp = dwNexExperience - dwPriorExperience;
		float fExp = dwExperience - dwPriorExperience;

		if (dwExperience < dwPriorExperience)
		{
			fExp = 0.f;
		}

		float fExpBarNum = 0.f;
		if (fExp > 0.f && fNeedExp > 0)
		{
			fExpBarNum = (fExp / fNeedExp) * 10.f;
		}

		float fProgress = fExpBarNum;
		fProgress = fProgress - (int)fProgress;

		float FixX = 220.f, FixY = 439.f, FixW = 194;
		if (m_bExpEffect == true)
		{
			float fPreProgress = 0.f;
			fExp = m_dwPreExp - dwPriorExperience;
			if (m_dwPreExp < dwPriorExperience)
			{
				x = FixX + DisplayWinExt;
				y = FixY + DisplayHeightExt;
				width = fProgress * FixW;
				height = PgExpH();
				RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
				glColor4f(1.f, 1.f, 1.f, 0.4f);
				RenderColor(x, y, width, height);
				EndRenderColor();
			}
			else
			{
				int iPreExpBarNum = 0;
				int iExpBarNum = 0;
				if (fExp > 0.f && fNeedExp > 0.f)
				{
					fPreProgress = (fExp / fNeedExp) * 10.f;
					iPreExpBarNum = (int)fPreProgress;
					fPreProgress = fPreProgress - (int)fPreProgress;
				}

				iExpBarNum = (int)fExpBarNum;

				if (iExpBarNum > iPreExpBarNum)
				{
					x = FixX + DisplayWinExt;
					y = FixY + DisplayHeightExt;
					width = fProgress * FixW;
					height = PgExpH();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					glColor4f(1.f, 1.f, 1.f, 0.4f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
				else
				{
					float fGapProgress = 0.f;
					fGapProgress = fProgress - fPreProgress;
					x = FixX + DisplayWinExt;
					y = FixY + DisplayHeightExt;
					width = fPreProgress * FixW;
					height = PgExpH();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					x += width;
					width = fGapProgress * FixW;
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					glColor4f(1.f, 1.f, 1.f, 0.4f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
			}
		}
		else
		{
			x = FixX + DisplayWinExt;
			y = FixY + DisplayHeightExt;
			width = fProgress * FixW;
			height = PgExpH();
			RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
		}

		int iExp = (int)fExpBarNum;
		x = 425 + DisplayWinExt;
		y = 437.f + DisplayHeightExt;
		SEASON3B::RenderNumber(x, y, iExp, 0.8);

		x = FixX + DisplayWinExt;
		y = FixY + DisplayHeightExt;
		width = FixW + 40;
		height = PgExpH();
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			char strTipText[256];

			sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
			RenderTipText(280 + DisplayWinExt, 418 + DisplayHeightExt, strTipText);
		}
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderButtonsSS2()
{
#if PG_USE_PEGASUS_HUD
	// Same as RenderButtons - the custom menu replaces these.
	return;
#endif

	char strTipText[256];
	double x, y, width, height;
	x = ButtonSS2.x;
	y = ButtonSS2.y;

	width = ButtonSS2W; height = ButtonSS2H;
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
	{
		RenderBitmap(IMAGE_MENUS2_BUTTON, x + 259 + 62, y, width, height, (width / 129) * 3, 0.f, width / 130, height / 33);
	}
	else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY))
	{
		RenderBitmap(IMAGE_MENUS2_BUTTON, x + 259 + 30, y, width, height, (width / 129) * 2, 0.f, width / 130, height / 33);
	}
	else if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO))
	{
		RenderBitmap(IMAGE_MENUS2_BUTTON, x + 259, y, width, height, (width / 130) * 1, 0.f, width / 130, height / 33);
	}

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{

		RenderTipText(x, y - 35, GlobalText[1744]);
		RenderBitmap(IMAGE_MENUS2_BUTTON, x, y, width, height, 0.f, 0.f, width / 130, height / 33);
	}

	if (SEASON3B::CheckMouseIn(x + 259, y, width, height) == true)
	{

		sprintf(strTipText, "Guild");
		RenderTipText(x + 259, y - 35, strTipText);

		RenderBitmap(IMAGE_MENUS2_BUTTON, x + 259, y, width, height, (width / 130) * 1, 0.f, width / 130, height / 33);
	}
	if (SEASON3B::CheckMouseIn(x + 259 + 30, y, width, height) == true)
	{

		RenderTipText(x + 259 + 30, y - 35, GlobalText[363]);
		RenderBitmap(IMAGE_MENUS2_BUTTON, x + 259 + 30, y, width, height, (width / 129) * 2, 0.f, width / 130, height / 33);
	}
	if (SEASON3B::CheckMouseIn(x + 259 + 62, y, width, height) == true)
	{
		RenderTipText(x + 259 + 62, y - 35, GlobalText[362]);
		RenderBitmap(IMAGE_MENUS2_BUTTON, x + 259 + 62, y, width, height, (width / 129) * 3, 0.f, width / 130, height / 33);
	}

}
bool SEASON3B::CNewUIMainFrameWindow::RenderSS2()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();

	m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

	g_pSkillList->RenderCurrentSkillAndHotSkillList();

	EnableAlphaTest();
	RenderLifeManaSS2();
	RenderGuageSDSS2();
	RenderGuageAGSS2();
	RenderButtonsSS2();
	RenderExperienceSS2();
	DisableAlphaBlend();
	return true;
}
bool SEASON3B::CNewUIMainFrameWindow::Render()
{
	if (gProtect.m_MainInfo.IsVersion == 1)
	{
		return RenderSS2();
	}
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();

	m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

	g_pSkillList->RenderCurrentSkillAndHotSkillList();

	EnableAlphaTest();
	RenderLifeMana();
	RenderGuageSD();
	RenderGuageAG();
	RenderButtons();
	RenderExperience();
	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIMainFrameWindow::Render3D()
{
	m_ItemHotKey.RenderItems();
}

void SEASON3B::CNewUIMainFrameWindow::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	g_pMainFrame->RenderHotKeyItemCount();

}

bool SEASON3B::CNewUIMainFrameWindow::IsVisible() const
{
	return CNewUIObj::IsVisible();
}

// ---- Pegasus HUD ---------------------------------------------------------
// Every position below is expressed in the coordinate space of
// TournamentMain_I7, the 1224x180 plate that carries the frame, the two gem
// sockets, the gauge channel and all eleven skill slots. They were measured
// straight off that file, so the whole HUD stays internally consistent no
// matter what size the plate ends up on screen - PgX/PgY do the mapping.
//
// I7 itself cannot be loaded: OpenTga rejects anything wider than MAX_WIDTH
// (1024). It ships pre-scaled as pg_hud_bar instead.
//
// All the pg_hud_* textures are authored power-of-two on purpose. OpenTga
// rounds non-power-of-two art up to the next power of two and never clears the
// extra rows and columns, so a UV that reaches into that padding samples
// uninitialised heap and draws as a white translucent block. That is what the
// bar was doing: it was loading I99 (640x53, padded to 1024x64) while asking
// for a 1024x151 source rect, so most of the quad was garbage.
namespace
{
	// --- source rects, in texels of their own textures ---
	const float PG_SRC_BAR_W  = 1024.f, PG_SRC_BAR_H  = 151.f;	// pg_hud_bar   1024x256
	const float PG_SRC_GEM    = 160.f;							// pg_hud_gem    256x256
	const float PG_SRC_FILL_W = 346.f,  PG_SRC_FILL_H = 11.f;	// pg_hud_fill_* 512x16
	const float PG_SRC_BTN_W  = 172.f,  PG_SRC_BTN_H  = 36.f;	// pg_hud_btn    256x64

	// --- layout, in I7 art space (1224 x 180) ---
	const float PG_ART_W = 1224.f;
	const float PG_ART_H = 180.f;

	// The two diamond-shaped holes in the plate. Same neutral gem art both
	// sides, tinted red and blue.
	const float PG_GEM_L_X = 204.5f;
	const float PG_GEM_R_X = 1019.5f;
	const float PG_GEM_Y   = 90.f;
	// Deliberately larger than the 114x108 opening. The gem is drawn under the
	// plate, so the overspill is masked by the frame and the crystal reaches
	// the bevel instead of floating inside it. Keep the 1.034 ratio when
	// changing these or the diamond stops matching the socket's taper.
	const float PG_GEM_W   = 150.f;
	const float PG_GEM_H   = 145.f;

	// Top and bottom of the socket opening itself, which is what the drain line
	// is measured against - see PgGem.
	const float PG_GEM_SOCK_TOP = 36.f;
	const float PG_GEM_SOCK_BOT = 144.f;

	// How many times the lit crystal is laid down. More passes = deeper colour.
	const int PG_GEM_PASSES = 2;

	// How solid the shield shell sits over the life gem. Low enough that the
	// fire still reads through it, high enough that the shield is unmistakable.
	const float PG_GEM_SHELL_ALPHA = 0.72f;

	// The spent part of a gem. It used to be the whole crystal at 22% brightness
	// and full opacity, which still reads as a complete gem sitting behind the
	// filled one - two pictures stacked rather than one draining. Dropping the
	// brightness and letting the socket show through most of it turns it into a
	// ghost of the facets instead, so what is left of the stat is the only lit
	// thing in the hole. Raise either value to bring the empty part back.
	const float PG_GEM_DRAIN_LEVEL = 0.09f;		// brightness of the spent part
	const float PG_GEM_DRAIN_ALPHA = 0.5f;		// how much of it shows at all

	// The gems are animated. TournamentMain_I3 and I2 hold a 60-frame loop each
	// - fire for life, drifting light for mana - as a 6x10 grid of 152px cells,
	// but at 912x1520 they are past MAX_HEIGHT and will not load. pg_gem_hp and
	// pg_gem_mp are those frames repacked 8x8 at 128px into a clean 1024x1024.
	const int   PG_GEM_COLS     = 8;
	const int   PG_GEM_FRAMES   = 60;
	const float PG_GEM_CELL     = 128.f;
	const DWORD PG_GEM_FRAME_MS = 55;	// ~18fps, so the loop runs about 3.3s

	// The gauge channel is a chain of hollow links running between the sockets,
	// broken in the middle by the crest. Fills are drawn underneath the plate,
	// so the links read as segment dividers over them.
	const float PG_GAUGE_Y   = 74.f;
	const float PG_GAUGE_H   = 8.f;
	const float PG_GAUGE_L_X = 275.f, PG_GAUGE_L_W = 295.f;
	const float PG_GAUGE_R_X = 645.f, PG_GAUGE_R_W = 300.f;

	// PegaShop / Start Auto plates, symmetric about the centre of the crest.
	const float PG_BTN_L_X = 114.5f, PG_BTN_R_X = 1109.5f;
	const float PG_BTN_Y   = 160.f;
	const float PG_BTN_W   = 125.f,  PG_BTN_H   = 25.f;

	// The quick-access icon row that used to sit above the right-hand gauge is
	// gone - everything it opened is one keypress or an F5 away, and it was
	// crowding the bar. The gift box took over its spot.

	// Round button at the far bottom-left that opens the custom game menu.
	const float PG_GEAR_X = 24.f, PG_GEAR_Y = 112.f, PG_GEAR_SZ = 34.f;

	// The window row above the right-hand gauge, on a 63px pitch. The plate is
	// blank from about x=290 to the gem, so the row grows leftwards; the right
	// end is fixed because the gem's art begins at 944.
	// Name-effect picker, one pitch further left again. The row can keep growing
	// this way: the plate is blank from about x=290 across to the gem.
	const float PG_NFX_X = 659.f, PG_NFX_Y = 42.f, PG_NFX_SZ = 46.f;
	const float PG_SRC_NFX_W = 224.f, PG_SRC_NFX_H = 176.f;

	// TournamentDaily_I11 is a 72x72 calendar.
	const float PG_DAILY_X = 722.f, PG_DAILY_Y = 42.f, PG_DAILY_SZ = 46.f;
	const float PG_SRC_DAILY = 72.f;

	// TournamentMain_I121 is 20x20.
	const float PG_GIFT_X = 785.f, PG_GIFT_Y = 42.f, PG_GIFT_SZ = 46.f;
	const float PG_SRC_GIFT = 20.f;

	// Pegasus_Dungeon_I19 is a 48x48 blue gate rune.
	const float PG_DUNG_X = 848.f, PG_DUNG_Y = 42.f, PG_DUNG_SZ = 46.f;
	const float PG_SRC_DUNG = 48.f;

	// TournamentMain_I123 is a 224x176 laurel shield. The row stops here: the
	// right gem's art begins at 944.
	const float PG_COLL_X = 911.f, PG_COLL_Y = 42.f, PG_COLL_SZ = 46.f;
	const float PG_SRC_COLL_W = 224.f, PG_SRC_COLL_H = 176.f;

	const char* const PG_LABEL_L = "Menu";
	const char* const PG_LABEL_R = "Start Auto";

	// Number anchors, all centres.
	const float PG_NUM_GAUGE_Y = 72.f;
	const float PG_NUM_L_X     = 345.f;		// inner end of the left gauge
	const float PG_NUM_R_X     = 880.f;		// inner end of the right gauge

	// Glyphs in newui_number1 are 12x14 in a 128x16 sheet. These are heights in
	// art space; PgDigits scales them along with everything else.
	const float PG_DIGIT_H_GEM   = 25.f;
	const float PG_DIGIT_H_GAUGE = 17.f;

	// The eleven skill slots, measured off the plate. Six wide ones carry the
	// five hotkey skills plus the current skill, then five narrow ones carry
	// the Q/W/E/R/T item hotkeys. The stock bar draws its icons at fixed
	// offsets inside the 640-wide block, which is nowhere near these, so
	// PgSlotRect below hands out the real rectangles and the two renderers
	// further down use them instead.
	const float PG_SLOT_CY        = 127.5f;
	const int   PG_SLOT_BIG_N     = 6;
	const float PG_SLOT_BIG_CX0   = 319.5f;
	const float PG_SLOT_BIG_PITCH = 65.1f;
	const float PG_SLOT_BIG_SZ    = 56.f;
	const int   PG_SLOT_SML_N     = 5;
	const float PG_SLOT_SML_CX0   = 704.5f;
	const float PG_SLOT_SML_PITCH = 51.f;
	const float PG_SLOT_SML_SZ    = 44.f;
	const int   PG_SLOT_COUNT     = PG_SLOT_BIG_N + PG_SLOT_SML_N;
	const int   PG_SLOT_CURRENT   = 5;		// wide socket the plate highlights
	const int   PG_SLOT_ITEM0     = 6;		// first of Q/W/E/R/T

	// The plate is drawn at its native 1224x180 on a 1080-tall screen and
	// scales with screen height from there, which is what the reference client
	// does - a fixed size would swallow the screen at low resolutions. Drop
	// PG_SCALE_TWEAK below 1 to shrink the whole HUD, raise it to grow it.
	const float PG_REF_HEIGHT   = 1080.f;
	const float PG_SCALE_TWEAK  = 1.00f;
	const float PG_SCALE_MIN    = 0.50f;
	const float PG_SCALE_MAX    = 1.40f;
	const float PG_BOTTOM_GAP   = 0.f;		// art px left below the plate

	// Filled in once per frame by RenderPegasusHUD.
	float g_fPgScale = 1.f;
	float g_fPgOrgX  = 0.f;
	float g_fPgOrgY  = 0.f;

	// Displayed fill levels, chasing the real ones so the gems and gauges slide
	// rather than jump. Start at zero so the first frame snaps to full.
	const float PG_EASE_RATE = 0.15f;
	const float PG_EASE_SNAP = 0.85f;

	float g_fPgShownHP = 0.f;
	float g_fPgShownMP = 0.f;
	float g_fPgShownSD = 0.f;
	float g_fPgShownAG = 0.f;

	// Called once per frame per stat. A change bigger than PG_EASE_SNAP is a
	// map change or a relog rather than damage, so it lands immediately instead
	// of crawling across the bar.
	float PgEase(float& fShown, float fTarget)
	{
		if (fTarget < 0.f) fTarget = 0.f;
		if (fTarget > 1.f) fTarget = 1.f;

		float fDelta = fTarget - fShown;

		if (fDelta > PG_EASE_SNAP || fDelta < -PG_EASE_SNAP)
		{
			fShown = fTarget;
			return fShown;
		}

		float fStep = PG_EASE_RATE * FPS_ANIMATION_FACTOR;
		if (fStep > 1.f) fStep = 1.f;
		if (fStep < 0.f) fStep = 0.f;

		fShown += fDelta * fStep;

		if (fShown < 0.f) fShown = 0.f;
		if (fShown > 1.f) fShown = 1.f;
		return fShown;
	}

	inline float PgS(float a)  { return a * g_fPgScale; }
	inline float PgX(float ax) { return g_fPgOrgX + ax * g_fPgScale; }
	inline float PgY(float ay) { return g_fPgOrgY + ay * g_fPgScale; }

	// Centred on the screen, sitting on the bottom edge. Recomputed rather than
	// cached because the skill bar and the item hotkeys are drawn from passes
	// that don't always run after RenderPegasusHUD.
	void PgUpdateTransform()
	{
		g_fPgScale = ((float)DisplayHeight / PG_REF_HEIGHT) * PG_SCALE_TWEAK;
		if (g_fPgScale < PG_SCALE_MIN) g_fPgScale = PG_SCALE_MIN;
		if (g_fPgScale > PG_SCALE_MAX) g_fPgScale = PG_SCALE_MAX;

		g_fPgOrgX = ((float)DisplayWin - PG_ART_W * g_fPgScale) * 0.5f;
		g_fPgOrgY = (float)DisplayHeight - (PG_ART_H + PG_BOTTOM_GAP) * g_fPgScale;
	}

	// Experience bar: full plate width, hard against the bottom edge. The art
	// space is 1224 wide, and the plate's own decorative border eats about 70
	// either side, so the bar is inset to land inside it.
	const float PG_EXP_INSET  = 72.f;	// art px trimmed off each end
	const float PG_EXP_BOTTOM = 3.f;	// art px between bar and screen bottom
	const float PG_EXP_H      = 9.f;	// art px tall
	const float PG_SRC_EXP_TRACK_W = 1024.f, PG_SRC_EXP_TRACK_H = 17.f;
	const float PG_SRC_EXP_FILL_W  = 1024.f, PG_SRC_EXP_FILL_H  = 6.f;

	float PgExpX()
	{
		PgUpdateTransform();
		return PgX(PG_EXP_INSET);
	}

	float PgExpY()
	{
		PgUpdateTransform();
		return PgY(PG_ART_H - PG_EXP_BOTTOM);
	}

	float PgExpW()
	{
		PgUpdateTransform();
		return PgS(PG_ART_W - PG_EXP_INSET * 2.f);
	}

	float PgExpH()
	{
		PgUpdateTransform();
		return PgS(PG_EXP_H);
	}

	int PgExpFill()
	{
		return BITMAP_PG_HUD_EXP_FILL;
	}

	float PgExpFillU()
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_HUD_EXP_FILL);
		return (pTex && pTex->Width > 0.f) ? PG_SRC_EXP_FILL_W / pTex->Width : 1.f;
	}

	float PgExpFillV()
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_HUD_EXP_FILL);
		return (pTex && pTex->Height > 0.f) ? PG_SRC_EXP_FILL_H / pTex->Height : 1.f;
	}

	// Screen rect of one slot. 0..5 are the wide ones (five hotkey skills then
	// the current skill), 6..10 the narrow Q/W/E/R/T item slots.
	bool PgSlotRect(int iSlot, float& x, float& y, float& w, float& h)
	{
		if (iSlot < 0 || iSlot >= PG_SLOT_COUNT)
			return false;

		PgUpdateTransform();

		float acx, asz;
		if (iSlot < PG_SLOT_BIG_N)
		{
			acx = PG_SLOT_BIG_CX0 + PG_SLOT_BIG_PITCH * iSlot;
			asz = PG_SLOT_BIG_SZ;
		}
		else
		{
			acx = PG_SLOT_SML_CX0 + PG_SLOT_SML_PITCH * (iSlot - PG_SLOT_BIG_N);
			asz = PG_SLOT_SML_SZ;
		}

		w = PgS(asz);
		h = PgS(asz);
		x = PgX(acx) - w * 0.5f;
		y = PgY(PG_SLOT_CY) - h * 0.5f;
		return true;
	}

	// Maps a whole source image into a destination rect.
	//
	// Goes through RenderBitmap, not RenderImage's colour overload: that one
	// lands in RenderColorBitmap, which binds a texture but never enables
	// GL_TEXTURE_2D. If texturing happens to be off the quad comes out as a
	// flat translucent block. RenderBitmap is what the rest of the client uses.
	void PgBlit(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, float r = 1.f, float g = 1.f, float b = 1.f,
		float a = 1.f)
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(iImage);
		if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
			return;

		// Neither RenderBitmap nor RenderColorBitmap turns texturing on - they
		// only bind. CSprite::Render does it via the shared TextureEnable flag,
		// which is why sprites draw fine and these did not. Do the same here.
		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		EnableAlphaTest();
		glColor4f(r, g, b, a);

		RenderBitmap(iImage, x, y, w, h, 0.f, 0.f,
			fSrcW / pTex->Width, fSrcH / pTex->Height);

		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	// A narrow bright band that travels across an icon, drawn additively over
	// it. Built by re-drawing thin vertical slices of the icon itself with the
	// brightness falling off from the middle of the band - which means the
	// light only lands where the icon is actually opaque, instead of painting a
	// rectangle over it. No gradient texture needed.
	//
	// Each icon gets its own dwOffsetMs so the light ripples along a row rather
	// than every icon flashing at once.
	const int   PG_SHEEN_SLICES = 6;
	const float PG_SHEEN_BAND   = 0.38f;	// fraction of the icon it spans
	const float PG_SHEEN_PEAK   = 0.60f;	// brightness at the band's centre
	const DWORD PG_SHEEN_PERIOD = 2600;		// ms between passes
	const DWORD PG_SHEEN_SWEEP  = 850;		// ms the band takes to cross

	void PgSheen(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, DWORD dwOffsetMs)
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(iImage);
		if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
			return;

		DWORD dwT = (GetTickCount() + dwOffsetMs) % PG_SHEEN_PERIOD;
		if (dwT > PG_SHEEN_SWEEP)
			return;		// resting between passes

		// Centre of the band, run from just off the left edge to just off the
		// right so it enters and leaves cleanly.
		float fCentre = ((float)dwT / (float)PG_SHEEN_SWEEP)
			* (1.f + PG_SHEEN_BAND) - PG_SHEEN_BAND * 0.5f;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		EnableAlphaBlend();		// GL_ONE / GL_ONE

		float fSliceW = PG_SHEEN_BAND / PG_SHEEN_SLICES;
		float fU = fSrcW / pTex->Width;
		float fV = fSrcH / pTex->Height;

		for (int i = 0; i < PG_SHEEN_SLICES; ++i)
		{
			float fA = fCentre - PG_SHEEN_BAND * 0.5f + fSliceW * i;
			float fB = fA + fSliceW;

			if (fB <= 0.f || fA >= 1.f)
				continue;

			if (fA < 0.f) fA = 0.f;
			if (fB > 1.f) fB = 1.f;

			// Triangular falloff, brightest in the middle of the band.
			float fT = ((float)i + 0.5f) / PG_SHEEN_SLICES;
			float fLevel = (1.f - fabsf(fT * 2.f - 1.f)) * PG_SHEEN_PEAK;

			glColor4f(fLevel, fLevel, fLevel, 1.f);

			RenderBitmap(iImage,
				x + fA * w, y, (fB - fA) * w, h,
				fA * fU, 0.f, (fB - fA) * fU, fV);
		}

		// Back to the blend mode the rest of the HUD expects.
		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	// Art-space wrapper around PgBlit. x/y are the top-left corner.
	void PgDraw(int iImage, float ax, float ay, float aw, float ah,
		float fSrcW, float fSrcH, float r = 1.f, float g = 1.f, float b = 1.f)
	{
		PgBlit(iImage, PgX(ax), PgY(ay), PgS(aw), PgS(ah), fSrcW, fSrcH, r, g, b);
	}

	// Empty track plus the fill clipped to ratio. The fill's UV is cut to match
	// so the gradient reveals left to right instead of squashing.
	void PgGauge(int iFill, float ax, float ay, float aw, float ratio)
	{
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;

		PgDraw(BITMAP_PG_HUD_TRACK, ax, ay, aw, PG_GAUGE_H,
			PG_SRC_FILL_W, PG_SRC_FILL_H);

		if (ratio > 0.f)
		{
			PgBlit(iFill, PgX(ax), PgY(ay), PgS(aw) * ratio, PgS(PG_GAUGE_H),
				PG_SRC_FILL_W * ratio, PG_SRC_FILL_H);
		}
	}

	// A gem that empties as the stat drops. The socket is drawn dark and full
	// first, then the lit crystal is drawn over the bottom `ratio` of it with
	// its UV cropped to match, so the fill line tracks damage and regen instead
	// of the crystal just squashing.
	void PgGem(int iImage, float acx, float acy, float aw, float ah, float ratio,
		float r, float g, float b)
	{
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;

		float ax = acx - aw * 0.5f;
		float ay = acy - ah * 0.5f;

		BITMAP_t* pTex = Bitmaps.FindTexture(iImage);
		if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
			return;

		// Current cell of the loop. Driven off the clock rather than a frame
		// counter so the fire runs at the same speed whatever the frame rate.
		int iFrame = (int)((GetTickCount() / PG_GEM_FRAME_MS) % PG_GEM_FRAMES);
		float fCellU = PG_GEM_CELL / pTex->Width;
		float fCellV = PG_GEM_CELL / pTex->Height;
		float fU0 = (iFrame % PG_GEM_COLS) * fCellU;
		float fV0 = (iFrame / PG_GEM_COLS) * fCellV;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		// Spent part: the same frame, but dim enough and thin enough that it
		// reads as an unlit socket rather than a second full gem behind the
		// first. See PG_GEM_DRAIN_LEVEL.
		EnableAlphaTest();
		glColor4f(r * PG_GEM_DRAIN_LEVEL, g * PG_GEM_DRAIN_LEVEL,
			b * PG_GEM_DRAIN_LEVEL, PG_GEM_DRAIN_ALPHA);
		RenderBitmap(iImage, PgX(ax), PgY(ay), PgS(aw), PgS(ah),
			fU0, fV0, fCellU, fCellV);
		glColor4f(1.f, 1.f, 1.f, 1.f);

		if (ratio <= 0.f)
			return;

		// The line runs the full height of the drawn gem, so a full stat fills
		// the crystal to its point and an empty one leaves nothing lit.
		//
		// It used to be measured against the socket opening instead, which is
		// shorter than the quad - the gem is drawn oversized on purpose so it
		// reaches the bevel. That left the top of the crystal unfilled even at
		// 100%, a permanent empty band, which is far more noticeable than the
		// small dead zone at each end of the range that this trades it for.
		float fLineY = ay + ah * (1.f - ratio);
		if (fLineY < ay) fLineY = ay;
		if (fLineY > ay + ah) fLineY = ay + ah;

		float fKeepH = (ay + ah) - fLineY;
		if (fKeepH <= 0.f)
			return;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		// v grows downward in this loader, so the kept part starts at the
		// fraction of the cell that has drained away. Everything stays inside
		// the current frame's cell - the crop is relative to it, not the sheet.
		float fVOff = fV0 + fCellV * ((fLineY - ay) / ah);
		float fVLen = fCellV * (fKeepH / ah);

		float dx = PgX(ax), dy = PgY(fLineY);
		float dw = PgS(aw), dh = PgS(fKeepH);

		// Laying the frame down twice deepens the colour without touching the
		// art, which is only part-opaque.
		EnableAlphaTest();
		glColor4f(r, g, b, 1.f);

		for (int i = 0; i < PG_GEM_PASSES; ++i)
			RenderBitmap(iImage, dx, dy, dw, dh, fU0, fVOff, fCellU, fVLen);

		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	// Shield, drawn as a shell over the life gem in the same socket.
	//
	// Damage in this game lands on shield before it reaches health, so the two
	// belong in one place: the green crystal covers the fire while the shield
	// holds, and its line recedes downward as the shield is spent, uncovering
	// the fire from the top. The moment the green is gone is the moment hits
	// start costing health, which is the thing worth seeing at a glance.
	//
	// It shares PgGem's fill maths so the two lines move on the same scale, but
	// skips the darkened base: what shows through where the shield is spent is
	// the life gem underneath, not a dimmed copy of the shield.
	void PgGemShell(int iImage, float acx, float acy, float aw, float ah,
		float ratio, float fAlpha)
	{
		if (ratio <= 0.f || fAlpha <= 0.f)
			return;
		if (ratio > 1.f) ratio = 1.f;

		BITMAP_t* pTex = Bitmaps.FindTexture(iImage);
		if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
			return;

		float ax = acx - aw * 0.5f;
		float ay = acy - ah * 0.5f;

		int iFrame = (int)((GetTickCount() / PG_GEM_FRAME_MS) % PG_GEM_FRAMES);
		float fCellU = PG_GEM_CELL / pTex->Width;
		float fCellV = PG_GEM_CELL / pTex->Height;
		float fU0 = (iFrame % PG_GEM_COLS) * fCellU;
		float fV0 = (iFrame / PG_GEM_COLS) * fCellV;

		// Same full-height mapping as PgGem, so the two lines move together.
		float fLineY = ay + ah * (1.f - ratio);
		if (fLineY < ay) fLineY = ay;
		if (fLineY > ay + ah) fLineY = ay + ah;

		float fKeepH = (ay + ah) - fLineY;
		if (fKeepH <= 0.f)
			return;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		float fVOff = fV0 + fCellV * ((fLineY - ay) / ah);
		float fVLen = fCellV * (fKeepH / ah);

		// One pass, and translucent, so the fire stays legible underneath
		// instead of being replaced. Two passes here would seal the socket.
		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, fAlpha);
		RenderBitmap(iImage, PgX(ax), PgY(fLineY), PgS(aw), PgS(fKeepH),
			fU0, fVOff, fCellU, fVLen);
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	// Caption centred in an art-space box. RenderText works in the same virtual
	// units as DisplayWin/DisplayHeight - both are the window size divided by
	// g_fScreenRate - so PgX/PgY feed it directly. The font itself is a fixed
	// pixel size and does not scale with the plate.
	void PgLabel(float acx, float acy, float aw, const char* szText, DWORD dwColor)
	{
		if (szText == NULL || *szText == '\0')
			return;

		float w = PgS(aw);

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(dwColor);
		g_pRenderText->RenderText((int)(PgX(acx) - w * 0.5f), (int)(PgY(acy) - 7.f),
			szText, (int)w, 0, RT3_SORT_CENTER);
		g_pRenderText->SetTextColor(CLRDW_WHITE);
	}

	// Digits, centred on (acx, acy) in art space, grouped in threes the way the
	// reference HUD shows them ("158 810"). CNewUIRenderNumber can't do the
	// grouping and always centres, so this walks the glyph sheet directly.
	void PgDigits(float acx, float acy, __int64 iValue, float fArtHeight,
		float r = 1.f, float g = 1.f, float b = 1.f)
	{
		if (iValue < 0) iValue = 0;

		char szNum[32];
		_i64toa(iValue, szNum, 10);
		int iLen = (int)strlen(szNum);

		float h = PgS(fArtHeight);
		float w = h * (12.f / 14.f);		// glyph aspect in the sheet
		float fAdvance = w * 0.80f;			// same tightening the stock numbers use
		float fGap = w * 0.35f;				// thousands separator

		int iGroups = (iLen - 1) / 3;
		float fTotal = fAdvance * iLen + fGap * iGroups;

		float x = PgX(acx) - fTotal * 0.5f;
		float y = PgY(acy) - h * 0.5f;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		EnableAlphaTest();
		glColor4f(r, g, b, 1.f);

		for (int i = 0; i < iLen; ++i)
		{
			float fU = (float)(szNum[i] - '0') * 12.f / 128.f;
			RenderBitmap(SEASON3B::CNewUIRenderNumber::IMAGE_NUMBER1,
				x, y, w, h, fU, 0.f, 12.f / 128.f, 14.f / 16.f);
			x += fAdvance;

			// A separator sits after this glyph when the number of digits still
			// to come is a multiple of three.
			int iLeft = iLen - 1 - i;
			if (iLeft > 0 && (iLeft % 3) == 0)
				x += fGap;
		}

		glColor4f(1.f, 1.f, 1.f, 1.f);
	}
}


bool SEASON3B::GetPegasusHudTransform(float& fOrgX, float& fOrgY, float& fScale)
{
#if PG_USE_PEGASUS_HUD
	PgUpdateTransform();
	fOrgX = g_fPgOrgX;
	fOrgY = g_fPgOrgY;
	fScale = g_fPgScale;
	return true;
#else
	fOrgX = fOrgY = 0.f;
	fScale = 1.f;
	return false;
#endif
}

void SEASON3B::CNewUIMainFrameWindow::RenderPegasusHUD()
{
	static bool s_bLoaded = false;
	if (s_bLoaded == false)
	{
		const char* const szDir = "Interface\\Pegasus\\Pegasus_Main\\";
		char szPath[256];

		sprintf(szPath, "%spg_hud_bar.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_BAR, GL_LINEAR);
		sprintf(szPath, "%spg_hud_gem.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_GEM, GL_LINEAR);
		sprintf(szPath, "%spg_gem_hp.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_GEM_HP, GL_LINEAR);
		sprintf(szPath, "%spg_gem_mp.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_GEM_MP, GL_LINEAR);
		sprintf(szPath, "%spg_gem_sd.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_GEM_SD, GL_LINEAR);
		sprintf(szPath, "%spg_hud_fill_sd.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_HP, GL_LINEAR);
		sprintf(szPath, "%spg_hud_fill_ag.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_MP, GL_LINEAR);
		sprintf(szPath, "%spg_hud_track.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_TRACK, GL_LINEAR);
		sprintf(szPath, "%spg_hud_btn.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_BTN, GL_LINEAR);
		sprintf(szPath, "%sTournamentMain_I121.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_GIFT, GL_LINEAR);
		// The backslashes here have to be doubled like everywhere else. Written
		// singly, \P and \D are not escape sequences and the compiler drops the
		// backslash, so the path came out as one run-on word and the rune never
		// loaded - the button was there and clickable, just invisible.
		LoadBitmap("Interface\\Pegasus\\Pegasus_Dungeon\\Pegasus_Dungeon_I19.tga",
			BITMAP_PG_HUD_DUNGEON, GL_LINEAR);
		sprintf(szPath, "%sTournamentMain_I123.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_COLLECT, GL_LINEAR);
		LoadBitmap("Interface\\Pegasus\\Pegasus_Daily\\TournamentDaily_I11.tga",
			BITMAP_PG_HUD_DAILY, GL_LINEAR);
		// The picker borrows the laurel and is tinted apart from the collection
		// button beside it, so no new asset has to be cut for it.
		sprintf(szPath, "%sTournamentMain_I123.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_NFX, GL_LINEAR);
		sprintf(szPath, "%spg_exp_track.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_EXP_TRACK, GL_LINEAR);
		sprintf(szPath, "%spg_exp_fill.tga", szDir);
		LoadBitmap(szPath, BITMAP_PG_HUD_EXP_FILL, GL_LINEAR);

		s_bLoaded = true;
	}

	PgUpdateTransform();

	// --- the four stats ---
	DWORD dwMaxHP = CharacterAttribute->PrintPlayer.ViewMaxHP;
	DWORD dwHP    = min(max(0, CharacterAttribute->PrintPlayer.ViewCurHP), dwMaxHP);
	DWORD dwMaxMP = CharacterAttribute->PrintPlayer.ViewMaxMP;
	DWORD dwMP    = min(max(0, CharacterAttribute->PrintPlayer.ViewCurMP), dwMaxMP);
	DWORD dwMaxSD = max(1, CharacterAttribute->PrintPlayer.ViewMaxSD);
	DWORD dwSD    = min(dwMaxSD, (DWORD)CharacterAttribute->PrintPlayer.ViewCurSD);
	DWORD dwMaxAG = max(1, CharacterAttribute->PrintPlayer.ViewMaxBP);
	DWORD dwAG    = min(dwMaxAG, (DWORD)CharacterAttribute->PrintPlayer.ViewCurBP);

	// Low-health heartbeat, the one behaviour the stock RenderLifeMana had that
	// isn't purely cosmetic.
	if (dwMaxHP > 0 && dwHP > 0 && (dwHP / (float)dwMaxHP) < 0.2f)
		PlayBuffer(SOUND_HEART);

	// --- gems first: they sit in holes cut through the plate, so drawing them
	// underneath lets the frame's bevel close over the edges cleanly. Both
	// drain as the stat drops, which is the only feedback there is now that the
	// stock life and mana gauges are gone. ---
	// All four slide toward their real value instead of snapping, so a hit
	// visibly drains the gem and a potion visibly refills it.
	float fHP = PgEase(g_fPgShownHP, (dwMaxHP > 0) ? (float)dwHP / (float)dwMaxHP : 0.f);
	float fMP = PgEase(g_fPgShownMP, (dwMaxMP > 0) ? (float)dwMP / (float)dwMaxMP : 0.f);
	float fSD = PgEase(g_fPgShownSD, (float)dwSD / (float)dwMaxSD);
	float fAG = PgEase(g_fPgShownAG, (float)dwAG / (float)dwMaxAG);

	// The frames are already coloured, so these go in at full white rather than
	// tinted the way the old neutral diamond had to be.
	PgGem(BITMAP_PG_HUD_GEM_HP, PG_GEM_L_X, PG_GEM_Y, PG_GEM_W, PG_GEM_H, fHP,
		1.f, 1.f, 1.f);

	PgGem(BITMAP_PG_HUD_GEM_MP, PG_GEM_R_X, PG_GEM_Y, PG_GEM_W, PG_GEM_H, fMP,
		1.f, 1.f, 1.f);

	// --- gauges, also under the plate so the links divide them into segments ---
	PgGauge(BITMAP_PG_HUD_HP, PG_GAUGE_L_X, PG_GAUGE_Y, PG_GAUGE_L_W, fSD);
	PgGauge(BITMAP_PG_HUD_MP, PG_GAUGE_R_X, PG_GAUGE_Y, PG_GAUGE_R_W, fAG);

	// --- the plate itself, over both ---
	PgDraw(BITMAP_PG_HUD_BAR, 0.f, 0.f, PG_ART_W, PG_ART_H,
		PG_SRC_BAR_W, PG_SRC_BAR_H);

	// --- empty experience trough, pinned to the bottom edge. The gold fill
	// goes over it later, from RenderExperience, which owns the progress
	// arithmetic including the master-level case. ---
	PgBlit(BITMAP_PG_HUD_EXP_TRACK, PgExpX(), PgExpY(), PgExpW(), PgExpH(),
		PG_SRC_EXP_TRACK_W, PG_SRC_EXP_TRACK_H);

	// --- PegaShop / Start Auto plates and the Battle Core tab. These are drawn
	// only; nothing is wired to a click yet. ---
	PgDraw(BITMAP_PG_HUD_BTN, PG_BTN_L_X - PG_BTN_W * 0.5f, PG_BTN_Y - PG_BTN_H * 0.5f,
		PG_BTN_W, PG_BTN_H, PG_SRC_BTN_W, PG_SRC_BTN_H);

	PgDraw(BITMAP_PG_HUD_BTN, PG_BTN_R_X - PG_BTN_W * 0.5f, PG_BTN_Y - PG_BTN_H * 0.5f,
		PG_BTN_W, PG_BTN_H, PG_SRC_BTN_W, PG_SRC_BTN_H);

	PgLabel(PG_BTN_L_X, PG_BTN_Y, PG_BTN_W, PG_LABEL_L, CLRDW_WHITE);
	// The plate says what the next click will do, so its state is readable
	// without hovering it.
	const bool bAutoOn =
		g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Started ? true : false;
	PgLabel(PG_BTN_R_X, PG_BTN_Y, PG_BTN_W,
		bAutoOn ? "Stop Auto" : PG_LABEL_R, CLRDW_WHITE);

	// Right plate carries what the two small buttons beside the map bar used
	// to: left click starts or stops the helper, right click opens its
	// settings window. Both live here now so the bar stays a bar.
	{
		static DWORD s_dwAutoClick = 0;

		float rx = PgX(PG_BTN_R_X - PG_BTN_W * 0.5f);
		float ry = PgY(PG_BTN_Y - PG_BTN_H * 0.5f);

		if (SEASON3B::CheckMouseIn(rx, ry, PgS(PG_BTN_W), PgS(PG_BTN_H)) == true)
		{
			RenderTipText((int)rx, (int)(ry - 18.f),
				bAutoOn ? "Stop Auto  -  right click for settings"
				: "Start Auto  -  right click for settings");

			// The 300ms gate is the one JShowButtonPlayAuto used. Without it a
			// single press toggles on several consecutive frames.
			if (GetTickCount() - s_dwAutoClick > 300)
			{
				if (SEASON3B::IsPress(VK_LBUTTON) == true)
				{
					s_dwAutoClick = GetTickCount();
					StartMuHelper(bAutoOn ? 1 : 0);
					PlayBuffer(SOUND_CLICK01);
				}
				else if (SEASON3B::IsPress(VK_RBUTTON) == true)
				{
					s_dwAutoClick = GetTickCount();
					g_pNewUISystem->Toggle(SEASON3B::INTERFACE_MuHelper);
					PlayBuffer(SOUND_CLICK01);
				}
			}
		}
	}

	// Left plate opens the same custom menu as F5.
	{
		float lx = PgX(PG_BTN_L_X - PG_BTN_W * 0.5f);
		float ly = PgY(PG_BTN_Y - PG_BTN_H * 0.5f);

		if (SEASON3B::CheckMouseIn(lx, ly, PgS(PG_BTN_W), PgS(PG_BTN_H)) == true
			&& SEASON3B::IsPress(VK_LBUTTON) == true)
		{
			gBCustomMenu.OpenCustomMenu();
		}
	}


	// --- name effects, first in the row ---
	{
		float nx = PgX(PG_NFX_X - PG_NFX_SZ * 0.5f);
		float ny = PgY(PG_NFX_Y - PG_NFX_SZ * 0.5f);
		float ns = PgS(PG_NFX_SZ);

		bool bHover = SEASON3B::CheckMouseIn(nx, ny, ns, ns);

		// The same 224x176 laurel the collection button uses, held to its own
		// aspect and tinted violet so the two do not read as the same thing.
		float nw = ns, nh = ns * (PG_SRC_NFX_H / PG_SRC_NFX_W);

		PgBlit(BITMAP_PG_HUD_NFX, nx, ny + (ns - nh) * 0.5f, nw, nh,
			PG_SRC_NFX_W, PG_SRC_NFX_H, 0.78f, 0.55f, 1.f, bHover ? 1.f : 0.85f);

		PgSheen(BITMAP_PG_HUD_NFX, nx, ny + (ns - nh) * 0.5f, nw, nh,
			PG_SRC_NFX_W, PG_SRC_NFX_H, 1200);

		if (bHover == true)
		{
			RenderTipText((int)nx, (int)(ny - 18.f), "Name Effect");

			if (SEASON3B::IsPress(VK_LBUTTON))
				gPegasusNameFx.Toggle();
		}
	}

	// --- daily rewards, first in the row ---
	{
		float dx = PgX(PG_DAILY_X - PG_DAILY_SZ * 0.5f);
		float dy = PgY(PG_DAILY_Y - PG_DAILY_SZ * 0.5f);
		float ds = PgS(PG_DAILY_SZ);

		bool bHover = SEASON3B::CheckMouseIn(dx, dy, ds, ds);

		PgBlit(BITMAP_PG_HUD_DAILY, dx, dy, ds, ds, PG_SRC_DAILY, PG_SRC_DAILY,
			1.f, 1.f, 1.f, bHover ? 1.f : 0.85f);

		// A larger offset runs the clock further ahead, so the row's sheen
		// offsets descend left to right and the light travels the same way.
		PgSheen(BITMAP_PG_HUD_DAILY, dx, dy, ds, ds, PG_SRC_DAILY, PG_SRC_DAILY, 900);

		if (bHover == true)
		{
			RenderTipText((int)dx, (int)(dy - 18.f), "Daily Rewards");

			if (SEASON3B::IsPress(VK_LBUTTON))
				gPegasusDaily.Toggle();
		}
	}

	// --- gift box: opens the rewards window. Pulses so it reads as something
	// waiting to be collected rather than a static decoration. ---
	{
		float gx = PgX(PG_GIFT_X - PG_GIFT_SZ * 0.5f);
		float gy = PgY(PG_GIFT_Y - PG_GIFT_SZ * 0.5f);
		float gs = PgS(PG_GIFT_SZ);

		bool bHover = SEASON3B::CheckMouseIn(gx, gy, gs, gs);

		PgBlit(BITMAP_PG_HUD_GIFT, gx, gy, gs, gs, PG_SRC_GIFT, PG_SRC_GIFT,
			1.f, 1.f, 1.f, bHover ? 1.f : 0.85f);

		PgSheen(BITMAP_PG_HUD_GIFT, gx, gy, gs, gs, PG_SRC_GIFT, PG_SRC_GIFT, 600);

		if (bHover == true)
		{
			RenderTipText((int)gx, (int)(gy - 18.f), "Battle Pass");

			if (SEASON3B::IsPress(VK_LBUTTON))
				gPegasusBattlePass.Toggle();
		}
	}

	// --- dungeon gateway, beside the gift box ---
	{
		float dx = PgX(PG_DUNG_X - PG_DUNG_SZ * 0.5f);
		float dy = PgY(PG_DUNG_Y - PG_DUNG_SZ * 0.5f);
		float ds = PgS(PG_DUNG_SZ);

		bool bHover = SEASON3B::CheckMouseIn(dx, dy, ds, ds);

		PgBlit(BITMAP_PG_HUD_DUNGEON, dx, dy, ds, ds, PG_SRC_DUNG, PG_SRC_DUNG,
			1.f, 1.f, 1.f, bHover ? 1.f : 0.85f);

		PgSheen(BITMAP_PG_HUD_DUNGEON, dx, dy, ds, ds, PG_SRC_DUNG, PG_SRC_DUNG, 300);

		if (bHover == true)
		{
			RenderTipText((int)dx, (int)(dy - 18.f), "Dungeons");

			if (SEASON3B::IsPress(VK_LBUTTON))
				gPegasusDungeon.Toggle();
		}
	}

	// --- collection, third in the row ---
	{
		float cx = PgX(PG_COLL_X - PG_COLL_SZ * 0.5f);
		float cy = PgY(PG_COLL_Y - PG_COLL_SZ * 0.5f);
		float cs = PgS(PG_COLL_SZ);

		bool bHover = SEASON3B::CheckMouseIn(cx, cy, cs, cs);

		// The art is 224x176, not square - keep that so the wreath is not
		// squashed, and centre it in the button's box.
		float cw = cs, ch = cs * (PG_SRC_COLL_H / PG_SRC_COLL_W);

		PgBlit(BITMAP_PG_HUD_COLLECT, cx, cy + (cs - ch) * 0.5f, cw, ch,
			PG_SRC_COLL_W, PG_SRC_COLL_H, 1.f, 1.f, 1.f, bHover ? 1.f : 0.85f);

		PgSheen(BITMAP_PG_HUD_COLLECT, cx, cy + (cs - ch) * 0.5f, cw, ch,
			PG_SRC_COLL_W, PG_SRC_COLL_H, 0);

		if (bHover == true)
		{
			RenderTipText((int)cx, (int)(cy - 18.f), "Collection");

			if (SEASON3B::IsPress(VK_LBUTTON))
				gPegasusCollection.Toggle();
		}
	}

	// --- gear button: opens the same custom menu as F5. Borrows the menu's own
	// round frame and cog icons, so it matches whatever art that ships with.
	// PgBlit skips silently if those textures aren't resident. ---
	{
		float gx = PgX(PG_GEAR_X - PG_GEAR_SZ * 0.5f);
		float gy = PgY(PG_GEAR_Y - PG_GEAR_SZ * 0.5f);
		float gs = PgS(PG_GEAR_SZ);

		bool bHover = SEASON3B::CheckMouseIn(gx, gy, gs, gs);

		PgBlit(INTERFACE_HERO_POSITION_INFO + 155, gx, gy, gs, gs, 74.f, 74.f,
			1.f, 1.f, 1.f, bHover ? 1.f : 0.65f);

		PgBlit(INTERFACE_HERO_POSITION_INFO + 300,
			gx + gs * 0.28f, gy + gs * 0.28f, gs * 0.44f, gs * 0.44f, 44.f, 39.f);

		if (bHover == true)
		{
			RenderTipText((int)gx, (int)(gy - 18.f), GlobalText[363]);

			if (SEASON3B::IsPress(VK_LBUTTON))
				gBCustomMenu.OpenCustomMenu();
		}
	}

	// --- readouts last, so nothing paints over them ---
	PgDigits(PG_GEM_L_X, PG_GEM_Y, dwHP, PG_DIGIT_H_GEM);
	PgDigits(PG_GEM_R_X, PG_GEM_Y, dwMP, PG_DIGIT_H_GEM);
	PgDigits(PG_NUM_L_X, PG_NUM_GAUGE_Y, dwSD, PG_DIGIT_H_GAUGE);
	PgDigits(PG_NUM_R_X, PG_NUM_GAUGE_Y, dwAG, PG_DIGIT_H_GAUGE);

	// --- tooltips, matching what the stock gauges offered ---
	char strTipText[256];
	float fTipY = PgY(PG_GEM_Y) - 60.f;

	if (SEASON3B::CheckMouseIn(PgX(PG_GEM_L_X - PG_GEM_W * 0.5f), PgY(PG_GEM_Y - PG_GEM_H * 0.5f),
		PgS(PG_GEM_W), PgS(PG_GEM_H)) == true)
	{
		sprintf(strTipText, GlobalText[358], dwHP, dwMaxHP);
		RenderTipText((int)PgX(PG_GEM_L_X), (int)fTipY, strTipText);
	}
	else if (SEASON3B::CheckMouseIn(PgX(PG_GEM_R_X - PG_GEM_W * 0.5f), PgY(PG_GEM_Y - PG_GEM_H * 0.5f),
		PgS(PG_GEM_W), PgS(PG_GEM_H)) == true)
	{
		sprintf(strTipText, GlobalText[359], dwMP, dwMaxMP);
		RenderTipText((int)PgX(PG_GEM_R_X), (int)fTipY, strTipText);
	}
	else if (SEASON3B::CheckMouseIn(PgX(PG_GAUGE_L_X), PgY(PG_GAUGE_Y - 4.f),
		PgS(PG_GAUGE_L_W), PgS(PG_GAUGE_H + 8.f)) == true)
	{
		sprintf(strTipText, GlobalText[2037], dwSD, dwMaxSD);
		RenderTipText((int)PgX(PG_NUM_L_X), (int)fTipY, strTipText);
	}
	else if (SEASON3B::CheckMouseIn(PgX(PG_GAUGE_R_X), PgY(PG_GAUGE_Y - 4.f),
		PgS(PG_GAUGE_R_W), PgS(PG_GAUGE_H + 8.f)) == true)
	{
		sprintf(strTipText, GlobalText[214], dwAG, dwMaxAG);
		RenderTipText((int)PgX(PG_NUM_R_X), (int)fTipY, strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderFrame()
{
#if PG_USE_PEGASUS_HUD
	// The Pegasus bar replaces the stock one outright - drawing both would
	// leave the old frame showing through around the edges.
	RenderPegasusHUD();
	return;
#endif

	float width, height;
	float x, y;
	if (gProtect.m_MainInfo.IsVersion == 1)
	{

		width = 640.f; height = 61.f;
		x = DisplayWinExt;
		y = (DisplayHeight)-height;
		SEASON3B::RenderImage(IMAGE_MENU_1, x, y, width, height);

	}
	else
	{
		width = 256.f; height = 51.f;
		x = DisplayWinExt;
		y = (DisplayHeight)-height;
		RenderBitmap(IMAGE_DECOR_WIDE, x - 70.28048780487805, y, 70.28048780487805, height, 0.0, 0.0, 0.8785000443, 0.6409999728, 1, 1, 0.0);
		SEASON3B::RenderImage(IMAGE_MENU_1, x, y, width, height);

		width = 128.f;
		x = 256.f + DisplayWinExt;
		SEASON3B::RenderImage(IMAGE_MENU_2, x, y, width, height);

		width = 256.f;
		x = 256.f + 128.f + DisplayWinExt;

		SEASON3B::RenderImage(IMAGE_MENU_3, x, y, width, height);
		RenderBitmap(IMAGE_DECOR_WIDE, x + width, y, 70.28048780487805, height, 0.8785000443, 0.0, -0.8785000443, 0.6409999728, 1, 1, 0.0);

		if (g_pSkillList->IsSkillListUp() == true)
		{
			width = 160.f; height = 40.f;
			x = 222.f + DisplayWinExt;
			SEASON3B::RenderImage(IMAGE_MENU_2_1, x, y, width, height);
		}
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderLifeMana()
{
#if PG_USE_PEGASUS_HUD
	// Pegasus draws its own gauges in RenderPegasusHUD.
	return;
#endif

	DWORD wLifeMax, wLife, wManaMax, wMana;

	{
		wLifeMax = CharacterAttribute->PrintPlayer.ViewMaxHP;
		wLife = min(max(0, CharacterAttribute->PrintPlayer.ViewCurHP), wLifeMax);
		wManaMax = CharacterAttribute->PrintPlayer.ViewMaxMP;
		wMana = min(max(0, CharacterAttribute->PrintPlayer.ViewCurMP), wManaMax);
	}

	if (wLifeMax > 0)
	{
		if (wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			PlayBuffer(SOUND_HEART);
		}
	}

	float fLife = 0.f;
	float fMana = 0.f;

	if (wLifeMax > 0)
	{
		fLife = (wLifeMax - wLife) / (float)wLifeMax;
	}
	if (wManaMax > 0)
	{
		fMana = (wManaMax - wMana) / (float)wManaMax;
	}

	float width, height;
	float x, y;
	float fY, fH, fV;

	width = 45.f;
	x = 158 + DisplayWinExt;
	height = 39.f;
	y = DisplayHeight - 48.f;

	fY = y + (fLife * height);
	fH = height - (fLife * height);
	fV = fLife;
	if (g_isCharacterBuff((&Hero->Object), eDeBuff_Poison))
	{
		RenderBitmap(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}
	else
	{
		RenderBitmap(IMAGE_GAUGE_RED, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
	}

	SEASON3B::RenderNumber(x + 25, (480 + DisplayHeightExt) - 38, wLife);

	char strTipText[256];
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[358], wLife, wLifeMax);
		RenderTipText((int)x, (int)(418 + DisplayHeightExt), strTipText);
	}

	width = 45.f;
	x = 256.f + 128.f + 53.f + DisplayWinExt;
	height = 39.f;
	y = DisplayHeight - 48.f;

	fY = y + (fMana * height);
	fH = height - (fMana * height);
	fV = fMana;
	RenderBitmap(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);

	SEASON3B::RenderNumber(x + 30, (480 + DisplayHeightExt) - 38, wMana);

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[359], wMana, wManaMax);
		RenderTipText((int)x, (int)(418 + DisplayHeightExt), strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageAG()
{
#if PG_USE_PEGASUS_HUD
	// Pegasus draws its own gauges in RenderPegasusHUD.
	return;
#endif

	float x, y, width, height;
	float fY, fH, fV;

	DWORD dwMaxSkillMana, dwSkillMana;

	{
		dwMaxSkillMana = max(1, CharacterAttribute->PrintPlayer.ViewMaxBP);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->PrintPlayer.ViewCurBP);
	}

	float fSkillMana = 0.0f;

	if (dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

	width = 16.f, height = 39.f;
	x = 256 + 128 + 36 + DisplayWinExt; y = DisplayHeight - 49.f;
	fY = y + (fSkillMana * height);
	fH = height - (fSkillMana * height);
	fV = fSkillMana;

	RenderBitmap(IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV * height / 64.f, width / 16.f, (1.0f - fV) * height / 64.f);
	SEASON3B::RenderNumber(x + 10, (480 + DisplayHeightExt) - 18, (int)dwSkillMana);

	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[214], dwSkillMana, dwMaxSkillMana);
		RenderTipText((int)x - 20, (int)(418 + DisplayHeightExt), strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageSD()
{
#if PG_USE_PEGASUS_HUD
	// Pegasus draws its own gauges in RenderPegasusHUD.
	return;
#endif

	float x, y, width, height;
	float fY, fH, fV;
	DWORD wMaxShield, wShield;

	{
		wMaxShield = max(1, CharacterAttribute->PrintPlayer.ViewMaxSD);
		wShield = min(wMaxShield, CharacterAttribute->PrintPlayer.ViewCurSD);
	}

	float fShield = 0.0f;

	if (wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}

	width = 16.f, height = 39.f;
	x = 204 + DisplayWinExt; y = DisplayHeight - 49.f;
	fY = y + (fShield * height);
	fH = height - (fShield * height);
	fV = fShield;

	RenderBitmap(IMAGE_GAUGE_SD, x, fY, width, fH, 0.f, fV * height / 64.f, width / 16.f, (1.0f - fV) * height / 64.f);
	SEASON3B::RenderNumber(x + 15, (480 + DisplayHeightExt) - 18, (int)wShield);

	height = 39.f;
	y = (480.f + DisplayHeightExt) - 10.f - 39.f;
	if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		char strTipText[256];

		sprintf(strTipText, GlobalText[2037], wShield, wMaxShield);
		RenderTipText((int)x - 20, (int)(418 + DisplayHeightExt), strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderExperience()
{

	__int64 wLevel;
	__int64 dwNexExperience;
	__int64 dwExperience;
	double x, y, width, height;

	{
		wLevel = CharacterAttribute->Level;
		dwNexExperience = CharacterAttribute->NextExperince;
		dwExperience = CharacterAttribute->Experience;
	}

	if (gCharacterManager.IsMasterLevel(CharacterAttribute->Class) == true)
	{
		x = DisplayWinExt; y = 470 + DisplayHeightExt; width = 6; height = 4;

		__int64 iTotalLevel = wLevel + 400;
		__int64 iTOverLevel = iTotalLevel - 255;
		__int64 iBaseExperience = 0;

		__int64 iData_Master =
			(
				(
					(__int64)9 + (__int64)iTotalLevel
					)
				* (__int64)iTotalLevel
				* (__int64)iTotalLevel
				* (__int64)10
				)
			+
			(
				(
					(__int64)9 + (__int64)iTOverLevel
					)
				* (__int64)iTOverLevel
				* (__int64)iTOverLevel
				* (__int64)1000
				);
		iBaseExperience = (iData_Master - (__int64)3892250000) / (__int64)2;

		double fNeedExp = (double)dwNexExperience - (double)iBaseExperience;
		double fExp = (double)dwExperience - (double)iBaseExperience;

		if (dwExperience < iBaseExperience)
		{
			fExp = 0.f;
		}

		double fExpBarNum = 0.f;
		if (fExp > 0.f && fNeedExp > 0)
		{
			fExpBarNum = ((double)fExp / (double)fNeedExp) * (double)10.f;
		}

		double fProgress = fExpBarNum - __int64(fExpBarNum);

		if (m_bExpEffect == true)
		{
			double fPreProgress = 0.f;
			double fPreExp = (double)m_loPreExp - (double)iBaseExperience;
			if (m_loPreExp < iBaseExperience)
			{
				x = PgExpX(); y = PgExpY(); width = fProgress * PgExpW(); height = PgExpH();
				RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
				glColor4f(1.f, 1.f, 1.f, 0.6f);
				RenderColor(x, y, width, height);
				EndRenderColor();
			}
			else
			{
				int iPreExpBarNum = 0;
				int iExpBarNum = 0;
				if (fPreExp > 0.f && fNeedExp > 0.f)
				{
					fPreProgress = ((double)fPreExp / (double)fNeedExp) * (double)10.f;
					iPreExpBarNum = (int)fPreProgress;
					fPreProgress = (double)fPreProgress - __int64(fPreProgress);
				}
				iExpBarNum = (int)fExpBarNum;

				if (iExpBarNum > iPreExpBarNum)
				{
					x = PgExpX(); y = PgExpY(); width = fProgress * PgExpW(); height = PgExpH();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					glColor4f(1.f, 1.f, 1.f, 0.6f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
				else
				{
					double fGapProgress = 0.f;
					fGapProgress = (double)fProgress - (double)fPreProgress;
					x = PgExpX(); y = PgExpY(); width = (double)fPreProgress * (double)PgExpW(); height = PgExpH();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());

					x += width; width = (double)fGapProgress * (double)PgExpW();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					glColor4f(1.f, 1.f, 1.f, 0.6f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
			}
		}
		else
		{
			x = PgExpX(); y = PgExpY(); width = fProgress * PgExpW(); height = PgExpH();
			RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
		}

		int iExp = (int)fExpBarNum;
		x = 635.f + DisplayWinExt; y = 469.f + DisplayHeightExt;
		SEASON3B::RenderNumber(x, y, iExp);

		x = PgExpX(); y = PgExpY(); width = PgExpW(); height = PgExpH();
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			char strTipText[256];

			sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
			RenderTipText(280 + DisplayWinExt, 418 + DisplayHeightExt, strTipText);
		}
	}
	else
	{
		x = 0 + DisplayWinExt; y = 470 + DisplayHeightExt; width = 6; height = 4;

		WORD wPriorLevel = wLevel - 1;
		DWORD dwPriorExperience = 0;

		if (wPriorLevel > 0)
		{
			dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

			if (wPriorLevel > 255)
			{
				int iLevelOverN = wPriorLevel - 255;
				dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
			}
		}

		float fNeedExp = dwNexExperience - dwPriorExperience;
		float fExp = dwExperience - dwPriorExperience;

		if (dwExperience < dwPriorExperience)
		{
			fExp = 0.f;
		}

		float fExpBarNum = 0.f;
		if (fExp > 0.f && fNeedExp > 0)
		{
			fExpBarNum = (fExp / fNeedExp) * 10.f;
		}

		float fProgress = fExpBarNum;
		fProgress = fProgress - (int)fProgress;

		if (m_bExpEffect == true)
		{
			float fPreProgress = 0.f;
			fExp = m_dwPreExp - dwPriorExperience;
			if (m_dwPreExp < dwPriorExperience)
			{
				x = PgExpX(); y = PgExpY(); width = fProgress * PgExpW(); height = PgExpH();
				RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
				glColor4f(1.f, 1.f, 1.f, 0.4f);
				RenderColor(x, y, width, height);
				EndRenderColor();
			}
			else
			{
				int iPreExpBarNum = 0;
				int iExpBarNum = 0;
				if (fExp > 0.f && fNeedExp > 0.f)
				{
					fPreProgress = (fExp / fNeedExp) * 10.f;
					iPreExpBarNum = (int)fPreProgress;
					fPreProgress = fPreProgress - (int)fPreProgress;
				}

				iExpBarNum = (int)fExpBarNum;

				if (iExpBarNum > iPreExpBarNum)
				{
					x = PgExpX(); y = PgExpY(); width = fProgress * PgExpW(); height = PgExpH();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					glColor4f(1.f, 1.f, 1.f, 0.4f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
				else
				{
					float fGapProgress = 0.f;
					fGapProgress = fProgress - fPreProgress;
					x = PgExpX(); y = PgExpY(); width = fPreProgress * PgExpW(); height = PgExpH();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					x += width; width = fGapProgress * PgExpW();
					RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
					glColor4f(1.f, 1.f, 1.f, 0.4f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
			}
		}
		else
		{
			x = PgExpX(); y = PgExpY(); width = fProgress * PgExpW(); height = PgExpH();
			RenderBitmap(PgExpFill(), x, y, width, height, 0.f, 0.f, PgExpFillU(), PgExpFillV());
		}

		int iExp = (int)fExpBarNum;
		x = 635.f + DisplayWinExt; y = 469.f + DisplayHeightExt;
		SEASON3B::RenderNumber(x, y, iExp);

		x = PgExpX(); y = PgExpY(); width = PgExpW(); height = PgExpH();
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			char strTipText[256];

			sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
			RenderTipText(280 + DisplayWinExt, 418 + DisplayHeightExt, strTipText);
		}
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderHotKeyItemCount()
{
	m_ItemHotKey.RenderItemCount();
}

void SEASON3B::CNewUIMainFrameWindow::RenderButtons()
{
#if PG_USE_PEGASUS_HUD
	// Dropped: the sockets they sat in hold the potion hotkeys, and the custom
	// menu already opens everything these did. Their mouse handling is switched
	// off in UpdateMouseEvent to match.
	return;
#endif

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_BtnCShop.Render();
#endif

	RenderCharInfoButton();
	m_BtnMyInven.Render();

	RenderFriendButton();

	m_BtnWindow.Render();
}

void SEASON3B::CNewUIMainFrameWindow::RenderCharInfoButton()
{
	m_BtnChaInfo.Render();

	if (g_QuestMng.IsQuestIndexByEtcListEmpty())
		return;

	if (g_Time.GetTimeCheck(5, 500))
		m_bButtonBlink = !m_bButtonBlink;

	if (m_bButtonBlink)
	{
		if (!(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_QUEST_PROGRESS_ETC)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)))
		{
			RenderImage(IMAGE_MENU_BTN_CHAINFO, 489 + 30 + DisplayWinExt, (480 + DisplayHeightExt) - 51, 30, 41, 0.0f, 41.f);
		}

	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButton()
{
	m_BtnFriend.Render();

	int iBlinkTemp = g_pFriendMenu->GetBlinkTemp();
	BOOL bIsAlertTime = (iBlinkTemp % 24 < 12);

	if (g_pFriendMenu->IsNewChatAlert() && bIsAlertTime)
	{
		RenderFriendButtonState();
	}
	if (g_pFriendMenu->IsNewMailAlert())
	{
		if (bIsAlertTime)
		{
			RenderFriendButtonState();

			if (iBlinkTemp % 24 == 11)
			{
				g_pFriendMenu->IncreaseLetterBlink();
			}
		}
	}
	else if (g_pLetterList->CheckNoReadLetter())
	{
		RenderFriendButtonState();
	}

	g_pFriendMenu->IncreaseBlinkTemp();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButtonState()
{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, 489 + DisplayWinExt + (30 * 3), (480 + DisplayHeightExt) - 51, 30, 41, 0.0f, 123.f);
	}
	else
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, 489 + DisplayWinExt + (30 * 3), (480 + DisplayHeightExt) - 51, 30, 41, 0.0f, 41.f);
	}
#else
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, 488 + 76, 480 - 51, 38, 42, 0.0f, 126.f);
	}
	else
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, 488 + 76, 480 - 51, 38, 42, 0.0f, 42.f);
	}
#endif
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateMouseEvent()
{
	if (g_pNewUIHotKey->IsStateGameOver() == true)
	{
		return true;
	}

	if (BtnProcess() == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::BtnProcess()
{
	if (gProtect.m_MainInfo.IsVersion == 1)
	{
		if (SEASON3B::CheckMouseIn((int)ButtonSS2.x, (int)ButtonSS2.y, (int)ButtonSS2W, (int)ButtonSS2H))
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{

				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_WINDOW_MENU);
				PlayBuffer(SOUND_CLICK01);
				return true;
			}
		}

		else if (SEASON3B::CheckMouseIn((int)ButtonSS2.x + 259, (int)ButtonSS2.y, (int)ButtonSS2W, (int)ButtonSS2H))
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{

				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_GUILDINFO);
				PlayBuffer(SOUND_CLICK01);
				return true;
			}
		}

		else if (SEASON3B::CheckMouseIn((int)ButtonSS2.x + 259 + 30, (int)ButtonSS2.y, (int)ButtonSS2W, (int)ButtonSS2H))
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{

				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
				PlayBuffer(SOUND_CLICK01);
				return true;
			}
		}

		else if (SEASON3B::CheckMouseIn((int)ButtonSS2.x + 259 + 60, (int)ButtonSS2.y, (int)ButtonSS2W, (int)ButtonSS2H))
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{

				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);
				PlayBuffer(SOUND_CLICK01);
				return true;
			}
		}
	}

#if PG_USE_PEGASUS_HUD
	// The five stock buttons are gone - the Q/W/E/R/T sockets belong to the
	// potion hotkeys, and the custom menu covers everything these opened.
	// Their hit testing has to go too, or they keep swallowing clicks over the
	// sockets from behind, invisible.
	if (false)
#else
	if (g_pNewUIHotKey->CanUpdateKeyEventRelatedMyInventory() == true)
#endif
	{
		if (m_BtnMyInven.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);

		}
	}
#if PG_USE_PEGASUS_HUD
	else if (false)
#else
	else if (g_pNewUIHotKey->CanUpdateKeyEvent() == true)
#endif
	{
		if (m_BtnMyInven.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
		else if (m_BtnChaInfo.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);

			PlayBuffer(SOUND_CLICK01);

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
				g_QuestMng.SendQuestIndexByEtcSelection();

			return true;
		}
		else if (m_BtnFriend.UpdateMouseEvent() == true)
		{
			if (gMapManager.InChaosCastle() == true)
			{
				PlayBuffer(SOUND_CLICK01);
				return true;
			}

			int iLevel = CharacterAttribute->Level;

			if (iLevel < 6)
			{
				if (g_pChatListBox->CheckChatRedundancy(GlobalText[1067]) == FALSE)
				{
					g_pChatListBox->AddText("", GlobalText[1067], SEASON3B::TYPE_SYSTEM_MESSAGE);
				}
			}
			else
			{
				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_FRIEND);
			}
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
		else if (m_BtnWindow.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_WINDOW_MENU);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
		else if (m_BtnCShop.UpdateMouseEvent() == true)
		{ 
			if (gMapManager.InBloodCastle() == true)
			{
				return false;
			}
			if (gMapManager.InChaosCastle() == true)
			{
				return false;
			}
			if (g_pInGameShop->IsInGameShopOpen() == false)
				return false;

			if (g_InGameShopSystem->IsScriptDownload() == true)
			{
				if (g_InGameShopSystem->ScriptDownload() == false)
					return false;
			}
			if (g_InGameShopSystem->IsBannerDownload() == true)
			{
				if (g_InGameShopSystem->BannerDownload() == true)
				{
					g_pInGameShop->InitBanner(g_InGameShopSystem->GetBannerFileName(), g_InGameShopSystem->GetBannerURL());
				}
			}

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP) == false)
			{
				if (g_InGameShopSystem->GetIsRequestShopOpenning() == false)
				{
					SendRequestIGS_CashShopOpen(0);
					g_InGameShopSystem->SetIsRequestShopOpenning(true);

					g_pMainFrame->SetBtnState(MAINFRAME_BTN_PARTCHARGE, true);
				}
			}
			else
			{
				SendRequestIGS_CashShopOpen(1);
				g_pNewUISystem->Hide(SEASON3B::INTERFACE_INGAMESHOP);
			}

			PlayBuffer(SOUND_CLICK01);
			return true;
		}
#endif
	}

	return false;
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateKeyEvent()
{
	if (m_ItemHotKey.UpdateKeyEvent() == false)
	{
		return false;
	}
	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::Update()
{
	if (m_bExpEffect == true)
	{
		if (timeGetTime() - m_dwExpEffectTime > 2000)
		{
			m_bExpEffect = false;
			m_dwExpEffectTime = 0;
			m_dwGetExp = 0;
		}
	}

	return true;
}

float SEASON3B::CNewUIMainFrameWindow::GetLayerDepth()
{
	return 10.6f;
}

float SEASON3B::CNewUIMainFrameWindow::GetKeyEventOrder()
{
	return 2.9f;
}

void SEASON3B::CNewUIMainFrameWindow::SetItemHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	m_ItemHotKey.SetHotKey(iHotKey, iItemType, iItemLevel);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKey(int iHotKey)
{
	return m_ItemHotKey.GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKeyLevel(int iHotKey)
{
	return m_ItemHotKey.GetHotKeyLevel(iHotKey);
}

void SEASON3B::CNewUIMainFrameWindow::UseHotKeyItemRButton()
{
	m_ItemHotKey.UseItemRButton();
}

void SEASON3B::CNewUIMainFrameWindow::UpdateItemHotKey()
{
	m_ItemHotKey.UpdateKeyEvent();
}

void SEASON3B::CNewUIMainFrameWindow::ResetSkillHotKey()
{
	g_pSkillList->Reset();
}

void SEASON3B::CNewUIMainFrameWindow::SetSkillHotKey(int iHotKey, int iSkillType)
{
	g_pSkillList->SetHotKey(iHotKey, iSkillType);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKey(int iHotKey)
{
	return g_pSkillList->GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKeyIndex(int iSkillType)
{
	return g_pSkillList->GetSkillIndex(iSkillType);
}

SEASON3B::CNewUIItemHotKey::CNewUIItemHotKey()
{
	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		m_iHotKeyItemType[i] = -1;
		m_iHotKeyItemLevel[i] = 0;
	}
}

SEASON3B::CNewUIItemHotKey::~CNewUIItemHotKey()
{

}

bool SEASON3B::CNewUIItemHotKey::UpdateKeyEvent()
{
	int iIndex = -1;

	if (SEASON3B::IsPress('Q') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_Q);
	}
	else if (SEASON3B::IsPress('W') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_W);
	}
	else if (SEASON3B::IsPress('E') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_E);
	}
	else if (SEASON3B::IsPress('R') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_R);
	}

	if (iIndex != -1)
	{
		ITEM* pItem = NULL;
		pItem = g_pMyInventory->FindItem(iIndex);
		if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
		{
			std::list<eBuffState> secretPotionbufflist;
			secretPotionbufflist.push_back(eBuff_SecretPotion1);
			secretPotionbufflist.push_back(eBuff_SecretPotion2);
			secretPotionbufflist.push_back(eBuff_SecretPotion3);
			secretPotionbufflist.push_back(eBuff_SecretPotion4);
			secretPotionbufflist.push_back(eBuff_SecretPotion5);

			if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone) {
				SEASON3B::CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255));
			}
			else {
				SendRequestUse(iIndex, 0);
			}
		}
		else

		{
			SendRequestUse(iIndex, 0);
		}
		return false;
	}

	return true;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemIndex(int iType, bool bItemCount)
{
	int iStartItemType = 0, iEndItemType = 0;
	int i, j;

	switch (iType)
	{
	case HOTKEY_Q:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
		}
		break;
	case HOTKEY_W:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
		}
		break;
	case HOTKEY_E:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 8; iEndItemType = ITEM_POTION + 8;
			}
		}
		break;
	case HOTKEY_R:
		if (GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if (m_iHotKeyItemType[iType] >= ITEM_POTION + 0 && m_iHotKeyItemType[iType] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else if (m_iHotKeyItemType[iType] >= ITEM_POTION + 4 && m_iHotKeyItemType[iType] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 37; iEndItemType = ITEM_POTION + 35;
			}
		}
		break;
	}

	int iItemCount = 0;
	ITEM* pItem = NULL;

	int iNumberofItems = g_pMyInventory->GetInventoryCtrl()->GetNumberOfItems();
	for (i = iStartItemType; i >= iEndItemType; --i)
	{
		if (bItemCount)
		{
			for (j = 0; j < iNumberofItems; ++j)
			{
				pItem = g_pMyInventory->GetInventoryCtrl()->GetItem(j);
				if (pItem == NULL)
				{
					continue;
				}

				if (
					(pItem->Type == i && ((pItem->Level >> 3) & 15) == m_iHotKeyItemLevel[iType])
					|| (pItem->Type == i && (pItem->Type >= ITEM_POTION + 0 && pItem->Type <= ITEM_POTION + 3))
					)
				{
					if (pItem->Type == ITEM_POTION + 9
						|| pItem->Type == ITEM_POTION + 10
						|| pItem->Type == ITEM_POTION + 20
						)
					{
						iItemCount++;
					}
					else
					{
						iItemCount += pItem->Durability;
					}
				}
			}
		}
		else
		{
			int iIndex = -1;
			if (i >= ITEM_POTION + 0 && i <= ITEM_POTION + 3)
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i);
			}
			else
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i, m_iHotKeyItemLevel[iType]);
			}

			if (-1 != iIndex)
			{
				pItem = g_pMyInventory->FindItem(iIndex);
				if ((pItem->Type != ITEM_POTION + 7
					&& pItem->Type != ITEM_POTION + 10
					&& pItem->Type != ITEM_POTION + 20)
					|| ((pItem->Level >> 3) & 15) == m_iHotKeyItemLevel[iType]
					)
				{
					return iIndex;
				}
			}
		}
	}

	if (bItemCount == true)
	{
		return iItemCount;
	}

	return -1;
}

bool SEASON3B::CNewUIItemHotKey::GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd)
{
	switch (m_iHotKeyItemType[iHotKey])
	{
	case ITEM_POTION + 7:
	case ITEM_POTION + 8:
	case ITEM_POTION + 9:
	case ITEM_POTION + 10:
	case ITEM_POTION + 20:
	case ITEM_POTION + 46:
	case ITEM_POTION + 47:
	case ITEM_POTION + 48:
	case ITEM_POTION + 49:
	case ITEM_POTION + 50:
	case ITEM_POTION + 70:
	case ITEM_POTION + 71:
	case ITEM_POTION + 78:
	case ITEM_POTION + 79:
	case ITEM_POTION + 80:
	case ITEM_POTION + 81:
	case ITEM_POTION + 82:
	case ITEM_POTION + 94:
	case ITEM_POTION + 85:
	case ITEM_POTION + 86:
	case ITEM_POTION + 87:
	case ITEM_POTION + 133:
		if (m_iHotKeyItemType[iHotKey] != ITEM_POTION + 20 || m_iHotKeyItemLevel[iHotKey] == 0)
		{
			iStart = iEnd = m_iHotKeyItemType[iHotKey];
			return true;
		}
		break;
	default:
		if (m_iHotKeyItemType[iHotKey] >= ITEM_POTION + 35 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION + 37)
		{
			iStart = ITEM_POTION + 37; iEnd = ITEM_POTION + 35;
			return true;
		}
		else if (m_iHotKeyItemType[iHotKey] >= ITEM_POTION + 38 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION + 40)
		{
			iStart = ITEM_POTION + 40; iEnd = ITEM_POTION + 38;
			return true;
		}
		break;
	}
	return false;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemCount(int iType)
{
	return 0;
}

void SEASON3B::CNewUIItemHotKey::SetHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	if (iHotKey != -1 && CNewUIMyInventory::CanRegisterItemHotKey(iItemType) == true
		)
	{
		m_iHotKeyItemType[iHotKey] = iItemType;
		m_iHotKeyItemLevel[iHotKey] = iItemLevel;
	}
	else
	{
		m_iHotKeyItemType[iHotKey] = -1;
		m_iHotKeyItemLevel[iHotKey] = 0;
	}
}

int SEASON3B::CNewUIItemHotKey::GetHotKey(int iHotKey)
{
	if (iHotKey != -1)
	{
		return m_iHotKeyItemType[iHotKey];
	}

	return -1;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyLevel(int iHotKey)
{
	if (iHotKey != -1)
	{
		return m_iHotKeyItemLevel[iHotKey];
	}

	return 0;
}

void SEASON3B::CNewUIItemHotKey::RenderItems()
{
	float x, y, width, height;

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iIndex = GetHotKeyItemIndex(i);
		if (iIndex != -1)
		{
			ITEM* pItem = g_pMyInventory->FindItem(iIndex);
			if (pItem)
			{
				if (gProtect.m_MainInfo.IsVersion == 1)
				{
					x = 210.0f + DisplayWinExt + (i * 30);
					y = 453.0f + DisplayHeightExt;
					if (i == 3) break;
				}
				else
				{
					x = 10 + DisplayWinExt + (i * 38);
					y = 443 + DisplayHeightExt;
				}
				width = 20; height = 20;
#if PG_USE_PEGASUS_HUD
				// Q/W/E/R/T live in the five narrow sockets on the plate.
				{
					float sx, sy, sw, sh;
					if (PgSlotRect(PG_SLOT_ITEM0 + i, sx, sy, sw, sh) == false)
						continue;

					x = sx + sw * 0.14f; y = sy + sh * 0.14f;
					width = sw * 0.72f;  height = sh * 0.72f;
				}
#endif
				RenderItem3D(x, y, width, height, pItem->Type, pItem->Level, 0, 0);
			}
		}
	}
}

void SEASON3B::CNewUIItemHotKey::RenderItemCount()
{
	float x, y, width, height;

	glColor4f(1.f, 1.f, 1.f, 1.f);

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iCount = GetHotKeyItemIndex(i, true);
		if (iCount > 0)
		{
			if (gProtect.m_MainInfo.IsVersion == 1)
			{
				x = 230 + DisplayWinExt + (i * 30); y = 445.0f + DisplayHeightExt; width = 8; height = 9;
				if (i == 3) break;
			}
			else
			{
				x = 30 + DisplayWinExt + (i * 38); y = 457 + DisplayHeightExt; width = 8; height = 9;
			}
#if PG_USE_PEGASUS_HUD
			// Bottom-right corner of the matching socket. RenderNumber centres
			// on x, so this sits the count just inside the frame.
			{
				float sx, sy, sw, sh;
				if (PgSlotRect(PG_SLOT_ITEM0 + i, sx, sy, sw, sh) == false)
					continue;

				x = sx + sw * 0.72f;
				y = sy + sh * 0.60f;
			}
#endif

			SEASON3B::RenderNumber(x, y, iCount);
		}
	}
}

void SEASON3B::CNewUIItemHotKey::UseItemRButton()
{
	int x, y, width, height;

	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		if (gProtect.m_MainInfo.IsVersion == 1)
		{
			x = 210 + DisplayWinExt + (i * 30); y = 445.0f + DisplayHeightExt; width = 20; height = 20;
			if (i == 3) break;
		}
		else
		{
			x = 10 + DisplayWinExt + (i * 38); y = 445 + DisplayHeightExt; width = 20; height = 20;
		}
#if PG_USE_PEGASUS_HUD
		// Right-click has to test where the icon actually is now.
		{
			float sx, sy, sw, sh;
			if (PgSlotRect(PG_SLOT_ITEM0 + i, sx, sy, sw, sh) == false)
				continue;

			x = (int)sx; y = (int)sy; width = (int)sw; height = (int)sh;
		}
#endif
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if (MouseRButtonPush)
			{
				MouseRButtonPush = false;
				int iIndex = GetHotKeyItemIndex(i);
				if (iIndex != -1)
				{
					SendRequestUse(iIndex, 0);
					break;
				}
			}
		}
	}
}

SEASON3B::CNewUISkillList::CNewUISkillList()
{
	m_pNewUIMng = NULL;
	Reset();
}

SEASON3B::CNewUISkillList::~CNewUISkillList()
{
	Release();
}

bool SEASON3B::CNewUISkillList::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_SKILL_LIST, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;

	LoadImages();

	Show(true);

	return true;
}

void SEASON3B::CNewUISkillList::Release()
{
	if (m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);
	}

	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUISkillList::Reset()
{
	m_bSkillList = false;
	m_bHotKeySkillListUp = false;

	m_bRenderSkillInfo = false;
	m_iRenderSkillInfoType = 0;
	m_iRenderSkillInfoPosX = 0;
	m_iRenderSkillInfoPosY = 0;

	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		m_iHotKeySkillType[i] = -1;
	}

	m_EventState = EVENT_NONE;
}

void SEASON3B::CNewUISkillList::LoadImages()
{
	LoadBitmap("Interface\\newui_skill.jpg", IMAGE_SKILL1, GL_LINEAR);
	LoadBitmap("Interface\\newui_skill2.jpg", IMAGE_SKILL2, GL_LINEAR);
	LoadBitmap("Interface\\newui_command.jpg", IMAGE_COMMAND, GL_LINEAR);
	LoadBitmap("Interface\\newui_skillbox.jpg", IMAGE_SKILLBOX, GL_LINEAR);
	LoadBitmap("Interface\\newui_skillbox2.jpg", IMAGE_SKILLBOX_USE, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill.jpg", IMAGE_NON_SKILL1, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill2.jpg", IMAGE_NON_SKILL2, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_command.jpg", IMAGE_NON_COMMAND, GL_LINEAR);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	LoadBitmap("Interface\\newui_skill3.jpg", IMAGE_SKILL3, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill3.jpg", IMAGE_NON_SKILL3, GL_LINEAR);
#endif
}

void SEASON3B::CNewUISkillList::UnloadImages()
{
	DeleteBitmap(IMAGE_SKILL1);
	DeleteBitmap(IMAGE_SKILL2);
	DeleteBitmap(IMAGE_COMMAND);
	DeleteBitmap(IMAGE_SKILLBOX);
	DeleteBitmap(IMAGE_SKILLBOX_USE);
	DeleteBitmap(IMAGE_NON_SKILL1);
	DeleteBitmap(IMAGE_NON_SKILL2);
	DeleteBitmap(IMAGE_NON_COMMAND);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	DeleteBitmap(IMAGE_SKILL3);
	DeleteBitmap(IMAGE_NON_SKILL3);
#endif
}

bool SEASON3B::CNewUISkillList::UpdateMouseEvent()
{
#ifdef MOD_SKILLLIST_UPDATEMOUSE_BLOCK
	if (GFxProcess::GetInstancePtr()->GetUISelect() == 1)
	{
		return true;
	}
#endif

	if (g_isCharacterBuff((&Hero->Object), eBuff_DuelWatch))
	{
		m_bSkillList = false;
		return true;
	}

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;
	BYTE bySkillMasterNumber = CharacterAttribute->SkillMasterNumber;

	float x, y, width, height, FixX;

	m_bRenderSkillInfo = false;

	if (bySkillNumber <= 0)
	{
		return true;
	}
	if (gProtect.m_MainInfo.IsVersion == 1)
	{
		FixX = 75;
	}
	else
	{
		FixX = 0;
	}
	x = 385.f - FixX + DisplayWinExt;
	y = 431.f + DisplayHeightExt;
	width = 32.f; height = 38.f;
#if PG_USE_PEGASUS_HUD
	// Current skill lives in the sixth wide socket now.
	PgSlotRect(PG_SLOT_CURRENT, x, y, width, height);
#endif
	if (m_EventState == EVENT_NONE && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_CURRENTSKILL;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && (MouseLButtonPush == true || MouseLButtonDBClick == true)
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_CURRENTSKILL;
		return false;
	}
	if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		if (MouseLButtonPush == false && MouseLButtonDBClick == false)
		{
			if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				m_bSkillList = !m_bSkillList;
				PlayBuffer(SOUND_CLICK01);
				m_EventState = EVENT_NONE;
				return false;
			}
			m_EventState = EVENT_NONE;
			return true;
		}

	}

	if (m_EventState == EVENT_BTN_HOVER_CURRENTSKILL)
	{
		m_bRenderSkillInfo = true;
		m_iRenderSkillInfoType = Hero->CurrentSkill;
		m_iRenderSkillInfoPosX = x - 5;
		m_iRenderSkillInfoPosY = y - 15;

		return false;
	}
	else if (m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		return false;
	}

	x = 222.f - FixX + DisplayWinExt; y = 431.f + DisplayHeightExt; width = 32.f * 5.f; height = 38.f;
#if PG_USE_PEGASUS_HUD
	// This strip is the gate: if the cursor isn't inside it the per-slot tests
	// below never run at all. Stretch it across the five sockets.
	{
		float ax, ay, aw, ah, bx, by, bw, bh;
		if (PgSlotRect(0, ax, ay, aw, ah) == true && PgSlotRect(4, bx, by, bw, bh) == true)
		{
			x = ax; y = ay; width = (bx + bw) - ax; height = ah;
		}
	}
#endif
	if (m_EventState == EVENT_NONE && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_SKILLHOTKEY;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == true
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_SKILLHOTKEY;
		return false;
	}

	x = 190.f - FixX + DisplayWinExt; y = 431.f + DisplayHeightExt; width = 32.f; height = 38.f;
	int iStartIndex = (m_bHotKeySkillListUp == true) ? 6 : 1;
	for (int i = 0, iIndex = iStartIndex; i < 5; ++i, iIndex++)
	{
		x += width;

#if PG_USE_PEGASUS_HUD
		// Has to test the same rectangles RenderCurrentSkillAndHotSkillList
		// draws into, or the icon is in the socket while the click that selects
		// it is still back on the old 640-block offsets.
		if (PgSlotRect(i, x, y, width, height) == false)
			continue;
#endif

		if (iIndex == 10)
		{
			iIndex = 0;
		}
		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if (m_iHotKeySkillType[iIndex] == -1)
			{
				if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
				{
					m_bRenderSkillInfo = false;
					m_iRenderSkillInfoType = -1;
				}
				if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY && MouseLButtonPush == false)
				{
					m_EventState = EVENT_NONE;
				}
				continue;
			}

			WORD bySkillType = CharacterAttribute->Skill[m_iHotKeySkillType[iIndex]];

			if (bySkillType == 0 || (bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF))
				continue;

			BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

			if (bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
			{
				continue;
			}

			if (m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
			{
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = m_iHotKeySkillType[iIndex];
				m_iRenderSkillInfoPosX = x - 5;
				m_iRenderSkillInfoPosY = y - 15;
				return true;
			}
			if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
			{
				if (MouseLButtonPush == false)
				{
					if (m_iRenderSkillInfoType == m_iHotKeySkillType[iIndex])
					{
						m_EventState = EVENT_NONE;
						m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];
						Hero->CurrentSkill = m_iHotKeySkillType[iIndex];
						PlayBuffer(SOUND_CLICK01);
						return false;
					}
					else
					{
						m_EventState = EVENT_NONE;
					}
				}
			}
		}
	}

	x = 222.f - FixX + DisplayWinExt; y = 431.f + DisplayHeightExt; width = 32.f * 5.f; height = 38.f;
	if (m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
	{
		if (MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}

	if (m_bSkillList == false)
		return true;

	WORD bySkillType = 0;

	int iSkillCount = 0;
	bool bMouseOnSkillList = false;

	x = 385.f - FixX + DisplayWinExt; y = 390 + DisplayHeightExt; width = 32; height = 38;
	float fOrigX = 385.f - FixX + DisplayWinExt;

	EVENT_STATE PrevEventState = m_EventState;

	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		bySkillType = CharacterAttribute->Skill[i];

		if (bySkillType == 0 || (bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF))
			continue;

		BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

		if (bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
		{
			continue;
		}

		if (iSkillCount == 18)
		{
			y -= height;
		}

		if (iSkillCount < 14)
		{
			int iRemainder = iSkillCount % 2;
			int iQuotient = iSkillCount / 2;

			if (iRemainder == 0)
			{
				x = fOrigX + iQuotient * width;
			}
			else
			{
				x = fOrigX - (iQuotient + 1) * width;
			}
		}
		else if (iSkillCount >= 14 && iSkillCount < 18)
		{
			x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
		}
		else
		{
			x = fOrigX - (12 * width) + ((iSkillCount - 17) * width);
		}

		iSkillCount++;

		if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			bMouseOnSkillList = true;
			if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
				break;
			}
		}

		if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true
			&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_BTN_DOWN_SKILLLIST;
			break;
		}

		if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false
			&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_bRenderSkillInfo = true;
			m_iRenderSkillInfoType = i;
			m_iRenderSkillInfoPosX = x;
			m_iRenderSkillInfoPosY = y - 15;
		}

		if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
			&& m_iRenderSkillInfoType == i && SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_NONE;

			m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

			Hero->CurrentSkill = i;
			m_bSkillList = false;

			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	if (PrevEventState != m_EventState)
	{
		if (m_EventState == EVENT_NONE || m_EventState == EVENT_BTN_HOVER_SKILLLIST)
			return true;
		return false;
	}

	if (Hero->m_pPet != NULL)
	{
		x = 353.f - FixX + DisplayWinExt; y = 352 + DisplayHeightExt; width = 32; height = 38;
		for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; ++i)
		{
			if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				bMouseOnSkillList = true;

				if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
				{
					m_EventState = EVENT_BTN_HOVER_SKILLLIST;
					return true;
				}
				if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true)
				{
					m_EventState = EVENT_BTN_DOWN_SKILLLIST;
					return false;
				}

				if (m_EventState == EVENT_BTN_HOVER_SKILLLIST)
				{
					m_bRenderSkillInfo = true;
					m_iRenderSkillInfoType = i;
					m_iRenderSkillInfoPosX = x;
					m_iRenderSkillInfoPosY = y - 15;
				}
				if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
					&& m_iRenderSkillInfoType == i)
				{
					m_EventState = EVENT_NONE;

					m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

					Hero->CurrentSkill = i;
					m_bSkillList = false;
					PlayBuffer(SOUND_CLICK01);
					return false;
				}
			}
			x += width;
		}
	}

	if (bMouseOnSkillList == false && m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if (bMouseOnSkillList == false && MouseLButtonPush == false
		&& m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return false;
	}
	if (m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		if (MouseLButtonPush == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}

	return true;
}

bool SEASON3B::CNewUISkillList::UpdateKeyEvent()
{
	for (int i = 0; i < 9; ++i)
	{
		if (SEASON3B::IsPress('1' + i))
		{
			UseHotKey(i + 1);
		}
	}

	if (SEASON3B::IsPress('0'))
	{
		UseHotKey(0);
	}

	if (m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		if (SEASON3B::IsRepeat(VK_CONTROL))
		{
			for (int i = 0; i < 9; ++i)
			{
				if (SEASON3B::IsPress('1' + i))
				{
					SetHotKey(i + 1, m_iRenderSkillInfoType);

					return false;
				}
			}

			if (SEASON3B::IsPress('0'))
			{
				SetHotKey(0, m_iRenderSkillInfoType);

				return false;
			}
		}
	}

	if (SEASON3B::IsRepeat(VK_SHIFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			if (SEASON3B::IsPress('1' + i))
			{
				Hero->CurrentSkill = AT_PET_COMMAND_DEFAULT + i;
				return false;
			}
		}
	}

	return true;
}

bool SEASON3B::CNewUISkillList::IsArrayUp(BYTE bySkill)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == bySkill)
		{
			if (i == 0 || i > 5)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool SEASON3B::CNewUISkillList::IsArrayIn(BYTE bySkill)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == bySkill)
		{
			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUISkillList::SetHotKey(int iHotKey, int iSkillType)
{
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == iSkillType)
		{
			m_iHotKeySkillType[i] = -1;
			break;
		}
	}

	m_iHotKeySkillType[iHotKey] = iSkillType;
}

int SEASON3B::CNewUISkillList::GetHotKey(int iHotKey)
{
	return m_iHotKeySkillType[iHotKey];
}

int SEASON3B::CNewUISkillList::GetSkillIndex(int iSkillType)
{
	int iReturn = -1;
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		if (CharacterAttribute->Skill[i] == iSkillType)
		{
			iReturn = i;
			break;
		}
	}

	return iReturn;
}

void SEASON3B::CNewUISkillList::UseHotKey(int iHotKey)
{
	if (m_iHotKeySkillType[iHotKey] != -1)
	{
		if (m_iHotKeySkillType[iHotKey] >= AT_PET_COMMAND_DEFAULT && m_iHotKeySkillType[iHotKey] < AT_PET_COMMAND_END)
		{
			if (Hero->m_pPet == NULL)
			{
				return;
			}
		}

		WORD wHotKeySkill = CharacterAttribute->Skill[m_iHotKeySkillType[iHotKey]];

		if (wHotKeySkill == 0)
		{
			return;
		}

		m_wHeroPriorSkill = CharacterAttribute->Skill[Hero->CurrentSkill];

		Hero->CurrentSkill = m_iHotKeySkillType[iHotKey];

		WORD bySkill = CharacterAttribute->Skill[Hero->CurrentSkill];

		if (
			g_pOption->IsAutoAttack() == true
			&& gMapManager.WorldActive != WD_6STADIUM
			&& gMapManager.InChaosCastle() == false
			&& (bySkill == AT_SKILL_TELEPORT || bySkill == AT_SKILL_TELEPORT_B))
		{
			SelectedCharacter = -1;
			Attacking = -1;
		}
	}
}

bool SEASON3B::CNewUISkillList::Update()
{
	if (IsArrayIn(Hero->CurrentSkill) == true)
	{
		if (IsArrayUp(Hero->CurrentSkill) == true)
		{
			m_bHotKeySkillListUp = true;
		}
		else
		{
			m_bHotKeySkillListUp = false;
		}
	}

	if (Hero->m_pPet == NULL)
	{
		if (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END)
		{
			Hero->CurrentSkill = 0;
		}
	}

	return true;
}

void SEASON3B::CNewUISkillList::RenderCurrentSkillAndHotSkillList()
{
	int i;
	float x, y, width, height;

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if (bySkillNumber > 0)
	{
		int iStartSkillIndex = 1;
		if (m_bHotKeySkillListUp)
		{
			iStartSkillIndex = 6;
		}
		if (gProtect.m_MainInfo.IsVersion == 1)
		{
			x = 310.0f + DisplayWinExt;
			y = 447.0f + DisplayHeightExt;
			width = 20;
			height = 28;
		}
		else
		{
			x = 190 + DisplayWinExt;
			y = 431 + DisplayHeightExt;
			width = 32; height = 38;
			for (i = 0; i < 5; ++i)
			{
				x += width;

#if PG_USE_PEGASUS_HUD
				// The Pegasus plate's slots are nowhere near the stock offsets,
				// so take the rectangle from the plate instead of stepping
				// across the 640 block.
				if (PgSlotRect(i, x, y, width, height) == false)
					continue;
#endif

				int iIndex = iStartSkillIndex + i;
				if (iIndex == 10)
				{
					iIndex = 0;
				}

				if (m_iHotKeySkillType[iIndex] == -1)
				{
					continue;
				}

				if (m_iHotKeySkillType[iIndex] >= AT_PET_COMMAND_DEFAULT && m_iHotKeySkillType[iIndex] < AT_PET_COMMAND_END)
				{
					if (Hero->m_pPet == NULL)
					{
						continue;
					}
				}

				if (Hero->CurrentSkill == m_iHotKeySkillType[iIndex])
				{
					SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
				}
#if PG_USE_PEGASUS_HUD
				// Fill the socket. 20x28 stays - that is the atlas cell, not a
				// size - and the socket size goes in as the draw size.
				RenderSkillIcon(m_iHotKeySkillType[iIndex],
					x + width * 0.10f, y + height * 0.08f, 20.f, 28.f, 0,
					width * 0.80f, height * 0.84f);
#else
				RenderSkillIcon(m_iHotKeySkillType[iIndex], x + 6, y + 6, 20, 28);
#endif
			}
			x = 392 + DisplayWinExt; y = 437 + DisplayHeightExt; width = 20; height = 28;
		}

#if PG_USE_PEGASUS_HUD
		// Sixth wide socket - the one the plate paints with a highlight - holds
		// whatever skill is currently selected, and doubles as the handle that
		// swings the rest of the skill list up and down.
		{
			float sx, sy, sw, sh;
			if (PgSlotRect(PG_SLOT_CURRENT, sx, sy, sw, sh) == true)
			{
				if (SEASON3B::CheckMouseIn(sx, sy, sw, sh) == true
					&& SEASON3B::IsPress(VK_LBUTTON) == true)
				{
					m_bHotKeySkillListUp = !m_bHotKeySkillListUp;
				}

				RenderSkillIcon(Hero->CurrentSkill,
					sx + sw * 0.10f, sy + sh * 0.08f, 20.f, 28.f, 0,
					sw * 0.80f, sh * 0.84f);
				return;
			}
		}
#endif

		RenderSkillIcon(Hero->CurrentSkill, x, y, width, height);
	}
}

bool SEASON3B::CNewUISkillList::RenderMuHelper(float X, float Y, int SkillSelect, int SkillBuff)
{
	int i;
	float x, y, width, height;
	bool hoverIconSkill = false;
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if (bySkillNumber > 0)
	{

		{
			x = X;
			y = Y;
			width = 32;
			height = 38;
			float fOrigX = X - 35;
			int iSkillType = 0;
			int iSkillCount = 0;

			for (i = 0; i < MAX_MAGIC; ++i)
			{
				iSkillType = CharacterAttribute->Skill[i];
				if (iSkillType >= AT_PET_COMMAND_DEFAULT && iSkillType < AT_PET_COMMAND_END)
				{
					continue;
				}

				if (iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
				{
					BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;

					if (bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
					{
						continue;
					}

					if (SkillBuff)
					{
						if (SkillAttribute[iSkillType].Magic_Icon == 0 || SkillAttribute[iSkillType].MasteryType != 255) continue;

						if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[0] == iSkillType
							|| g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[1] == iSkillType
							|| g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[2] == iSkillType)
						{
							continue;
						}
					}
					else
					{
						if (SkillAttribute[iSkillType].Magic_Icon > 0 && SkillAttribute[iSkillType].MasteryType == 255) continue;

						if (g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[0] == iSkillType
							|| g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[1] == iSkillType
							|| g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[2] == iSkillType)
						{
							continue;
						}
					}
					if (iSkillCount % 8 == 0 && iSkillCount > 0)
					{
						x -= width;
						y = Y;
					}
					y += height;

					iSkillCount++;

					if (i == Hero->CurrentSkill)
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
					}
					else
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);
					}

					RenderSkillIcon(iSkillType, x + 6, y + 6, 20, 28, TRUE);
					if (SEASON3B::CheckMouseIn(x + 6, y + 6, 20, 28) == true)
					{
						g_pNewUISystem->GetUI_NewBCustomMenuInfo()->SetBlockCur();
						m_bRenderSkillInfo = true;
						m_iRenderSkillInfoType = i;
						m_iRenderSkillInfoPosX = x - 5;
						m_iRenderSkillInfoPosY = y - 15;

						if (SEASON3B::IsRelease(VK_LBUTTON))
						{
							if (SkillBuff == 0)
							{
								g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Skill[SkillSelect] = iSkillType;
							}
							else if (SkillBuff == 1)
							{
								g_pNewUISystem->Get_pNewUIMuHelper()->DataAutoMu.Buff[SkillSelect] = iSkillType;
							}
							return true;
						}
					}

				}
			}

		}
	}

	if (m_bRenderSkillInfo == true && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

		m_bRenderSkillInfo = false;
	}

	return false;
}

bool SEASON3B::CNewUISkillList::Render()
{
	int i;
	float x, y, width, height, FixX;
	if (gProtect.m_MainInfo.IsVersion == 1)
	{
		FixX = 75;
	}
	else
	{
		FixX = 0;
	}
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if (bySkillNumber > 0)
	{
		if (m_bSkillList == true)
		{
			x = 385 - FixX + DisplayWinExt; y = 390 + DisplayHeightExt; width = 32; height = 38;
			float fOrigX = 385.f - FixX + DisplayWinExt;

#if PG_USE_PEGASUS_HUD
			// 390 + DisplayHeightExt lands inside the Pegasus plate, so the
			// list opened underneath it and looked like nothing happened. Sit
			// it just above the plate instead, and centre it: the row grows
			// outwards from fOrigX in both directions, so that origin has to be
			// half an icon left of the middle for the fan to come out even.
			{
				float fOrgX, fOrgY, fScale;
				if (SEASON3B::GetPegasusHudTransform(fOrgX, fOrgY, fScale) == true)
				{
					y = fOrgY - height - 8.f;
					fOrigX = (float)DisplayWinExt + 320.f - width * 0.5f;
					x = fOrigX;
				}
			}
#endif
			int iSkillType = 0;
			int iSkillCount = 0;

			for (i = 0; i < MAX_MAGIC; ++i)
			{
				iSkillType = CharacterAttribute->Skill[i];
				if (iSkillType >= AT_PET_COMMAND_DEFAULT && iSkillType < AT_PET_COMMAND_END)
				{
					continue;
				}
				if (iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
				{
					BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;

					if (bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
					{
						continue;
					}

					if (iSkillCount == 18)
					{
						y -= height;
					}

					if (iSkillCount < 14)
					{
						int iRemainder = iSkillCount % 2;
						int iQuotient = iSkillCount / 2;

						if (iRemainder == 0)
						{
							x = fOrigX + iQuotient * width;
						}
						else
						{
							x = fOrigX - (iQuotient + 1) * width;
						}
					}
					else if (iSkillCount >= 14 && iSkillCount < 18)
					{
						x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
					}
					else
					{
						x = fOrigX - (12 * width) + ((iSkillCount - 17) * width);
					}

					iSkillCount++;

					if (i == Hero->CurrentSkill)
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
					}
					else
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);
					}

					RenderSkillIcon(i, x + 6, y + 6, 20, 28);
				}
			}
			RenderPetSkill();
		}
	}

	if (m_bRenderSkillInfo == true && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

		m_bRenderSkillInfo = false;
	}

	return true;
}

void SEASON3B::CNewUISkillList::RenderSkillInfo()
{
	::RenderSkillInfo(m_iRenderSkillInfoPosX + 15, m_iRenderSkillInfoPosY - 10, m_iRenderSkillInfoType);
}

float SEASON3B::CNewUISkillList::GetLayerDepth()
{
	return 5.2f;
}

WORD SEASON3B::CNewUISkillList::GetHeroPriorSkill()
{
	return m_wHeroPriorSkill;
}

void SEASON3B::CNewUISkillList::SetHeroPriorSkill(BYTE bySkill)
{
	m_wHeroPriorSkill = bySkill;
}

void SEASON3B::CNewUISkillList::RenderPetSkill()
{
	if (Hero->m_pPet == NULL)
	{
		return;
	}

	float x, y, width, height, FixX;
	if (gProtect.m_MainInfo.IsVersion == 1)
	{
		FixX = 75;
	}
	else
	{
		FixX = 0;
	}
	x = 353.f - FixX + DisplayWinExt; y = 352 + DisplayHeightExt; width = 32; height = 38;
	for (int i = AT_PET_COMMAND_DEFAULT; i < AT_PET_COMMAND_END; ++i)
	{
		if (i == Hero->CurrentSkill)
		{
			SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
		}
		else
		{
			SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);
		}

		RenderSkillIcon(i, x + 6, y + 6, 20, 28);
		x += width;
	}
}

void SEASON3B::CNewUISkillList::RenderSkillIcon(int iIndex, float x, float y, float width, float height, int TypeMuHelper,
	float fDrawW, float fDrawH)
{
	// Destination size, kept separate from the atlas cell size above.
	if (fDrawW <= 0.f) fDrawW = width;
	if (fDrawH <= 0.f) fDrawH = height;

	WORD bySkillType = (TypeMuHelper == 1) ? iIndex : CharacterAttribute->Skill[iIndex];

	if (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType < AT_PET_COMMAND_END)
	{
		goto Jump;
	}
	if (bySkillType == 0 || !gSkillManager.FindHeroSkill((ActionSkillType)bySkillType))
	{
		return;
	}

Jump:
	if (iIndex >= AT_PET_COMMAND_DEFAULT)
	{
		bySkillType = iIndex;
	}

	bool bCantSkill = false;

	BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;
	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;

	if (!gSkillManager.DemendConditionCheckSkill(bySkillType))
	{
		bCantSkill = true;
	}

	if (IsCanBCSkill(bySkillType) == false)
	{
		bCantSkill = true;
	}
	if (g_isCharacterBuff((&Hero->Object), eBuff_AddSkill) && bySkillUseType == SKILL_USE_TYPE_BRAND)
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type<MODEL_HELPER + 2 || Hero->Helper.Type>MODEL_HELPER + 3) && Hero->Helper.Type != MODEL_HELPER + 37)
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_SPEAR && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;
		if ((iTypeL < ITEM_SPEAR || iTypeL >= ITEM_BOW) && (iTypeR < ITEM_SPEAR || iTypeR >= ITEM_BOW))
		{
			bCantSkill = true;
		}
	}

	if (
		bySkillType == MASTER_SKILL_ADD_CYCLONE_IMPROVED1 ||
		bySkillType == MASTER_SKILL_ADD_CYCLONE_IMPROVED2 ||
		bySkillType == MASTER_SKILL_ADD_SLASH_IMPROVED ||
		bySkillType == MASTER_SKILL_ADD_LUNGE_IMPROVED ||
		bySkillType >= AT_SKILL_BLOCKING && bySkillType <= AT_SKILL_SWORD5 && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		bCantSkill = true;
	}

	if ((bySkillType == AT_SKILL_ICE_BLADE ||
		bySkillType == MASTER_SKILL_ADD_POWER_SLASH_IMPROVED ||
		(AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP + 4 >= bySkillType)) && (Hero->Helper.Type == MODEL_HELPER + 2 || Hero->Helper.Type == MODEL_HELPER + 3 || Hero->Helper.Type == MODEL_HELPER + 37))
	{
		bCantSkill = true;
	}

	int iEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

	if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && PartyNumber <= 0)
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && (IsDoppelGanger1() || IsDoppelGanger2() || IsDoppelGanger3() || IsDoppelGanger4()))
	{
		bCantSkill = true;
	}

	if (bySkillType == AT_SKILL_DARK_HORSE ||
		bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED ||
		bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED
		|| (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP + 4))
	{
		BYTE byDarkHorseLife = 0;
		byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
		if (byDarkHorseLife == 0 || Hero->Helper.Type != MODEL_HELPER + 4)
		{
			bCantSkill = true;

		}
	}
#ifdef PJH_FIX_SPRIT

	if (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType < AT_PET_COMMAND_END)
	{
		int iCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
		PET_INFO PetInfo;
		giPetManager::GetPetInfo(PetInfo, 421 - PET_TYPE_DARK_SPIRIT);
		int RequireCharisma = (185 + (PetInfo.m_wLevel * 15));
		if (RequireCharisma > iCharisma)
		{
			bCantSkill = true;
		}
	}
#endif
	if (
		(bySkillType == AT_SKILL_INFINITY_ARROW) ||
		(bySkillType == MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED) ||
		(bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER) || (bySkillType == MASTER_SKILL_ADD_MAGIC_CIRCLE_ENHANCED) || bySkillType == MASTER_SKILL_ADD_MAGIC_CIRCLE_IMPROVED)
	{
		if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
		{
			bCantSkill = true;
		}
		if (
			(g_isCharacterBuff((&Hero->Object), eBuff_InfinityArrow)) ||
			(g_isCharacterBuff((&Hero->Object), EFFECT_INFINITY_ARROW_IMPROVED))
			|| (g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower))
			|| (g_isCharacterBuff((&Hero->Object), EFFECT_MAGIC_CIRCLE_IMPROVED))
			|| (g_isCharacterBuff((&Hero->Object), EFFECT_MAGIC_CIRCLE_ENHANCED))
			)
		{
			bCantSkill = true;
		}
	}

	if (bySkillType == AT_SKILL_REDUCEDEFENSE
		|| bySkillType == MASTER_SKILL_ADD_FIRE_SLASH_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_FIRE_SLASH_ENHANCED

		|| (AT_SKILL_BLOOD_ATT_UP <= bySkillType && bySkillType <= AT_SKILL_BLOOD_ATT_UP + 4))
	{
		WORD Strength;
		const WORD wRequireStrength = 596;
		Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
		if (Strength < wRequireStrength)
		{
			bCantSkill = true;
		}
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			bCantSkill = true;
		}
	}

	switch (bySkillType)
	{

	case AT_SKILL_PARALYZE:
	case MASTER_SKILL_ADD_ICE_ARROW_IMPROVED:
	{
		WORD  Dexterity;
		const WORD wRequireDexterity = 646;
		Dexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		if (Dexterity < wRequireDexterity)
		{
			bCantSkill = true;
		}
	}break;
	}

	if (bySkillType == AT_SKILL_WHEEL
		|| bySkillType == MASTER_SKILL_ADD_TWISTING_SLASH_ENHANCED
		|| bySkillType == MASTER_SKILL_ADD_TWISTING_SLASH_IMPROVED1
		|| bySkillType == MASTER_SKILL_ADD_TWISTING_SLASH_IMPROVED2
		|| (AT_SKILL_TORNADO_SWORDA_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDA_UP + 4) || (AT_SKILL_TORNADO_SWORDB_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDB_UP + 4)
		)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			bCantSkill = true;
		}
	}

	if (gMapManager.InChaosCastle() == true)
	{
		if (bySkillType == AT_SKILL_DARK_HORSE ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED ||
			bySkillType == AT_SKILL_RIDER || (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_TARGET) || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP + 4))
		{
			bCantSkill = true;
		}
	}
	else
	{
		if (bySkillType == AT_SKILL_DARK_HORSE ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_IMPROVED ||
			bySkillType == MASTER_SKILL_ADD_EARTHQUAKE_ENHANCED ||
			(AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP + 4))
		{
			BYTE byDarkHorseLife = 0;
			byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
			if (byDarkHorseLife == 0)
			{
				bCantSkill = true;
			}
		}
	}

	int iCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

	if (g_csItemOption.IsDisableSkill(bySkillType, iEnergy, iCharisma))
	{
		bCantSkill = true;
	}

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if (!g_CMonkSystem.IsSwordformGlovesUseSkill(bySkillType))
	{
		bCantSkill = true;
	}
	if (g_CMonkSystem.IsRideNotUseSkill(bySkillType, Hero->Helper.Type))
	{
		bCantSkill = true;
	}

	ITEM* pLeftRing = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	ITEM* pRightRing = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];

	if (g_CMonkSystem.IsChangeringNotUseSkill(pLeftRing->Type, pRightRing->Type, pLeftRing->Level, pRightRing->Level)
		&& (gCharacterManager.GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER))
	{
		bCantSkill = true;
	}
#endif

	float fU, fV;
	int iKindofSkill = 0;

	if (g_csItemOption.Special_Option_Check() == false && (
		bySkillType == AT_SKILL_ICE_BLADE
		|| bySkillType == MASTER_SKILL_ADD_POWER_SLASH_IMPROVED
		|| (AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP + 4 >= bySkillType)))
	{
		bCantSkill = true;
	}

	if (g_csItemOption.Special_Option_Check(1) == false && (
		bySkillType == AT_SKILL_CROSSBOW ||
		bySkillType == MASTER_SKILL_ADD_TRIPLE_SHOT_IMPROVED ||
		bySkillType == MASTER_SKILL_ADD_TRIPLE_SHOT_ENHANCED ||
		(AT_SKILL_MANY_ARROW_UP <= bySkillType && AT_SKILL_MANY_ARROW_UP + 4 >= bySkillType)))
		bCantSkill = true;

	if (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_END)
	{
		fU = ((bySkillType - AT_PET_COMMAND_DEFAULT) % 8) * width / 256.f;
		fV = ((bySkillType - AT_PET_COMMAND_DEFAULT) / 8) * height / 256.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if (bySkillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		fU = 4 * width / 256.f;
		fV = 0.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if ((bySkillType >= AT_SKILL_ALICE_DRAINLIFE && bySkillType <= AT_SKILL_ALICE_THORNS))
	{
		fU = ((bySkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_ALICE_SLEEP && bySkillType <= AT_SKILL_ALICE_BLIND)
	{
		fU = ((bySkillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_ALICE_BERSERKER)
	{
		fU = 10 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_ALICE_WEAKNESS && bySkillType <= AT_SKILL_ALICE_ENERVATION)
	{
		fU = (bySkillType - AT_SKILL_ALICE_WEAKNESS + 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_SUMMON_EXPLOSION && bySkillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		fU = ((bySkillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_SUMMON_POLLUTION)
	{
		fU = 11 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		fU = 7 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_GAOTIC)
	{
		fU = 3 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_RECOVER)
	{
		fU = 9 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_MULTI_SHOT)
	{
		if (gCharacterManager.GetEquipedBowType_Skill() == BOWTYPE_NONE)
		{
			bCantSkill = true;
		}

		fU = 0 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;

		if (!(iTypeR != -1 && (iTypeR < ITEM_STAFF || iTypeR >= ITEM_STAFF + MAX_ITEM_INDEX) && (iTypeL < ITEM_STAFF || iTypeL >= ITEM_STAFF + MAX_ITEM_INDEX)))
		{
			bCantSkill = true;
		}

		fU = 1 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_GIGANTIC_STORM)
	{
		fU = 2 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		fU = 2 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (AT_SKILL_LIGHTNING_SHOCK_UP <= bySkillType && bySkillType <= AT_SKILL_LIGHTNING_SHOCK_UP + 4)
	{
		fU = 6 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		fU = 8 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillUseType == 4)
	{
		fU = (width / 256.f) * (Skill_Icon % 12);
		fV = (height / 256.f) * ((Skill_Icon / 12) + 4);
		iKindofSkill = KOS_SKILL2;
	}
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	else if (bySkillType >= AT_SKILL_THRUST)
	{
		fU = ((bySkillType - 260) % 12) * width / 256.f;
		fV = ((bySkillType - 260) / 12) * height / 256.f;
		iKindofSkill = KOS_SKILL3;
	}
#endif
	else if (bySkillType >= 57)
	{
		fU = ((bySkillType - 57) % 8) * width / 256.f;
		fV = ((bySkillType - 57) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else
	{
		fU = ((bySkillType - 1) % 8) * width / 256.f;
		fV = ((bySkillType - 1) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL1;
	}
	int iSkillIndex = 0;
	switch (iKindofSkill)
	{
	case KOS_COMMAND:
	{
		iSkillIndex = IMAGE_COMMAND;
	}break;
	case KOS_SKILL1:
	{
		iSkillIndex = IMAGE_SKILL1;
	}break;
	case KOS_SKILL2:
	{
		iSkillIndex = IMAGE_SKILL2;
	}break;
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	case KOS_SKILL3:
	{
		iSkillIndex = IMAGE_SKILL3;
	}break;
#endif
	}

	if (bCantSkill == true)
	{
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		iSkillIndex += 6;
#else
		iSkillIndex += 5;
#endif
	}

	if (iSkillIndex != 0)
	{
		if (bySkillUseType == 4)
		{
			JCCoord GetXY;

			if (g_pMasterSkillTreeInterface->GetXYImgMaster(&GetXY, bySkillType))
			{
				if (bCantSkill == true)
				{
					SEASON3B::RenderImage(BITMAP_INTERFACE_MASTER_BEGIN + 3, x, y, 20.0, 28.0, GetXY.CalcX, GetXY.CalcY, 0.0390625, 0.053710938);
				}
				else
				{
					SEASON3B::RenderImage(BITMAP_INTERFACE_MASTER_BEGIN + 2, x, y, 20.0, 28.0, GetXY.CalcX, GetXY.CalcY, 0.0390625, 0.053710938);
				}
			}

		}

		else
		{
			RenderBitmap(iSkillIndex, x, y, fDrawW, fDrawH, fU, fV, width / 256.f, height / 256.f);
		}
	}

	int iHotKey = -1;
	for (int i = 0; i < SKILLHOTKEY_COUNT; ++i)
	{
		if (m_iHotKeySkillType[i] == iIndex)
		{
			iHotKey = i;
			break;
		}
	}

	if (iHotKey != -1)
	{
		glColor3f(1.f, 0.9f, 0.8f);
		SEASON3B::RenderNumber(x + 20, y + 20, iHotKey);
	}

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if ((bySkillType == AT_SKILL_GIANTSWING
		|| bySkillType == MASTER_SKILL_ADD_CHAIN_DRIVER_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_CHAIN_DRIVER_ENHANCED
		|| bySkillType == AT_SKILL_DRAGON_KICK
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_SLAYER_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_SLAYER_ENHANCED
		|| bySkillType == AT_SKILL_MUITENPHUONGHOANG
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_LORE_IMPROVED
		|| bySkillType == MASTER_SKILL_ADD_DRAGON_LORE_ENHANCED
		|| bySkillType == AT_SKILL_DRAGON_LOWER) && (bCantSkill))
		return;
#endif

	if (
		(bySkillType != AT_SKILL_INFINITY_ARROW) &&
		(bySkillType != MASTER_SKILL_ADD_INFINITY_ARROW_IMPROVED)
		&& (bySkillType != AT_SKILL_SWELL_OF_MAGICPOWER)
		&& (bySkillType != MASTER_SKILL_ADD_MAGIC_CIRCLE_IMPROVED) && (bySkillType != MASTER_SKILL_ADD_MAGIC_CIRCLE_ENHANCED))

	{
		RenderSkillDelay(iIndex, x, y, fDrawW, fDrawH);
	}
}

void SEASON3B::CNewUISkillList::RenderSkillDelay(int iIndex, float x, float y, float width, float height)
{
	int iSkillDelay = CharacterAttribute->SkillDelay[iIndex];
	if (iSkillDelay > 0)
	{
		int iSkillType = CharacterAttribute->Skill[iIndex];

		if (iSkillType == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			if (!CheckAttack())
			{
				return;
			}
		}

		int iSkillMaxDelay = SkillAttribute[iSkillType].Delay;

		float fPersent = (float)(iSkillDelay / (float)iSkillMaxDelay);

		EnableAlphaTest();
		glColor4f(1.f, 0.5f, 0.5f, 0.5f);
		float fdeltaH = height * fPersent;
		RenderColor(x, y + height - fdeltaH, width, fdeltaH);
		EndRenderColor();
	}
}

bool SEASON3B::CNewUISkillList::IsSkillListUp()
{
	return m_bHotKeySkillListUp;
}

void SEASON3B::CNewUISkillList::ResetMouseLButton()
{
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void SEASON3B::CNewUISkillList::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if (pClass)
	{
		CNewUISkillList* pSkillList = (CNewUISkillList*)(pClass);
		pSkillList->RenderSkillInfo();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp_Wide(__int64 dwPreExp)
{
	m_loPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp_Wide(__int64 dwGetExp)
{
	m_loGetExp = dwGetExp;

	if (m_loGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp(DWORD dwPreExp)
{
	m_dwPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp(DWORD dwGetExp)
{
	m_dwGetExp = dwGetExp;

	if (m_dwGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetBtnState(int iBtnType, bool bStateDown)
{
	switch (iBtnType)
	{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	case MAINFRAME_BTN_PARTCHARGE:
	{
		if (bStateDown)
		{
			m_BtnCShop.UnRegisterButtonState();
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CSHOP, 2);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CSHOP, 3);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CSHOP, 2);
			m_BtnCShop.ChangeImgIndex(IMAGE_MENU_BTN_CSHOP, 2);
		}
		else
		{
			m_BtnCShop.UnRegisterButtonState();
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CSHOP, 0);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CSHOP, 1);
			m_BtnCShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CSHOP, 2);
			m_BtnCShop.ChangeImgIndex(IMAGE_MENU_BTN_CSHOP, 0);
		}
	}
	break;
#endif
	case MAINFRAME_BTN_CHAINFO:
	{
		if (bStateDown)
		{
			m_BtnChaInfo.UnRegisterButtonState();
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CHAINFO, 2);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CHAINFO, 3);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CHAINFO, 2);
			m_BtnChaInfo.ChangeImgIndex(IMAGE_MENU_BTN_CHAINFO, 2);

		}
		else
		{
			m_BtnChaInfo.UnRegisterButtonState();
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CHAINFO, 0);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CHAINFO, 1);
			m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CHAINFO, 2);
			m_BtnChaInfo.ChangeImgIndex(IMAGE_MENU_BTN_CHAINFO, 0);
		}
	}
	break;
	case MAINFRAME_BTN_MYINVEN:
	{
		if (bStateDown)
		{
			m_BtnMyInven.UnRegisterButtonState();
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_MYINVEN, 2);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_MYINVEN, 3);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_MYINVEN, 2);
			m_BtnMyInven.ChangeImgIndex(IMAGE_MENU_BTN_MYINVEN, 2);
		}
		else
		{
			m_BtnMyInven.UnRegisterButtonState();
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_MYINVEN, 0);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_MYINVEN, 1);
			m_BtnMyInven.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_MYINVEN, 2);
			m_BtnMyInven.ChangeImgIndex(IMAGE_MENU_BTN_MYINVEN, 0);
		}
	}
	break;
	case MAINFRAME_BTN_FRIEND:
	{
		if (bStateDown)
		{
			m_BtnFriend.UnRegisterButtonState();
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_FRIEND, 2);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_FRIEND, 3);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_FRIEND, 2);
			m_BtnFriend.ChangeImgIndex(IMAGE_MENU_BTN_FRIEND, 2);
		}
		else
		{
			m_BtnFriend.UnRegisterButtonState();
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_FRIEND, 0);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_FRIEND, 1);
			m_BtnFriend.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_FRIEND, 2);
			m_BtnFriend.ChangeImgIndex(IMAGE_MENU_BTN_FRIEND, 0);
		}
	}
	break;
	case MAINFRAME_BTN_WINDOW:
	{
		if (bStateDown)
		{
			m_BtnWindow.UnRegisterButtonState();
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_WINDOW, 2);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_WINDOW, 3);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_WINDOW, 2);
			m_BtnWindow.ChangeImgIndex(IMAGE_MENU_BTN_WINDOW, 2);
		}
		else
		{
			m_BtnWindow.UnRegisterButtonState();
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_WINDOW, 0);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_WINDOW, 1);
			m_BtnWindow.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_WINDOW, 2);
			m_BtnWindow.ChangeImgIndex(IMAGE_MENU_BTN_WINDOW, 0);
		}
	}
	break;
	}
}
