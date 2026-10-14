
#if !defined(AFX_LOADINGSCENE_H__D5107C47_C7D8_49B8_8056_B21DDC7DACE0__INCLUDED_)
#define AFX_LOADINGSCENE_H__D5107C47_C7D8_49B8_8056_B21DDC7DACE0__INCLUDED_

#pragma once

#include "Sprite.h"
#include "ZzzScene.h"

// Pegasus world-loading screen: the tiled wallpaper, nothing over it.
// Art is loaded by LoadPegasusLoadingArt().
#define LDS_MAX		PG_LOADING_TILE_MAX

class CLoadingScene
{
protected:
	CSprite	m_asprBack[LDS_MAX];

public:
	CLoadingScene();
	virtual ~CLoadingScene();

	void Create();
	void Release();
	void Render();
};

#endif
