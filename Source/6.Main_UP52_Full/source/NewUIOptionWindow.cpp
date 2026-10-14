#include "stdafx.h"
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "DSPlaySound.h"
#include "ZzzInterface.h"
#include "CBInterface.h"
#include "Protocol.h"
#include "Input.h"
#include <Util.h>
#include "Utilities/Log/muConsoleDebug.h"
#ifdef CSK_LH_DEBUG_CONSOLE
#include "Utilities/Log/WindowsConsole.h"
#endif 
using namespace SEASON3B;


SEASON3B::CNewUIOptionWindow::CNewUIOptionWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bAutoAttack = true;
	m_bWhisperSound = false;
	m_bSlideHelp = true;
#ifdef CSK_LH_DEBUG_CONSOLE
	m_bConsoleVisible = false;
	if(leaf::GetConsoleWndHandle() != NULL)
	{
		leaf::ShowConsole(false);
	}
#else
	m_bConsoleVisible = false;
#endif 
	m_iVolumeLevel = 0;
	m_iRenderLevel = 4;
	for (int i = 0; i < eEndOnOffGrap; i++) this->OnOffGrap[i] = true;

	//==Sound Settings
	m_bSoundEnabled = GetPrivateProfileInt("Sound", "SoundEnabled", 1, ".\\config.ini") != 0;
	m_iSoundVolumeLevel = GetPrivateProfileInt("Sound", "SoundVolumeLevel", 10, ".\\config.ini");
	if (m_iSoundVolumeLevel < 0) m_iSoundVolumeLevel = 0;
	if (m_iSoundVolumeLevel > 10) m_iSoundVolumeLevel = 10;
	
	//==Effect Sound Settings
	m_bEffectSoundEnabled = GetPrivateProfileInt("Sound", "EffectSoundEnabled", 1, ".\\config.ini") != 0;
	m_iVolumeLevel = GetPrivateProfileInt("Sound", "EffectVolumeLevel", 10, ".\\config.ini");
	if (m_iVolumeLevel < 0) m_iVolumeLevel = 0;
	if (m_iVolumeLevel > 10) m_iVolumeLevel = 10;

	//==Graphics

	this->OnOffGrap[eLowSettings] = GetPrivateProfileInt("Graphics", "LowSettings", 0, ".\\config.ini");
	this->OnOffGrap[eMediumSettings] = GetPrivateProfileInt("Graphics", "MediumSettings", 0, ".\\config.ini");
	this->OnOffGrap[eHighSettings] = GetPrivateProfileInt("Graphics", "HighSettings", 1, ".\\config.ini");
	this->OnOffGrap[eGlowEffect] = GetPrivateProfileInt("Graphics", "GlowEffect", 1, ".\\config.ini");
	this->OnOffGrap[eEffectDynamic] = GetPrivateProfileInt("Graphics", "EffectDynamic", 1, ".\\config.ini");
	this->OnOffGrap[eEffectStatic] = GetPrivateProfileInt("Graphics", "EffectStatic", 1, ".\\config.ini");
	this->OnOffGrap[eBMDPlayer] = GetPrivateProfileInt("Graphics", "BMDPlayer",1, ".\\config.ini");
	this->OnOffGrap[eBMDWings] = GetPrivateProfileInt("Graphics", "BMDWings", 1, ".\\config.ini");
	this->OnOffGrap[eBMDWeapons] = GetPrivateProfileInt("Graphics", "BMDWeapons", 1, ".\\config.ini");
	this->OnOffGrap[eBMDImg] = GetPrivateProfileInt("Graphics", "BMDImg", 1 ,".\\config.ini");
	this->OnOffGrap[eBMDMonter] = GetPrivateProfileInt("Graphics", "BMDMonter", 1, ".\\config.ini");
	this->OnOffGrap[eRenderObjects] = GetPrivateProfileInt("Graphics", "RenderObjects", 1, ".\\config.ini");
	this->OnOffGrap[eRenderTerrain] = GetPrivateProfileInt("Graphics", "RenderTerrain", 1, ".\\config.ini");
	this->OnOffGrap[eExcellentEffect] = GetPrivateProfileInt("Graphics", "ExcellentEffect", 1, ".\\config.ini");
	this->OnOffGrap[eBMDZen] = GetPrivateProfileInt("Graphics", "BMDZen", 1, ".\\config.ini");
}

SEASON3B::CNewUIOptionWindow::~CNewUIOptionWindow()
{
	Release();
}

bool SEASON3B::CNewUIOptionWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_OPTION, this);
	SetPos(x, y);
	LoadImages();
	SetButtonInfo();
	Show(false);
	return true;
}

void SEASON3B::CNewUIOptionWindow::SetButtonInfo()
{
	//m_BtnClose.ChangeTextBackColor(RGBA(255,255,255,0));
	//m_BtnClose.ChangeButtonImgState( true, IMAGE_OPTION_BTN_CLOSE, true );
	//m_BtnClose.ChangeButtonInfo(m_Pos.x+68, m_Pos.y+209, 54, 30);
	//m_BtnClose.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	//m_BtnClose.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
}

void SEASON3B::CNewUIOptionWindow::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}


