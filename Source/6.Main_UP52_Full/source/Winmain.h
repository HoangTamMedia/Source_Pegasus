#ifndef __WINMAIN_H__
#define __WINMAIN_H__

#define FOR_WORK

#define LANGUAGE_KOREAN			( 0 )
#define LANGUAGE_ENGLISH		( 1 )
#define LANGUAGE_TAIWANESE		( 2 )
#define LANGUAGE_CHINESE		( 3 )
#define LANGUAGE_JAPANESE		( 4 )
#define LANGUAGE_THAILAND		( 5 )
#define LANGUAGE_PHILIPPINES	( 6 )
#define LANGUAGE_VIETNAMESE		( 7 )
#define NUM_LANGUAGE			( 8 )

#define SELECTED_LANGUAGE	( LANGUAGE_ENGLISH )

#ifdef _DEBUG
#define ENABLE_EDIT
#define ENABLE_EDIT2

#endif

#define CSK_LH_DEBUG_CONSOLE
#ifdef CSK_LH_DEBUG_CONSOLE
#define CONSOLE_DEBUG
#endif

#ifdef FOR_WORK
#ifdef _DEBUG

#define CSK_DEBUG_MAP_ATTRIBUTE

#define CSK_DEBUG_RENDER_BOUNDINGBOX

#define CSK_DEBUG_MAP_PATHFINDING

#endif
#endif

#define WINDOWMODE

#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
#define ACTIVE_FOCUS_OUT
#endif

#define DUEL_SYSTEM
#define PROTECT_SYSTEMKEY

#define WM_USER_MEMORYHACK	( WM_USER + 1)
#define WM_NPROTECT_EXIT_TWO  (WM_USER + 10001)

extern char* szServerIpAddress;
extern unsigned short g_ServerPort;
extern BYTE Version[5];
extern BYTE Serial[17];

extern bool ashies;
extern int weather;
extern float Time_Effect;
extern HWND      g_hWnd;
extern HINSTANCE g_hInst;
extern HDC       g_hDC;
extern HGLRC     g_hRC;
extern HFONT     g_hFont;
extern HFONT     g_hFontBold;
extern HFONT     g_hFontBoldName;
extern HFONT     g_hFontGachChan;
extern HFONT     g_hFontBig;
extern HFONT     g_hFixFont;
extern HFONT     g_hFontMini;
extern bool      Destroy;
extern int       RandomTable[];

extern char m_ID[];
extern char m_Pass[];
extern char m_Version[];
extern int  m_SavePassOnOff;
extern int  m_SoundOnOff;
extern int  m_MusicOnOff;
extern int  m_Resolution;
extern int m_nColorDepth;

extern void CheckHack(void);
extern DWORD GetCheckSum(WORD wKey);
extern void StopMp3(char* Name, BOOL bEnforce = false);
extern void PlayMp3(char* Name, BOOL bEnforce = false);
extern bool IsEndMp3();
extern int GetMp3PlayPosition();
extern unsigned int GenID();
extern double CPU_AVG;

extern HFONT g_hFont, g_hFontBold, g_hFontBoldName, g_hFontBig, g_hFixFont, g_hFontMini;

void CreateCustomFonts(int screenWidth, int m_FontSizePlus);

extern void CloseMainExe(void);
extern GLvoid KillGLWindow(GLvoid);
extern void DestroyWindow();
extern void DestroySound();
extern int mShowName;
extern int mShowHPBar;
extern int mShowMiniMap;
extern int mAutoHP;
extern void SetResolution();

#define FAKE_CODE( pos)\
	_asm { jmp pos };\
	_asm { __emit 0xFF };\
	_asm { __emit 0x15 }

#endif
