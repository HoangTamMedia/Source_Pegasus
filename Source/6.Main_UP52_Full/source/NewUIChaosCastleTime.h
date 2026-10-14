
#if !defined(AFX_NEWUICHAOSCASTLETIME_H__96004B21_205A_40BE_9B6B_3C099D8E41F6__INCLUDED_)
#define AFX_NEWUICHAOSCASTLETIME_H__96004B21_205A_40BE_9B6B_3C099D8E41F6__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "NewUIBloodCastleTime.h"

namespace SEASON3B
{
	class CNewUIChaosCastleTime : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_CHAOSCASTLE_TIME_WINDOW = CNewUIBloodCastle::IMAGE_BLOODCASTLE_TIME_WINDOW
		};

	private:
		enum BLOODCASTLE_TIME_WINDOW_SIZE
		{
			CHAOSCASTLE_TIME_WINDOW_WIDTH = 124,
			CHAOSCASTLE_TIME_WINDOW_HEIGHT = 81,
		};

		enum
		{
			CC_TIME_STATE_NORMAL = 1,
			CC_TIME_STATE_IMMINENCE,
		};

		enum
		{
			MAX_KILL_MONSTER = 65535,
		};

	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		unicode::t_char				m_szTime[256];
		int							m_iTime;
		int							m_iTimeState;
		int							m_iMaxKillMonster;
		int							m_iKilledMonster;

	public:
		CNewUIChaosCastleTime();
		virtual ~CNewUIChaosCastleTime();

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

	private:
		void LoadImages();
		void UnloadImages();

	public:
		void SetTime( int m_iTime );
		void SetKillMonsterStatue( int iKilled, int iMaxKill );

	};
};

#endif