void SEASON3B::CNewUIOptionWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIOptionWindow::UpdateMouseEvent()
{
	// Verifica se m_Pos é válido (não é NaN)
	if(m_Pos.x != m_Pos.x || m_Pos.y != m_Pos.y) // NaN check: NaN != NaN é sempre true
	{
		// m_Pos é inválido, não processa eventos de mouse
		if(g_ConsoleDebug)
		{
			static int invalidPosCounter = 0;
			if(invalidPosCounter++ % 60 == 0)
			{
				g_ConsoleDebug->Write(MCD_ERROR, "[DEBUG UpdateMouseEvent] ERRO: m_Pos inválido (NaN)! PosX=%.1f, PosY=%.1f", 
					m_Pos.x, m_Pos.y);
			}
		}
		return true;
	}
	
	// Garante que m_Pos seja válido (pode estar NaN se RenderFrame ainda não foi chamado)
	extern int DisplayHeight;
	extern int MAX_WIN_WIDTH;
	float MainWidth = 270;  // Aumentado de 220 para 280
	float MainHeight = 560;
	float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2);
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 120;
	
	// Se m_Pos for válido, usa ele; senão, usa os valores calculados
	float posX = (m_Pos.x == m_Pos.x && m_Pos.x > 0) ? m_Pos.x : StartX; // NaN check e valor positivo
	float posY = (m_Pos.y == m_Pos.y && m_Pos.y > 0) ? m_Pos.y : StartY; // NaN check e valor positivo
	
	// Se posY ainda estiver negativo ou inválido, força um valor mínimo
	if(posY < 0 || posY != posY)
	{
		posY = 50.0f; // Valor mínimo seguro
	}
	if(posX < 0 || posX != posX)
	{
		posX = StartX;
	}
	
	// Verificar se o bot?o de fechar da janela custom foi clicado
	// O bot?o de fechar ? desenhado pela gDrawWindowCustom na posi??o: X + Width - 33
	// MainWidth j? foi declarado acima, reutilizar
	int CloseX = (int)(m_Pos.x + MainWidth - 33.0f);
	int CloseY = (int)m_Pos.y;
	if (SEASON3B::CheckMouseIn(CloseX, CloseY, 36, 36) == 1)
	{
		// Processar clique do bot?o de fechar (mesma l?gica do gDrawWindowCustom)
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && GetTickCount() - gInterface.Data[eMenu_OPTION].EventTick > 300)
		{
			// Verificar se n?o h? bloqueios
			if (!g_pBCustomMenuInfo->BLockButtonHover && !g_pBCustomMenuInfo->BLockMessageBox && !g_pBCustomMenuInfo->BLockButtonMoveWindow)
			{
				PlayBuffer(SOUND_CLICK01);
				gInterface.Data[eMenu_OPTION].Close();
				g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
				return false;
			}
		}
	}

	/*if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x+150, m_Pos.y+43, 15, 15))
	{
		m_bAutoAttack = !m_bAutoAttack;
	}*/
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x+150, m_Pos.y+65, 15, 15))
	{
		m_bWhisperSound = !m_bWhisperSound;
	}
	/*if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x+150, m_Pos.y+127, 15, 15))
	{
		m_bSlideHelp = !m_bSlideHelp;
	}*/
#ifdef CSK_LH_DEBUG_CONSOLE
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x+150, m_Pos.y+519, 15, 15))
	{
		m_bConsoleVisible = !m_bConsoleVisible;
		if(leaf::GetConsoleWndHandle() != NULL)
		{
			leaf::ShowConsole(m_bConsoleVisible);
		}
		// N?o salvar no config.ini - sempre iniciar como false
	}
