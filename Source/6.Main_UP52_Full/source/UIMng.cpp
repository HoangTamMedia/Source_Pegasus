
#include "stdafx.h"
#include "UIMng.h"
#include "Input.h"
#include "Sprite.h"
#include "GaugeBar.h"
#include "ZzzOpenglUtil.h"
#include "Zzzinfomation.h"
#include "ZzzBMD.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "GameCensorship.h"
#include "UIControls.h"
#include "ServerListManager.h"
#include "ZzzScene.h"

#define	DOCK_EXTENT		10

// Pegasus title screen. Art comes from LoadPegasusLoadingArt().
#define	UIM_TS_TILE			0								// wallpaper grid, PG_LOADING_TILE_MAX slots
#define	UIM_TS_TRACK		(PG_LOADING_TILE_MAX)			// progress track
#define	UIM_TS_PCT_100		(PG_LOADING_TILE_MAX + 1)		// hundreds digit
#define	UIM_TS_PCT_10		(PG_LOADING_TILE_MAX + 2)		// tens digit
#define	UIM_TS_PCT_1		(PG_LOADING_TILE_MAX + 3)		// units digit
#define	UIM_TS_PCT_SIGN		(PG_LOADING_TILE_MAX + 4)		// '%'
#define	UIM_TS_MAX			(PG_LOADING_TILE_MAX + 5)

// Authored in the 800x600 space the title scene has always used.
#define	UIM_TS_SCREEN_W		800
#define	UIM_TS_SCREEN_H		600

#define	UIM_TS_TRACK_W		800		// TournamentLoading_I13, 1024x119
#define	UIM_TS_TRACK_H		93
#define	UIM_TS_TRACK_Y		470

// The gold arrow line sits at row 68 of the 119-tall track art, i.e. 53 of the
// 93 it is drawn at. Offset the 4px fill by half its height to sit on it.
#define	UIM_TS_BAR_W		750		// TournamentLoading_I12, native width
#define	UIM_TS_BAR_H		4
#define	UIM_TS_BAR_X		25
#define	UIM_TS_BAR_Y		(UIM_TS_TRACK_Y + 53 - (UIM_TS_BAR_H / 2))

#define	UIM_TS_DIGIT_W		12		// TournamentLoading_I16 is ten 12x14 cells
#define	UIM_TS_DIGIT_H		14
#define	UIM_TS_PCT_W		19		// TournamentLoading_I15
#define	UIM_TS_PCT_Y		(UIM_TS_BAR_Y - 26)

CUIMng::CUIMng()
{
	m_asprTitle = NULL;
	m_pgbLoding = NULL;
	m_pLoadingScene = NULL;

#ifdef MOVIE_DIRECTSHOW
	m_bMoving = false;
#endif
}

CUIMng::~CUIMng()
{

}

CUIMng& CUIMng::Instance()
{
	static CUIMng s_UIMng;
	return s_UIMng;
}

