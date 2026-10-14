#include "StdAfx.h"
#include "Util.h"
#include "CBInterfaceVIPChar.h"
#include "CBInterface.h"
#include "MemScript.h"
#include "NewUISystem.h"
CBInterfaceVIPChar gBInterfaceVIPChar;

CBInterfaceVIPChar::CBInterfaceVIPChar()
{
	this->m_VIPCharInfoList.clear();
	FillMemory(&this->m_VIPCharInfo, sizeof(this->m_VIPCharInfo), 0);
	this->LoadData();
}

CBInterfaceVIPChar::~CBInterfaceVIPChar()
{
}

void CBInterfaceVIPChar::ErrorMessageBox(char* message, ...)
{
	char buff[256];

	memset(buff, 0, sizeof(buff));

	va_list arg;
	va_start(arg, message);
	vsprintf_s(buff, message, arg);
	va_end(arg);

	MessageBox(0, buff, "Error", MB_OK | MB_ICONERROR);

	ExitProcess(0);
}

void CBInterfaceVIPChar::LoadData()
{
	this->m_VipCharRank.clear();
	char* path = "Data\\Custom\\VIPCharRank.txt";

	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		CBInterfaceVIPChar::ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);
		return;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		CBInterfaceVIPChar::ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}
	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				 if (section == 2)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					CUSTOMVIPCHAR_DATA infoVIPChar;
					memset(&infoVIPChar, 0, sizeof(infoVIPChar));
					infoVIPChar.Index = lpMemScript->GetNumber();
					infoVIPChar.TypeRank = lpMemScript->GetAsNumber();
					strncpy_s(infoVIPChar.TextDir, lpMemScript->GetAsString(), sizeof(infoVIPChar.TextDir));

					char TempHex[11];
					strcpy_s(TempHex, lpMemScript->GetAsString());
					sscanf_s(TempHex, "%X", &infoVIPChar.ColorText);
					infoVIPChar.SizeW = lpMemScript->GetAsFloatNumber();
					infoVIPChar.H = lpMemScript->GetAsFloatNumber();
					infoVIPChar.X = lpMemScript->GetAsFloatNumber();
					infoVIPChar.Y = lpMemScript->GetAsFloatNumber();
					infoVIPChar.CacheRank = 0;
					this->m_VipCharRank.insert(std::pair<int, CUSTOMVIPCHAR_DATA>(infoVIPChar.Index, infoVIPChar));

				}
			}
		}
	}
	catch (...)
	{
		CBInterfaceVIPChar::ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

#include "Other.h"
char* VipOptionText(int Line)
{
	return gOther.Text_Option[Line];
};

int ExpVipChar3 = 0;

void DrawWindowVipChar()
{

	if (!gInterface.Data[eWinDowQuanHam].OnShow)
	{
		ExpVipChar3 = 0;
		return;
	}

	int LevelVipChar = gBInterfaceVIPChar.m_VIPCharInfo.LevelVIP;

	gInterface.CheckVipLevel = LevelVipChar;

	std::map<int, CUSTOMVIPCHAR_DATA>::iterator it = gBInterfaceVIPChar.m_VipCharRank.find(LevelVipChar);

	// Was 430; the text and the Atributos bar stop around 220, so the frame was
	// running well past the content.
	float WindowW = 290;
	float WindowH = 240;
	float MainWidth = 200.0;
	float MainHeight = 260.0;
	float StartX = (MAX_WIN_WIDTH / 2) - (WindowW / 2) + 107;
	float StartY = ((MAX_WIN_HEIGHT - 51) / 2) - (WindowW / 2) + 80;
	float MainCenter = StartX + (MAX_WIN_WIDTH / 3);
	float WKhungInfo = WindowW - 28;

	// Empty caption on purpose. gDrawWindowCustom draws the frame - which this
	// window needs - but also its own centred title bar and close button on top
	// of the ones drawn below, which is where the duplicate "Graduacao Militar"
	// and the grey slab came from. F4 (ChangeClass) already passes "" and comes
	// out clean, which is what pinned it down.
	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, WindowW, WindowH, eWinDowQuanHam, "");

	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);

	int CloseX = StartX + 190.0f;

	int CloseY = StartY + 2.0f ;

	RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30.0f, 30.0f);

	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 30, 30) == 1)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000 && GetTickCount() - gInterface.Data[eWinDowQuanHam].EventTick > 300)
		{

			gInterface.Data[eWinDowQuanHam].Close();
		}
		RenderImage(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_BCUSTOM_WINDOW_32453, CloseX, CloseY, 30, 30, 0, 0, RGBA(255, 255, 255, 255));
	}

	gInterface.DrawFormat(eYellow, StartX + 25, StartY + 11, 100, 1, "Graduacao Militar [F1]");

	DisableAlphaBlend();
	if (it == gBInterfaceVIPChar.m_VipCharRank.end())
	{
		EnableAlphaTest();

		glColor3f(1.0, 1.0, 1.0);

		gInterface.DrawFormat(eYellow, StartX + 65, StartY + (WindowH / 2) - 7, 300, 1, "Voce nao atingiu o maximo nivel de graduacao militar");

		return;
		DisableAlphaBlend();
	}

	if (SEASON3B::IsPress(VK_ESCAPE) == true)
	{
		gInterface.Data[eWinDowQuanHam].OnShow ^= 1;
		PlayBuffer(SOUND_CLICK01);

	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHARACTER);
	}

	float BarW = 165;
	float BarH = 10;
	float BarX = (StartX + (WindowW / 2)) - (BarW / 2) - 90;
	float BarY = StartY + 120;
	float RankVipY = (StartY + 30) + (70 / 2);

	g_pBCustomMenuInfo->RenderToolTipItem(BarX + 34, BarY - 85 - (it->second.H / 50), 80, 30, 0x00000060, 0, 0);

	if (it->second.TypeRank == 1)
	{
		int SizeTextFont = it->second.SizeW;
		TextDraw(g_hFont, BarX, RankVipY - (SizeTextFont / 2), it->second.ColorText, 0x0, BarW, 0, 3, it->second.TextDir);

	}
	else if (it->second.TypeRank == 2)
	{
		EnableAlphaTest();
		glColor3f(1.0, 1.0, 1.0);
		RenderBitmap(150000 + LevelVipChar, BarX -10, BarY - 115 - (it->second.H / 50), it->second.SizeW, it->second.H, 0.0, 0.0, 1.0, 1.0, 1, 1, 0.0);
		DisableAlphaBlend();
	}

	int ExpDangCo = gBInterfaceVIPChar.m_VIPCharInfo.UserPointVIP;
	int ExpYeuCauUp = gBInterfaceVIPChar.m_VIPCharInfo.PointVIP;
	int PhamTramExp = 0;
	if (ExpYeuCauUp != 0)
		{
	PhamTramExp = (ExpDangCo * 100) / ExpYeuCauUp;
	}
	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	int textColor = (ExpDangCo < ExpYeuCauUp) ? eRed : eExcellent;

	gInterface.DrawFormat(eWhite, BarX - 10, BarY - 57, BarW, 1,"Experiencia:");

	gInterface.DrawFormat(textColor, BarX + 28, BarY - 57, BarW, 1, "%s/%s", gInterface.NumberFormat(ExpDangCo), gInterface.NumberFormat(ExpYeuCauUp));

	float TextWCoin = BarW / 3;

	gInterface.DrawFormat(eWhite, BarX - 10, BarY - 45, BarW, 1, "Pegasus Coin: %s",gInterface.NumberFormat(gBInterfaceVIPChar.m_VIPCharInfo.Coin[0]));

	gInterface.DrawFormat(eWhite, BarX - 10, BarY - 30, BarW, 1, "Experiencia: %s" , gInterface.NumberFormat(gBInterfaceVIPChar.m_VIPCharInfo.Coin[1]));

	float SizeButton = 100;
	float ButtonX = (StartX + (WindowW / 2)) - (SizeButton / 2);
	float ButtonY = (StartY + WindowH) - 40;

	gInterface.DrawFormat(eWhite, BarX - 10, BarY - 15, BarW, 1, "Clique com o Botao Direito no Item Graduacao Militar");

	g_pBCustomMenuInfo->gBoxDropPegasus(BarX  - 15, BarY - 4, 180, 20);

	gInterface.DrawFormat(eYellow, BarX + 20, BarY , BarW, 1, "Atributos [ Nao Acumulativos ]");

	DisableAlphaBlend();

		for (int n = 0; n < 6; n++)
		{

			EnableAlphaTest();
			glColor3f(1.0, 1.0, 1.0);

			TextDraw(g_hFont, BarX - 10, BarY  - 5 + 20 + (n * 12), 0xFFFFFFFF, 0x0, BarW, 0, 1, VipOptionText(gBInterfaceVIPChar.m_VIPCharInfo.TypeOp[n]), gInterface.NumberFormat (gBInterfaceVIPChar.m_VIPCharInfo.ValueOp[n]));
			DisableAlphaBlend();
		}

		if (g_pBCustomMenuInfo->DrawButton(BarX + 22.0f, BarY + 89 , 120, 12, "Aumentar Nivel", 100))
		{

			XULY_CGPACKET pMsg;
			pMsg.header.set(0xD3, 0x23, sizeof(pMsg));
			pMsg.ThaoTac = 1;
			DataSend((LPBYTE)&pMsg, pMsg.header.size);
		}
}

