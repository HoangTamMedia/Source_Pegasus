
#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewUIExchangeLuckyCoin : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{

			IMAGE_EXCHANGE_LUCKYCOIN_WINDOW_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_EXCHANGE_LUCKYCOIN_WINDOW_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,
			IMAGE_EXCHANGE_LUCKYCOIN_WINDOW_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_EXCHANGE_LUCKYCOIN_WINDOW_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_EXCHANGE_LUCKYCOIN_WINDOW_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_EXCHANGE_LUCKYCOIN_WINDOW_BTN_EXIT = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_EMPTY_SMALL,

			IMAGE_EXCHANGE_LUCKYCOIN_EXCHANGE_BTN = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_EMPTY
		};

	private:
		enum ENTERBC_WINDOW_SIZE
		{
			EXCHANGE_LUCKYCOIN_WINDOW_WIDTH = 190,
			EXCHANGE_LUCKYCOIN_WINDOW_HEIGHT = 429,
		};

	public:
		enum
		{
			MAX_EXCHANGE_BTN = 3,
			EXCHANGE_BTN_VAL = 33,
			EXCHANGE_TEXT_VAL = 14,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;
		POINT						m_TextPos;
		POINT						m_FirstBtnPos;

		CNewUIButton				m_BtnExchange[MAX_EXCHANGE_BTN];
		CNewUIButton				m_BtnExit;

	public:
		CNewUIExchangeLuckyCoin();
		virtual ~CNewUIExchangeLuckyCoin();

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

		void LockExchangeBtn();
		void UnLockExchangeBtn();

	private:
		void SetBtnPos(int x, int y);
		void RenderFrame();
		void RenderTexts();
		void RenderBtn();
		void LoadImages();
		void UnloadImages();
	};
}