#endif
	

	// Botões simples + e - para Volume de Som
	// Em RenderCustomFrame: y inicia em m_Pos.y + 35, depois:
	// +45 (Opções de Renderização) +17 +17 +17 (3 linhas checkboxes) +50 (Combate) +17 +50 (Resolução) +120 +35 (Som) +5 = 408
	float soundBtnY = m_Pos.y + 35.0f + 45.0f + 17.0f + 17.0f + 17.0f + 50.0f + 17.0f + 50.0f + 120.0f + 35.0f + 5.0f;
	
	// Botão - (menos) para Volume de Som - à esquerda da barra (movido mais à direita)
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 40, soundBtnY, 20, 16))
	{
		if(m_iSoundVolumeLevel > 0)
		{
			m_iSoundVolumeLevel--;
			char string[10];
			WritePrivateProfileStringA("Sound", "SoundVolumeLevel", itoa(m_iSoundVolumeLevel, string, 10), "./config.ini");
			extern void SetMasterVolume(long vol);
			long volume = (long)(-10000L + (m_iSoundVolumeLevel * 1000L));
			SetMasterVolume(volume);
			PlayBuffer(SOUND_CLICK01);
		}
	}
	
	// Botão + (mais) para Volume de Som - à direita da barra (movido mais à direita)
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 175, soundBtnY, 20, 16))
	{
		if(m_iSoundVolumeLevel < 10)
		{
			m_iSoundVolumeLevel++;
			char string[10];
			WritePrivateProfileStringA("Sound", "SoundVolumeLevel", itoa(m_iSoundVolumeLevel, string, 10), "./config.ini");
			extern void SetMasterVolume(long vol);
			long volume = (long)(-10000L + (m_iSoundVolumeLevel * 1000L));
			SetMasterVolume(volume);
			PlayBuffer(SOUND_CLICK01);
		}
	}
	
	// Botões + e - para Volume de Efeito
	// effectVolumeBarY = soundVolumeBarY + 10 (depois da barra) + 35 (checkbox) + 5 = soundBtnY + 50
	float effectBtnY = soundBtnY + 50.0f;
	
	// Botão - (menos) para Volume de Efeito - à esquerda da barra (movido mais à direita)
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 32, effectBtnY, 20, 29))
	{
		if(m_iVolumeLevel > 0)
		{
			m_iVolumeLevel--;
			char string[10];
			WritePrivateProfileStringA("Sound", "EffectVolumeLevel", itoa(m_iVolumeLevel, string, 10), "./config.ini");
			extern void SetEffectVolumeLevel(int level);
			SetEffectVolumeLevel(m_bEffectSoundEnabled ? m_iVolumeLevel : 0);
			PlayBuffer(SOUND_CLICK01);
		}
	}
	
	// Botão + (mais) para Volume de Efeito - à direita da barra (movido mais à direita)
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 185, effectBtnY, 20, 29))
	{
		if(m_iVolumeLevel < 10)
		{
			m_iVolumeLevel++;
			char string[10];
			WritePrivateProfileStringA("Sound", "EffectVolumeLevel", itoa(m_iVolumeLevel, string, 10), "./config.ini");
			extern void SetEffectVolumeLevel(int level);
			SetEffectVolumeLevel(m_bEffectSoundEnabled ? m_iVolumeLevel : 0);
			PlayBuffer(SOUND_CLICK01);
		}
	}
	
	if(CheckMouseIn(m_Pos.x+25, m_Pos.y+168, 141, 29))
	{
		if(SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 25);
			float fValue = (5.f * x) / 141.f;
			m_iRenderLevel = (int)fValue;
		}
	}

	// Verificar se o mouse est? dentro da ?rea da janela (usar MainWidth atualizado)
	float MainWidthCheck = 270;
	if(CheckMouseIn(m_Pos.x, m_Pos.y, MainWidthCheck, 560) == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIOptionWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderContents();
	RenderButtons();
	RenderCustomFrame();
	DisableAlphaBlend();
	return true;
}

float SEASON3B::CNewUIOptionWindow::GetLayerDepth()
{
	return 10.5f;
}

float SEASON3B::CNewUIOptionWindow::GetKeyEventOrder()
{
	return 10.0f;
}

void SEASON3B::CNewUIOptionWindow::OpenningProcess()
{
	gInterface.Data[eMenu_OPTION].Open();
	
#ifdef CSK_LH_DEBUG_CONSOLE
	// Sempre inicializar como false, n?o ler do config.ini
	m_bConsoleVisible = false;
#endif

	extern void SetEnableSound(bool b);
	extern void SetMasterVolume(long vol);
	extern void SetEffectVolumeLevel(int level);
	SetEnableSound(m_bSoundEnabled);
	long volume = (long)(-10000L + (m_iSoundVolumeLevel * 1000L));
	SetMasterVolume(volume);
	SetEffectVolumeLevel(m_bEffectSoundEnabled ? m_iVolumeLevel : 0);
}

void SEASON3B::CNewUIOptionWindow::ClosingProcess()
{
	gInterface.Data[eMenu_OPTION].Close();
}

void SEASON3B::CNewUIOptionWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_button_close.tga", IMAGE_OPTION_BTN_CLOSE, GL_LINEAR);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_OPTION_FRAME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_OPTION_FRAME_DOWN, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_top.tga", IMAGE_OPTION_FRAME_UP, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_back06(L).tga", IMAGE_OPTION_FRAME_LEFT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_back06(R).tga", IMAGE_OPTION_FRAME_RIGHT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_line.jpg", IMAGE_OPTION_LINE, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_point.tga", IMAGE_OPTION_POINT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_check.tga", IMAGE_OPTION_BTN_CHECK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_effect03.tga", IMAGE_OPTION_EFFECT_BACK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_effect04.tga", IMAGE_OPTION_EFFECT_COLOR, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_VOLUME_BACK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_VOLUME_COLOR, GL_LINEAR);	
}

void SEASON3B::CNewUIOptionWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_OPTION_BTN_CLOSE);
	DeleteBitmap(IMAGE_OPTION_FRAME_BACK);
	DeleteBitmap(IMAGE_OPTION_FRAME_DOWN);
	DeleteBitmap(IMAGE_OPTION_FRAME_UP);
	DeleteBitmap(IMAGE_OPTION_FRAME_LEFT);
	DeleteBitmap(IMAGE_OPTION_FRAME_RIGHT);
	DeleteBitmap(IMAGE_OPTION_LINE);
	DeleteBitmap(IMAGE_OPTION_POINT);
	DeleteBitmap(IMAGE_OPTION_BTN_CHECK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_BACK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_COLOR);
	DeleteBitmap(IMAGE_OPTION_VOLUME_BACK);
	DeleteBitmap(IMAGE_OPTION_VOLUME_COLOR);
}