#include "CBNewUiEx.h"

int VipCharListMaxPerPage = 15;
bool UpdateMaxPosVIP = false;

void CBInterfaceVIPChar::CGSendOpenWinwdowVIP()
{

	XULY_CGPACKET pMsg;
	pMsg.header.set(0xD3, 0x25, sizeof(pMsg));
	pMsg.ThaoTac = 1;
	DataSend((LPBYTE)& pMsg, pMsg.header.size);

	pMsg.header.set(0xD3, 0x24, sizeof(pMsg));
	pMsg.ThaoTac = 1;
	DataSend((LPBYTE)&pMsg, pMsg.header.size);

}

void CBInterfaceVIPChar::GCSetInfoVIPChar(BPMSG_CUSTOM_VIPCHAR_SEND* lpMsg)
{

	m_VIPCharInfo.UserPointVIP = lpMsg->UserPointVIP;

	if (lpMsg->TypeSend == 0 || lpMsg->TypeSend == 2)
	{
		m_VIPCharInfo.LevelVIP = lpMsg->LevelVIP;
		m_VIPCharInfo.PointVIP = lpMsg->PointVIP;
		m_VIPCharInfo.Coin[0] = lpMsg->Coin[0];
		m_VIPCharInfo.Coin[1] = lpMsg->Coin[1];
		m_VIPCharInfo.Coin[2] = lpMsg->Coin[2];

	}

	for (int n = 0; n < 6; n++)
	{
		m_VIPCharInfo.TypeOp[n] = lpMsg->TypeOp[n];
		m_VIPCharInfo.ValueOp[n] = lpMsg->ValueOp[n];

	}
	if (lpMsg->TypeSend == 0)
	{
		gInterface.Data[eWinDowQuanHam].OnShow = 1;
	}

}

