
#if !defined(AFX_NEWUIMAINFRAMEWINDOW_H__46A029CA_44A5_4050_9216_FA8A25EC4629__INCLUDED_)
#define AFX_NEWUIMAINFRAMEWINDOW_H__46A029CA_44A5_4050_9216_FA8A25EC4629__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "ZzzTexture.h"
#include "NewUI3DRenderMng.h"
#include "NewUIButton.h"

namespace SEASON3B
{
	// Where the Pegasus HUD plate currently sits: origin of its top-left corner
	// in UI units, plus the factor its 1224x180 art space is drawn at. Lets
	// other windows - the buff row, for one - hang off the plate instead of
	// guessing at screen offsets. Returns false when the HUD is compiled out.
	bool GetPegasusHudTransform(float& fOrgX, float& fOrgY, float& fScale);

	enum
	{
		HOTKEY_Q = 0,
		HOTKEY_W,
		HOTKEY_E,
		HOTKEY_R,
		HOTKEY_COUNT
	};

	enum
	{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
		MAINFRAME_BTN_PARTCHARGE = 0,
#endif
		MAINFRAME_BTN_CHAINFO,
		MAINFRAME_BTN_MYINVEN,
		MAINFRAME_BTN_FRIEND,
		MAINFRAME_BTN_WINDOW,
	};

	enum KINDOFSKILL
	{
		KOS_COMMAND = 1,
		KOS_SKILL1,
		KOS_SKILL2,
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		KOS_SKILL3,
#endif
	};

	class CNewUIItemHotKey
	{

	public:
		CNewUIItemHotKey();
		virtual ~CNewUIItemHotKey();

		bool UpdateKeyEvent();

		void SetHotKey(int iHotKey, int iItemType, int iItemLevel);
		int GetHotKey(int iHotKey);
		int GetHotKeyLevel(int iHotKey);
		void UseItemRButton();
		void RenderItems();
		void RenderItemCount();

	private:
		int GetHotKeyItemIndex(int iType, bool bItemCount = false);
		bool GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd);
		int GetHotKeyItemCount(int iType);

		int m_iHotKeyItemType[HOTKEY_COUNT];
		int m_iHotKeyItemLevel[HOTKEY_COUNT];
	};

	class CNewUISkillList : public CNewUIObj
	{
		enum
		{
			SKILLHOTKEY_COUNT = 10
		};
		enum EVENT_STATE
		{
			EVENT_NONE = 0,

			EVENT_BTN_HOVER_CURRENTSKILL,
			EVENT_BTN_DOWN_CURRENTSKILL,

			EVENT_BTN_HOVER_SKILLHOTKEY,
			EVENT_BTN_DOWN_SKILLHOTKEY,

			EVENT_BTN_HOVER_SKILLLIST,
			EVENT_BTN_DOWN_SKILLLIST,
		};

	public:
		enum IMAGE_LIST
		{
			IMAGE_SKILL1 = BITMAP_INTERFACE_NEW_SKILLICON_BEGIN,
			IMAGE_SKILL2,
			IMAGE_COMMAND,
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			IMAGE_SKILL3,
#endif
			IMAGE_SKILLBOX,
			IMAGE_SKILLBOX_USE,
			IMAGE_NON_SKILL1,
			IMAGE_NON_SKILL2,
			IMAGE_NON_COMMAND,
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			IMAGE_NON_SKILL3,
#endif
		};

		CNewUISkillList();
		virtual ~CNewUISkillList();

		bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng);
		void Release();

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		bool RenderMuHelper(float X, float Y, int SkillSelect, int SkillBuff = 0);
		void RenderSkillInfo();
		float GetLayerDepth();

		WORD GetHeroPriorSkill();
		void SetHeroPriorSkill(BYTE bySkill);

		void Reset();

		void SetHotKey(int iHotKey, int iSkillType);
		int GetHotKey(int iHotKey);
		int GetSkillIndex(int iSkillType);
		void RenderCurrentSkillAndHotSkillList();

		bool IsSkillListUp();

		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);
		// width/height pick the cell out of the 256x256 icon atlas as well as
		// sizing the quad, so they have to stay at the atlas cell size (20x28).
		// Pass fDrawW/fDrawH to draw that cell at a different size instead.
		void RenderSkillIcon(int iIndex, float x, float y, float width, float height, int TypeMuHelper = 0,
			float fDrawW = 0.f, float fDrawH = 0.f);
	private:
		void LoadImages();
		void UnloadImages();
		bool IsArrayUp(BYTE bySkill);
		bool IsArrayIn(BYTE bySkill);
		void UseHotKey(int iHotKey);

		void RenderSkillDelay(int iIndex, float x, float y, float width, float height);
		void RenderPetSkill();

		void ResetMouseLButton();

	private:
		CNewUIManager* m_pNewUIMng;
		CNewUI3DRenderMng* m_pNewUI3DRenderMng;

		bool m_bHotKeySkillListUp;
		int m_iHotKeySkillType[SKILLHOTKEY_COUNT];

		bool m_bSkillList;

		bool m_bRenderSkillInfo;
		int m_iRenderSkillInfoType;
		int m_iRenderSkillInfoPosX;
		int m_iRenderSkillInfoPosY;

		EVENT_STATE m_EventState;
		WORD m_wHeroPriorSkill;
	};

	class CNewUIMainFrameWindow : public CNewUIObj, public INewUI3DRenderObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_MENU_1 = BITMAP_INTERFACE_NEW_MAINFRAME_BEGIN,
			IMAGE_MENU_2,
			IMAGE_MENU_3,
			IMAGE_MENU_2_1,
			IMAGE_GAUGE_BLUE,
			IMAGE_GAUGE_GREEN,
			IMAGE_GAUGE_RED,
			IMAGE_GAUGE_AG,
			IMAGE_GAUGE_SD,
			IMAGE_GAUGE_EXBAR,
			IMAGE_MASTER_GAUGE_BAR,
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
			IMAGE_MENU_BTN_CSHOP,