void SEASON3B::CNewUIOptionWindow::RenderFrame()
{
	
	if (!gInterface.Data[eMenu_OPTION].OnShow)
	{
		g_pNewUISystem->Hide(INTERFACE_OPTION);
		return;
	}
	
	float MainWidth = 270;  // Aumentado de 220 para 280
	float MainHeight = 560;


	float StartY = ((DisplayHeight - 51) / 2) - (MainHeight / 2);
	float StartX = (MAX_WIN_WIDTH / 2) - (MainWidth / 2) + 120;
	

	static int lastDisplayHeight = 0;
	static int lastDisplayWin = 0;
	if (lastDisplayHeight != DisplayHeight || lastDisplayWin != DisplayWin)
	{
		gInterface.Data[eMenu_OPTION].FirstLoad = false;
		lastDisplayHeight = DisplayHeight;
		lastDisplayWin = DisplayWin;
	}
	

	g_pBCustomMenuInfo->gDrawWindowCustom(&StartX, &StartY, MainWidth, MainHeight, eMenu_OPTION, "Opções");

	StartY += 5;
	
	SetPos(StartX, StartY);

	EnableAlphaTest();
	glColor3f(1.0, 1.0, 1.0);

	g_pBCustomMenuInfo->gInfoBox(StartX + 17.0f, StartY + 35.0f, 230, 40, 0x00000060, 0, 0);
	g_pBCustomMenuInfo->gInfoBox(StartX + 17.0f, StartY + 80.0f, 230, 95, 0x00000060, 0, 0);
	g_pBCustomMenuInfo->gInfoBox(StartX + 17.0f, StartY + 178.0f, 230, 60, 0x00000060, 0, 0);
	g_pBCustomMenuInfo->gInfoBox(StartX + 17.0f, StartY + 240.0f, 230, 135, 0x00000060, 0, 0);
	g_pBCustomMenuInfo->gInfoBox(StartX + 17.0f, StartY + 375.0f, 230, 140, 0x00000060, 0, 0);
	
	DisableAlphaBlend();
}
extern int DisplayWin;
extern int DisplayHeight;
extern int DisplayHeightExt;
extern int DisplayWinCDepthBox;
extern int DisplayWinMid;
extern int DisplayWinExt;
extern int DisplayWinReal;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

void reload_ui_system()
{
	if (g_pNewUISystem)
	{
		g_pNewUISystem->UnloadMainSceneInterface();
		g_pNewUISystem->LoadMainSceneInterface();
	}
}


