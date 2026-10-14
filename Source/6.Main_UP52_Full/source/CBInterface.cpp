#include "stdafx.h"
#include "CBInterface.h"
#include "NewUISystem.h"
#include "UIControls.h"
#include "TrayMode.h"
#include "DSPlaySound.h"
#include "ZzzInterface.h"
#include "Other.h"
#include "MenuCustom.h"
#include "PegasusBattlePass.h"
#include "PegasusDungeon.h"
#include "PegasusCollection.h"
#include "PegasusDaily.h"
#include "PegasusNameFx.h"
#include "Protocol.h"
#include "CustomEventTime.h"
#include "MapManager.h"
#include "CustomRanking.h"
#include "CharacterManager.h"
#include "StatsAdvance.h"
#include "CBInterfaceVIPChar.h"
#include "CustomRanking.h"
#include "Util.h"
#include "ChangeClass.h"

extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayWinMid;
extern int DisplayHeightExt;
extern int DisplayWinExt;
extern int DisplayWinReal;

Interface gInterface;

Interface::Interface()
{
	this->IndexCustomTextture = 0;

	this->MsgBoxCaption.clear();
	this->MsgBoxText.clear();
	this->MsgBoxType = 0;
	this->MsgLine = 0;
	ZeroMemory(&Data, sizeof(Data));
	this->CloseAllWindowCustom();
	this->vCaptcha = this->generateCaptcha(4);
	this->KepChuot = false;
	this->AutoPK = false;

}

Interface::~Interface()
{

}

void Interface::DrawMessageBox()
{
	if (!gInterface.Data[eWindowMessageBox].OnShow)
	{
		return;
	}
	float CuaSoW = 220;
	float CuaSoH = 80 + (this->MsgLine * 10);

	float StartX = (MAX_WIN_WIDTH / 2) - (CuaSoW / 2);
	float StartY = 45.0;

	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, CuaSoW, CuaSoH, eWindowMessageBox, this->MsgBoxCaption.c_str());
	EnableAlphaTest(true);
	glColor3f(1.0, 1.0, 1.0);
	g_pBCustomMenuInfo->SetBlockCur(TRUE);
	this->MsgLine = TextDraw((HFONT)g_hFont, StartX, StartY + 35, 0xFFFFFFFF, 0x0, CuaSoW, 0, 3, this->MsgBoxText.c_str());
	if (gInterface.Data[eWindowMessageBox].Height < (80 + (this->MsgLine * 10))) gInterface.Data[eWindowMessageBox].Height = (float)(80 + (this->MsgLine * 10));

	if (this->MsgBoxType == 0)
	{
		float BtnHeight = 20.0;
		float BtnWidth = 120.0;
		if (g_pBCustomMenuInfo->DrawButton(StartX + CuaSoW / 2 - (BtnWidth / 2), StartY + CuaSoH - BtnHeight - 10, BtnWidth, 12, "Fechar", 0.0, 1))
		{
			gInterface.Data[eWindowMessageBox].OnShow = 0;
		}
	}
	else if (this->MsgBoxType == 1)
	{
		float BtnHeight = 20.0;
		float BtnWidth = 55.0;
		if (g_pBCustomMenuInfo->DrawButton(StartX + CuaSoW / 2 - (BtnWidth + 7.5), StartY + CuaSoH - BtnHeight - 10, BtnWidth, 12, "OK"))
		{
			this->MsgBoxCallback(this);
			this->MsgBoxCallback = 0;
			gInterface.Data[eWindowMessageBox].OnShow = 0;
		}

		if (g_pBCustomMenuInfo->DrawButton(StartX + CuaSoW / 2 + (7.5), StartY + CuaSoH - BtnHeight - 10, BtnWidth, 12, "Fechar",0.0,1))
		{
			gInterface.Data[eWindowMessageBox].OnShow = 0;
		}
	}
	DisableAlphaBlend();
	EnableAlphaTest(0);

}

void Interface::OpenMessageBox(char* caption, char* Format, ...)
{
	if (gInterface.Data[eWindowMessageBox].OnShow)
	{
		return;
	}

	char text[1024] = { 0 };
	va_list va;
	va_start(va, Format);
	vsprintf_s(text, Format, va);
	va_end(va);
	this->MsgLine = 0;
	this->MsgBoxCaption.clear();
	this->MsgBoxText.clear();
	this->MsgBoxCaption = caption;
	this->MsgBoxText = Format;

	this->MsgLine = 1;
	gInterface.Data[eWindowMessageBox].OnShow = true;
	this->MsgBoxType = 0;

}

void Interface::OpenMessageBoxOkCancel(PUSHEVENT_CALLBACK_LPVOID pCallbackFunc, char* caption, char* Format, ...)
{
	if (gInterface.Data[eWindowMessageBox].OnShow)
	{
		return;
	}

	char text[1024] = { 0 };
	va_list va;
	va_start(va, Format);
	vsprintf_s(text, Format, va);
	va_end(va);

	this->MsgLine = 0;
	this->MsgBoxCaption.clear();
	this->MsgBoxText.clear();
	this->MsgBoxCaption = caption;
	this->MsgBoxText = Format;
	this->MsgLine = 1;
	gInterface.Data[eWindowMessageBox].OnShow = true;
	this->MsgBoxType = 1;
	this->MsgBoxCallback = pCallbackFunc;

}

void CBRenderText(char* text, int x, int y, int sx, int sy, DWORD color, DWORD backcolor, int sort)
{
	g_pRenderText->SetFont(g_hFont);

	DWORD backuptextcolor = g_pRenderText->GetTextColor();
	DWORD backuptextbackcolor = g_pRenderText->GetBgColor();

	g_pRenderText->SetTextColor(color);
	g_pRenderText->SetBgColor(backcolor);
	g_pRenderText->RenderText(x, y, text, sx, sy, sort);

	g_pRenderText->SetTextColor(backuptextcolor);
	g_pRenderText->SetBgColor(backuptextbackcolor);
}
char* Interface::fNumberFormat(float number)
{
	std::ostringstream stream;
	stream.imbue(std::locale(""));
	stream << std::fixed << std::setprecision(2) << number;
	std::string formattedNumber = stream.str();
	formattedNumber.erase(formattedNumber.find_last_not_of('0'), std::string::npos);
	return strdup(formattedNumber.c_str());
}

