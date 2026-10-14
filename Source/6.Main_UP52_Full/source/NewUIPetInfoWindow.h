
#if !defined(AFX_NEWUIPETINFORMATION_H__A1B6EB0C_4F9F_4285_B89A_7BB680BF84D6__INCLUDED_)
#define AFX_NEWUIPETINFORMATION_H__A1B6EB0C_4F9F_4285_B89A_7BB680BF84D6__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIButton.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIInventoryctrl.h"
#include "NewUIGuildInfoWindow.h"
#include "NewUIMainFrameWindow.h"

namespace SEASON3B
{
	class CNewUIPetInfoWindow : public CNewUIObj
	{

	public:
		enum IMAGE_LIST
		{
			IMAGE_PETINFO_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_PETINFO_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,
			IMAGE_PETINFO_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_PETINFO_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_PETINFO_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_PETINFO_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_PETINFO_TAB_BUTTON = CNewUIGuildInfoWindow::IMAGE_GUILDINFO_TAB_BUTTON,

			IMAGE_PETINFO_SKILL = CNewUISkillList::IMAGE_COMMAND,
			IMAGE_PETINFO_SKILLBOX = CNewUISkillList::IMAGE_SKILLBOX,

			IMAGE_PETINFO_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,
			IMAGE_PETINFO_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,
			IMAGE_PETINFO_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,
			IMAGE_PETINFO_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,
			IMAGE_PETINFO_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,
			IMAGE_PETINFO_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,
			IMAGE_PETINFO_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,
			IMAGE_PETINFO_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,

			IMAGE_PETINFO_LIFEBAR = BITMAP_INTERFACE_NEW_PETINFO_WINDOW_BEGIN,
			IMAGE_PETINFO_LIFE,
		};

	private:
		enum
		{
			PETINFOWINDOW_WIDTH = 190,
			PETINFOWINDOW_HEIGHT = 429,
		};

		enum CUR_OPEN_TAB_BUTTON
		{
			TAB_TYPE_DARKHORSE = 0,
			TAB_TYPE_DARKSPIRIT
		};

		enum EVENT_STATE
		{
			EVENT_NONE = 0,
			EVENT_EXIT_BTN_HOVER,
			EVENT_EXIT_BTN_HOLDING
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		CNewUIRadioGroupButton		m_BtnTab;
		CNewUIButton				m_BtnExit;
		POINT						m_Pos;

		int							m_iNumCurOpenTab;

		EVENT_STATE					m_EventState;

		int							m_aiDamage[2];
		float						m_fAddDamagePercent;

	public:
		CNewUIPetInfoWindow();
		virtual ~CNewUIPetInfoWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		void SetPos(int x, int y);

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		bool BtnProcess();

		float GetLayerDepth();

		void OpenningProcess();
		void ClosingProcess();

	private:
		void LoadImages();
		void UnloadImages();

		void InitButtons( );
		void RenderGroupBox( int iPosX, int iPosY, int iWidth, int iHeight, int iTitleWidth=60, int iTitleHeight=20 );
		void CalcDamage( int iNumTapButton );
		bool RenderDarkHorseInfo( PET_INFO* pPetInfo );
		bool RenderDarkSpiritInfo( PET_INFO* pPetInfo );
		bool RenderToolTip();
	};

}

#endif
