
#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"

namespace SEASON3B
{

	class CNewUICharacterInfoWindow : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_CHAINFO_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_CHAINFO_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,
			IMAGE_CHAINFO_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,
			IMAGE_CHAINFO_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,
			IMAGE_CHAINFO_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,

			IMAGE_CHAINFO_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,
			IMAGE_CHAINFO_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,
			IMAGE_CHAINFO_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,
			IMAGE_CHAINFO_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,
			IMAGE_CHAINFO_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,
			IMAGE_CHAINFO_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,
			IMAGE_CHAINFO_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,
			IMAGE_CHAINFO_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,

			IMAGE_CHAINFO_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_CHAINFO_TEXTBOX = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN,
			IMAGE_CHAINFO_BTN_STAT,
			IMAGE_CHAINFO_BTN_STATViewPoint,
			IMAGE_CHAINFO_BTN_QUEST,
			IMAGE_CHAINFO_BTN_PET,
			IMAGE_CHAINFO_BTN_MASTERLEVEL,
			IMAGE_CHAINFO_BTN_Advance,

			// Pegasus skin (Data\Interface\Pegasus). The blue plates, tabs and
			// '+' buttons come from Pegasus_UI; portrait and emblems from
			// Pegasus_Character.
			IMAGE_CHAINFO_PG_PANEL,
			IMAGE_CHAINFO_PG_STATROW,
			IMAGE_CHAINFO_PG_TAB,
			IMAGE_CHAINFO_PG_PLUS,
			IMAGE_CHAINFO_PG_CLOSE,
			IMAGE_CHAINFO_PG_PORTRAIT,
			IMAGE_CHAINFO_PG_ICON_ZEN,
			IMAGE_CHAINFO_PG_ICON_POINT,
			IMAGE_CHAINFO_PG_ICON_MONTH,
			IMAGE_CHAINFO_PG_BTN_EXIT,
			IMAGE_CHAINFO_PG_BTN_QUEST,
			IMAGE_CHAINFO_PG_BTN_PET,
			IMAGE_CHAINFO_PG_BTN_MASTER,
			IMAGE_CHAINFO_PG_FRAME,		// gold winged ring the portrait sits in
			IMAGE_CHAINFO_PG_BOX,		// black rounded panel behind each field
		};

		// g_fScreenRate_y is a fixed 1.5 (1.35 under 1600x900), so DisplayHeight
		// is 720 at 1080p - a 580 tall window fits with room to spare.
		// Element geometry is the Pegasus reference measured as a fraction of
		// its window and scaled to this one.
		enum
		{
			CHAINFO_WINDOW_WIDTH = 260,
			// Squeezed down from 580 to sit closer to the inventory's 429. It
			// cannot actually reach 429: the '+' buttons are 31 tall so a stat
			// row can't go under 28, the five rows carry eight description
			// lines between them at a fixed 13px font, and the four bottom
			// buttons are 34 - about 471px of content that does not compress.
			// Everything that could give (gaps, panel heights) already has.
			CHAINFO_WINDOW_HEIGHT = 505,
			// Row spacing follows how many description lines sit under each:
			// strength 2, dexterity 3, the rest 1.
			HEIGHT_STRENGTH = 190,
			HEIGHT_DEXTERITY = 250,
			HEIGHT_VITALITY = 323,
			HEIGHT_ENERGY = 370,
			HEIGHT_CHARISMA = 417,
			BTN_STAT_COUNT = 5,
			STAT_STRENGTH = 0,
			STAT_DEXTERITY,
			STAT_VITALITY,
			STAT_ENERGY,
			STAT_CHARISMA,
		};

		// Geometry of one stat row: [ blue label plate | dark value box ] [ + ]
		enum
		{
			// TournamentUI_I7 (374x46) is the whole row, not just the label:
			// blue plate 0..163, value box 168..300, empty slot 310..373 that
			// the '+' sits in. Widths below are those fractions of PG_ROW_W.
			PG_ROW_X = 8,
			PG_ROW_W = 244,
			PG_ROW_H = 28,
			PG_ROW_LABEL_W = 106,		// blue plate
			PG_ROW_VALUE_X = 110,
			PG_ROW_VALUE_W = 86,

			// Every W/H below that belongs to a button MUST match the cell size
			// baked into its pg_chainfo_* strip, or the state offset lands
			// between cells and the button draws garbage. See make_btn_strips.
			PG_PLUS_X = 214,			// centred in I7's right-hand slot
			PG_PLUS_W = 33,
			PG_PLUS_H = 31,

			PG_TAB_COUNT = 3,
			PG_TAB_X = 10,
			PG_TAB_Y = 48,
			PG_TAB_W = 80,
			PG_TAB_H = 25,
			PG_TAB_GAP = 1,

			PG_CLOSE_X = 219,
			PG_CLOSE_Y = 9,
			PG_CLOSE_W = 24,
			PG_CLOSE_H = 29,

			// Gold ring, with the round portrait centred inside its opening.
			PG_FRAME_X = 8,
			PG_FRAME_Y = 78,
			PG_FRAME_W = 58,
			PG_FRAME_H = 40,
			// Sized and placed to sit in the ring's opening, which sits at
			// roughly (175,120) r=75x80 of I10's 359x263.
			PG_PORTRAIT_X = 24,
			PG_PORTRAIT_Y = 86,
			PG_PORTRAIT_SIZE = 24,

			// Black field boxes: one wide one for level/reset, three for money.
			PG_BOX_INFO_X = 68,
			PG_BOX_INFO_Y = 78,
			PG_BOX_INFO_W = 184,
			PG_BOX_INFO_H = 46,
			PG_BOX_MONEY_Y = 128,
			PG_BOX_MONEY_W = 79,
			PG_BOX_MONEY_H = 38,
			PG_BOX_MONEY_GAP = 3,
			PG_BOX_MONEY_X = 8,

			// Inside the black box, which starts at PG_BOX_INFO_X.
			PG_INFO_X = PG_BOX_INFO_X + 8,
			PG_INFO_LEVEL_Y = 88,
			PG_INFO_RESET_Y = 104,
			// Three lines have to fit between the currency icons (end at 168)
			// and the first stat plate (starts at HEIGHT_STRENGTH). 13 apart,
			// with the free-point line last so it lands where the mock has it.
			PG_FREEPOINT_X = 16,
			// Has to clear HEIGHT_STRENGTH - the first stat row starts at 190
			// and this line plus the two above it sit in the gap under the
			// money boxes.
			PG_FREEPOINT_Y = 174,
			PG_POINTLINE_1_Y = PG_FREEPOINT_Y - 26,
			PG_POINTLINE_2_Y = PG_FREEPOINT_Y - 13,
			PG_HELP_X = 232,
			PG_HELP_Y = 168,

			PG_ICON_Y = 122,
			PG_ICON_SIZE = 28,
			PG_ICON_ZEN_X = 13,
			PG_ICON_POINT_X = 97,
			PG_ICON_MONTH_X = 179,

			PG_BOTTOM_COUNT = 4,
			// Sits inside CHAINFO_WINDOW_HEIGHT (505) with the row 34 tall and
			// a small margin under it. Leaving this at the old 531 is what put
			// the four buttons outside the frame.
			PG_BOTTOM_Y = 462,
			PG_BOTTOM_X = 7,
			PG_BOTTOM_W = 57,
			PG_BOTTOM_H = 34,
			PG_BOTTOM_GAP = 7,
		};

		enum CURR_OPEN_TAB_BUTTON
		{
			TAB_POINT,
			TAB_INFO,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		CNewUIButton m_BtnStat[BTN_STAT_COUNT];
		CNewUIButton m_BtnStatPoint[BTN_STAT_COUNT];
		CNewUIButton m_BtnClose;		// round X on the title bar
		CNewUIButton m_BtnExit;			// leftmost of the bottom action row
		CNewUIButton m_BtnQuest;
		CNewUIButton m_BtnPet;
		CNewUIButton m_Advance;
		CNewUIButton m_BtnMasterLevel;
		CNewUIRadioGroupButton	m_TabBtn;
		int						m_iNumCurOpenTab;
	public:
		CNewUICharacterInfoWindow();
		virtual ~CNewUICharacterInfoWindow();
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void SetPos(int x, int y);
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		float GetLayerDepth();
		void OpenningProcess();
		const POINT& GetPos() const;

	private:
		void LoadImages();
		void LoadPortraitImage();
		void UnloadImages();
		void ButtonCharacter(CNewUIButton* m_Btn, int imgindex, int x, int y, int width, int height, float su, float sv, float uw, float vh, bool overflg, bool bClickEffect, bool istoppos);
		void ResetEquipmentLevel();
		void SetButtonInfo();
		bool BtnProcess();
		void RenderFrame();
		void RenderTexts();
		void ToolTipCharacter(int sx, int sy, const char* Text);
		void RenderSubjectTexts();
		void RenderTableTexts();
		void RenderAttribute();
		void RenderButtons();
	};

}
