

#include "stdafx.h"

#include "NewUICommon.h"
#include "NewUIRenderNumber.h"
#include "NewUISystem.h"
#include "NewUICommonMessageBox.h"
#include "ZzzTexture.h"
#include "ZzzOpenglUtil.h"
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayWinMid;
extern int DisplayWinExt;
extern int DisplayWinReal;
extern int MouseX, MouseY;
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
extern bool g_bWndActive;
#endif

bool SEASON3B::CreateOkMessageBox(const unicode::t_string& strMsg, DWORD dwColor, float fPriority)
{
	CNewUICommonMessageBox* pMsgBox = g_MessageBox->NewMessageBox(MSGBOX_CLASS(CNewUICommonMessageBox));
	if(pMsgBox)
	{
		return pMsgBox->Create(MSGBOX_COMMON_TYPE_OK, strMsg, dwColor);
	}
	return false;
}

int SEASON3B::IsPurchaseShop()
{
	if( g_pMyShopInventory->IsVisible() )
	{
		return 1;
	}
	else if( g_pPurchaseShopInventory->IsVisible() )
	{
		return 2;
	}

	return -1;
}

bool SEASON3B::CheckMouseIn(int x, int y, int width, int height)
{
	if (MouseX >= x && MouseX < x + width && MouseY >= y && MouseY < y + height)
	{

		return true;
	}

	return false;
}

void SEASON3B::RenderImage(GLuint uiImageType, float x, float y, float width, float height)
{

	BITMAP_t *pImage = &Bitmaps[uiImageType];

	float u, v, uw, vh;

	u = 0.5f / (float)pImage->Width;
	v = 0.5f / (float)pImage->Height;
	uw = (width - 0.5f) / (float)pImage->Width;
	vh = (height - 0.5f) / (float)pImage->Height;

	RenderBitmap(uiImageType, x, y, width, height, u, v, uw - u, vh - v);
}

int SEASON3B::TextDraw(HFONT font, int PosX, int PosY, DWORD color, DWORD bkcolor, int Width, int Height, BYTE Align, LPCTSTR Text, ...)
{
	char Buff[2048];
	int BuffLen = sizeof(Buff) - 1;
	ZeroMemory(Buff, BuffLen);

	va_list args;
	va_start(args, Text);
	int Len = vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);

	int LineCount = 0;

	char* Line = strtok(Buff, "\n##");
	DWORD BKTextColor = g_pRenderText->GetTextColor();
	DWORD BKBGColor = g_pRenderText->GetBgColor();
	while (Line != NULL)
	{
		g_pRenderText->SetFont(font);
		g_pRenderText->SetTextColor((color & 0xff000000) >> 24, (color & 0x00ff0000) >> 16, (color & 0x0000ff00) >> 8, color & 0x000000ff);
		g_pRenderText->SetBgColor((bkcolor & 0xff000000) >> 24, (bkcolor & 0x00ff0000) >> 16, (bkcolor & 0x0000ff00) >> 8, bkcolor & 0x000000ff);
		g_pRenderText->RenderText(PosX, PosY, Line, Width, Height, Align, 0);

		PosY += 10;
		Line = strtok(NULL, "\n##");
		g_pRenderText->SetFont(g_hFont);
		LineCount++;
	}
	g_pRenderText->SetTextColor(BKTextColor);
	g_pRenderText->SetBgColor(BKBGColor);
	return LineCount;
}

void SEASON3B::DrawFrame(GLuint ModelID, float X, float Y, float W, float H, float SourceX, float SourceY, float SourceW, float SourceH, float ScaleW, float ScaleH, bool ScaleUp)
{
	float v1, v2;

	if (W) {
		W /= ScaleW;
		v1 = ScaleW / (SourceW / W);
	}
	else {
		v1 = ScaleW;
	}

	if (H) {
		H /= ScaleH;
		v2 = ScaleH / (SourceH / H);
	}
	else {
		v2 = ScaleH;
	}
	RenderBitmap(ModelID, X, Y, W, H, SourceX / SourceW, SourceY / SourceH, v1, v2, ScaleUp, 1, 0.0);
}
void SEASON3B::RenderImageFFF(GLuint Image, float x, float y, float width, float height, float su, float sv, float uw, float vh)
{

	BITMAP_t* pText = &Bitmaps[Image];
	RenderBitmap(Image, x, y, width, height, su / pText->Width, sv / pText->Height, uw / pText->Width, vh / pText->Height, 1, 1, 0.0);
}

void SEASON3B::RenderImage(GLuint uiImageType, float x, float y, float width, float height, float su, float sv,float uw, float vh, DWORD color)
{
	RenderColorBitmap(uiImageType, x, y, width, height, su, sv, uw, vh, color);
}

void SEASON3B::RenderImage(GLuint uiImageType, float x, float y, float width, float height, float su, float sv)
{
	BITMAP_t *pImage = &Bitmaps[uiImageType];

	float u, v, uw, vh;
	u = ((su + 0.5f) / (float)pImage->Width);
	v = ((sv + 0.5f) / (float)pImage->Height);
	uw = (width - 0.5f) / (float)pImage->Width - (0.5f / (float)pImage->Width);
	vh = (height - 0.5f) / (float)pImage->Height - (0.5f / (float)pImage->Height);

	RenderBitmap(uiImageType, x, y, width, height, u, v, uw, vh);
}

