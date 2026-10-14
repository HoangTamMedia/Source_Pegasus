
#if !defined(AFX_NEWUICOMMANDWINDOW_H__6C0AA8A8_EF69_45F3_BCE4_F957F08310C5__INCLUDED_)
#define AFX_NEWUICOMMANDWINDOW_H__6C0AA8A8_EF69_45F3_BCE4_F957F08310C5__INCLUDED_
#define pDrawButton					((void(__cdecl*)(DWORD ModelID, float X, float Y, float W, float H, float CurrentX, float CurrentY)) 0x00790E40)

#pragma once

#include "NewUIMyInventory.h"
#include "NewUIButton.h"

#define MAX_COMMAND_BTN		( 11 )

#define COMMAND_BTN_INTERVAL_SIZE	(1)

namespace SEASON3B
{
	class CNewUICommandWindow : public CNewUIObj
	{
	public:
		enum eIMAGE_LIST
		{

			IMAGE_COMMAND_BASE_WINDOW_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_COMMAND_BASE_WINDOW_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP,
			IMAGE_COMMAND_BASE_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_COMMAND_BASE_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_COMMAND_BASE_WINDOW_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_COMMAND_BASE_WINDOW_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_COMMAND_BTN = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_EMPTY,
			IMAGE_COMMAND_SELECTID_BG = BITMAP_COMMAND_WINDOW_BEGIN,
		};

		enum eCOMMAND_WINDOW_SIZE
		{
			COMMAND_WINDOW_WIDTH = 190,
			COMMAND_WINDOW_HEIGHT = 429,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		CNewUIButton				m_BtnCommand[MAX_COMMAND_BTN];

		CNewUIButton				m_BtnExit;
		int							m_iCurSelectCommand;
		int							m_iCurMouseCursor;
		bool						m_bSelectedChar;
		bool						m_bCanCommand;

	public:
		CNewUICommandWindow();
		virtual ~CNewUICommandWindow();

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

		int	GetCurCommandType( );

		void SetMouseCursor( int iCursorType );
		int	 GetMouseCursor( );

		bool CommandTrade(CHARACTER* pSelectedCha);
		bool CommandPurchase(CHARACTER* pSelectedCha);
		bool CommandParty(SHORT iChaKey);
		bool CommandWhisper(CHARACTER* pSelectedCha);
		bool CommandGuild(CHARACTER* pSelectedCha);
 		bool CommandGuildUnion(CHARACTER* pSelectedCha);
 		bool CommandGuildRival(CHARACTER* pSelectedCha);
 		bool CommandCancelGuildRival(CHARACTER* pSelectedCha);
 		bool CommandAddFriend(CHARACTER* pSelectedCha);
 		bool CommandFollow(int iSelectedChaIndex);
 		int CommandDual(CHARACTER* pSelectedCha);
	private:
		void LoadImages( );
		void UnloadImages( );
		void InitButtons( );
		void RenderBaseWindow( );
		void RunCommand( );
		void SelectCommand( );
		void SetBtnState(int iBtnType, bool bStateDown);

	};
};

#endif
