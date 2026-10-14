
#ifndef _NEWUIHOTKEY_H_
#define _NEWUIHOTKEY_H_

#pragma once

#include "NewUIBase.h"

namespace SEASON3B
{
	class CNewUIManager;

	class CNewUIHotKey : public CNewUIObj
	{
		CNewUIManager*	m_pNewUIMng;
		bool			m_bStateGameOver;

	public:
		CNewUIHotKey();
		virtual ~CNewUIHotKey();

		bool Create(CNewUIManager* pNewUIMng);
		void Release();

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		float GetLayerDepth();
		float GetKeyEventOrder();

		bool CanUpdateKeyEvent();
		bool CanUpdateKeyEventRelatedMyInventory();

		void SetStateGameOver( bool bGameOver );
		bool IsStateGameOver( );

		bool AutoGetItem();

	private:
		void ResetMouseRButton();

	};
}

#endif