
#if !defined(AFX_MSGBOXIGSUSEITEMCONFIRM_H__71CD07AD_7713_4096_B88D_E06A464E39B6__INCLUDED_)
#define AFX_MSGBOXIGSUSEITEMCONFIRM_H__71CD07AD_7713_4096_B88D_E06A464E39B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM

#include "NewUIMessageBox.h"
#include "NewUICommonMessageBox.h"
#include "UIControls.h"
#include "NewUIOptionWindow.h"

using namespace SEASON3B;

class CMsgBoxIGSUseItemConfirm : public CNewUIMessageBoxBase
{
public:
	enum IMAGE_IGS_USEITEM_CONFIRM
	{
		IMAGE_IGS_BUTTON	= BITMAP_IGS_MSGBOX_BUTTON,
		IMAGE_IGS_BACK		= CNewUIOptionWindow::IMAGE_OPTION_FRAME_BACK,
		IMAGE_IGS_UP		= CNewUIOptionWindow::IMAGE_OPTION_FRAME_UP,
		IMAGE_IGS_DOWN		= CNewUIOptionWindow::IMAGE_OPTION_FRAME_DOWN,
		IMAGE_IGS_LEFTLINE	= CNewUIOptionWindow::IMAGE_OPTION_FRAME_LEFT,
		IMAGE_IGS_RIGHTLINE = CNewUIOptionWindow::IMAGE_OPTION_FRAME_RIGHT,
	};

	enum IMAGE_IGS_USEITEM_CONFIRM_SIZE
	{
		IMAGE_IGS_WINDOW_WIDTH	= 640,
		IMAGE_IGS_WINDOW_HEIGHT = 429,
		IMAGE_IGS_FRAME_WIDTH	= 190,
		IMAGE_IGS_FRAME_HEIGHT	= 179,
		IMAGE_IGS_UP_HEIGHT		= 64,
		IMAGE_IGS_DOWN_HEIGHT	= 45,
		IMAGE_IGS_LINE_WIDTH	= 21,
		IMAGE_IGS_LINE_HEIGHT	= 10,
		IMAGE_IGS_BTN_WIDTH		= 52,
		IMAGE_IGS_BTN_HEIGHT	= 26,
	};

	enum IGS_USEITEM_CONFIRM_POS
	{
		IGS_BTN_OK_POS_X		= 33,
		IGS_BTN_CANCEL_POS_X	= 105,
		IGS_BTN_POS_Y			= 140,
		IGS_TEXT_TITLE_Y		= 10,
		IGS_TEXT_DIVIDE_WIDTH	= 150,
		IGS_TEXT_DESCRIPTION_POS_X	= 20,
		IGS_TEXT_DESCRIPTION_POS_Y	= 50,
		IGS_TEXT_DESCRIPTION_INTERVAL = 12,
		IGS_TEXT_DESCRIPTION_WIDTH	= 170,
	};

public:
	CMsgBoxIGSUseItemConfirm();
	~CMsgBoxIGSUseItemConfirm();

	bool Create(float fPriority = 3.f);
	void Release();

	bool Update();
	bool Render();

	void Initialize(int iStorageSeq, int iStorageItemSeq, WORD wItemCode, unicode::t_char szItemType, unicode::t_char* pszItemName);

	static CALLBACK_RESULT LButtonUp(class CNewUIMessageBoxBase* pOwner, const leaf::xstreambuf& xParam);
	static CALLBACK_RESULT OKButtonDown(class CNewUIMessageBoxBase* pOwner, const leaf::xstreambuf& xParam);
	static CALLBACK_RESULT CancelButtonDown(class CNewUIMessageBoxBase* pOwner, const leaf::xstreambuf& xParam);

private:
	void SetAddCallbackFunc();
	void SetButtonInfo();

	void RenderFrame();
	void RenderTexts();
	void RenderButtons();

	void LoadImages();
	void UnloadImages();

private:

	CNewUIMessageBoxButton m_BtnOk;
	CNewUIMessageBoxButton m_BtnCancel;

	int m_iMiddleCount;

	unicode::t_char m_szDescription[UIMAX_TEXT_LINE][MAX_TEXT_LENGTH];

	int			m_iDesciptionLine;

	int			m_iStorageSeq;
	int			m_iStorageItemSeq;
	WORD		m_wItemCode;
	unicode::t_char m_szItemName[MAX_TEXT_LENGTH];
	unicode::t_char m_szItemType;
};

class CMsgBoxIGSUseItemConfirmLayout : public TMsgBoxLayout<CMsgBoxIGSUseItemConfirm>
{
public:
	bool SetLayout();
};

#endif

#endif
