#include "stdafx.h"
#include "MenuCustom.h"
#include "Util.h"
#include "Protect.h"
#include "CBInterface.h"
#include "NewUISystem.h"
#include "Other.h"
#include "CustomEventTime.h"
#include "StatsAdvance.h"
#include <CustomRanking.h>
#include "CBInterfaceVIPChar.h"
#include "NewUICustomMessageBox.h"
#include "ChangeClass.h"
cCustomMenu gBCustomMenu;

cCustomMenu::cCustomMenu()
{

}

cCustomMenu::~cCustomMenu()
{
}


void cCustomMenu::GetCountButton()
{

}
void cCustomMenu::ActionButton(int TypeButton)
{

	switch (TypeButton)
	{
	case eButtonVipShop: //VIP
	{
		gInterface.Data[eVip_MAIN].OpenClose();
	}
	break;
	case eButtonRanking: //Ranking
	{

	}
	break;
	case eButtonChangeClass: //Change Class
	{

	}
	break;
	case eButtonMarKet: //market
	{


	}
	break;
#if(DANH_HIEU_NEW == 1)
	case eButtonDanhHieu: //titles
	{

	}
	break;
#endif

	case eButtonJewelBank: //jewel bank
	{

	}
	break;
	default:
		break;
	}
	gInterface.Data[eMenu_MAIN].OnShow = 0;
}
extern int  AlphaBlendType;
void cCustomMenu::ToolTipPegasus(int sx, int sy, const char* Text)
{
	SIZE TextSize = { 0, 0 };
	g_pRenderText->SetFont(g_hFont);
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), Text, lstrlen(Text), &TextSize);

	int BackupAlphaBlendType = AlphaBlendType;
	EnableAlphaTest();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);

	// Nudged up and left so the cursor doesn't cover the label.
	g_pRenderText->RenderText(sx - 10.0f, sy - 10.0f, Text);

	// Put the blend mode back the way the caller had it.
	switch (BackupAlphaBlendType)
	{
	case 0:
		DisableAlphaBlend();
		break;
	case 1:
		EnableLightMap();
		break;
	case 2:
		EnableAlphaTest();
		break;
	case 3:
		EnableAlphaBlend();
		break;
	case 4:
		EnableAlphaBlendMinus();
		break;
	case 5:
		EnableAlphaBlend2();
		break;
	default:
		DisableAlphaBlend();
		break;
	}
}

