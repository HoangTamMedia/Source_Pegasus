#pragma once
#include "DSPlaySound.h"
#include "Protocol.h"
#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "UIControls.h"
#include "Defined_Global.h"

typedef void(*PUSHEVENT_CALLBACK_LPVOID)(LPVOID);
#define BMAX_CHAT_SIZE_UTF16  (90/2)
#define Color4f(r, b, g, o)			((o << 24) + (g << 16) + (b << 8) + r)
#define pWinFontHeight				*(int*)0x0081C0380

#define pGameWindow				g_hWnd
extern bool StatusAutoReset;
using namespace SEASON3B;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayWinMid;
extern int DisplayWinExt;
extern int DisplayWinReal;

#define MAX_WIN_WIDTH		DisplayWin
#define MAX_WIN_HEIGHT		DisplayHeight

struct InterfaceObject
{
	DWORD	ModelID;
	float	Width;
	float	Height;
	float	X;
	float	Y;
	float	MaxX;
	float	MaxY;
	DWORD	EventTick;
	bool	OnClick;
	bool	 OnShow;
	BYTE	Attribute;
	bool	ByClose;
	long	OpenedValue;
	BYTE	Speed;
	char	StatValue[20];
	int		Type;

	bool	FirstLoad;
	float  curX;
	float	curY;
	bool	Hover;
	bool  AllowMove;
	void OpenClose(int Type = -1)
	{
		this->OnShow ^= 1;
		this->ByClose ^= 1;
		this->Hover ^= 1;
		if (Type != -1) this->Type = Type;
		if (!this->OnShow) this->Type = 0;

	};
	void Close()
	{
		this->OnShow = false;
		this->ByClose = false;
		this->Hover = false;
		this->Type = 0;
		this->AllowMove = true;
	};

	void Open()
	{
		this->OnShow = true;
		this->ByClose = false;
	};

	void Open(int Value, int Speed)
	{
		this->OnShow = true;
		this->OpenedValue = Value;
		this->Speed = Speed;
		this->ByClose = false;
		this->FirstLoad = true;
	};

	void Close(int Value, int Speed)
	{
		this->OnShow = false;
		this->OpenedValue = Value;
		this->Speed = Speed;
		this->ByClose = false;
		this->Type = 0;
	}

	void CloseAnimated(int Speed)
	{
		this->Speed = Speed;
		this->ByClose = true;
		this->Type = 0;
	}

};
enum ColorMacro
{
	eTextShop = Color4f(0xFFu, 0xDCu, 0x96u, 0xFFu),
	eTextGuild = Color4f(0xFFu, 0xBDu, 0x19u, 0xFFu),
	eShinyGreen = Color4f(172, 255, 56, 255),
	eShinyGreen100 = Color4f(172, 255, 56, 100),
	eGold = Color4f(255, 189, 25, 255),
	eGold150 = Color4f(255, 189, 25, 150),
	eGold180 = Color4f(255, 189, 25, 180),
	eGold220 = Color4f(255, 189, 25, 220),
	eGold225 = Color4f(255, 180, 60, 150),
	eWhite = Color4f(255, 255, 255, 255),
	eWhite150 = Color4f(255, 255, 255, 150),
	eWhite160 = Color4f(255, 255, 255, 160),
	eWhite170 = Color4f(255, 255, 255, 170),
	eWhite180 = Color4f(255, 255, 255, 180),
	eWhite190 = Color4f(255, 255, 255, 200),
	eOrange = Color4f(255, 105, 25, 255),
	eGray100 = Color4f(211, 211, 211, 100),
	eGray150 = Color4f(50, 50, 50, 150),
	eGray190 = Color4f(50, 50, 50, 190),
	eBlowPink = Color4f(220, 20, 60, 255),
	eBlack = Color4f(0, 0, 0, 255),
	eRed = Color4f(225, 0, 0, 255),
	eRed2 = Color4f(225, 95, 45, 255),
	eExcellent = Color4f(0, 225, 139, 255),
	eAncient = Color4f(1, 223, 119, 255),
	eSocket = Color4f(153, 102, 204, 255),
	eBlue = Color4f(36, 242, 252, 255),
	eBlue250 = Color4f(36, 242, 252, 250),
	eYellow = Color4f(255, 255, 0, 255),
	eYellow170 = Color4f(255, 255, 0, 170),
	eBrown = Color4f(69, 39, 18, 255),
	eColorAdvance = Color4f(109, 25, 192, 255),
	eColorHealth = Color4f(191, 191, 146, 255),

};
#define MAX_DANH_HIEU_WEB		7
enum ObjectID
{
	eFlag01 = 1,
	eTIME,
	eStatsAdvance,
	eIconBomMau,