void CBInterfaceVIPChar::GCSetInfoVIPCharList(CSEV_COUNTLIST* lpMsg)
{

	m_VIPCharInfoList.clear();

	for (int n = 0; n < lpMsg->Count; n++)
	{
		DATA_ITEMVIPPOINT* lpInfo = (DATA_ITEMVIPPOINT*)(((BYTE*)lpMsg) + sizeof(CSEV_COUNTLIST) + (sizeof(DATA_ITEMVIPPOINT) * n));

		m_VIPCharInfoList.push_back(*lpInfo);
	}

	gInterface.Data[eWinDowQuanHam].OnShow = 1;
}

void CBInterfaceVIPChar::Draw()
{

	DrawWindowVipChar();
}

bool CBInterfaceVIPChar::SendItem(ITEM* ItemSell, int Slot, bool KeyClick)
{
	if (KeyClick && gInterface.Data[eWinDowQuanHam].OnShow)
	{
		if (ItemSell->Type < 0) { return 0; }

		PMSG_ITEM_MOVE_RECV pMsg = { 0 };

		pMsg.h.set(0xD3, 0x22, sizeof(pMsg));
		pMsg.Target = -1;
		pMsg.sFlag = 0;
		pMsg.tFlag = 0;
		pMsg.Target = 0;
		pMsg.Source = Slot;
		::PlayBuffer(SOUND_GET_ITEM01);
		DataSend((BYTE*)&pMsg, pMsg.h.size);

		return 1;
	}

	return 0;
}