void CUIMng::CreateTitleSceneUI()
{
	ReleaseTitleSceneUI();

	g_GameCensorship->SetVisible(true);
	g_GameCensorship->SetState(SEASON3A::CGameCensorship::STATE_LOADING);

	CInput& rInput = CInput::Instance();
	float fScaleX = (float)rInput.GetScreenWidth() / (float)UIM_TS_SCREEN_W;
	float fScaleY = (float)rInput.GetScreenHeight() / (float)UIM_TS_SCREEN_H;

	m_asprTitle = new CSprite[UIM_TS_MAX];

	// Wallpaper grid. Each tile is stretched from its own source size, so a mix
	// of 512 and 1024 tiles lands correctly.
	int nCols = GetPegasusLoadingCols();
	int nRows = GetPegasusLoadingRows();
	int nTileW = UIM_TS_SCREEN_W / nCols;
	int nTileH = UIM_TS_SCREEN_H / nRows;

	for (int r = 0; r < nRows; ++r)
	{
		for (int c = 0; c < nCols; ++c)
		{
			int n = r * nCols + c;
			BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_LOADING_TILE + n);
			int nSrcW = (pTex != NULL) ? (int)pTex->Width : 1024;
			int nSrcH = (pTex != NULL) ? (int)pTex->Height : 1024;

			// Last column/row absorbs the rounding remainder so there is no seam.
			int nW = (c == nCols - 1) ? (UIM_TS_SCREEN_W - nTileW * c) : nTileW;
			int nH = (r == nRows - 1) ? (UIM_TS_SCREEN_H - nTileH * r) : nTileH;

			m_asprTitle[UIM_TS_TILE + n].Create(nSrcW, nSrcH, BITMAP_PG_LOADING_TILE + n,
				0, NULL, 0, 0, false, SPR_SIZING_DATUMS_LT, fScaleX, fScaleY);
			m_asprTitle[UIM_TS_TILE + n].SetSize(nW, nH);
			m_asprTitle[UIM_TS_TILE + n].SetPosition(nTileW * c, nTileH * r);
		}
	}

	m_asprTitle[UIM_TS_TRACK].Create(1024, 119, BITMAP_PG_LOADING_TRACK, 0, NULL, 0, 0,
		false, SPR_SIZING_DATUMS_LT, fScaleX, fScaleY);
	m_asprTitle[UIM_TS_TRACK].SetSize(UIM_TS_TRACK_W, UIM_TS_TRACK_H);
	m_asprTitle[UIM_TS_TRACK].SetPosition(0, UIM_TS_TRACK_Y);

	// Percentage readout. The digit strip is ten cells side by side, so each
	// sprite is one cell that gets pointed at the right frame per redraw.
	SFrameCoord aDigitFrame[10];
	for (int i = 0; i < 10; ++i)
	{
		aDigitFrame[i].nX = i * UIM_TS_DIGIT_W;
		aDigitFrame[i].nY = 0;
	}

	for (int i = UIM_TS_PCT_100; i <= UIM_TS_PCT_1; ++i)
	{
		m_asprTitle[i].Create(UIM_TS_DIGIT_W, UIM_TS_DIGIT_H, BITMAP_PG_LOADING_DIGIT,
			10, aDigitFrame, 0, 0, false, SPR_SIZING_DATUMS_LT, fScaleX, fScaleY);
		m_asprTitle[i].SetAction(0, 9, 0.0, false);
	}

	m_asprTitle[UIM_TS_PCT_SIGN].Create(UIM_TS_PCT_W, UIM_TS_DIGIT_H, BITMAP_PG_LOADING_PCT,
		0, NULL, 0, 0, false, SPR_SIZING_DATUMS_LT, fScaleX, fScaleY);

	// Progress fill: tiled sprite, so shrinking it reveals the gradient
	// left to right instead of squashing it.
	m_pgbLoding = new CGaugeBar;

	RECT rc = { 0, 0, UIM_TS_BAR_W, UIM_TS_BAR_H };
	m_pgbLoding->Create(UIM_TS_BAR_W, UIM_TS_BAR_H, BITMAP_PG_LOADING_FILL, &rc, 0, 0, -1,
		true, fScaleX, fScaleY);
	m_pgbLoding->SetPosition(UIM_TS_BAR_X, UIM_TS_BAR_Y);

	for (int i = 0; i < UIM_TS_MAX; ++i)
	{
		m_asprTitle[i].Show();
	}
	m_pgbLoding->Show();
	m_nScene = UIM_SCENE_TITLE;
}

void CUIMng::ReleaseTitleSceneUI()
{
	g_GameCensorship->SetVisible(false);

	SAFE_DELETE_ARRAY(m_asprTitle);
	SAFE_DELETE(m_pgbLoding);

	m_nScene = UIM_SCENE_NONE;
}