void change_resolution(int index)
{
	gInterface.CloseAllWindowCustom();
	g_pNewUISystem->HideAll();

	switch (index) 
	{
		case 0:WindowWidth = 640; WindowHeight = 480; break;
		case 1:WindowWidth = 1280; WindowHeight = 768; break;
		case 2:WindowWidth = 1280; WindowHeight = 1024; break;
		case 3:WindowWidth = 1366; WindowHeight = 768; break;
		case 4:WindowWidth = 1600; WindowHeight = 1280; break;
		case 5:WindowWidth = 1400; WindowHeight = 900; break;
		case 6:WindowWidth = 1600; WindowHeight = 900; break;
		case 7:WindowWidth = 1680; WindowHeight = 1050; break;
		case 8:WindowWidth = 1920; WindowHeight = 1080; break;
		default:WindowWidth = 1920; WindowHeight = 1080; break;
	}
	
	extern int m_Resolution;
	m_Resolution = index;
	
	extern HDC g_hDC;
	extern int g_iRenderTextType;
	if (g_pRenderText)
	{
		g_pRenderText->Release();
	}
	
	if (WindowWidth >= 1600 && WindowHeight >= 900) {
		g_fScreenRate_x = 1.50f;
		g_fScreenRate_y = 1.50f;
	}
	else {
		g_fScreenRate_x = 1.35f;
		g_fScreenRate_y = 1.35f;
	}

	DisplayWinCDepthBox = (int)WindowWidth / g_fScreenRate_y - 640;
	DisplayWin = (int)WindowWidth / g_fScreenRate_y;
	DisplayWinMid = (int)WindowWidth / g_fScreenRate_y / 2;
	DisplayWinExt = (int)((DisplayWin * 0.5) - 320.0);
	DisplayWinReal = (int)DisplayWin / 640.0;
	DisplayHeight = (int)((double)WindowHeight / g_fScreenRate_y);
	DisplayHeightExt = (int)((double)DisplayHeight - 480.0);
	
	RECT rc = { 0, 0, WindowWidth, WindowHeight };
	
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, NULL);

	rc.right -= rc.left;
	rc.bottom -= rc.top;

	SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN);

	SetWindowPos(g_hWnd, HWND_NOTOPMOST, (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
		rc.right,
		rc.bottom, SWP_SHOWWINDOW);
	
	BGetScreenWidth();
	
	extern void glViewport2(int x, int y, int width, int height);
	glViewport2(0, 0, WindowWidth, WindowHeight);
	
	extern HFONT g_hFont;
	extern HFONT g_hFontBold;
	extern HFONT g_hFontBoldName;
	extern HFONT g_hFontBig;
	extern HFONT g_hFixFont;
	extern HFONT g_hFontMini;
	extern char m_FontName[];
	extern int m_FontSizePlus;
	
	if(g_hFont)
		DeleteObject((HGDIOBJ)g_hFont);
	if(g_hFontBold)
		DeleteObject((HGDIOBJ)g_hFontBold);
	if(g_hFontBig)
		DeleteObject((HGDIOBJ)g_hFontBig);
	if(g_hFixFont)
		DeleteObject((HGDIOBJ)g_hFixFont);
	if(g_hFontMini)
		DeleteObject((HGDIOBJ)g_hFontMini);
	if(g_hFontBoldName)
		DeleteObject((HGDIOBJ)g_hFontBoldName);
	
	int FontHeight;
	switch (WindowWidth)
	{
		case 640: FontHeight = 12; break;
		case 1280: FontHeight = 15; break;
		case 1366: FontHeight = 15; break;
		case 1400: FontHeight = 16; break;
		case 1600: FontHeight = 17; break;
		case 1680: FontHeight = 17; break;
		case 1920: FontHeight = 18; break;
		default: FontHeight = 15; break;
	}
	
	int nFixFontHeight = 13 + m_FontSizePlus;
	int iFontSize = FontHeight - 1;
	int nFixFontSize = nFixFontHeight - 1;

	g_hFont = CreateFont(14, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_FontName);
	g_hFontBold = CreateFont(iFontSize, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_FontName);
	g_hFontBoldName = CreateFont(14, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_FontName);
	g_hFontBig = CreateFont(iFontSize * 2, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_FontName);
	g_hFixFont = CreateFont(nFixFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_FontName);
	g_hFontMini = CreateFont(10, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_FontName);

	extern HWND g_hWnd;
	CInput::Instance().Create(g_hWnd, WindowWidth, WindowHeight);
	
	if (g_pRenderText && g_hDC)
	{
		g_pRenderText->Create(g_iRenderTextType, g_hDC);
		g_pRenderText->SetFont(g_hFont);
	}
	
	extern HDC g_hDC;
	if (g_hDC)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SwapBuffers(g_hDC);
	}
	
	glViewport2(0, 0, WindowWidth, WindowHeight);
	

	extern int OpenglWindowWidth;
	extern int OpenglWindowHeight;
	extern int OpenglWindowX;
	extern int OpenglWindowY;
	
	OpenglWindowWidth = WindowWidth;
	OpenglWindowHeight = WindowHeight;
	OpenglWindowX = 0;
	OpenglWindowY = 0;
	
	extern float CameraFOV;
	extern float CameraViewNear;
	extern float CameraViewFar;
	extern void gluPerspective2(float Fov, float Aspect, float ZNear, float ZFar);

	float Aspect = (float)OpenglWindowWidth / (float)OpenglWindowHeight;
	gluPerspective2(CameraFOV, Aspect, CameraViewNear, CameraViewFar);
	

	if (g_pNewUISystem && g_pNewUISystem->GetNewUI3DRenderMng())
	{
		g_pNewUISystem->GetNewUI3DRenderMng()->UpdateCamerasResolution(WindowWidth, WindowHeight);
	}
	

	extern void BeginBitmap();
	extern void EndBitmap();

	EndBitmap();
	BeginBitmap();
	
	for(int i = eBeginWindowCustom; i < eEndWindowCustom; i++)
	{
		gInterface.Data[i].FirstLoad = false;
		gInterface.Data[i].curX = 0;
		gInterface.Data[i].curY = 0;
		gInterface.Data[i].X = 0;
		gInterface.Data[i].Y = 0;
		gInterface.Data[i].Close();
	}
	
	if (g_pMainFrame)
	{
		g_pMainFrame->SetButtonInfo();
	}
	
	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
	{
		RegSetValueEx(key, "Resolution", 0, REG_DWORD, (const BYTE*)&index, sizeof(index));
		RegCloseKey(key);
	}
	
	EndBitmap();
	BeginBitmap();
	InvalidateRect(g_hWnd, NULL, TRUE);
	UpdateWindow(g_hWnd);
	
	Sleep(100);
}

