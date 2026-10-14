

#if !defined(AFX_NEWUIHEROPOSITIONINFO_H__5D452B62_B315_41EE_A862_5929286925F7__INCLUDED_)
#define AFX_NEWUIHEROPOSITIONINFO_H__5D452B62_B315_41EE_A862_5929286925F7__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
enum IMAGE_LIST
{
	IMAGE_HERO_POSITION_INFO_BASE_WINDOW = BITMAP_HERO_POSITION_INFO_BEGIN,
};
namespace SEASON3B
{
	class CNewUIHeroPositionInfo : public CNewUIObj
	{
	public:

	private:
		enum HERO_POSITION_INFO_BASE_WINDOW_SIZE
		{
			HERO_POSITION_INFO_BASEA_WINDOW_WIDTH = 22,
			HERO_POSITION_INFO_BASE_WINDOW_HEIGHT = 25,
			HERO_POSITION_INFO_BASEB_WINDOW_WIDTH = 80,
			HERO_POSITION_INFO_BASEC_WINDOW_WIDTH = 22,
		};
		struct LoadTypeMap
		{
			   DWORD ModelID;
			   DWORD XSW_Minimap_Frame;
			   DWORD YSW_Minimap_Frame;
			  DWORD XSW_Minimap;
			  DWORD YSW_Minimap;
			  DWORD XSW_Time_Frame;
			  DWORD YSW_Time_Frame;
			  DWORD This_92;
			  DWORD This_96;
			 DWORD DropMoveUserX;
			 DWORD DropMoveUserY;
			 DWORD This_108;
			 DWORD This_112;
			 float CurrentMoveX;
			 float CurrentMoveY;
			 DWORD Scale;
			 DWORD TimerEventVK;
			 float Alpha;
		};

	private:
		int							WidenX;
		CNewUIManager* m_pNewUIMng;
		POINT						m_Pos;

		CNewUIButton				m_BtnConfig;
		CNewUIButton				m_BtnStart;
		CNewUIButton				m_BtnStop;
	public:
		POINT						m_CurHeroPosition;
		CNewUIHeroPositionInfo();
		virtual ~CNewUIHeroPositionInfo();

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

		void SetCurHeroPosition(int x, int y);
		void MoveTextTipPos(CNewUIButton* m_Btn, int iX, int iY);
		void SetButtonInfo(CNewUIButton* m_Btn, int imgindex, int x, int y, int sx, int sy, bool overflg, bool isimgwidth, bool bClickEffect, bool MoveTxt, unicode::t_string btname, unicode::t_string tooltiptext, bool istoppos);

		void MiniMapLoad();
		void DrawMiniMap();
		LoadTypeMap DataMap;
		void DataMapLoad();
		void DataViewPortMapLoad(float x, float y);
		int IndexIMGMap;
	private:
		void LoadImages();
		void UnloadImages();

		DWORD TimeClickMouseL;

	};
}

#endif