	eButtonMXH,
	eButtonNext,
	eButtonPrevious,
	eVipCharImg,
	eVipCharEnd = eVipCharImg + 50,
	eDanhImg,
	eDanhImgEnd,
	eDanhHieuWeb,
	eDanhHieuWebEnd = eDanhHieuWeb + MAX_DANH_HIEU_WEB,
	eUICTCImg,
	eUICTCImgEnd = eUICTCImg + 8,
	eCharView,
	eTickCount,

	eBeginWindowCustom,
	eMuHelperConfig,
	eMenu_MAIN,
	eMenu_OPTION,
	eCharacter,
	eMenu,
	eExp700,
	ePegasusLucChien,
	eMenuBG,
	eWindowEventTime,
	eVip_MAIN,
	eMAIN_RANKING,
	eWindowChangeClass,
	eCongDiem,
	eWindowMessageBox,
	eMainReg,
	eMainViewChar,
	eWindowChotroi,
	eWindowNhapPass,
	eWindowHarmony,
	eWindowQRCode,
	eWindowAutoBaking,
	eWindowCongDiem,
	eWindowDanhHieu,
	eWindowJewelBank,
	eWindowNPCBossGuild,
	NewBankSau,
	eINVASION_MAIN,
	eINVASION_MAIN_OPEN,
	JEWELBANK_WINDOW,
	eWindowInputWare,
	eSauMenu,
	eCHANGINGCLASS_MAIN,
	eWindowVip,
	eWinDowQuanHam,
	eWindowVipItem,
	eSauNewEvent,
	eRankPANEL_MAIN,
	eWindowClick,
	ePG_NAMEFX_WINDOW,		// name-effect picker, see PegasusNameFx

	eInventory,
	eNewUIBCustomMessageBox,
	eEndWindowCustom,
	EndObjectCustom,

};
#if(JEWELBANKVER2)
enum TypeRutGuiBank
{
	eRut1,
	eRut10,
	eRut20,
	eRut30,
	eRutAll,
	eGuiAll,
	eAutoPick,

};

#endif

#if(JEWELBANKVER2)
struct USER_BANK_DATA
{
	int ItemIndex;
	int ItemLevel;
	int ItemCount;
	int AutoPick;
	void Clear()
	{
		ItemIndex = -1;
		ItemLevel = 0;
		ItemCount = 0;
		AutoPick = 0;
	}
};

struct ItemBmdStruct
{
	char Name[30];
		BYTE TwoHand;
		BYTE gap01;
		BYTE ItemLvl;
		BYTE gap02;
		BYTE Slot;
		BYTE gap03;
		BYTE Skill;
		BYTE gap04;
		BYTE ItemWidth;
		BYTE ItemHeight;
		BYTE MinDmg;
		BYTE MaxDmg;
		BYTE gap05;
		BYTE Defence;
		BYTE gap06;
		BYTE Speed;
		BYTE gap07;
		BYTE Dur;
		BYTE MagicDur;
		BYTE MagicPwr;
		BYTE ReqStr;
		BYTE ReqStrValue;
		BYTE ReqAgi;
		BYTE ReqAgiValue;
		BYTE ReqEne;
		BYTE ReqEneValue;
		BYTE ReqVit;
		BYTE ReqVitValue;
		BYTE ReqCmd;
		BYTE ReqCmdValue;
		BYTE ReqLvl;
		BYTE gap08[3];
		unsigned int Money;
		BYTE Type;
		BYTE DW;
		BYTE DK;
		BYTE ELF;
		BYTE MG;
		BYTE DL;
		BYTE Sum;
		BYTE RF;
	BYTE Resists[8];
};

struct REQUEST_RUTGUIBANK
{
	PSBMSG_HEAD header;
	BYTE TypeSend;
	short IndexList;
};
#endif
class Interface
{
public:
	struct RECV_CUSTOMTIMECOUNT
	{
		PSBMSG_HEAD header;
		BYTE MapNumber;
		BYTE TypeStyle;
		DWORD TimeCount;
		char TextName[60];
	};

	struct XULY_CGPACKET_CONGP
	{
		PSBMSG_HEAD header;
		DWORD SoPoint;
		int Type;
	};