char* Interface::NumberFormat(int Number)
{

	if (Number < 0) { return "0"; }

	char OutPut[15];

	if (Number < 1000) {
		sprintf(OutPut, "%d", Number);
		return strdup(OutPut);
	}
	else if (Number < 1000000) {
		int part1 = Number % 1000;
		int part2 = (Number - part1) / 1000;
		sprintf(OutPut, "%d,%03d", part2, part1);
		return strdup(OutPut);
	}
	else if (Number < 1000000000) {
		int part1 = Number % 1000;
		Number = (Number - part1) / 1000;
		int part2 = Number % 1000;
		Number = (Number - part2) / 1000;
		int part3 = Number % 1000;

		sprintf(OutPut, "%d,%03d,%03d", part3, part2, part1);
		return strdup(OutPut);
	}
	else {
		int part1 = Number % 1000;
		Number = (Number - part1) / 1000;
		int part2 = Number % 1000;
		Number = (Number - part2) / 1000;
		int part3 = Number % 1000;
		int part4 = (Number - part3) / 1000;

		sprintf(OutPut, "%d,%03d,%03d,%03d", part4, part3, part2, part1);
		return strdup(OutPut);
	}
	return "0";
}

void Interface::DrawBarForm(float PosX, float PosY, float Width, float Height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	EnableAlphaTest(true);
	glColor4f((GLfloat)red, (GLfloat)green, (GLfloat)blue, (float)alpha);
	RenderColor(PosX, PosY, Width, Height, 0.0f, 0);
	EndRenderColor();
	glColor3f(1.0, 1.0, 1.0);
	EnableAlphaTest(false);
}

int Interface::DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	int LineCount = 0;

	char* Line = strtok(Buff, "\n");

	while (Line != NULL)
	{
		CBRenderText(Line, PosX, PosY, Width, 0, Color, 0, Align);

		PosY += 10;
		Line = strtok(NULL, "\n");
	}

	return PosY;
}

bool Interface::CheckWindow(int WindowID)
{
	return g_pNewUISystem->IsVisible(WindowID);
}
bool SetCamera = false;

void SetCameraZoom()
{

	if (SetCamera == false)
	{
		Camera3DEnable ^= 1;
		Camera3DSet = true;
		CameraFOV = 60;
		SetCamera = true;
	}

}

const float MIN_FOV = 35.0f;