//==Config Custom
void SEASON3B::CNewUIOptionWindow::RenderCustomFrame()
{
	float x, y;
	x = m_Pos.x +5;
	y = m_Pos.y + 35.f;
	
	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);

	g_pRenderText->RenderText(x + 30, y + 5, "Qualidade Gráfica :");
	
	
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15, y + 15, 0xFFCC00C8, this->OnOffGrap[eLowSettings] == 1 ? TRUE : FALSE, "Baixo"))
	{
		
		this->OnOffGrap[eLowSettings] = 1;
		this->OnOffGrap[eMediumSettings] = 0;
		this->OnOffGrap[eHighSettings] = 0;

	
		this->OnOffGrap[eGlowEffect] = 0;
		this->OnOffGrap[eEffectDynamic] = 0;
		this->OnOffGrap[eEffectStatic] = 0;
		this->OnOffGrap[eExcellentEffect] = 0;

		
		char string[10];
		WritePrivateProfileStringA("Graphics", "LowSettings", "1", "./config.ini");
		WritePrivateProfileStringA("Graphics", "MediumSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "HighSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "GlowEffect", itoa(this->OnOffGrap[eGlowEffect], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectDynamic", itoa(this->OnOffGrap[eEffectDynamic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectStatic", itoa(this->OnOffGrap[eEffectStatic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "ExcellentEffect", itoa(this->OnOffGrap[eExcellentEffect], string, 10), "./config.ini");
	}

	
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 70, y + 15, 0xFFCC00C8, this->OnOffGrap[eMediumSettings] == 1 ? TRUE : FALSE, "Médio"))
	{
		
		this->OnOffGrap[eLowSettings] = 0;
		this->OnOffGrap[eMediumSettings] = 1;
		this->OnOffGrap[eHighSettings] = 0;

		
		this->OnOffGrap[eGlowEffect] = 1;
		this->OnOffGrap[eEffectDynamic] = 0;
		this->OnOffGrap[eEffectStatic] = 0;
		this->OnOffGrap[eExcellentEffect] = 1;

		
		char string[10];
		WritePrivateProfileStringA("Graphics", "LowSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "MediumSettings", "1", "./config.ini");
		WritePrivateProfileStringA("Graphics", "HighSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "GlowEffect", itoa(this->OnOffGrap[eGlowEffect], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectDynamic", itoa(this->OnOffGrap[eEffectDynamic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectStatic", itoa(this->OnOffGrap[eEffectStatic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "ExcellentEffect", itoa(this->OnOffGrap[eExcellentEffect], string, 10), "./config.ini");
	}

	
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 145, y + 15, 0xFFCC00C8, this->OnOffGrap[eHighSettings] == 1 ? TRUE : FALSE, "Alto"))
	{
		
		this->OnOffGrap[eLowSettings] = 0;
		this->OnOffGrap[eMediumSettings] = 0;
		this->OnOffGrap[eHighSettings] = 1;

		
		this->OnOffGrap[eGlowEffect] = 1;
		this->OnOffGrap[eEffectDynamic] = 1;
		this->OnOffGrap[eEffectStatic] = 1;
		this->OnOffGrap[eExcellentEffect] = 1;

		
		char string[10];
		WritePrivateProfileStringA("Graphics", "LowSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "MediumSettings", "0", "./config.ini");
		WritePrivateProfileStringA("Graphics", "HighSettings", "1", "./config.ini");
		WritePrivateProfileStringA("Graphics", "GlowEffect", itoa(this->OnOffGrap[eGlowEffect], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectDynamic", itoa(this->OnOffGrap[eEffectDynamic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "EffectStatic", itoa(this->OnOffGrap[eEffectStatic], string, 10), "./config.ini");
		WritePrivateProfileStringA("Graphics", "ExcellentEffect", itoa(this->OnOffGrap[eExcellentEffect], string, 10), "./config.ini");
	}


	/*if (g_pBCustomMenuInfo->RenderCheckBox(x + 15, y + 15, 0xFFCC00C8, this->OnOffGrap[eGlowEffect] == 1 ? TRUE : FALSE, "Glow Efflect"))
	{
		this->OnOffGrap[eGlowEffect] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eEffectDynamic] == 1 ? TRUE : FALSE, "Effect Dynamic"))
	{
		this->OnOffGrap[eEffectDynamic] ^= 1;
	}
	y = y + 17;
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15, y + 15, 0xFFCC00C8, this->OnOffGrap[eEffectStatic] == 1 ? TRUE : FALSE, "Effect Static"))
	{
		this->OnOffGrap[eEffectStatic] ^= 1;
	}

	if (g_pBCustomMenuInfo->RenderCheckBox(x + 15 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eExcellentEffect] == 1 ? TRUE : FALSE, "Excellent Effect"))
	{
		this->OnOffGrap[eExcellentEffect] ^= 1;
	}*/

	y = y + 45;
	g_pRenderText->RenderText(x + 30, y, "Opções de Renderização :");
	x = m_Pos.x + 25;
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDWings] == 1 ? TRUE : FALSE, "Ocultar Asas"))
	{
		this->OnOffGrap[eBMDWings] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDWeapons] == 1 ? TRUE : FALSE, "Ocultar Armas"))
	{
		this->OnOffGrap[eBMDWeapons] ^= 1;
	}
	y = y + 17;
	if (g_pBCustomMenuInfo->RenderCheckBox(x  , y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDImg] == 1 ? TRUE : FALSE, "Reduzir Lag"))
	{
		this->OnOffGrap[eBMDImg] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDMonter] == 1 ? TRUE : FALSE, "Ocultar Monstros"))
	{
		this->OnOffGrap[eBMDMonter] ^= 1;
	}
	y = y + 17;
	if (g_pBCustomMenuInfo->RenderCheckBox(x  , y + 15, 0xFFCC00C8, this->OnOffGrap[eRenderObjects] == 1 ? TRUE : FALSE, "Ocultar objetos"))
	{
		this->OnOffGrap[eRenderObjects] ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eRenderTerrain] == 1 ? TRUE : FALSE, "Ocultar terreno"))
	{
		this->OnOffGrap[eRenderTerrain] ^= 1;
	}
	y = y + 17;
	if (g_pBCustomMenuInfo->RenderCheckBox(x , y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDPlayer] == 1 ? TRUE : FALSE, "Ocultar Personagens"))
	{
		this->OnOffGrap[eBMDPlayer] ^= 1;
	}
	
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, this->OnOffGrap[eBMDZen] == 1 ? TRUE : FALSE, "Ocultar Drop Zen"))
	{
		this->OnOffGrap[eBMDZen] ^= 1;
	}
	//==================
	y = y + 50;

	g_pRenderText->RenderText(x+10, y, "Opções de Combate :");

	if (g_pBCustomMenuInfo->RenderCheckBox(x , y+15, 0xFFCC00C8, mShowHPBar == 1 ? TRUE : FALSE, "Mostrar Barra HP"))
	{
		mShowHPBar ^= 1;
	}
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80,y + 15, 0xFFCC00C8, mShowName == 1 ? TRUE : FALSE, "Mostrar Nome"))
	{
		mShowName ^= 1;
		g_bGMObservation = mShowName;
	}
	y = y + 17;
	if (g_pBCustomMenuInfo->RenderCheckBox(x , y + 15, 0xFFCC00C8, gInterface.AutoPK == 1 ? TRUE : FALSE, "Auto PvP"))
	{
		gInterface.AutoPK ^= 1;
	}
	
	if (g_pBCustomMenuInfo->RenderCheckBox(x + 30 + 80, y + 15, 0xFFCC00C8, mAutoHP == 1 ? TRUE : FALSE, "Auto HP"))
	{
		mAutoHP ^= 1;
		XULY_CGPACKET pMsg;
		pMsg.header.set(0xFC, 103, sizeof(pMsg));
		DataSend((BYTE*)&pMsg, pMsg.header.size);
	}
	
	y = y + 50;
	g_pRenderText->RenderText(x+10, y, "Resolução :");
	
	static int currentResolution = -1;
	static DWORD lastCheckTime = 0;
	DWORD currentTime = GetTickCount();
	
	if (currentResolution == -1 || (currentTime - lastCheckTime) > 500)
	{
		HKEY key;
		DWORD dwSize = sizeof(int);
		int tempResolution = 8;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_READ, &key) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(key, "Resolution", 0, NULL, (LPBYTE)&tempResolution, &dwSize) != ERROR_SUCCESS)
			{
				tempResolution = 8;
			}
			RegCloseKey(key);
		}
		currentResolution = tempResolution;
		lastCheckTime = currentTime;
	}

	const char* resolutions[] = {
		"640x480", "1280x768", "1280x1024", "1366x768", 
		"1600x1280", "1400x900", "1600x900", "1680x1050", "1920x1080"
	};
	int resolutionIndices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	
	for (int i = 0; i < 9; i++)
	{
		int col = i % 2;
		int row = i / 2;
		float resX = x + (col * 100);
		float resY = y + 20 + (row * 20);
		
		if (g_pBCustomMenuInfo->RenderCheckBox(resX, resY, 0xFFCC00C8, currentResolution == resolutionIndices[i] ? TRUE : FALSE, resolutions[i]))
		{
			if (currentResolution != resolutionIndices[i])
			{
				currentResolution = resolutionIndices[i];
				change_resolution(resolutionIndices[i]);
				gInterface.Data[eMenu_OPTION].Close();
				g_pNewUISystem->Hide(INTERFACE_OPTION);
				gInterface.Data[eMenu_OPTION].FirstLoad = false;
			}
		}
	}

	y = y + 120;
	float soundSectionY = y;
	g_pRenderText->RenderText(x+15, y + 10, "Configurações de Som :");
	
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 20, 0xFFCC00C8, m_bSoundEnabled ? TRUE : FALSE, "Habilitar Som"))
	{
		m_bSoundEnabled = !m_bSoundEnabled;
		char string[10];
		WritePrivateProfileStringA("Sound", "SoundEnabled", itoa(m_bSoundEnabled ? 1 : 0, string, 10), "./config.ini");
		extern void SetEnableSound(bool b);
		SetEnableSound(m_bSoundEnabled);
	}
	
	y = y + 35;

	float volumeBarY = y + 5;
	// Move a barra de volume mais para a direita
	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 48, volumeBarY, 124.f, 16.f);
	if (m_iSoundVolumeLevel > 0)
	{
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 48, volumeBarY, 124.f * 0.1f * m_iSoundVolumeLevel, 16.f);
	}
	
	// Renderiza botões + e - para Volume de Som (movidos mais à direita)
	g_pRenderText->RenderText(m_Pos.x + 40, volumeBarY, "-", 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 175, volumeBarY, "+", 0, 0, RT3_SORT_LEFT);

	y = y + 10;
	if (g_pBCustomMenuInfo->RenderCheckBox(x, y + 20, 0xFFCC00C8, m_bEffectSoundEnabled ? TRUE : FALSE, "Habilitar Efeitos"))
	{
		m_bEffectSoundEnabled = !m_bEffectSoundEnabled;
		char string[10];
		WritePrivateProfileStringA("Sound", "EffectSoundEnabled", itoa(m_bEffectSoundEnabled ? 1 : 0, string, 10), "./config.ini");
		extern void SetEffectVolumeLevel(int level);
		SetEffectVolumeLevel(m_bEffectSoundEnabled ? m_iVolumeLevel : 0);
	}
	
	y = y + 35;

	float effectVolumeBarY = y + 5;
	// Move a barra de efeito mais para a direita
	RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x + 40, effectVolumeBarY, 141.f, 29.f);
	if (m_iVolumeLevel >= 0 && m_bEffectSoundEnabled)
	{
		RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x + 40, effectVolumeBarY, 141.f * 0.2f * (m_iVolumeLevel + 1), 29.f);
	}
	
	// Renderiza botões + e - para Volume de Efeito (movidos mais à direita)
	g_pRenderText->RenderText(m_Pos.x + 32, effectVolumeBarY + 5, "-", 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 185, effectVolumeBarY + 5, "+", 0, 0, RT3_SORT_LEFT);

	DisableAlphaBlend();
}
void SEASON3B::CNewUIOptionWindow::RenderContents()
{
	float x, y;
	x = m_Pos.x + 20.f;
	y = m_Pos.y + 46.f;
	EnableAlphaTest();

	glColor3f(1.0, 1.0, 1.0);

	//RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 22.f;
	//RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 22.f;
	//RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 40.f;
	//RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 22.f;
	//RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	//g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+48, GlobalText[386]);
	//g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+70, GlobalText[387]);
	//g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+92, GlobalText[389]);
	//g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+132, GlobalText[919]);
	//g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+154, GlobalText[1840]);
	
#ifdef CSK_LH_DEBUG_CONSOLE
	g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+519, "Mostrar Console (Debug)", 0, 0, RT3_SORT_LEFT);
