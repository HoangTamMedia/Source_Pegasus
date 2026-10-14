#pragma once
#include "zzzinfomation.h"
#include "SpinLock.h"

extern int MenuStateCurrent;
extern int MenuStateNext;
extern int  SceneFlag;
extern int  MoveSceneFrame;

extern int  ErrorMessage;
extern bool InitServerList;
extern bool InitLogIn;
extern bool InitLoading;
extern bool InitCharacterScene;
extern bool InitMainScene;
extern bool EnableMainRender;
inline SpinLock* g_render_lock = new SpinLock();
extern int g_iLengthAuthorityCode;

// ---- Pegasus loading art -------------------------------------------------
// The wallpapers are 2048x1024, which is over MAX_WIDTH, so each one ships cut
// into <=1024 tiles. LoadPegasusLoadingArt picks a set, loads its tiles into
// BITMAP_PG_LOADING_TILE + n, and publishes the grid through the getters.
#define PG_LOADING_TILE_MAX		8

#define BITMAP_PG_LOADING_TILE	(BITMAP_TITLE)
#define BITMAP_PG_LOADING_LOGO	(BITMAP_TITLE + PG_LOADING_TILE_MAX)
#define BITMAP_PG_LOADING_TRACK	(BITMAP_TITLE + PG_LOADING_TILE_MAX + 1)
#define BITMAP_PG_LOADING_FILL	(BITMAP_TITLE + PG_LOADING_TILE_MAX + 2)
#define BITMAP_PG_LOADING_DIGIT	(BITMAP_TITLE + PG_LOADING_TILE_MAX + 3)
#define BITMAP_PG_LOADING_PCT	(BITMAP_TITLE + PG_LOADING_TILE_MAX + 4)
#define BITMAP_PG_LOADING_END	(BITMAP_TITLE + PG_LOADING_TILE_MAX + 5)

void LoadPegasusLoadingArt();
void UnloadPegasusLoadingArt();
int  GetPegasusLoadingCols();
int  GetPegasusLoadingRows();

extern void LogInScene(HDC hDC);
extern void LoadingScene(HDC hDC);
extern void Scene(HDC Hdc);
extern bool CheckName();
void    StartGame();
void SetTargetFps(double targetFps);

BOOL	ShowCheckBox( int num, int index, int message=MESSAGE_TRADE_CHECK );

int	SeparateTextIntoLines( const char* lpszText, char *lpszSeparated, int iMaxLine, int iLineSize);

bool	GetTimeCheck(int DelayTime);
void	SetEffectVolumeLevel(int level);
void    SetViewPortLevel ( int level );

bool IsEnterPressed();
void SetEnterPressed( bool enterpressed );
extern float   DeltaT;
extern int FpsTime;
extern int FpsCount;
extern int AnimationFrameConstant;