bool Interface::UseHotKey()
{

	if (GetForegroundWindow() != g_hWnd || g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
	{
		return false;
	}
	SetCameraZoom();

	if (SEASON3B::IsPress(VK_F12))
	{
		gTrayMode.Toggle();
		return true;
	}

	if (SEASON3B::IsPress(VK_F5))
	{
		gBCustomMenu.OpenCustomMenu();
		gInterface.Data[eWinDowQuanHam].Close();
		gInterface.Data[eVip_MAIN].Close();
		gInterface.Data[JEWELBANK_WINDOW].Close();
		gInterface.Data[eRankPANEL_MAIN].Close();
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (SEASON3B::IsPress('H'))
	{
		gSauEventTime.OpenSauEvent();
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (SEASON3B::IsPress(VK_F2))
	{
		gInterface.DrawWindowMuaVIP();
		gInterface.Data[eVip_MAIN].OnShow ^= 1;
		PlayBuffer(SOUND_CLICK01);
		return false;
	}

	if (SEASON3B::IsPress(VK_F3))
	{
		gCustomRanking.OpenWindow();
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (SEASON3B::IsPress(VK_F4))
	{
		gChangeClass.ChangeClass();
		gInterface.Data[eCHANGINGCLASS_MAIN].OnShow;
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (SEASON3B::IsPress('O'))
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_OPTION);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	if (SEASON3B::IsPress('C'))
	{
		gInterface.Data[eCongDiem].Close();
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	else if (SEASON3B::IsPress(VK_F1) == true)
	{

		gBInterfaceVIPChar.CGSendOpenWinwdowVIP();

		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
		PlayBuffer(SOUND_CLICK01);
		return false;
	}

	if (SEASON3B::IsPress(VK_END) == true)
	{
		this->KepChuot ^= 1;
		return true;
	}

#if(Interface_Legend)

	else if (SEASON3B::IsPress(VK_ESCAPE) == true)
	{

		g_StatsAdvance.DrawStart();

		gInterface.Data[eStatsAdvance].Close();

		PlayBuffer(SOUND_CLICK01);

		return false;
	}
#else

#endif
	else if (SEASON3B::IsPress('J'))
	{

		gInterface.OnOffWindowBank();
		gInterface.Data[eMenu_MAIN].Close();
		PlayBuffer(SOUND_CLICK01);
		return false;
	}

	if (SEASON3B::IsPress(VK_F10))
	{
		Camera3DEnable ^= 1;

		if (Camera3DEnable)
		{
			CreateNotice("Ligar Camera 3D", 1);
		}
		else
		{
			CreateNotice("Desligar Camera 3D", 1);
		}
		PlayBuffer(SOUND_CLICK01);
		return true;
	}
	if (SEASON3B::IsPress(VK_F11))
	{
		Camera3DSet = true;
		CameraAngle[2] = -45.f;
		Camera3DPosY = 0.f;
		Camera3DPosZ = 0.f;
		CameraFOV = MIN_FOV;
		Camera3DFov = 0.f;

		CreateNotice("Resetar Camera 3D", 1);
		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	return false;
}
int Interface::DrawMessage(int Mode, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff);
	ZeroMemory(Buff, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	CreateNotice(Buff, Mode);
	return 1;
}

void Interface::CloseAllWindowCustom()
{
	for(int i = eBeginWindowCustom; i < eEndWindowCustom; i++) gInterface.Data[i].Close();
}

bool Interface::isCustomDraw()
{
	for (int i = eBeginWindowCustom; i < eEndWindowCustom; i++)
	{
		if (gInterface.Data[i].OnShow == true)
		{
			return 1;
		}
	}

	return 0;
}

bool Interface::ActionSendItemGS(ITEM* ItemSell, int Slot)
{
	if (gBInterfaceVIPChar.SendItem(ItemSell, Slot, GetKeyState(VK_RBUTTON))) return 1;
	return 0;
}

bool PegasusMouseOnWindow()
{
	return gPegasusBattlePass.IsMouseOver()
		|| gPegasusDungeon.IsMouseOver()
		|| gPegasusCollection.IsMouseOver()
		|| gPegasusDaily.IsMouseOver()
		|| gPegasusNameFx.IsMouseOver();
}

DWORD EventTick = 0;
void Interface::Work()
{
	if ((GetTickCount() - EventTick) > 500)
	{
		if (gInterface.UseHotKey())
		{
			EventTick = GetTickCount();
		}
	}

	if (KepChuot == true)
	{
		MouseRButtonPush = 1;
		TextDraw((HFONT)g_hFontBold, 1, 200, 0xFFAA00FF, 0x0, 180.0, 0, 1, gOther.Text_Chung[45]);
		TextDraw((HFONT)g_hFontBold, 1, 212, 0xFFAA00FF, 0x0, 180.0, 0, 1, gOther.Text_Chung[46]);
	}
	else
	{
		MouseRButtonPush = 0;
	}

	gChangeClass.drawNewInterfaceClassChange();

	gInterface.DrawWindowMuaVIP();

	gBCustomMenu.Draw();

	// Mouse first, and in reverse draw order, so the window nearest the front
	// takes the click and the ones behind it never see it. These all centre
	// themselves, so any two open at once overlap almost completely - handing
	// each of them the same click meant a press in the overlap selected a tier
	// in one window and a set in another at the same time.
	if (gPegasusNameFx.UpdateMouseEvent() == false
		&& gPegasusDaily.UpdateMouseEvent() == false
		&& gPegasusCollection.UpdateMouseEvent() == false
		&& gPegasusDungeon.UpdateMouseEvent() == false)
	{
		gPegasusBattlePass.UpdateMouseEvent();
	}

	// Drawn back to front, the other way round.
	gPegasusBattlePass.Render();
	gPegasusDungeon.Render();
	gPegasusCollection.Render();
	gPegasusDaily.Render();
	gPegasusNameFx.Render();

	gSauEventTime.DrawSauEventTime();

	gCustomRanking.DrawRankPanelWindow();

	gInterface.DrawCongDiem();

	gBInterfaceVIPChar.Draw();

	g_StatsAdvance.DrawStart();

#if(JEWELBANKVER2)
	gInterface.DrawWindowJewelBankVer2();
#endif
	gBInterfaceVIPChar.Draw();

	gInterface.DrawInfoMini();
	gInterface.DrawMessageBox();
}

void Interface::BLoadImage()
{
	g_pBCustomMenuInfo->BindObject(eMenu, "Interface\\GFx\\MacroUI\\MenuSetting.tga", 51, 41, -1, -1);

	for (std::map<int, CUSTOMVIPCHAR_DATA>::iterator it = gBInterfaceVIPChar.m_VipCharRank.begin(); it != gBInterfaceVIPChar.m_VipCharRank.end(); it++)
	{
		if (it->second.TypeRank <= 1)
		{
			continue;
		}
		char path[MAX_PATH] = { 0 };

		wsprintf(path, "%s", it->second.TextDir);

		LoadBitmap(path, 150000 + it->second.Index, GL_LINEAR);

	}
}
std::string Interface::generateCaptcha(int n)
{
	time_t t;
	srand((unsigned)time(&t));
	char* required_chars = "0123456789";
	std::string captcha = "";
	while (n--)
		captcha.push_back(required_chars[rand() % sizeof(required_chars)]);
	return captcha;
}

bool Interface::check_Captcha(std::string& captcha, std::string& user_input)
{
	return captcha.compare(user_input) == 0;
}
void Interface::RenderCaptchaNumber(float PosX, float PosY, CUITextInputBox* a6, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	gInterface.DrawBarForm(PosX, PosY + 3.5, 120, 17, 0.0, 0.0, 0.0, 1.0);
	gInterface.DrawBarForm(PosX + 2, PosY + 4, 50, 15, 1.0, 0.2167, 0.0, 1.0);
	TextDraw((HFONT)g_hFontBold, PosX + 2, PosY + 4 + 2, 0xFFFFFFB8, 0x0, 50, 0, 3, Buff);
	a6->SetPosition(PosX + 60, PosY + 6.5);
	a6->Render();

	if (SEASON3B::CheckMouseIn(PosX - 5, PosY - 5, 120, 30) == 1)
	{

		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			a6->GiveFocus(1);
			PlayBuffer(25, 0, 0);
		}
	}

}

void Interface::RenderImages(GLuint Image, float x, float y, float width, float height, float su, float sv, float uw, float vh)
{
	BITMAP_t* pText = &Bitmaps[Image];
	RenderBitmap(Image, x, y, width, height, su / pText->Width, sv / pText->Height, uw / pText->Width, vh / pText->Height, 1, 1, 0.0);
}

void Interface::CreateNButton(int Image, float x, float y, float width, float heith, int windows)
{
	float sv = 0.0;

	if (SEASON3B::CheckMouseIn((int)x, (int)y, (int)width, (int)heith))
	{
		sv += (MouseLButton) ? 108.f : 54.f;
	}

	if (g_pNewUISystem->IsVisible(windows))
	{
		sv = 108.f;
	}
	RenderImages(Image, x, y, width, heith, 0.0, sv, 53.5f, 54.f);
}

void Interface::DrawInfoMini()
{
	if (gInterface.CheckWindow(ObjWindow::CashShop)
		|| gInterface.CheckWindow(ObjWindow::FriendList)
		|| gInterface.CheckWindow(ObjWindow::MoveList)
		|| gInterface.CheckWindow(ObjWindow::Party)
		|| gInterface.CheckWindow(ObjWindow::Quest)
		|| gInterface.CheckWindow(ObjWindow::NPC_Devin)
		|| gInterface.CheckWindow(ObjWindow::Guild)
		|| gInterface.CheckWindow(ObjWindow::Trade)
		|| gInterface.CheckWindow(ObjWindow::Warehouse)
		|| gInterface.CheckWindow(ObjWindow::ChaosBox)
		|| gInterface.CheckWindow(ObjWindow::CommandWindow)
		|| gInterface.CheckWindow(ObjWindow::PetInfo)
		|| gInterface.CheckWindow(ObjWindow::Shop)
		|| gInterface.CheckWindow(ObjWindow::Inventory)
		|| gInterface.CheckWindow(ObjWindow::Store)
		|| gInterface.CheckWindow(ObjWindow::OtherStore)
		|| gInterface.CheckWindow(ObjWindow::Character)
		|| gInterface.CheckWindow(ObjWindow::DevilSquare)
		|| gInterface.CheckWindow(ObjWindow::BloodCastle)
		|| gInterface.CheckWindow(ObjWindow::CreateGuild)
		|| gInterface.CheckWindow(ObjWindow::GuardNPC)
		|| gInterface.CheckWindow(ObjWindow::SeniorNPC)
		|| gInterface.CheckWindow(ObjWindow::GuardNPC2)
		|| gInterface.CheckWindow(ObjWindow::CastleGateSwitch)
		|| gInterface.CheckWindow(ObjWindow::CatapultNPC)
		|| gInterface.CheckWindow(ObjWindow::CrywolfGate)
		|| gInterface.CheckWindow(ObjWindow::IllusionTemple)
		|| gInterface.CheckWindow(ObjWindow::HeroList)
		|| gInterface.CheckWindow(ObjWindow::ChatWindow)
		|| gInterface.CheckWindow(ObjWindow::FastMenu)
		|| gInterface.CheckWindow(ObjWindow::Options)
		|| gInterface.CheckWindow(ObjWindow::Help)
		|| gInterface.CheckWindow(ObjWindow::FastDial)
		|| gInterface.CheckWindow(ObjWindow::SkillTree)
		|| gInterface.CheckWindow(ObjWindow::GoldenArcher1)
		|| gInterface.CheckWindow(ObjWindow::GoldenArcher2)
		|| gInterface.CheckWindow(ObjWindow::LuckyCoin1)
		|| gInterface.CheckWindow(ObjWindow::LuckyCoin2)
		|| gInterface.CheckWindow(ObjWindow::NPC_Duel)
		|| gInterface.CheckWindow(ObjWindow::NPC_Titus)
		|| gInterface.CheckWindow(ObjWindow::CashShop)
		|| gInterface.CheckWindow(ObjWindow::Lugard)
		|| gInterface.CheckWindow(ObjWindow::QuestList1)
		|| gInterface.CheckWindow(ObjWindow::QuestList2)
		|| gInterface.CheckWindow(ObjWindow::Jerint)
		|| gInterface.CheckWindow(ObjWindow::FullMap)
		|| gInterface.CheckWindow(ObjWindow::NPC_Dialog)
		|| gInterface.CheckWindow(ObjWindow::GensInfo)
		|| gInterface.CheckWindow(ObjWindow::NPC_Julia)
		|| gInterface.CheckWindow(ObjWindow::NPC_ChaosMix)
		|| gInterface.CheckWindow(ObjWindow::ExpandInventory)
		|| gInterface.CheckWindow(ObjWindow::ExpandWarehouse)
		|| gInterface.CheckWindow(ObjWindow::MuHelper))
	{
		return;
	}

	float SizeButtonW = 26;
	float SizeButtonH = 26;
	float StartX = 5;
	float StartY = DisplayHeight - SizeButtonW - 50;
	EnableAlphaTest(true);
	glColor3f(1.0, 1.0, 1.0);

	if (gProtect.m_MainInfo.EnableButtonFB)
	{
		if (SEASON3B::CheckMouseIn(gProtect.m_MainInfo.ViTriFB[0], gProtect.m_MainInfo.ViTriFB[1]+ DisplayHeightExt, SizeButtonW, SizeButtonH) && GetForegroundWindow() == g_hWnd)
		{
			glColor4f(0.0, 0.5, 0.5, 1.0);
			RenderBitmap(gInterface.Data[eButtonMXH].ModelID, gProtect.m_MainInfo.ViTriFB[0], gProtect.m_MainInfo.ViTriFB[1]+ DisplayHeightExt, SizeButtonW, SizeButtonH, 0.0, 0.0, 0.37, 0.78, 1, 1, 0);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			g_pBCustomMenuInfo->SetBlockCur();

			if (GetTickCount() - gInterface.Data[eTIME].EventTick > 500)
			{
				if (GetKeyState(VK_LBUTTON) & 0x8000)
				{
					ShellExecute(0, 0, gProtect.m_MainInfo.LinkFacbook, 0, 0, SW_SHOW);
					gInterface.Data[eTIME].EventTick = GetTickCount();
					PlayBuffer(25, 0, 0);
				}
			}
		}
		else
		{
			RenderBitmap(gInterface.Data[eButtonMXH].ModelID, gProtect.m_MainInfo.ViTriFB[0], gProtect.m_MainInfo.ViTriFB[1] + DisplayHeightExt, SizeButtonW, SizeButtonH, 0.0, 0.0, 0.37, 0.78, 1, 1, 0);
		}
	}
	if (gProtect.m_MainInfo.EnableButtonZL)
	{
		if (SEASON3B::CheckMouseIn(gProtect.m_MainInfo.ViTriZL[0], gProtect.m_MainInfo.ViTriZL[1] + DisplayHeightExt, SizeButtonW, SizeButtonH) && GetForegroundWindow() == pGameWindow)
		{
			glColor4f(0.0, 0.5, 0.5, 1.0);
			RenderBitmap(gInterface.Data[eButtonMXH].ModelID, gProtect.m_MainInfo.ViTriZL[0], gProtect.m_MainInfo.ViTriZL[1] + DisplayHeightExt, SizeButtonW, SizeButtonH, 0.37, 0.0, 0.37, 0.78, 1, 1, 0);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			g_pBCustomMenuInfo->SetBlockCur();

			if (GetTickCount() - gInterface.Data[eTIME].EventTick > 500)
			{
				if (GetKeyState(VK_LBUTTON) & 0x8000)
				{
					ShellExecute(0, 0, gProtect.m_MainInfo.LinkZalo, 0, 0, SW_SHOW);
					gInterface.Data[eTIME].EventTick = GetTickCount();
					PlayBuffer(25, 0, 0);
				}
			}
		}
		else
		{
			RenderBitmap(gInterface.Data[eButtonMXH].ModelID, gProtect.m_MainInfo.ViTriZL[0], gProtect.m_MainInfo.ViTriZL[1] + DisplayHeightExt, SizeButtonW, SizeButtonH, 0.37, 0.0, 0.37, 0.78, 1, 1, 0);
		}
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	DisableAlphaBlend();
	EnableAlphaTest(0);

}

void Interface::DrawWindowMuaVIP()
{
	if (!gInterface.Data[eVip_MAIN].OnShow)
	{
		return;
	}

	float MainWidth = 250.0;
	float MainHeight = 270;

	float StartY = 50.0;

	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 30.0f;
	float MainCenter = StartX + (MainWidth / 3);
	float ButtonX = MainCenter - (float)(29.0 / 2);

	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, eVip_MAIN, gCustomMessage.GetMessage(5));

	float StartBody = StartY;
	EnableAlphaTest(true);
	glColor3f(1.0, 1.0, 1.0);

	int CloseX = StartX + 192.0f;

	int CloseY = StartY + 3.0f;

	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 30, 30) == 1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eVip_MAIN].EventTick > 300)
		{

			gInterface.Data[eVip_MAIN].Close();
		}
		RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30.0f, 30.0f);
	}

	int LineY = gInterface.DrawFormat(eWhite, (int)StartX + 30, (int)StartBody + 35, 210, 1, gCustomMessage.GetMessage(6), CharacterAttribute->Name);
	LineY = gInterface.DrawFormat(eGold, (int)StartX + 30, (int)LineY, 210, 1, gCustomMessage.GetMessage(14));
	LineY = gInterface.DrawFormat(eWhite, (int)StartX + 30, (int)LineY, 210, 1, gCustomMessage.GetMessage(7));
	LineY = gInterface.DrawFormat(eWhite, (int)StartX + 30, (int)LineY, 210, 1, gCustomMessage.GetMessage(8));

	gInterface.DrawFormat(eGold, (int)StartX + 30, (int)StartBody + 95 - 2, 210, 1, "Plano");
	gInterface.DrawFormat(eGold, (int)StartX + 80, (int)StartBody + 95 - 2, 210, 1, "Experiencia");
	gInterface.DrawFormat(eGold, (int)StartX + 130, (int)StartBody + 95 - 2, 210, 1, "Drop");
	gInterface.DrawFormat(eGold, (int)StartX + 170, (int)StartBody + 95 - 2, 210, 1, "Tempo");

	int HoverSelect = -1;
	for (int i = 0; i < MAX_CUSTOM_BUYVIP; i++)
	{
		if (gCustomBuyVip.m_CustomBuyVipInfo[i].Index == -1) continue;
		EnableAlphaTest(true);
		glColor3f(1.0, 1.0, 1.0);
		gInterface.DrawFormat(eWhite, (int)StartX + 30, (int)StartBody + 110 + (12 * i) - 5, 210, 1, gCustomBuyVip.m_CustomBuyVipInfo[i].VipName);

		gInterface.DrawFormat(eWhite, (int)StartX + 80, (int)StartBody + 110 + (12 * i) - 5, 210, 1, "+%d%%", gCustomBuyVip.m_CustomBuyVipInfo[i].Exp);

		gInterface.DrawFormat(eWhite, (int)StartX + 130, (int)StartBody + 110 + (12 * i) - 5, 210, 1, "+%d%%", gCustomBuyVip.m_CustomBuyVipInfo[i].Drop);

		gInterface.DrawFormat(eWhite, (int)StartX + 170, (int)StartBody + 110 + (12 * i) - 5, 210, 1, "%d dia(s)", gCustomBuyVip.m_CustomBuyVipInfo[i].Days);

		RenderImage(CNewUIWindowMenu::IMAGE_WINDOW_MENU_LINE, StartX + 30, StartBody + 110 + (12 * i) + 9 - 5, 82.0, 2.0);
		RenderImage(CNewUIWindowMenu::IMAGE_WINDOW_MENU_LINE, StartX + 30 + 82, StartBody + 110 + (12 * i) + 9 - 5, 82.0, 2.0);

		if (g_pBCustomMenuInfo->DrawButton((int)StartX + 60, (int)StartBody + 160 + (25 * i) - 5, 120, 12, gCustomBuyVip.m_CustomBuyVipInfo[i].VipName))
		{
			gCustomBuyVip.BuyStatus(i+1);
		}
		if (SEASON3B::CheckMouseIn((int)StartX + 60, (int)StartBody + 160 + (25 * i) - 5, 120, 25) == 1)
		{
			EnableAlphaTest(true);
			glColor3f(1.0, 1.0, 1.0);
			gInterface.DrawFormat(eGold, (int)StartX + 30, (int)StartBody + 142, 210, 1, gCustomMessage.GetMessage(10));
			gInterface.DrawFormat(eGold, (int)StartX + 70, (int)StartBody + 142, 210, 1, gCustomMessage.GetMessage(11), gCustomBuyVip.m_CustomBuyVipInfo[i].Coin1);
			DisableAlphaBlend();
		}
		DisableAlphaBlend();
	}
	DisableAlphaBlend();
}

