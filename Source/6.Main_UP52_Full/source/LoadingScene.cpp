
#include "stdafx.h"
#include "LoadingScene.h"

#include "Input.h"
#include "GlobalBitmap.h"

// Layout is authored in the 800x600 space this scene has always used; the
// sprite scale below maps it onto whatever the window actually is.
#define LDS_SCREEN_W	800
#define LDS_SCREEN_H	600

CLoadingScene::CLoadingScene()
{

}

CLoadingScene::~CLoadingScene()
{

}

void CLoadingScene::Create()
{
	CInput rInput = CInput::Instance();
	float fScaleX = (float)rInput.GetScreenWidth() / (float)LDS_SCREEN_W;
	float fScaleY = (float)rInput.GetScreenHeight() / (float)LDS_SCREEN_H;

	// Wallpaper grid. Tiles divide the 800x600 field evenly; each is stretched
	// from its own source size so a 512 and a 1024 tile both land correctly.
	int nCols = GetPegasusLoadingCols();
	int nRows = GetPegasusLoadingRows();
	int nTileW = LDS_SCREEN_W / nCols;
	int nTileH = LDS_SCREEN_H / nRows;

	for (int r = 0; r < nRows; ++r)
	{
		for (int c = 0; c < nCols; ++c)
		{
			int n = r * nCols + c;
			BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_LOADING_TILE + n);
			int nSrcW = (pTex != NULL) ? (int)pTex->Width : 1024;
			int nSrcH = (pTex != NULL) ? (int)pTex->Height : 1024;

			// Last column/row absorbs the rounding remainder so there is no seam.
			int nW = (c == nCols - 1) ? (LDS_SCREEN_W - nTileW * c) : nTileW;
			int nH = (r == nRows - 1) ? (LDS_SCREEN_H - nTileH * r) : nTileH;

			m_asprBack[n].Create(nSrcW, nSrcH, BITMAP_PG_LOADING_TILE + n, 0, NULL, 0, 0,
				false, SPR_SIZING_DATUMS_LT, fScaleX, fScaleY);
			m_asprBack[n].SetSize(nW, nH);
			m_asprBack[n].SetPosition(nTileW * c, nTileH * r);
			m_asprBack[n].Show(true);
		}
	}
}

void CLoadingScene::Release()
{
	for (int i = 0; i < LDS_MAX; ++i)
		m_asprBack[i].Release();
}

void CLoadingScene::Render()
{
	for (int i = 0; i < LDS_MAX; ++i)
	{
		m_asprBack[i].Render();
	}
}