#endif
			IMAGE_MENU_BTN_CHAINFO,
			IMAGE_MENU_BTN_MYINVEN,
			IMAGE_MENU_BTN_FRIEND,
			IMAGE_MENU_BTN_WINDOW,
			IMAGE_DECOR_WIDE,
			IMAGE_MENUS2_BUTTON,
		};

		CNewUIMainFrameWindow();
		virtual ~CNewUIMainFrameWindow();

		bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng);
		void Release();

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		void Render3D();

		bool IsVisible() const;

		float GetLayerDepth();
		float GetKeyEventOrder();

		void SetItemHotKey(int iHotKey, int iItemType, int iItemLevel);
		int GetItemHotKey(int iHotKey);
		int GetItemHotKeyLevel(int iHotKey);
		void UseHotKeyItemRButton();

		void UpdateItemHotKey();

		void ResetSkillHotKey();
		void SetSkillHotKey(int iHotKey, int iSkillType);
		int GetSkillHotKey(int iHotKey);
		int GetSkillHotKeyIndex(int iSkillType);

		void SetPreExp_Wide(__int64 dwPreExp);
		void SetGetExp_Wide(__int64 dwGetExp);

		void SetPreExp(DWORD dwPreExp);
		void SetGetExp(DWORD dwGetExp);

		void SetBtnState(int iBtnType, bool bStateDown);
		void SetButtonInfo();

		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);

	private:
		bool RenderSS2();
		void RenderLifeManaSS2();
		void RenderGuageSDSS2();
		void RenderGuageAGSS2();
		void RenderExperienceSS2();

		void RenderButtonsSS2();
		POINT ButtonSS2;
		float ButtonSS2W;
		float ButtonSS2H;

		void LoadImages();
		void UnloadImages();

		bool BtnProcess();

		void RenderFrame();
		void RenderPegasusHUD();
		void RenderLifeMana();
		void RenderGuageAG();
		void RenderGuageSD();
		void RenderExperience();
		void RenderHotKeyItemCount();
		void RenderButtons();
		void RenderCharInfoButton();
		void RenderFriendButton();
		void RenderFriendButtonState();

	public:
		__int64	m_loPreExp;
		__int64	m_loGetExp;

	private:
		CNewUIManager* m_pNewUIMng;
		CNewUI3DRenderMng* m_pNewUI3DRenderMng;

		CNewUIItemHotKey m_ItemHotKey;

		bool m_bExpEffect;
		DWORD m_dwExpEffectTime;

		DWORD m_dwPreExp;
		DWORD m_dwGetExp;

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
		CNewUIButton m_BtnCShop;
#endif
		CNewUIButton m_BtnChaInfo;
		CNewUIButton m_BtnMyInven;
		CNewUIButton m_BtnFriend;
		CNewUIButton m_BtnWindow;

		bool m_bButtonBlink;
	};

}

#endif