#if(JEWELBANKVER2)

#define SET_ITEMOPT_LEVEL(x)  (x << 3)
int PageBank1 = 0;
int MaxPerPageBank1 = 8;
static void BPagePrevAction(LPVOID pClass)
{
	if (GetTickCount() < gInterface.Data[JEWELBANK_WINDOW].EventTick + 300) return;
	gInterface.Data[JEWELBANK_WINDOW].EventTick = GetTickCount();

	if (PageBank1 > 0)
	{
		PageBank1--;
	}

}
static void BPageNextAction(LPVOID pClass)
{
	if (GetTickCount() < gInterface.Data[JEWELBANK_WINDOW].EventTick + 300) return;
	gInterface.Data[JEWELBANK_WINDOW].EventTick = GetTickCount();

	if (PageBank1 < (gInterface.mCListItemBank.size() / MaxPerPageBank1))
	{
		PageBank1++;
	}

}
float FixYItemBMD(int ItemIndex)
{

	switch (ItemIndex)
	{
	case 6159:
	{
		return 6.f;
	}
	break;
	case 7199:
	{
		return 9.5f;
	}
	break;
	default:
		break;
	}
	return 0;
}
void NewBankSendGS(int Type, int SelectIndex)
{
	if ((GetTickCount() - gInterface.Data[JEWELBANK_WINDOW].EventTick) < 300)
	{
		return;
	}

	REQUEST_RUTGUIBANK pMsg;
	pMsg.header.set(0xFA, 0x20, sizeof(pMsg));
	pMsg.TypeSend = Type;
	pMsg.IndexList = SelectIndex;
	DataSend((LPBYTE)&pMsg, pMsg.header.size);

	gInterface.Data[JEWELBANK_WINDOW].EventTick = GetTickCount();
}
void Interface::DrawWindowJewelBankVer2()
{
	if (gInterface.Data[JEWELBANK_WINDOW].OnShow == false)
	{
		if (PageBank1) PageBank1 = 0;
		return;
	}

	if (gInterface.mCListItemBank.empty())
	{
		gInterface.Data[JEWELBANK_WINDOW].OnShow = false;
	}

	const BYTE state[3] = { 0, 1, 2 };

	float MainWidth = 200;

	float MainHeight = 350;

	float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2) ;

	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 60.0f;

	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, JEWELBANK_WINDOW, "Banco de Joias");

	int CloseX = StartX + MainWidth - 33.0f;
	int CloseY = StartY;

	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 36, 36) == 1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[JEWELBANK_WINDOW].EventTick > 300)
		{
			gInterface.Data[JEWELBANK_WINDOW].Close();
			gInterface.Data[JEWELBANK_WINDOW].EventTick = GetTickCount();
			PlayBuffer(SOUND_CLICK01);
		}
	}

	StartY -= 30;

	::EnableAlphaTest(true);

	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	StartX += 25;

	StartY += - 20;

	float StartXItem = StartX + 20;

	float StartYItem = StartY + 82;

	float KhoangCach = 25;

	static int selectedItemIndex = -1;

	static DWORD lastRightClickTime = 0;

	static const DWORD rightClickDelay = 300;

	for (int n = 0, r = (PageBank1 * 10); n < 10 && r < this->mCListItemBank.size(); r++)
	{
		if (selectedItemIndex != -1 && selectedItemIndex != r)
			continue;

		int ItemIndex = this->mCListItemBank[r].ItemIndex;
		int ItemLevel = this->mCListItemBank[r].ItemLevel;

		const ITEM_ATTRIBUTE* is = &ItemAttribute[ItemIndex];

		float size = max(is->Width, is->Height);

		float itemScale = 1.0;

		float addY = FixYItemBMD(ItemIndex);

		int nCount = 0;

		EnableAlphaTest();

		glColor3f(1.0, 1.0, 1.0);

		bool bMouseIn = SEASON3B::CheckMouseIn(StartXItem - 10, (StartYItem + (n * KhoangCach)) + 28, 162, 16.0f);

		bool bRightClick = bMouseIn && SEASON3B::IsPress(VK_RBUTTON);

		if (bRightClick)
		{
			DWORD currentTime = GetTickCount();
			if (currentTime - lastRightClickTime > rightClickDelay)
			{
				selectedItemIndex = (selectedItemIndex == r) ? -1 : r;
				lastRightClickTime = currentTime;
			}
		}

		if (SEASON3B::CheckMouseIn(StartXItem - 15, (StartYItem + (n * KhoangCach)) + 28, 162, 16.0f))
		{
			FrameTarget(StartXItem - 14, (StartYItem + (n * KhoangCach)) + 28, 160, 13, RGBA(144, 238, 144, 0xFF));
		}

		g_pNewUISystem->RenderItem3DFree(StartXItem - 10, (StartYItem + (n * KhoangCach)) + 5, 13, 13, this->mCListItemBank[r].ItemIndex, this->mCListItemBank[r].ItemLevel * 0, 0, 0, 0, 1.3);

		EnableAlphaTest();

		glColor3f(1.0, 1.0, 1.0);

		TextDraw(g_hFont, StartXItem + 20, (StartYItem + (n * KhoangCach)) + 31, 0xFFDD00FF, 0x0, 100, 0, 1, BGetItemName(ItemIndex, ItemLevel * 8));

		TextDraw(g_hFont, StartXItem + 80, (StartYItem + (n * KhoangCach)) + 31, 0xFFDD00A9, 0x0, 60, 0, 3, "x%d", this->mCListItemBank[r].ItemCount);

		DisableAlphaBlend();

		if (selectedItemIndex == r)
		{
			int buttonX = StartXItem - 7;
			int buttonY = (StartYItem + (n * KhoangCach)) + 50;

			if (g_pBCustomMenuInfo->DrawButton(buttonX, buttonY, 60, 30, "Retirar 1"))
			{
				NewBankSendGS(eRut1, r);

			}
			if (g_pBCustomMenuInfo->DrawButton(buttonX + 85, buttonY, 60, 30, "Retirar 10"))
			{
				NewBankSendGS(eRut10, r);

			}

			if (g_pBCustomMenuInfo->DrawButton(buttonX, buttonY + 40, 60, 30, "Retirar 20"))
			{
				NewBankSendGS(eRut20, r);

			}
			if (g_pBCustomMenuInfo->DrawButton(buttonX + 85, buttonY + 40, 60, 30, "Retirar 30"))
			{
				NewBankSendGS(eRut30, r);

			}

			if (g_pBCustomMenuInfo->DrawButton(buttonX, buttonY + 80, 60, 30, "Retirar Tudo"))
			{
				NewBankSendGS(eRutAll, r);

			}

			if (g_pBCustomMenuInfo->DrawButton(buttonX + 85, buttonY + 80, 60, 30, "Enviar Tudo"))
			{
				NewBankSendGS(eGuiAll, r);

			}
		}

		n++;
		DisableAlphaBlend();
	}

}

