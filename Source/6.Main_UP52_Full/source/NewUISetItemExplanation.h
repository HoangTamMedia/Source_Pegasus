
#if !defined(AFX_NEWUISETITEMEXPLANATION_H__31F3D8C3_34A7_45F8_BEC6_A915E8B5B6BF__INCLUDED_)
#define AFX_NEWUISETITEMEXPLANATION_H__31F3D8C3_34A7_45F8_BEC6_A915E8B5B6BF__INCLUDED_

#pragma once

#include "NewUIManager.h"

namespace SEASON3B
{

	class CNewUISetItemExplanation  : public CNewUIObj
	{
	public:
		CNewUISetItemExplanation();
		virtual ~CNewUISetItemExplanation();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		void SetPos(int x, int y);

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		float GetLayerDepth();
		float GetKeyEventOrder();

		void OpenningProcess();
		void ClosingProcess();

	private:
		CNewUIManager*	m_pNewUIMng;
		POINT			m_Pos;

	};

}

#endif