void CUIMng::RenderTitleSceneUI(HDC hDC, DWORD dwNow, DWORD dwTotal)
{
	::BeginOpengl();
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::BeginBitmap();

	int nTileCount = GetPegasusLoadingCols() * GetPegasusLoadingRows();
	for (int i = 0; i < nTileCount; ++i)
		m_asprTitle[UIM_TS_TILE + i].Render();

	m_asprTitle[UIM_TS_TRACK].Render();

	m_pgbLoding->SetValue(dwNow, dwTotal);
	m_pgbLoding->Render();

	// Lay the percentage out fresh each step - it grows from 1 to 3 glyphs.
	int nPercent = (dwTotal > 0) ? (int)(dwNow * 100 / dwTotal) : 0;
	if (nPercent > 100) nPercent = 100;

	int anDigit[3];
	int nDigitCount = 0;
	if (nPercent >= 100)	anDigit[nDigitCount++] = (nPercent / 100) % 10;
	if (nPercent >= 10)		anDigit[nDigitCount++] = (nPercent / 10) % 10;
	anDigit[nDigitCount++] = nPercent % 10;

	int nTextW = nDigitCount * UIM_TS_DIGIT_W + UIM_TS_PCT_W;
	int nTextX = (UIM_TS_SCREEN_W - nTextW) / 2;

	for (int i = 0; i < 3; ++i)
	{
		CSprite& rDigit = m_asprTitle[UIM_TS_PCT_100 + i];
		if (i >= nDigitCount)
		{
			rDigit.Show(false);
			continue;
		}

		rDigit.Show(true);
		rDigit.SetNowFrame(anDigit[i]);
		rDigit.SetPosition(nTextX + i * UIM_TS_DIGIT_W, UIM_TS_PCT_Y);
		rDigit.Render();
	}

	m_asprTitle[UIM_TS_PCT_SIGN].SetPosition(nTextX + nDigitCount * UIM_TS_DIGIT_W, UIM_TS_PCT_Y);
	m_asprTitle[UIM_TS_PCT_SIGN].Render();

	::EndBitmap();
	::EndOpengl();
	::glFlush();
	::SwapBuffers(hDC);
}

void CUIMng::Create()
{
	m_bCursorOnUI = false;
	m_bBlockCharMove = false;
	m_bWinActive = false;
	m_nScene = UIM_SCENE_NONE;

	return;
}

void CUIMng::RemoveWinList()
{
	CWin* pWin;
	while (m_WinList.GetCount())
	{
		pWin = (CWin*)m_WinList.RemoveHead();
		pWin->Release();
	}
}

void CUIMng::Release()
{
	RemoveWinList();

	m_CharInfoBalloonMng.Release();

	m_nScene = UIM_SCENE_NONE;
}

void CUIMng::CreateLoginScene()
{
	g_GameCensorship->SetVisible(false);

	RemoveWinList();

	m_CharInfoBalloonMng.Release();

	CInput& rInput = CInput::Instance();

	m_MsgWin.Create();
	m_WinList.AddHead(&m_MsgWin);
	m_MsgWin.SetPosition((rInput.GetScreenWidth() - 352) / 2,
		(rInput.GetScreenHeight() - 113) / 2);

	m_SysMenuWin.Create();
	m_WinList.AddHead(&m_SysMenuWin);

	m_OptionWin.Create();
	m_WinList.AddHead(&m_OptionWin);

	m_LoginMainWin.Create();
	m_WinList.AddHead(&m_LoginMainWin);

	int nBaseY = int(567.0f / 600.0f * (float)rInput.GetScreenHeight());
	m_LoginMainWin.SetPosition(30, nBaseY - m_LoginMainWin.GetHeight() - 11);

	m_ServerSelWin.Create();
	m_WinList.AddHead(&m_ServerSelWin);
	m_ServerSelWin.SetPosition(
		(rInput.GetScreenWidth() - m_ServerSelWin.GetWidth()) / 2,
		(rInput.GetScreenHeight() - m_ServerSelWin.GetHeight()) / 2);

	m_LoginWin.Create();
	m_WinList.AddHead(&m_LoginWin);
	m_LoginWin.SetPosition(
		(rInput.GetScreenWidth() - m_LoginWin.GetWidth()) / 2,
		(rInput.GetScreenHeight() - m_LoginWin.GetHeight()) * 2 / 3);

	m_CreditWin.Create();
	m_WinList.AddHead(&m_CreditWin);

	m_bSysMenuWinShow = false;
	m_nScene = UIM_SCENE_LOGIN;
}