bool Interface::gDrawPegasus(int IDGUID, float PosX, float PosY, float SizeW, float SizeH, int TypeButton, LPCSTR Text, const BYTE* a9)
{
	bool mreturn = false;

	char WindowName[2048];
	int BuffLen = sizeof(WindowName) - 1;
	ZeroMemory(WindowName, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(WindowName, BuffLen, Text, args);
	va_end(args);

	if (TypeButton == 3)
	{
		if (SEASON3B::CheckMouseIn(PosX, PosY, SizeW, SizeH) == 1)
		{
			if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eTIME].EventTick > 500 && !BLockButtonHover && !BLockMessageBox && !BLockButtonMoveWindow)
			{
				gInterface.Data[eWindowClick].EventTick = GetTickCount();
				PlayBuffer(25, 0, 0);
				mreturn = true;
				pDrawButton(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 2);
			}
			else {
				pDrawButton(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 1);
			}

		}
		else {
			pDrawButton(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, 0.0);
		}
	}
	else if (TypeButton == 2)
	{
		if (SEASON3B::CheckMouseIn(PosX, PosY, SizeW, SizeH) == 1)
		{

			if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eWindowClick].EventTick > 500 && !gInterface.BLockButtonHover)
			{
				gInterface.Data[eWindowClick].EventTick = GetTickCount();
				PlayBuffer(25, 0, 0);
				mreturn = true;
				pDrawButton(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, SizeH * 1);
			}
			else {
				pDrawButton(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, 0.0);
			}

		}
		else {
			pDrawButton(IDGUID, PosX, PosY, SizeW, SizeH, 0.0, 0.0);
		}
	}

	else
	{
		if (SEASON3B::CheckMouseIn(PosX, PosY, SizeW, SizeH) == 1)
		{

			if (GetTickCount() - gInterface.Data[eWindowClick].EventTick > 500 && !gInterface.BLockButtonHover)
			{
				if ((GetKeyState(VK_LBUTTON) & 0x8000))
				{
					gInterface.Data[eWindowClick].EventTick = GetTickCount();
					PlayBuffer(25, 0, 0);
					mreturn = true;
				}
			}
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH, 0, 0, RGBA(255, 228, 107, 255));

		}
		else
		{
			RenderImage(IDGUID, PosX, PosY, SizeW, SizeH);
		}
	}

	return mreturn;
}