void SEASON3B::RenderImage(GLuint uiImageType, float x, float y, float width, float height, float su, float sv, DWORD color)
{
	BITMAP_t *pImage = &Bitmaps[uiImageType];

	float u, v, uw, vh;
	u = ((su + 0.5f) / (float)pImage->Width);
	v = ((sv + 0.5f) / (float)pImage->Height);
	uw = (width - 0.5f) / (float)pImage->Width - (0.5f / (float)pImage->Width);
	vh = (height - 0.5f) / (float)pImage->Height - (0.5f / (float)pImage->Height);

	RenderColorBitmap(uiImageType, x, y, width, height, u, v, uw, vh, color);
}

void SEASON3B::FrameTarget(float iPos_x, float iPos_y, float width, float height, DWORD color)
{
	EnableAlphaTest(true);

	BYTE red = GetRed(color);
	BYTE green = GetGreen(color);
	BYTE blue = GetBlue(color);

	glColor4ub(red, green, blue, 30);
	RenderColor(iPos_x, iPos_y, width, height, 0.0, 0);

	glColor4ub(red, green, blue, 255);
	RenderColor(iPos_x, iPos_y, width, 1.f, 0.0, 0);
	RenderColor(iPos_x, iPos_y + height, width, 1.f, 0.0, 0);

	RenderColor(iPos_x, iPos_y + 1.f, 1.f, height - 1.f, 0.0, 0);
	RenderColor(iPos_x + width - 1.f, iPos_y + 1.f, 1.f, height - 1.f, 0.0, 0);

	EndRenderColor();
}

float SEASON3B::RenderNumber(float x, float y, int iNum, float fScale)
{
	return g_RenderNumber->RenderNumber(x, y, iNum, fScale);
}

bool SEASON3B::IsNone(int iVirtKey)
{
	return g_pNewKeyInput->IsNone(iVirtKey);
}

bool SEASON3B::IsRelease(int iVirtKey)
{
	return g_pNewKeyInput->IsRelease(iVirtKey);
}

bool SEASON3B::IsPress(int iVirtKey)
{
	return g_pNewKeyInput->IsPress(iVirtKey);
}

bool SEASON3B::IsRepeat(int iVirtKey)
{
	return g_pNewKeyInput->IsRepeat(iVirtKey);
}

SEASON3B::CNewKeyInput::CNewKeyInput()
{
	Init();
}

SEASON3B::CNewKeyInput::~CNewKeyInput()
{

}

void SEASON3B::CNewKeyInput::Init()
{
	memset(&m_pInputInfo, 0, sizeof(INPUTSTATEINFO)*256);
}

SEASON3B::CNewKeyInput* SEASON3B::CNewKeyInput::GetInstance()
{
	static SEASON3B::CNewKeyInput s_Instance;
	return &s_Instance;
}

void SEASON3B::CNewKeyInput::ScanAsyncKeyState()
{
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
	if (!g_bWndActive)
		return;
#endif

	for(int key=0; key < 256; key++)
	{
		if(HIBYTE(GetAsyncKeyState(key)) & 0x80)
		{
			if(m_pInputInfo[key].byKeyState == KEY_NONE || m_pInputInfo[key].byKeyState == KEY_RELEASE)
			{

				m_pInputInfo[key].byKeyState = KEY_PRESS;
			}
			else if(m_pInputInfo[key].byKeyState == KEY_PRESS)
			{

				m_pInputInfo[key].byKeyState = KEY_REPEAT;
			}
		}
		else
		{
			if(m_pInputInfo[key].byKeyState == KEY_REPEAT || m_pInputInfo[key].byKeyState == KEY_PRESS)
			{

				m_pInputInfo[key].byKeyState = KEY_RELEASE;
			}
			else if(m_pInputInfo[key].byKeyState == KEY_RELEASE)
			{
				m_pInputInfo[key].byKeyState = KEY_NONE;
			}
		}
	}

	if( IsPress(VK_RETURN) && IsEnterPressed() == false ) {
		m_pInputInfo[VK_RETURN].byKeyState = KEY_NONE;
	}
	SetEnterPressed( false );
}

bool SEASON3B::CNewKeyInput::IsNone(int iVirtKey)
{
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
	if (!g_bWndActive)
		return false;
#endif
	return (m_pInputInfo[iVirtKey].byKeyState == KEY_NONE) ? true : false;
}

bool SEASON3B::CNewKeyInput::IsRelease(int iVirtKey)
{
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
	if (!g_bWndActive)
		return false;
#endif
	return (m_pInputInfo[iVirtKey].byKeyState == KEY_RELEASE) ? true : false;
}

bool SEASON3B::CNewKeyInput::IsPress(int iVirtKey)
{
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
	if (!g_bWndActive)
		return false;
#endif
	return (m_pInputInfo[iVirtKey].byKeyState == KEY_PRESS) ? true : false;
}

bool SEASON3B::CNewKeyInput::IsRepeat(int iVirtKey)
{
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
	if (!g_bWndActive)
		return false;
#endif
	return (m_pInputInfo[iVirtKey].byKeyState == KEY_REPEAT) ? true : false;
}

void SEASON3B::CNewKeyInput::SetKeyState(int iVirtKey, KEY_STATE KeyState)
{
	m_pInputInfo[iVirtKey].byKeyState = KeyState;
}