void CUIMng::CreateCharacterScene()
{

	g_GameCensorship->SetState(g_ServerListManager->GetCensorshipIndex());

	RemoveWinList();

	m_CharInfoBalloonMng.Create();

	CInput& rInput = CInput::Instance();

	m_MsgWin.Create();
	m_WinList.AddHead(&m_MsgWin);
	m_MsgWin.SetPosition((rInput.GetScreenWidth() - 352) / 2,
		(rInput.GetScreenHeight() - 113) / 2);

	m_ServerMsgWin.Create();
	m_WinList.AddHead(&m_ServerMsgWin);
	int nBaseY = int(31.0f / 600.0f * (float)rInput.GetScreenHeight());
	m_ServerMsgWin.SetPosition(10, nBaseY + 10);

	m_SysMenuWin.Create();
	m_WinList.AddHead(&m_SysMenuWin);

	m_OptionWin.Create();
	m_WinList.AddHead(&m_OptionWin);

	m_CharSelMainWin.Create();
	m_WinList.AddHead(&m_CharSelMainWin);
	nBaseY = int(567.0f / 600.0f * (float)rInput.GetScreenHeight());
	m_CharSelMainWin.SetPosition(22, nBaseY - m_CharSelMainWin.GetHeight() - 11);

	m_CharMakeWin.Create();
	m_WinList.AddHead(&m_CharMakeWin);

	m_CharMakeWin.SetPosition((rInput.GetScreenWidth() - 454) / 2,
		(rInput.GetScreenHeight() - 406) / 2);

	m_CharSelMainWin.UpdateDisplay();
	m_CharInfoBalloonMng.UpdateDisplay();

	ShowWin(&m_CharSelMainWin);

	m_bSysMenuWinShow = false;
	m_nScene = UIM_SCENE_CHARACTER;
}

void CUIMng::CreateMainScene()
{
	RemoveWinList();

	m_CharInfoBalloonMng.Release();

	m_nScene = UIM_SCENE_MAIN;
}

CWin* CUIMng::SetActiveWin(CWin* pWin)
{
	CWin* pBeforeActWin = (CWin*)m_WinList.GetHead();

	if (pBeforeActWin == NULL)
		return NULL;

	if (pBeforeActWin->IsActive())
		pBeforeActWin->Active(FALSE);
	else
		pBeforeActWin = NULL;

	if (pWin->IsShow())
	{
		if (!m_WinList.RemoveAt(m_WinList.Find(pWin)))
			return NULL;

		m_bWinActive = true;
		m_WinList.AddHead(pWin);
	}

	return pBeforeActWin;
}

void CUIMng::ShowWin(CWin* pWin)
{
	pWin->Show(TRUE);
	SetActiveWin(pWin);
}

void CUIMng::HideWin(CWin* pWin)
{
	if (!m_WinList.RemoveAt(m_WinList.Find(pWin)))
		return;

	pWin->Show(FALSE);
	pWin->Active(FALSE);
	m_WinList.AddTail(pWin);

	pWin = (CWin*)m_WinList.GetHead();
	if (pWin->IsShow())
		m_bWinActive = true;
}