#endif
	
	DisableAlphaBlend();
}

float SEASON3B::CNewUIOptionWindow::CalculateSoundVolumeBarY()
{
	float y = m_Pos.y + 35.0f;
	y = y + 45.0f;
	y = y + 68.0f;
	y = y + 50.0f;
	y = y + 17.0f;
	y = y + 50.0f;
	y = y + 100.0f;
	y = y + 120.0f;
	y = y + 35.0f;
	return y + 5.0f;
}

float SEASON3B::CNewUIOptionWindow::CalculateEffectVolumeBarY()
{
	float soundBarY = CalculateSoundVolumeBarY();
	float y = soundBarY - 5.0f + 10.0f;
	y = y + 35.0f;
	return y + 5.0f;
}

void SEASON3B::CNewUIOptionWindow::RenderButtons()
{
	//m_BtnClose.Render();

	/*if(m_bAutoAttack)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+43, 15, 15, 0, 0);
	}
	else 
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+43, 15, 15, 0, 15.f);
	}*/

	/*if(m_bWhisperSound)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+65, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+65, 15, 15, 0, 15.f);
	}*/
	
#ifdef CSK_LH_DEBUG_CONSOLE
	if(m_bConsoleVisible)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+519, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+519, 15, 15, 0, 15.f);
	}
