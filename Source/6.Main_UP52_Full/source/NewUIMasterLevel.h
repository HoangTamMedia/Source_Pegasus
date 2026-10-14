
#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"

#include "NewUIMainFrameWindow.h"
#include "NewUIChatLogWindow.h"
#include "NewUIMyInventory.h"

namespace SEASON3B
{
	class CNewUIMasterLevel : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MASTER_SCROLLBAR_ON	= CNewUIChatLogWindow::IMAGE_SCROLLBAR_ON,
			IMAGE_MASTER_SCROLLBAR_OFF	= CNewUIChatLogWindow::IMAGE_SCROLLBAR_OFF,
			IMAGE_MASTER_SKILL_ICON		= CNewUISkillList::IMAGE_SKILL2,
			IMAGE_MASTER_EXIT			= CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_MASTER_INTERFACE		= BITMAP_INTERFACE_MASTER_BEGIN,
		};

		enum MASTER_DATA
		{
			SKILL_ICON_DATA_WDITH = 4,
			SKILL_ICON_DATA_HEIGHT = 8,
			SKILL_ICON_WIDTH = 20,
			SKILL_ICON_HEIGHT = 28,
			SKILL_ICON_STARTX1 = 75,
			SKILL_ICON_STARTY1 = 75,
		};

		enum EVENT_STATE
		{
			EVENT_NONE = 0,
			EVENT_SCROLL_BTN_DOWN,
		};

	private:

		CNewUIManager*			m_pNewUIMng;
		POINT					m_Pos;
		POINT					m_Width;
		EVENT_STATE				m_EventState[4];
		int						m_Loc[4];
		int						m_BackUp;
		CNewUIButton				m_BtnExit;

	public:
		CNewUIMasterLevel();
		virtual ~CNewUIMasterLevel();

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
		void OpenMasterLevel( const char* filename );
		CNewUIButton m_BtnToolTip;

	private:
		void LoadImages();
		void UnloadImages();
		void Render_Text();
		void Render_Icon();
		void Render_Scroll();
		bool Check_Mouse(int mx,int my);
		bool Check_Btn();

	};
}