void CUIMng::CheckDockWin()
{
	NODE* position = m_WinList.GetHeadPosition();
	if (NULL == position)
		return;

	CWin* pMovWin = (CWin*)m_WinList.GetNext(position);

	if (pMovWin->GetState() != WS_MOVE)
		return;

	pMovWin->SetDocking(false);

	RECT rcMovWin = { pMovWin->GetTempXPos(), pMovWin->GetTempYPos(),
		pMovWin->GetTempXPos() + pMovWin->GetWidth(),
		pMovWin->GetTempYPos() + pMovWin->GetHeight() };

	RECT rcDock[4] =
	{
		{ rcMovWin.left - DOCK_EXTENT, rcMovWin.top - DOCK_EXTENT,
			rcMovWin.left + DOCK_EXTENT, rcMovWin.top + DOCK_EXTENT },
		{ rcMovWin.right - DOCK_EXTENT, rcMovWin.top - DOCK_EXTENT,
			rcMovWin.right + DOCK_EXTENT, rcMovWin.top + DOCK_EXTENT },
		{ rcMovWin.left - DOCK_EXTENT, rcMovWin.bottom - DOCK_EXTENT,
			rcMovWin.left + DOCK_EXTENT, rcMovWin.bottom + DOCK_EXTENT },
		{ rcMovWin.right - DOCK_EXTENT, rcMovWin.bottom - DOCK_EXTENT,
			rcMovWin.right + DOCK_EXTENT, rcMovWin.bottom + DOCK_EXTENT }
	};

	CInput& rInput = CInput::Instance();

	POINT pt[4] = { { 0, 0 }, { rInput.GetScreenWidth(), 0 },
		{ 0, rInput.GetScreenHeight() },
		{ rInput.GetScreenWidth(), rInput.GetScreenHeight() } };

	if (::PtInRect(&rcDock[0], pt[0]))
	{
		pMovWin->SetPosition(pt[0].x, pt[0].y);
		pMovWin->SetDocking(true);
	}
	else if (::PtInRect(&rcDock[1], pt[1]))
	{
		pMovWin->SetPosition(pt[1].x - pMovWin->GetWidth(), pt[1].y);
		pMovWin->SetDocking(true);
	}
	else if (::PtInRect(&rcDock[2], pt[2]))
	{
		pMovWin->SetPosition(pt[2].x, pt[2].y - pMovWin->GetHeight());
		pMovWin->SetDocking(true);
	}
	else if (::PtInRect(&rcDock[3], pt[3]))
	{
		pMovWin->SetPosition(pt[3].x - pMovWin->GetWidth(),
			pt[3].y - pMovWin->GetHeight());
		pMovWin->SetDocking(true);
	}
	else if (rcDock[0].top < 0 && rcDock[0].bottom > 0)
	{
		pMovWin->SetPosition(rcMovWin.left, 0);
		pMovWin->SetDocking(true);
	}
	else if (rcDock[2].top < pt[2].y && rcDock[2].bottom > pt[2].y)
	{
		pMovWin->SetPosition(rcMovWin.left, pt[2].y - pMovWin->GetHeight());
		pMovWin->SetDocking(true);
	}
	else if (rcDock[0].left < 0 && rcDock[0].right > 0)
	{
		pMovWin->SetPosition(0, rcMovWin.top);
		pMovWin->SetDocking(true);
	}
	else if (rcDock[1].left < pt[1].x && rcDock[1].right > pt[1].x)
	{
		pMovWin->SetPosition(pt[1].x - pMovWin->GetWidth(), rcMovWin.top);
		pMovWin->SetDocking(true);
	}

	BOOL bEdgeDocking = FALSE;
	int i, j, nXCoord, nYCoord;
	CWin* pWin;

	while (position)
	{
		pWin = (CWin*)m_WinList.GetNext(position);
		if (!pWin->IsShow())
			continue;

		pt[0].x = pWin->GetXPos();
		pt[0].y = pWin->GetYPos();
		pt[1].x = pWin->GetXPos() + pWin->GetWidth();
		pt[1].y = pt[0].y;
		pt[2].x = pt[0].x;
		pt[2].y = pWin->GetYPos() + pWin->GetHeight();
		pt[3].x = pt[1].x;
		pt[3].y = pt[2].y;

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (i != j && ::PtInRect(&rcDock[i], pt[j]))
				{
					bEdgeDocking = TRUE;
					goto DOCKING;
				}
			}
		}

		if (pt[0].x < rcDock[1].left && pt[1].x > rcDock[0].right)
		{
			nXCoord = rcMovWin.left;
			if (pt[2].y > rcDock[0].top && pt[2].y < rcDock[0].bottom)
			{
				if (SetDockWinPosition(pMovWin, nXCoord, pt[2].y))
					continue;
			}
			else if (pt[0].y > rcDock[2].top && pt[0].y < rcDock[2].bottom)
			{
				if (SetDockWinPosition(pMovWin,
					nXCoord, pt[0].y - pMovWin->GetHeight()))
					continue;
			}
		}
		else if (pt[0].y < rcDock[2].top && pt[2].y > rcDock[0].bottom)
		{
			nYCoord = rcMovWin.top;
			if (pt[1].x > rcDock[0].left && pt[1].x < rcDock[0].right)
			{
				if (SetDockWinPosition(pMovWin, pt[1].x, nYCoord))
					continue;
			}
			else if (pt[0].x > rcDock[1].left && pt[0].x < rcDock[1].right)
			{
				if (SetDockWinPosition(pMovWin,
					pt[0].x - pMovWin->GetWidth(), nYCoord))
					continue;
			}
		}
	}

