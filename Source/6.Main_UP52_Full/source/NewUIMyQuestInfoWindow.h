
#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIGuildInfoWindow.h"
#include "UIControls.h"

typedef std::list<DWORD> DWordList;

namespace SEASON3B
{
	class CNewUIMyQuestInfoWindow  : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MYQUEST_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_MYQUEST_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_MYQUEST_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_MYQUEST_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_MYQUEST_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_MYQUEST_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_MYQUEST_LINE = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN,
			IMAGE_MYQUEST_BTN_OPEN = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+1,
			IMAGE_MYQUEST_BTN_GIVE_UP = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+2,
			IMAGE_MYQUEST_TAB_BACK = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+3,
			IMAGE_MYQUEST_TAB_SMALL = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+4,
			IMAGE_MYQUEST_TAB_BIG = BITMAP_INTERFACE_MYQUEST_WINDOW_BEGIN+5,
		};
		enum
		{
			MYQUESTINFO_WINDOW_WIDTH = 190,
			MYQUESTINFO_WINDOW_HEIGHT = 429,
		};

		enum TAB_BUTTON_INDEX
		{
			TAB_NON	= -1,
			TAB_QUEST = 0,
			TAB_JOB_CHANGE,
			TAB_CASTLE_TEMPLE,
		};

	public:
		CNewUIMyQuestInfoWindow();
		virtual ~CNewUIMyQuestInfoWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		void SetPos(int x, int y);

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		float GetLayerDepth();

		void OpenningProcess();
		void ClosingProcess();

		void UnselectQuestList();
		void SetCurQuestList(DWordList* pDWordList);
		void SetSelQuestSummary();
		void SetSelQuestRequestReward();

		void QuestOpenBtnEnable(bool bEnable);
		void QuestGiveUpBtnEnable(bool bEnable);

		DWORD GetSelQuestIndex();

	private:
		void LoadImages();
		void UnloadImages();

		void SetButtonInfo();

		bool BtnProcess();

		void RenderFrame();
		void RenderSubjectTexts();
		void RenderQuestInfo();
		void RenderJobChangeContents();
		void RenderJobChangeState();
		void RenderCastleInfo();
		void RenderTempleInfo();

		TAB_BUTTON_INDEX UpdateTabBtn();
		void RenderTabBtn();

		void SetMessage(int nGlobalTextIndex);

	private:
		CNewUIManager* m_pNewUIMng;
		POINT m_Pos;

		CNewUIButton m_BtnExit;
		CNewUIButton	m_btnQuestOpen;
		CNewUIButton	m_btnQuestGiveUp;

		TAB_BUTTON_INDEX m_eTabBtnIndex;

		CUICurQuestListBox		m_CurQuestListBox;
		CUIQuestContentsListBox	m_QuestContentsListBox;
		char			m_aszMsg[2][64];
		int				m_nMsgLine;
	};
}
