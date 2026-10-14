
#if !defined(AFX_NEWUIMYINVENTORY_H__74DA6D7A_CF5A_46E9_8C72_9D38F0DC95EC__INCLUDED_)
#define AFX_NEWUIMYINVENTORY_H__74DA6D7A_CF5A_46E9_8C72_9D38F0DC95EC__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIInventoryCtrl.h"
#include "NewUIMessageBox.h"
#include "NewUI3DRenderMng.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	class CNewUIMyInventory : public CNewUIObj, public INewUI3DRenderObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_INVENTORY_MYSHOP_OPEN_BTN = BITMAP_MYSHOPINTERFACE_NEW_PERSONALINVENTORY_BEGIN + 1,
			IMAGE_INVENTORY_MYSHOP_CLOSE_BTN = BITMAP_MYSHOPINTERFACE_NEW_PERSONALINVENTORY_BEGIN + 2,
			IMAGE_INVENTORY_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_INVENTORY_BACK_TOP = BITMAP_INTERFACE_NEW_PERSONALINVENTORY_BEGIN,
			IMAGE_INVENTORY_BACK_TOP2,
			IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_INVENTORY_BACK_BOTTOM,
			IMAGE_INVENTORY_ITEM_BOOT,
			IMAGE_INVENTORY_ITEM_HELM,
			IMAGE_INVENTORY_ITEM_FAIRY,
			IMAGE_INVENTORY_ITEM_WING,
			IMAGE_INVENTORY_ITEM_RIGHT,
			IMAGE_INVENTORY_ITEM_LEFT,
			IMAGE_INVENTORY_ITEM_ARMOR,
			IMAGE_INVENTORY_ITEM_GLOVES,
			IMAGE_INVENTORY_ITEM_PANTS,
			IMAGE_INVENTORY_ITEM_RING,
			IMAGE_INVENTORY_ITEM_NECKLACE,
			IMAGE_INVENTORY_MONEY,
			IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_INVENTORY_REPAIR_BTN,
			IMAGE_INVENTORY_EXPAND_BTN,
		};

		enum REPAIR_MODE
		{
			REPAIR_MODE_OFF = 0,
			REPAIR_MODE_ON,
		};

		enum MYSHOP_MODE
		{
			MYSHOP_MODE_OPEN = 0,
			MYSHOP_MODE_CLOSE,
		};

	private:
		enum ITEM_OPTION
		{
			ITEM_SET_OPTION = 1,
			ITEM_SOCKET_SET_OPTION = 2,
		};

		enum
		{
			INVENTORY_WIDTH = 190,
			INVENTORY_HEIGHT = 429,
		};
		typedef struct tagEQUIPMENT_ITEM
		{
			int x, y;
			int width, height;
			DWORD dwBgImage;
		} EQUIPMENT_ITEM;

		CNewUIManager* m_pNewUIMng;
		CNewUI3DRenderMng* m_pNewUI3DRenderMng;
		CNewUIInventoryCtrl* m_pNewInventoryCtrl;
		POINT m_Pos;

		float m_fCurrentCameraZOrder;

		EQUIPMENT_ITEM m_EquipmentSlots[MAX_EQUIPMENT_INDEX];
		int	 m_iPointedSlot;

		CNewUIButton m_BtnRepair;
		CNewUIButton m_BtnExit;
		CNewUIButton m_BtnMyShop;
		CNewUIButton m_BtnExpand;
		CNewUIButton m_BtnJewel;

		MYSHOP_MODE m_MyShopMode;
		REPAIR_MODE m_RepairMode;
		DWORD m_dwStandbyItemKey;

		bool m_bRepairEnableLevel;
		bool m_bMyShopOpen;

	public:
		CNewUIMyInventory();
		virtual ~CNewUIMyInventory();

		bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng, int x, int y);
		void Release();

		bool EquipItem(int iIndex, BYTE* pbyItemPacket);
		void UnequipItem(int iIndex);
		void UnequipAllItems();

		void CNewUIMyInventory::ButtonInventory(CNewUIButton* m_Btn, int imgindex, int x, int y, int width, int height, float su, float sv, float uw, float vh, bool overflg, bool bClickEffect, bool istoppos);

		bool IsEquipable(int iIndex, ITEM* pItem);

		bool InsertItem(int iIndex, BYTE* pbyItemPacket) const;
		void DeleteItem(int iIndex) const;
		void DeleteAllItems() const;

		void SetPos(int x, int y);
		const POINT& GetPos() const;
		REPAIR_MODE GetRepairMode() const;
		void SetRepairMode(bool bRepair);