DOCKING:
	if (bEdgeDocking)
	{
		switch (j)
		{
		case 0:
			switch (i)
			{
			case 1:
				nXCoord = pWin->GetXPos() - pMovWin->GetWidth();
				nYCoord = pWin->GetYPos();
				break;
			case 2:
				nXCoord = pWin->GetXPos();
				nYCoord = pWin->GetYPos() - pMovWin->GetHeight();
				break;
			case 3:
				nXCoord = pWin->GetXPos() - pMovWin->GetWidth();
				nYCoord = pWin->GetYPos() - pMovWin->GetHeight();
			}
			break;

		case 1:
			switch (i)
			{
			case 0:
				nXCoord = pWin->GetXPos() + pWin->GetWidth();
				nYCoord = pWin->GetYPos();
				break;
			case 2:
				nXCoord = pWin->GetXPos() + pWin->GetWidth();
				nYCoord = pWin->GetYPos() - pMovWin->GetHeight();
				break;
			case 3:
				nXCoord = pWin->GetXPos() + pWin->GetWidth()
					- pMovWin->GetWidth();
				nYCoord = pWin->GetYPos() - pMovWin->GetHeight();
			}
			break;

		case 2:
			switch (i)
			{
			case 0:
				nXCoord = pWin->GetXPos();
				nYCoord = pWin->GetYPos() + pWin->GetHeight();
				break;
			case 1:
				nXCoord = pWin->GetXPos() - pMovWin->GetWidth();
				nYCoord = pWin->GetYPos() + pWin->GetHeight();
				break;
			case 3:
				nXCoord = pWin->GetXPos() - pMovWin->GetWidth();
				nYCoord = pWin->GetYPos() + pWin->GetHeight()
					- pMovWin->GetHeight();
			}
			break;

		case 3:
			switch (i)
			{
			case 0:
				nXCoord = pWin->GetXPos() + pWin->GetWidth();
				nYCoord = pWin->GetYPos() + pWin->GetHeight();
				break;
			case 1:
				nXCoord = pWin->GetXPos() + pWin->GetWidth()
					- pMovWin->GetWidth();
				nYCoord = pWin->GetYPos() + pWin->GetHeight();
				break;
			case 2:
				nXCoord = pWin->GetXPos() + pWin->GetWidth();
				nYCoord = pWin->GetYPos() + pWin->GetHeight()
					- pMovWin->GetHeight();
			}
		}
		SetDockWinPosition(pMovWin, nXCoord, nYCoord);
	}
}

bool CUIMng::SetDockWinPosition(CWin* pMoveWin, int nDockX, int nDockY)
{
	CInput& rInput = CInput::Instance();
	RECT rcDummy;
	RECT rcScreen = { 0, 0, rInput.GetScreenWidth(), rInput.GetScreenHeight() };
	RECT rcMoveWin = { nDockX, nDockY,
		nDockX + pMoveWin->GetWidth(), nDockY + pMoveWin->GetHeight() };

	if (::IntersectRect(&rcDummy, &rcScreen, &rcMoveWin))
	{
		pMoveWin->SetPosition(nDockX, nDockY);
		pMoveWin->SetDocking(true);
		return true;
	}

	return false;
}