void cCustomMenu::Draw()
{

	if (!gInterface.Data[eMenu_MAIN].OnShow) return;

	float MainWidth = 300;
	float MainHeight = 290;
	float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2) + 1.0f;
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 25.0f;


	g_pBCustomMenuInfo->gDrawWindowPega(&StartX, &StartY, MainWidth, MainHeight, eMenu_MAIN, "Menu Game");


	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);


	RenderImageFFF(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_Frame_Menu, StartX, StartY, MainWidth, MainHeight, 0.0, 0.0, 548, 752);

	gInterface.DrawFormat(eYellow, StartX + 35.0f, StartY + 14.0f, 100, 1, "Menu Game [F5]");



	// Game Option
	gInterface.DrawFormat(eWhite, StartX + 45.0f, StartY + 75.0f, 100, 1, "[O]");
	bool Option = SEASON3B::CheckMouseIn(StartX + 30.0f, StartY + 37.0f, 40, 40);
	bool OptionEffect = Option && SEASON3B::IsPress(VK_LBUTTON);
	if (Option)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 25.0f, StartY + 37.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 30.0f, StartY + 37.0f, "Game Option [O]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 300, StartX + 35.0f, StartY + 43.0f, 27, 24, 0.0, 0.0, 72, 72);
	if (OptionEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_OPTION);
			gInterface.Data[eMenu_MAIN].Close();
		}
	}

	// Character
	gInterface.DrawFormat(eWhite, StartX + 95.0f, StartY + 75.0f, 100, 1, "[C]");
	bool Character = SEASON3B::CheckMouseIn(StartX + 85.0f, StartY + 37.0f, 40, 40);
	bool CharacterEffect = Character && SEASON3B::IsPress(VK_LBUTTON);
	if (Character)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 73.0f, StartY + 37.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 80.0f, StartY + 37.0f, "Character Window [C]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 301, StartX + 85.0f, StartY + 42.0f, 27, 28, 0.0, 0.0, 72, 72);
	if (CharacterEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHARACTER);
		}
		else
		{

			g_pNewUISystem->Show(SEASON3B::INTERFACE_CHARACTER);


			gInterface.Data[eMenu_MAIN].Close();
		}
	}


	// Inventory
	gInterface.DrawFormat(eWhite, StartX + 142.0f, StartY + 75.0f, 100, 1, "[I/V]");
	bool Inventory = SEASON3B::CheckMouseIn(StartX + 127.0f, StartY + 37.0f, 40, 40);
	bool InventoryEffect = Inventory && SEASON3B::IsPress(VK_LBUTTON);
	if (Inventory)
	{

		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 125.0f, StartY + 37.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 127.0f, StartY + 37.0f, "Inventory Window [I/V]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 302, StartX + 139.0f, StartY + 43.0f, 25, 27, 0.0, 0.0, 72, 72);
	if (InventoryEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_INVENTORY);
			gInterface.Data[eMenu_MAIN].Close();
		}
	}



	// Command
	gInterface.DrawFormat(eWhite, StartX + 195.0f, StartY + 75.0f, 100, 1, "[D]");
	bool Command = SEASON3B::CheckMouseIn(StartX + 175.0f, StartY + 37.0f, 40, 40);
	bool CommandEffect = Command && SEASON3B::IsPress(VK_LBUTTON);
	if (Command)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 175.0f, StartY + 37.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 180.0f, StartY + 37.0f, "Command Window [D]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 303, StartX + 185.0f, StartY + 43.0f, 30, 27, 0.0, 0.0, 72, 72);
	if (CommandEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_COMMAND))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_COMMAND);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_COMMAND);
			gInterface.Data[eMenu_MAIN].Close();
		}
	}




	// Guild
	gInterface.DrawFormat(eWhite, StartX + 245.0f, StartY + 75.0f, 100, 1, "[G]");
	bool Guild = SEASON3B::CheckMouseIn(StartX + 230.0f, StartY + 37.0f, 40, 40);
	bool GuildEffect = Guild && SEASON3B::IsPress(VK_LBUTTON);
	if (Guild)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 225.0f, StartY + 37.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 230.0f, StartY + 37.0f, "Guild Window [G]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 304, StartX + 240.0f, StartY + 43.0f, 22, 27, 0.0, 0.0, 72, 72);
	if (GuildEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_GUILDINFO);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_GUILDINFO);
			gInterface.Data[eMenu_MAIN].Close();
		}
	}

	// Row 2

	// Change Class
	gInterface.DrawFormat(eWhite, StartX + 43.0f, StartY + 131.0f, 100, 1, "[F4]");
	bool Change = SEASON3B::CheckMouseIn(StartX + 30.0f, StartY + 93.0f, 40, 40);
	bool ChangeEffect = Change && SEASON3B::IsPress(VK_LBUTTON);

	if (Change)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 25.0f, StartY + 93.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 30.0f, StartY + 93.0f, "Change Class [F4]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 305, StartX + 35.0f, StartY + 100.0f, 30, 29, 0.0, 0.0, 72, 72);
	if (ChangeEffect)
	{
		gChangeClass.ChangeClass();
		gInterface.Data[eCHANGINGCLASS_MAIN].OnShow;
		gInterface.Data[eMenu_MAIN].Close();

	}

	// VipMain
	gInterface.DrawFormat(eWhite, StartX + 95.0f, StartY + 131.0f, 100, 1, "[F2]");
	bool VipMain = SEASON3B::CheckMouseIn(StartX + 80.0f, StartY + 93.0f, 40, 40);
	bool VipMainEffect = VipMain && SEASON3B::IsPress(VK_LBUTTON);
	if (VipMain)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 76.0f, StartY + 93.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 80.0f, StartY + 93.0f, "Vip Train Windown [F2]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 306, StartX + 87.0f, StartY + 100.0f, 29, 26, 0.0, 0.0, 72, 72);
	if (VipMainEffect)
	{
		gInterface.DrawWindowMuaVIP();
		gInterface.Data[eVip_MAIN].OnShow ^= 1;
		gInterface.Data[eMenu_MAIN].Close();
	}



	// Party
	gInterface.DrawFormat(eWhite, StartX + 145.0f, StartY + 131.0f, 100, 1, "[P]");
	bool Party = SEASON3B::CheckMouseIn(StartX + 127.0f, StartY + 93.0f, 40, 40);
	bool PartyEffect = Party && SEASON3B::IsPress(VK_LBUTTON);
	if (Party)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 126.0f, StartY + 93.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 132.0f, StartY + 93.0f, "Party Window [P]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 307, StartX + 137.0f, StartY + 100.0f, 27, 26, 0.0, 0.0, 72, 72);
	if (PartyEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_PARTY);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_PARTY);
			gInterface.Data[eMenu_MAIN].Close();
		}
	}




	// Event Time
	gInterface.DrawFormat(eWhite, StartX + 195.0f, StartY + 131.0f, 100, 1, "[H]");
	bool Event = SEASON3B::CheckMouseIn(StartX + 180.0f, StartY + 93.0f, 40, 40);
	bool EventEffect = Event && SEASON3B::IsPress(VK_LBUTTON);

	if (Event)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 176.0f, StartY + 93.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 180.0f, StartY + 93.0f, "EventTime Windown [H]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 308, StartX + 190.0f, StartY + 100.0f, 25, 27, 0.0, 0.0, 72, 72);
	if (EventEffect)
	{
		gSauEventTime.OpenSauEvent();
		gInterface.Data[eMenu_MAIN].Close();

	}

	// Jewel
	gInterface.DrawFormat(eWhite, StartX + 248.0f, StartY + 131.0f, 100, 1, "[J]");
	bool Jewel = SEASON3B::CheckMouseIn(StartX + 230.0f, StartY + 93.0f, 40, 40);
	bool JewelEffect = Jewel && SEASON3B::IsPress(VK_LBUTTON);
	if (Jewel)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 227.0f, StartY + 93.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 230.0f, StartY + 93.0f, "Jewel Window [J]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 309, StartX + 238.0f, StartY + 100.0f, 28, 26, 0.0, 0.0, 72, 72);
	if (JewelEffect)
	{
		gInterface.OnOffWindowBank();
		gInterface.Data[eMenu_MAIN].Close();
	}

	// Row 3
	// MoveMap
	gInterface.DrawFormat(eWhite, StartX + 45.0f, StartY + 187.0f, 100, 1, "[M]");
	bool MoveMap = SEASON3B::CheckMouseIn(StartX + 30.0f, StartY + 149.0f, 40, 40);
	bool MoveMapEffect = MoveMap && SEASON3B::IsPress(VK_LBUTTON);
	if (MoveMap)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 25.0f, StartY + 149.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 30.0f, StartY + 149.0f, "MoveMap Windown [M]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 310, StartX + 35.0f, StartY + 155.0f, 30, 26, 0.0, 0.0, 72, 72);
	if (MoveMapEffect)
	{
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_MOVEMAP))
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_MOVEMAP);
		}
		else
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_MOVEMAP);
			gInterface.Data[eMenu_MAIN].Close();
		}
	}

	// Ranking
	gInterface.DrawFormat(eWhite, StartX + 95.0f, StartY + 187.0f, 100, 1, "[F5]");
	bool Ranking = SEASON3B::CheckMouseIn(StartX + 80.0f, StartY + 149.0f, 40, 40);
	bool RankingEffect = Ranking && SEASON3B::IsPress(VK_LBUTTON);

	if (Ranking)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 77.0f, StartY + 149.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 80.0f, StartY + 149.0f, "Ranking Windown [F5]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 311, StartX + 88.0f, StartY + 155.0f, 28, 27, 0.0, 0.0, 72, 72);
	if (RankingEffect)
	{
		gCustomRanking.OpenWindow();
		gInterface.Data[eMenu_MAIN].Close();
	}

	// Military rank
	gInterface.DrawFormat(eWhite, StartX + 146.0f, StartY + 187.0f, 100, 1, "[F1]");
	bool QuanHam = SEASON3B::CheckMouseIn(StartX + 127.0f, StartY + 149.0f, 40, 40);
	bool QuanHamEffect = QuanHam && SEASON3B::IsPress(VK_LBUTTON);
	if (QuanHam)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 155, StartX + 128.0f, StartY + 149.0f, 50, 50, 0.0, 0.0, 96, 96);
		ToolTipPegasus(StartX + 127.0f, StartY + 149.0f, "Military Rank[F1]");
	}
	RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 312, StartX + 139.0f, StartY + 155.0f, 25, 28, 0.0, 0.0, 72, 72);
	if (QuanHamEffect)
	{

		g_pNewUISystem->Show(SEASON3B::INTERFACE_INVENTORY);
		gBInterfaceVIPChar.CGSendOpenWinwdowVIP();
		gInterface.Data[eMenu_MAIN].Close();

	}
	static bool Close = false;
	static float CloseP = 0.0f;

	bool CloseA = SEASON3B::CheckMouseIn(StartX + 125, StartY + MainHeight - 37.0f, 52, 25);
	bool CloseB = CloseA && SEASON3B::IsPress(VK_LBUTTON);

	if (CloseB)
	{
		Close = true;
		CloseP = 0.2f;

		gInterface.Data[eMenu_MAIN].Close();
	}


	if (Close)
	{
		CloseP -= 0.01f;

		if (CloseP <= 0.0f)
		{
			Close = false;
		}
	}
	if (Close)
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + 130, StartX + 125, StartY + MainHeight - 37.0f, 52, 25, 0.0, 0.0, 28, 28);
	}
	else
	{
		RenderImageFFF(INTERFACE_HERO_POSITION_INFO + (CloseA ? 131 : 130), StartX + 125, StartY + MainHeight - 37.0f, 52, 25, 0.0, 0.0, 28, 28);
	}
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(StartX + 102, StartY + MainHeight - 28.0f, "Close", 100, 0, RT3_SORT_CENTER);


	DisableAlphaBlend();
}

void cCustomMenu::OpenCustomMenu()
{
	if ((GetTickCount() - gInterface.Data[eMenu_MAIN].EventTick) < 500)
	{
		return;
	}
	gInterface.Data[eMenu_MAIN].EventTick = GetTickCount();
	gInterface.Data[eMenu_MAIN].OnShow ^= 1;
}
