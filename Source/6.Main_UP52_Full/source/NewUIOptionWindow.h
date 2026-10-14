// NewUIOptionWindow.h: interface for the CNewUIOptionWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIOPTIONWINDOW_H__1469FA1D_7C15_4AFE_AD6E_59C303E72BC0__INCLUDED_)
#define AFX_NEWUIOPTIONWINDOW_H__1469FA1D_7C15_4AFE_AD6E_59C303E72BC0__INCLUDED_

#pragma once

#include "NewUIManager.h"
#include "NewUIMyInventory.h"

namespace SEASON3B
{
	
	class CNewUIOptionWindow  : public CNewUIObj 
	{
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	public:
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
		enum IMAGE_LIST
		{
			IMAGE_OPTION_FRAME_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,
			IMAGE_OPTION_BTN_CLOSE = CNewUIMessageBoxMng::IMAGE_MSGBOX_BTN_CLOSE,
			IMAGE_OPTION_FRAME_DOWN = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,

			IMAGE_OPTION_FRAME_UP = BITMAP_OPTION_BEGIN,
			IMAGE_OPTION_FRAME_LEFT,
			IMAGE_OPTION_FRAME_RIGHT,
			IMAGE_OPTION_LINE,
			IMAGE_OPTION_POINT,	
			IMAGE_OPTION_BTN_CHECK,
			IMAGE_OPTION_EFFECT_BACK,
			IMAGE_OPTION_EFFECT_COLOR,
			IMAGE_OPTION_VOLUME_BACK,
			IMAGE_OPTION_VOLUME_COLOR,
		};
		enum ONOFFGRAP_OPTION
		{
			eLowSettings,
			eMediumSettings,
			eHighSettings,
			eGlowEffect,
			eEffectDynamic,
			eEffectStatic,
			eBMDPlayer,
			eBMDWings,
			eBMDWeapons,
			eBMDImg,
			eBMDMonter,
			eRenderObjects,
			eRenderTerrain,
			eExcellentEffect,
			eBMDZen,
			eEndOnOffGrap,
		};

	public:
		CNewUIOptionWindow();
		virtual ~CNewUIOptionWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		float GetLayerDepth();	//. 10.5f
		float GetKeyEventOrder();	// 10.f;
		
		void OpenningProcess();
		void ClosingProcess();

		void SetAutoAttack(bool bAuto);
		bool IsAutoAttack();
		void SetWhisperSound(bool bSound);
		bool IsWhisperSound();
		void SetSlideHelp(bool bHelp);
		bool IsSlideHelp();
		void SetVolumeLevel(int iVolume);
		int GetVolumeLevel();
		void SetRenderLevel(int iRender);
		int GetRenderLevel();
		void SetRenderAllEffects(bool bRenderAllEffects);
		bool GetRenderAllEffects();
		void SetSoundEnabled(bool bEnabled);
		bool IsSoundEnabled();
		void SetSoundVolumeLevel(int iVolume);
		int GetSoundVolumeLevel();
		void SetEffectSoundEnabled(bool bEnabled);
		bool IsEffectSoundEnabled();
		bool DrawCustomButtonOption(int x, int y, int width, int height, const char* text);
		//===
		bool OnOffGrap[eEndOnOffGrap];

	private:
		void LoadImages();
		void UnloadImages();

		void SetButtonInfo();

		void RenderFrame();
		void RenderContents();
		void RenderCustomFrame();
		void RenderButtons();
		
		// Funções auxiliares para calcular posições Y
		float CalculateSoundVolumeBarY();
		float CalculateEffectVolumeBarY();

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		CNewUIButton m_BtnClose;
		
		bool m_bAutoAttack;		// ÀÚµ¿ °ø°Ý
		bool m_bWhisperSound;	// ±Ó¸» ¾Ë¸²À½ 
		bool m_bSlideHelp;		// ½½¶óÀÌµå µµ¿ò¸»
		bool m_bConsoleVisible;	// Console visibility
		int m_iVolumeLevel;		// º¼·ýÁ¶Àý
		int m_iRenderLevel;		// È¿°úÁ¦ÇÑ
		bool m_bRenderAllEffects;
		bool m_bSoundEnabled;	// Habilitar/Desabilitar Som
		int m_iSoundVolumeLevel; // Nível de Volume do Som (0-10)
		bool m_bEffectSoundEnabled; // Habilitar/Desabilitar Som dos Efeitos
	};
	
}

#endif // !defined(AFX_NEWUIOPTIONWINDOW_H__1469FA1D_7C15_4AFE_AD6E_59C303E72BC0__INCLUDED_)