#endif

	/*if(m_bSlideHelp)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+127, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x+150, m_Pos.y+127, 15, 15, 0, 15.f);
	}*/
	
	//RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x+40, m_Pos.y+270, 124.f, 16.f);

	//if(m_iVolumeLevel > 0)
	//{
	//	//RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x+ 40, m_Pos.y + 270, 124.f * 0.1f * 	(m_iVolumeLevel), 16.f);
	//}

	//RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x+25, m_Pos.y+168, 141.f, 29.f);

	//if(m_iRenderLevel >= 0)
	//{
	//	RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x+25, m_Pos.y+168, 141.f * 0.2f * 	(m_iRenderLevel+1), 29.f);
	//}
	//if (m_bRenderAllEffects)
	//{
	//	RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 210, 15, 15, 0, 0);
	//}
	//else
	//{
	//	RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 210, 15, 15, 0, 15.f);
	//}
}

void SEASON3B::CNewUIOptionWindow::SetAutoAttack(bool bAuto)
{
	m_bAutoAttack = bAuto;
}

bool SEASON3B::CNewUIOptionWindow::IsAutoAttack()
{
	return m_bAutoAttack;
}

void SEASON3B::CNewUIOptionWindow::SetWhisperSound(bool bSound)
{
	m_bWhisperSound = bSound;
}

bool SEASON3B::CNewUIOptionWindow::IsWhisperSound()
{
	return m_bWhisperSound;
}

void SEASON3B::CNewUIOptionWindow::SetSlideHelp(bool bHelp)
{
	m_bSlideHelp = bHelp;
}

bool SEASON3B::CNewUIOptionWindow::IsSlideHelp()
{
	return m_bSlideHelp;
}

void SEASON3B::CNewUIOptionWindow::SetVolumeLevel(int iVolume)
{
	m_iVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetVolumeLevel()
{
	return m_iVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderLevel(int iRender)
{
	m_iRenderLevel = iRender;
}

int SEASON3B::CNewUIOptionWindow::GetRenderLevel()
{
	return m_iRenderLevel;
}
void SEASON3B::CNewUIOptionWindow::SetRenderAllEffects(bool bRenderAllEffects)
{
	m_bRenderAllEffects = bRenderAllEffects;
}

bool SEASON3B::CNewUIOptionWindow::GetRenderAllEffects()
{
	return m_bRenderAllEffects;
}

void SEASON3B::CNewUIOptionWindow::SetSoundEnabled(bool bEnabled)
{
	m_bSoundEnabled = bEnabled;
}

bool SEASON3B::CNewUIOptionWindow::IsSoundEnabled()
{
	return m_bSoundEnabled;
}

void SEASON3B::CNewUIOptionWindow::SetSoundVolumeLevel(int iVolume)
{
	if (iVolume < 0) iVolume = 0;
	if (iVolume > 10) iVolume = 10;
	m_iSoundVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetSoundVolumeLevel()
{
	return m_iSoundVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetEffectSoundEnabled(bool bEnabled)
{
	m_bEffectSoundEnabled = bEnabled;
}

bool SEASON3B::CNewUIOptionWindow::IsEffectSoundEnabled()
{
	return m_bEffectSoundEnabled;
}