void CUIMng::Update(double dDeltaTick)
{
	if (UIM_SCENE_NONE == m_nScene || m_WinList.IsEmpty())
		return;

	if (m_bWinActive)
	{
		CWin* pWin = (CWin*)m_WinList.GetHead();
		if (pWin->IsShow())
		{
			pWin->Active(true);
			m_bWinActive = false;
		}
	}

	CInput& rInput = CInput::Instance();
	if (rInput.IsKeyDown(VK_ESCAPE))
	{
		m_bSysMenuWinShow = !m_bSysMenuWinShow;
	}

	CWin* pWin;
	NODE* position;

	m_bCursorOnUI = false;

	if (rInput.IsLBtnDn())
	{
		bool bWinClick = false;
		position = m_WinList.GetHeadPosition();
		while (position)
		{
			pWin = (CWin*)m_WinList.GetNext(position);

			if (pWin->CursorInWin(WA_ALL))
			{
				SetActiveWin(pWin);
				bWinClick = true;
				break;
			}
		}

		if (!bWinClick)
		{
			pWin = (CWin*)m_WinList.GetHead();
			pWin->Active(false);
		}
	}
	else if (rInput.IsLBtnUp())
	{
		m_bBlockCharMove = false;
	}
	int nlist = m_WinList.GetCount();
	CWin** apTempWin = new (CWin * [nlist]);

	position = m_WinList.GetHeadPosition();
	for (int i = 0; i < nlist; ++i)
	{
		apTempWin[i] = (CWin*)m_WinList.GetNext(position);
		apTempWin[i]->ActiveBtns(false);
	}

	position = m_WinList.GetHeadPosition();
	while (position)
	{
		pWin = (CWin*)m_WinList.GetNext(position);
		if (pWin->CursorInWin(WA_ALL))
		{
			pWin->ActiveBtns(true);
			break;
		}
	}

	for (int i = 0; i < nlist; ++i)
	{
		apTempWin[i]->Update(dDeltaTick);
	}

	SAFE_DELETE_ARRAY(apTempWin);

	CheckDockWin();

	position = m_WinList.GetHeadPosition();
	while (position)
	{
		pWin = (CWin*)m_WinList.GetNext(position);

		switch (pWin->GetState())
		{
		case WS_ETC:
			m_bCursorOnUI = true;
			break;

		case WS_MOVE:

			m_bCursorOnUI = true;
			break;

		case WS_EXTEND_UP:

			m_bCursorOnUI = true;
			break;

		case WS_EXTEND_DN:

			m_bCursorOnUI = true;
			break;
		}

		if (m_bCursorOnUI)
			break;

		if (pWin->CursorInWin(WA_ALL))
		{
			m_bCursorOnUI = true;
			break;
		}
	}
}

void CUIMng::Render()
{
	if (UIM_SCENE_NONE == m_nScene)
		return;

	m_CharInfoBalloonMng.Render();

	CWin* pWin;
	NODE* position = m_WinList.GetTailPosition();
	while (position)
	{
		pWin = (CWin*)m_WinList.GetPrev(position);
		pWin->Render();
	}
}

void CUIMng::PopUpMsgWin(int nMsgCode, char* pszMsg)
{
	if (UIM_SCENE_NONE == m_nScene || UIM_SCENE_TITLE == m_nScene || UIM_SCENE_LOADING == m_nScene)
		return;

	if (UIM_SCENE_MAIN == m_nScene)	return;

	m_MsgWin.PopUp(nMsgCode, pszMsg);
}

void CUIMng::AddServerMsg(char* pszMsg)
{
	if (UIM_SCENE_CHARACTER != m_nScene)
		return;

	m_ServerMsgWin.AddMsg(pszMsg);
}