#ifdef LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY
		BOOL IsInvenItem(const short sType);
#endif

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		void Render3D();

		bool IsVisible() const;

		void OpenningProcess();
		void ClosingProcess();

		float GetLayerDepth();

		CNewUIInventoryCtrl* GetInventoryCtrl() const;

		ITEM* FindItem(int iLinealPos) const;
		ITEM* FindItemByKey(DWORD dwKey) const;
		int FindItemIndex(short int siType, int iLevel = -1) const;
		int FindItemReverseIndex(short sType, int iLevel = -1) const;
		int FindEmptySlot(IN int cx, IN int cy) const;
		int FindEmptySlot(ITEM* pItem) const;
		bool IsItem(short int siType, bool bcheckPick = false) const;
		int	GetNumItemByKey(DWORD dwItemKey) const;
		int GetNumItemByType(short sItemType) const;
		BYTE GetDurabilityPointedItem() const;
		int GetPointedItemIndex() const;
		int FindHPItemIndex() const;
		int FindManaItemIndex() const;

		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);

		void SetStandbyItemKey(DWORD dwItemKey);
		DWORD GetStandbyItemKey() const;
		int GetStandbyItemIndex() const;
		ITEM* GetStandbyItem() const;

		bool IsRepairEnableLevel() const;
		void SetRepairEnableLevel(bool bOver);

		void ChangeMyShopButtonStateOpen();
		void ChangeMyShopButtonStateClose();
		void LockMyShopButtonOpen();
		void UnlockMyShopButtonOpen();

		void CreateEquippingEffect(ITEM* pItem);

		static bool CanRegisterItemHotKey(int iType);
		static bool HandleInventoryActions(CNewUIInventoryCtrl* targetControl);

		enum GetTypeAutoMoveB
		{
			MOVE_INV_TO_MIX = 1,
			MOVE_MIX_TO_INV,
			MOVE_INV_TO_TRADE,
			MOVE_INV_TO_CHAR,
			MOVE_CHAR_TO_INV,
		};
		void BMoveItemNew(int Type);
	protected:
		void DeleteEquippingEffect();
		void DeleteEquippingEffectBug(ITEM* pItem);

		void SetEquipmentSlotInfo();
		void SetButtonInfo();

	private:
		void LoadImages() const;
		void UnloadImages();

		void RenderFrame() const;
		void RenderSetOption();
		void RenderSocketOption();
		void RenderEquippedItem();
		void RenderButtons();
		void RenderInventoryDetails() const;

		bool EquipmentWindowProcess();
		static bool RepairItemAtMousePoint(CNewUIInventoryCtrl* targetControl);
		static bool ApplyJewels(CNewUIInventoryCtrl* targetControl, CNewUIPickedItem* pPickedItem, ITEM* pPickItem,
			int iSourceIndex, int iTargetIndex);
		static bool TryStackItems(CNewUIInventoryCtrl* targetControl, ITEM* pPickItem, int iSourceIndex,
			int iTargetIndex);
		static bool TryConsumeItem(CNewUIInventoryCtrl* targetControl, ITEM* pItem, const int iIndex);

		bool InventoryProcess() const;
		bool BtnProcess();

		void RenderItemToolTip(int iSlotIndex) const;
		bool CanOpenMyShopInterface();
		void ToggleRepairMode();

		void ResetMouseLButton();
		void ResetMouseRButton();
	};
}

#endif