void Interface::OnOffWindowBank()
{
	if ((GetTickCount() - gInterface.Data[JEWELBANK_WINDOW].EventTick) < 500)
	{
		return;
	}
	gInterface.Data[JEWELBANK_WINDOW].EventTick = GetTickCount();
	gInterface.Data[JEWELBANK_WINDOW].OnShow ^= 1;
}
#endif

SIZE Interface::getTextSize(HFONT fontsize, char* a1)
{
	SelectObject(g_pRenderText->GetFontDC(), fontsize);
	SIZE TextSize = { 0, 0 };
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), a1, strlen(a1), &TextSize);
	return TextSize;
}

void Interface::OpenCongDiem(int Type)
{
	if ((GetTickCount() - gInterface.Data[eCongDiem].EventTick) < 100)
	{
		return;
	}
	TypeCongDiem = Type;
	gInterface.Data[eCongDiem].EventTick = GetTickCount();
	gInterface.Data[eCongDiem].OnShow = 1;
}

CUITextInputBox* m_NhapPoint = nullptr;

void Interface::DrawCongDiem()
{
	if (gInterface.Data[eCongDiem].OnShow == false)
	{
		if(m_NhapPoint)
		{
			m_NhapPoint = nullptr;
		}
		return;
	}

	if (gInterface.CheckWindow(ObjWindow::CashShop)
		|| gInterface.CheckWindow(ObjWindow::FriendList)
		|| gInterface.CheckWindow(ObjWindow::MoveList)
		|| gInterface.CheckWindow(ObjWindow::Party)
		|| gInterface.CheckWindow(ObjWindow::Quest)
		|| gInterface.CheckWindow(ObjWindow::NPC_Devin)
		|| gInterface.CheckWindow(ObjWindow::Guild)
		|| gInterface.CheckWindow(ObjWindow::Trade)
		|| gInterface.CheckWindow(ObjWindow::Warehouse)
		|| gInterface.CheckWindow(ObjWindow::ChaosBox)
		|| gInterface.CheckWindow(ObjWindow::CommandWindow)
		|| gInterface.CheckWindow(ObjWindow::PetInfo)
		|| gInterface.CheckWindow(ObjWindow::Shop)
		|| gInterface.CheckWindow(ObjWindow::Store)
		|| gInterface.CheckWindow(ObjWindow::OtherStore)
		|| gInterface.CheckWindow(ObjWindow::DevilSquare)
		|| gInterface.CheckWindow(ObjWindow::BloodCastle)
		|| gInterface.CheckWindow(ObjWindow::CreateGuild)
		|| gInterface.CheckWindow(ObjWindow::GuardNPC)
		|| gInterface.CheckWindow(ObjWindow::SeniorNPC)
		|| gInterface.CheckWindow(ObjWindow::GuardNPC2)
		|| gInterface.CheckWindow(ObjWindow::CastleGateSwitch)
		|| gInterface.CheckWindow(ObjWindow::CatapultNPC)
		|| gInterface.CheckWindow(ObjWindow::CrywolfGate)
		|| gInterface.CheckWindow(ObjWindow::IllusionTemple)
		|| gInterface.CheckWindow(ObjWindow::HeroList)
		|| gInterface.CheckWindow(ObjWindow::ChatWindow)
		|| gInterface.CheckWindow(ObjWindow::FastMenu)
		|| gInterface.CheckWindow(ObjWindow::Options)
		|| gInterface.CheckWindow(ObjWindow::Help)
		|| gInterface.CheckWindow(ObjWindow::FastDial)
		|| gInterface.CheckWindow(ObjWindow::SkillTree)
		|| gInterface.CheckWindow(ObjWindow::GoldenArcher1)
		|| gInterface.CheckWindow(ObjWindow::GoldenArcher2)
		|| gInterface.CheckWindow(ObjWindow::LuckyCoin1)
		|| gInterface.CheckWindow(ObjWindow::LuckyCoin2)
		|| gInterface.CheckWindow(ObjWindow::NPC_Duel)
		|| gInterface.CheckWindow(ObjWindow::NPC_Titus)
		|| gInterface.CheckWindow(ObjWindow::CashShop)
		|| gInterface.CheckWindow(ObjWindow::Lugard)
		|| gInterface.CheckWindow(ObjWindow::QuestList1)
		|| gInterface.CheckWindow(ObjWindow::QuestList2)
		|| gInterface.CheckWindow(ObjWindow::Jerint)
		|| gInterface.CheckWindow(ObjWindow::FullMap)
		|| gInterface.CheckWindow(ObjWindow::NPC_Dialog)
		|| gInterface.CheckWindow(ObjWindow::GensInfo)
		|| gInterface.CheckWindow(ObjWindow::NPC_ChaosMix)
		|| gInterface.CheckWindow(ObjWindow::ExpandInventory)
		|| gInterface.CheckWindow(ObjWindow::ExpandWarehouse)
		|| gInterface.CheckWindow(ObjWindow::MuHelper))
	{

		if (gInterface.Data[eCongDiem].OnShow == true)
		{
			gInterface.Data[eCongDiem].OnShow = false;

			return;
		}
	}
	char TempPoint[30] = { 0 };
	float CuaSoW = 190;
	float CuaSoH = 100;

	float StartX = (MAX_WIN_WIDTH / 2) - (CuaSoW / 2) - 50.0f;
	float StartY = 100.0;

	char* CongDiemType[] = { "Fora", "Agilidade", "Vitalidade","Energia", "Comando", "desconhecido"};

	char* CongDiemType1[] = { "Adicionar Pontos de Fora", "Adicionar Pontos de Agilidade", "Adicionar Pontos de Vitalidade","Adicionar Pontos de Energia", "Adicionar Pontos de Comando", "desconhecido" };

	g_pBCustomMenuInfo->gDrawPegasusCongDiem(&StartX, &StartY, CuaSoW, CuaSoH, eCongDiem, CongDiemType[TypeCongDiem]);

	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);

	TextDraw(g_hFontBold, StartX, StartY + 10 , eWhite, 0x0, CuaSoW, 0, 3, CongDiemType[TypeCongDiem]);

	TextDraw(g_hFontBold, StartX, StartY + 35, eColorAdvance, 0x0, CuaSoW, 0, 3, CongDiemType1[TypeCongDiem]);

	DisableAlphaBlend();

	StartY += 30;

	g_pBCustomMenuInfo->RenderToolTipItem(StartX + 65, StartY + 20, 50, 20, 0x00000060, 0, 0);

	int SoPoint = 0;

	if (g_pBCustomMenuInfo->RenderInputBox(StartX + 72, StartY + 23, 100, 20, TempPoint, m_NhapPoint, UIOPTION_NUMBERONLY, 5))
	{
		char m_GetText[15];
		memset(&m_GetText, 0, sizeof(char) * 15);
		m_NhapPoint->GetText(m_GetText);

		int inputValue = atoi(m_GetText);
		int maxPoint = CharacterAttribute->PrintPlayer.ViewPoint;

		if (inputValue < 1)
		{
			SoPoint = 1;
			m_NhapPoint->SetText("1");
		}
		else if (inputValue > maxPoint)
		{
			SoPoint = maxPoint;
			char buffer[16];
			sprintf(buffer, "%d", maxPoint);
			m_NhapPoint->SetText(buffer);
		}
		else
		{
			SoPoint = inputValue;
		}

		m_NhapPoint->SetTextColor(255, 255, 255, 255);
	}

}