	enum ObjWindow
	{
		FriendList = 1,
		MoveList = 2,
		Party = 3,
		Quest = 4,
		NPC_Devin = 5,
		Guild = 6,
		Trade = 7,
		Warehouse = 8,
		ChaosBox = 9,
		CommandWindow = 10,
		PetInfo = 11,
		Shop = 12,
		Inventory = 13,
		Store = 14,
		OtherStore = 15,
		Character = 16,
		DevilSquare = 19,
		BloodCastle = 20,
		CreateGuild = 21,
		GuardNPC = 22,
		SeniorNPC = 23,
		GuardNPC2 = 24,
		CastleGateSwitch = 25,
		CatapultNPC = 26,
		CrywolfGate = 29,
		IllusionTemple = 30,
		HeroList = 32,
		ChatWindow = 33,
		FastMenu = 34,
		Options = 35,
		Help = 36,
		FastDial = 39,
		SkillTree = 57,
		GoldenArcher1 = 58,
		GoldenArcher2 = 59,
		LuckyCoin1 = 60,
		LuckyCoin2 = 61,
		NPC_Duel = 62,
		NPC_Titus = 63,
		CashShop = 65,
		Lugard = 66,
		QuestList1 = 68,
		QuestList2 = 69,
		Jerint = 70,
		FullMap = 72,
		NPC_Dialog = 73,
		GensInfo = 74,
		NPC_Julia = 75,
		NPC_ChaosMix = 76,
		ExpandInventory = 77,
		ExpandWarehouse = 78,
		MuHelper = 79,
	};
	Interface();
	virtual ~Interface();
	void Interface::CloseAllWindowCustom();
	int Interface::DrawMessage(int Mode, LPCSTR Text, ...);
	bool Interface::CheckWindow(int WindowID);
	int Interface::DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...);
	char* Interface::NumberFormat(int Vaule);
	char* Interface::fNumberFormat(float number);
	void	    DrawBarForm(float PosX, float PosY, float Width, float Height, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	InterfaceObject Data[EndObjectCustom];
	void Interface::Work();
	void Interface::BLoadImage();
	void CreateNButton(int Image, float x, float y, float width, float heith, int windows);
	void RenderImages(GLuint Image, float x, float y, float width, float height, float su, float sv, float uw, float vh);
#if(JEWELBANKVER2)

	void DrawWindowJewelBankVer2();
	std::vector<USER_BANK_DATA> mCListItemBank;
	void OnOffWindowBank();
#endif
	bool isCustomDraw();
	int IndexCustomTextture;
	bool Interface::UseHotKey();
	bool Interface::ActionSendItemGS(ITEM* ItemSell, int Slot);
	bool mAutoHP;

	std::string MsgBoxCaption;
	std::string MsgBoxText;
	int MsgBoxType;
	int MsgLine;
	int CheckVipLevel;
	PUSHEVENT_CALLBACK_LPVOID MsgBoxCallback;
	void DrawMessageBox();
	bool BLockButtonMoveWindow;
	bool BLockButtonHover;
	bool BLockMessageBox;
	void OpenMessageBox(char* caption, char* Format, ...);
	void OpenMessageBoxOkCancel(PUSHEVENT_CALLBACK_LPVOID pCallbackFunc, char* caption, char* Format, ...);

	void		RenderCaptchaNumber(float PosX, float PosY, CUITextInputBox* a6, LPCSTR Text, ...);
	bool		check_Captcha(std::string& captcha, std::string& user_input);
	std::string generateCaptcha(int n);
	std::string vCaptcha;
	bool Interface::gDrawPegasus(int IDGUID, float PosX, float PosY, float SizeW, float SizeH, int TypeButton, LPCSTR Text, const BYTE* a9);
	bool Interface::DrawCustomButton(int x, int y, int width, int height, const char* text);
	void Interface::DrawInfoMini();
	void Interface::DrawWindowMuaVIP();

	std::string TextMapTime;
	void DrawCongDiem();
	void OpenCongDiem(int Type = 0);
	int TypeCongDiem;
	SIZE getTextSize(HFONT fontsize, char* a1);
	bool KepChuot;
	bool AutoPK;
	int EventTimeTickCount;
};

extern Interface gInterface;

// True while the cursor is over any open Pegasus window. The world's click
// handling reads MouseOnWindow during the move phase, and these windows do
// their hit testing from Interface::Work, which runs in the render phase - so
// they have to be asked separately and early. Called from ZzzScene.
bool PegasusMouseOnWindow();