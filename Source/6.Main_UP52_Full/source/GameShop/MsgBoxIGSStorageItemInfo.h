
#if !defined(AFX_MSGBOXIGSUSEITEM_H__5E717B05_9D6D_4E85_B168_47D5EEA59CF7__INCLUDED_)
#define AFX_MSGBOXIGSUSEITEM_H__5E717B05_9D6D_4E85_B168_47D5EEA59CF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM

#include "UIControls.h"
#include "NewUIMessageBox.h"
#include "NewUICommonMessageBox.h"

using namespace SEASON3B;

class CMsgBoxIGSStorageItemInfo : public CNewUIMessageBoxBase, public INewUI3DRenderObj
{
public:
	enum IMAGE_IGS_STORAGE_ITEM_INFO
	{
		IMAGE_IGS_BUTTON	= BITMAP_IGS_MSGBOX_BUTTON,
		IMAGE_IGS_FRAME		= BITMAP_IGS_MSGBOX_STORAGE_ITEM,
	};

	enum IMAGESIZE_IGS_STORAGE_ITEM_INFO
	{
		IMAGE_IGS_WINDOW_WIDTH = 640,
		IMAGE_IGS_WINDOW_HEIGHT = 429,
		IMAGE_IGS_FRAME_WIDTH = 210,
		IMAGE_IGS_FRAME_HEIGHT = 202,
		IMAGE_IGS_BTN_WIDTH = 52,
		IMAGE_IGS_BTN_HEIGHT = 26,
	};

	enum IGS_STORAGE_ITEM_INFO_POS
	{
		IGS_BTN_OK_POS_X		= 43,
		IGS_BTN_CANCEL_POS_X	= 115,
		IGS_BTN_POS_Y			= 168,
		IGS_TEXT_TITLE_POS_Y	= 10,
		IGS_TEXT_ITEM_NAME_POS_Y		= 100,
		IGS_TEXT_ITEM_INFO_POS_X		= 30,
		IGS_TEXT_ITEM_INFO_NUM_POS_Y	= 124,
		IGS_TEXT_ITEM_INFO_PERIOD_POS_Y	= 140,
		IGS_TEXT_ITEM_INFO_WIDTH		= 150,
		IGS_3DITEM_POS_X = 56,
		IGS_3DITEM_POS_Y = 34,
		IGS_3DITEM_WIDTH = 97,
		IGS_3DITEM_HEIGHT = 60,
	};

public:
	CMsgBoxIGSStorageItemInfo();
	~CMsgBoxIGSStorageItemInfo();

	bool Create(float fPriority = 3.f);
	void Release();
	bool Update();
	bool Render();

	bool IsVisible() const;

	void Render3D();

	void Initialize(int iStorageSeq, int iStorageItemSeq, WORD wItemCode, unicode::t_char szItemType,
					unicode::t_char* pszName, unicode::t_char* pszNum, unicode::t_char* pszPeriod);

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

	CNewUIMessageBoxButton m_BtnUse;
	CNewUIMessageBoxButton m_BtnCancel;

	int		m_iStorageSeq;
	int		m_iStorageItemSeq;
	WORD	m_wItemCode;

	unicode::t_char	m_szName[MAX_TEXT_LENGTH];
	unicode::t_char	m_szNum[MAX_TEXT_LENGTH];
	unicode::t_char	m_szPeriod[MAX_TEXT_LENGTH];
	unicode::t_char m_szItemType;
};

class CMsgBoxIGSStorageItemInfoLayout : public TMsgBoxLayout<CMsgBoxIGSStorageItemInfo>
{
public:
	bool SetLayout();
};

#endif

#endif
