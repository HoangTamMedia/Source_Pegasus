
#if !defined(AFX_NEWUIPARTYINFO_H__8F1E7392_C9CC_487E_8EAD_10DBD47AF20F__INCLUDED_)
#define AFX_NEWUIPARTYINFO_H__8F1E7392_C9CC_487E_8EAD_10DBD47AF20F__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewUIPartyInfoWindow : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{

			IMAGE_PARTY_BASE_WINDOW_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_PARTY_BASE_WINDOW_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_PARTY_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_PARTY_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_PARTY_BASE_WINDOW_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_PARTY_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_PARTY_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,
			IMAGE_PARTY_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,
			IMAGE_PARTY_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,
			IMAGE_PARTY_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,
			IMAGE_PARTY_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,
			IMAGE_PARTY_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,
			IMAGE_PARTY_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,
			IMAGE_PARTY_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,

			IMAGE_PARTY_HPBAR_BACK = BITMAP_PARTY_INFO_BEGIN,
			IMAGE_PARTY_HPBAR,
			IMAGE_PARTY_FLAG,
			IMAGE_PARTY_EXIT,
		};

	private:
		enum PARTY_WINDOW_SIZE
		{
			PARTY_INFO_WINDOW_WIDTH = 190,
			PARTY_INFO_WINDOW_HEIGHT = 429,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		CNewUIButton				m_BtnExit;
		CNewUIButton				m_BtnPartyExit[MAX_PARTYS];

		int							m_iSelectedCharID;

		bool						m_bParty;

	public:
		CNewUIPartyInfoWindow();
		virtual ~CNewUIPartyInfoWindow();

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

		bool LeaveParty( const int iIndex );

		void SetParty( bool bParty );

	private:
		void LoadImages();
		void UnloadImages();

		void InitButtons( );
		void RenderGroupBox( int iPosX, int iPosY, int iWidth, int iHeight, int iTitleWidth=60, int iTitleHeight=20 );

		void RenderMemberStatue( int iIndex, PARTY_t* pMember, bool bExitBtnRender=false );
		void RenderPartyMiniList( int iIndex, PARTY_t* pMember );
	};
}

#endif
