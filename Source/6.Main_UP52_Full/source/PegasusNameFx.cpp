#include "stdafx.h"
#include "PegasusNameFx.h"
#include "PgFont.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "ZzzOpenglUtil.h"
#include "ZzzInterface.h"
#include "ZzzCharacter.h"
#include "UIControls.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "CBInterface.h"		// DisplayWin / DisplayHeight

CPegasusNameFx gPegasusNameFx;

// Set by this window, read by RenderHPBar when it draws the hero.
extern DWORD g_dwPgNameFx;
extern float g_fPgNameR, g_fPgNameG, g_fPgNameB;

namespace
{
	// Everything is derived from the rows on a page rather than from the number
	// of effects, so the frame stays put however many get added.
	const int   NF_PER_PAGE = 8;

	const float NF_W = 330.f;

	const float NF_TITLE_Y = 26.f;
	const float NF_LIST_Y  = 60.f;
	const float NF_ROW_H   = 30.f;

	const float NF_PAGE_Y  = NF_LIST_Y + NF_ROW_H * NF_PER_PAGE + 4.f;
	const float NF_ARROW_W = 30.f, NF_ARROW_H = 22.f;

	const float NF_PREVIEW_Y = NF_PAGE_Y + NF_ARROW_H + 14.f;

	const float NF_SWATCH_SZ  = 22.f;
	const float NF_SWATCH_GAP = 4.f;
	const int   NF_SWATCH_COLS = 6;
	const float NF_SWATCH_Y = NF_PREVIEW_Y + 24.f;

	const float NF_SLIDER_X = 40.f;
	const float NF_SLIDER_W = NF_W - NF_SLIDER_X - 46.f;
	const float NF_SLIDER_H = 13.f;
	const float NF_SLIDER_GAP = 20.f;
	const float NF_SLIDER_Y = NF_SWATCH_Y
		+ (NF_SWATCH_SZ + NF_SWATCH_GAP) * 2 + 12.f;

	// How many quads a gradient strip is built from. Enough that the steps do
	// not read as bands at this width.
	const int NF_SLIDER_STEPS = 44;

	const float NF_FOOT_Y = NF_SLIDER_Y + NF_SLIDER_GAP * 3 + 8.f;
	const float NF_H = NF_FOOT_Y + 22.f;

	const float NF_CLOSE_SZ = 22.f;

	struct FX_ROW { const char* szName; DWORD dwFx; };

	// Every effect on its own, plus the pairs worth having as a single choice.
	const FX_ROW g_aFx[] =
	{
		{ "None",        PG_TFX_NONE },
		{ "Gradient",    PG_TFX_GRADIENT },
		{ "Sheen",       PG_TFX_GRADIENT | PG_TFX_SHEEN },
		{ "Wave",        PG_TFX_GRADIENT | PG_TFX_WAVE },
		{ "Bounce",      PG_TFX_GRADIENT | PG_TFX_BOUNCE },
		{ "Pulse",       PG_TFX_GRADIENT | PG_TFX_PULSE },
		{ "Swell",       PG_TFX_GRADIENT | PG_TFX_SWELL },
		{ "Tremble",     PG_TFX_GRADIENT | PG_TFX_TREMBLE },
		{ "Shake",       PG_TFX_GRADIENT | PG_TFX_SHAKE },
		{ "Fade",        PG_TFX_GRADIENT | PG_TFX_FADE },
		{ "Sheen Wave",  PG_TFX_GRADIENT | PG_TFX_SHEEN | PG_TFX_WAVE },
		{ "Fire",        PG_TFX_FIRE },
		{ "Ice",         PG_TFX_ICE | PG_TFX_SHEEN },
		{ "Rainbow",     PG_TFX_GRADIENT | PG_TFX_RAINBOW },
		{ "Glitch",      PG_TFX_GRADIENT | PG_TFX_GLITCH },
		{ "Typewriter",  PG_TFX_GRADIENT | PG_TFX_TYPE },
	};

	const int NF_COUNT = sizeof(g_aFx) / sizeof(g_aFx[0]);

	// Starting points, not the whole choice - the sliders are where a colour is
	// actually settled on.
	struct FX_COLOUR { float r, g, b; };

	const FX_COLOUR g_aColour[] =
	{
		{ 0.45f, 1.00f, 0.55f },	{ 0.35f, 0.85f, 0.55f },
		{ 0.55f, 0.85f, 1.00f },	{ 0.35f, 0.55f, 1.00f },
		{ 0.85f, 0.60f, 1.00f },	{ 1.00f, 0.55f, 0.90f },
		{ 1.00f, 0.45f, 0.45f },	{ 1.00f, 0.60f, 0.30f },
		{ 1.00f, 0.85f, 0.35f },	{ 0.95f, 0.95f, 0.60f },
		{ 0.70f, 0.78f, 0.88f },	{ 1.00f, 1.00f, 1.00f },
	};

	const int NF_COLOURS = sizeof(g_aColour) / sizeof(g_aColour[0]);

	float g_fNfX = 0.f, g_fNfY = 0.f;

	inline float NfX(float x) { return g_fNfX + x; }
	inline float NfY(float y) { return g_fNfY + y; }

	// gDrawWindowCustom takes the origin by pointer and moves it when the title
	// bar is dragged, so this only seeds it the first time. After that the
	// window is wherever the player left it, like every other one.
	bool g_bPlaced = false;

	void NfUpdateOrigin()
	{
		if (g_bPlaced == false)
		{
			g_fNfX = (MAX_WIN_WIDTH * 0.5f) - (NF_W * 0.5f);
			g_fNfY = ((DisplayHeight - 51) * 0.5f) - (NF_H * 0.5f);
			g_bPlaced = true;
		}
	}

	// Row position within the page, not within the whole list.
	inline float RowY(int iOnPage)
	{
		return NF_LIST_Y + NF_ROW_H * iOnPage;
	}

	// Swatches wrap, so the palette can grow without the window growing too.
	void SwatchRect(int i, float& x, float& y)
	{
		x = 20.f + (NF_SWATCH_SZ + NF_SWATCH_GAP) * (i % NF_SWATCH_COLS);
		y = NF_SWATCH_Y + (NF_SWATCH_SZ + NF_SWATCH_GAP) * (i / NF_SWATCH_COLS);
	}

	inline float SliderY(int iChannel)
	{
		return NF_SLIDER_Y + NF_SLIDER_GAP * iChannel;
	}

	inline int PageCount()
	{
		return (NF_COUNT + NF_PER_PAGE - 1) / NF_PER_PAGE;
	}

	void NfFill(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		EnableAlphaTest();
		glColor4f(r, g, b, a);
		RenderColor(x, y, w, h);
		EndRenderColor();
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

}

CPegasusNameFx::CPegasusNameFx()
	: m_bShow(false)
	, m_iSelected(2)		// Sheen, which is what the name shipped with
	, m_iHover(-1)
	, m_iPage(0)
	, m_fR(0.45f), m_fG(1.00f), m_fB(0.55f)
	, m_iDragging(-1)
{
}

CPegasusNameFx::~CPegasusNameFx()
{
}

void CPegasusNameFx::LoadConfig()
{
	m_iSelected = GetPrivateProfileInt("Custom", "NameFx", 2, ".\\config.ini");
	if (m_iSelected < 0 || m_iSelected >= NF_COUNT)
		m_iSelected = 2;

	// Kept as 0-255 in the file, because that is what anyone opening it by hand
	// will expect to see.
	m_fR = GetPrivateProfileInt("Custom", "NameFxR", 115, ".\\config.ini") / 255.f;
	m_fG = GetPrivateProfileInt("Custom", "NameFxG", 255, ".\\config.ini") / 255.f;
	m_fB = GetPrivateProfileInt("Custom", "NameFxB", 140, ".\\config.ini") / 255.f;

	m_iPage = m_iSelected / NF_PER_PAGE;
	Apply();
}

void CPegasusNameFx::SaveConfig()
{
	char szBuf[16];
	WritePrivateProfileStringA("Custom", "NameFx",
		itoa(m_iSelected, szBuf, 10), "./config.ini");
	WritePrivateProfileStringA("Custom", "NameFxR",
		itoa((int)(m_fR * 255.f + 0.5f), szBuf, 10), "./config.ini");
	WritePrivateProfileStringA("Custom", "NameFxG",
		itoa((int)(m_fG * 255.f + 0.5f), szBuf, 10), "./config.ini");
	WritePrivateProfileStringA("Custom", "NameFxB",
		itoa((int)(m_fB * 255.f + 0.5f), szBuf, 10), "./config.ini");
}

void CPegasusNameFx::Apply()
{
	g_dwPgNameFx = g_aFx[m_iSelected].dwFx;
	g_fPgNameR = m_fR;
	g_fPgNameG = m_fG;
	g_fPgNameB = m_fB;
}

void CPegasusNameFx::Open()
{
	m_bShow = true;

	// gDrawWindowPega returns immediately unless the window is marked open in
	// gInterface.Data, and it is that record which carries the position and the
	// dragging. Holding only a local flag is what made this window invisible.
	NfUpdateOrigin();
	gInterface.Data[ePG_NAMEFX_WINDOW].OnShow = true;
	gInterface.Data[ePG_NAMEFX_WINDOW].AllowMove = true;
}

void CPegasusNameFx::Close()
{
	m_bShow = false;
	m_iDragging = -1;
	gInterface.Data[ePG_NAMEFX_WINDOW].OnShow = false;
	SaveConfig();
}

void CPegasusNameFx::Toggle()
{
	if (m_bShow) Close();
	else         Open();
}

bool CPegasusNameFx::IsMouseOver()
{
	if (m_bShow == false)
		return false;

	NfUpdateOrigin();
	return SEASON3B::CheckMouseIn(NfX(0.f), NfY(0.f), NF_W, NF_H);
}

bool CPegasusNameFx::UpdateMouseEvent()
{
	if (m_bShow == false)
		return false;

	NfUpdateOrigin();
	m_iHover = -1;

	// A slider keeps the pointer once the button went down on it, so dragging
	// past the end of the strip still works instead of letting go.
	if (m_iDragging >= 0)
	{
		if (MouseLButton == false)
		{
			m_iDragging = -1;
			SaveConfig();
		}
		else
		{
			float t = ((float)MouseX / g_fScreenRate_x - NfX(NF_SLIDER_X)) / NF_SLIDER_W;
			if (t < 0.f) t = 0.f;
			if (t > 1.f) t = 1.f;

			if (m_iDragging == 0)      m_fR = t;
			else if (m_iDragging == 1) m_fG = t;
			else                       m_fB = t;

			Apply();
		}
		return true;
	}

	if (SEASON3B::CheckMouseIn(NfX(NF_W - 32.f), NfY(4.f), 30.f, 30.f) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			Close();
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	// --- effect rows on this page ---
	const int iFirst = m_iPage * NF_PER_PAGE;
	for (int k = 0; k < NF_PER_PAGE; ++k)
	{
		const int i = iFirst + k;
		if (i >= NF_COUNT)
			break;

		if (SEASON3B::CheckMouseIn(NfX(16.f), NfY(RowY(k)),
			NF_W - 32.f, NF_ROW_H - 2.f) == true)
		{
			m_iHover = i;
			if (SEASON3B::IsPress(VK_LBUTTON) && m_iSelected != i)
			{
				m_iSelected = i;
				Apply();
				SaveConfig();
				PlayBuffer(SOUND_CLICK01);
			}
			return true;
		}
	}

	// --- paging ---
	if (SEASON3B::CheckMouseIn(NfX(18.f), NfY(NF_PAGE_Y),
		NF_ARROW_W, NF_ARROW_H) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && m_iPage > 0)
		{
			--m_iPage;
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	if (SEASON3B::CheckMouseIn(NfX(NF_W - 18.f - NF_ARROW_W), NfY(NF_PAGE_Y),
		NF_ARROW_W, NF_ARROW_H) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && m_iPage < PageCount() - 1)
		{
			++m_iPage;
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	// --- swatches, as quick starting points ---
	for (int i = 0; i < NF_COLOURS; ++i)
	{
		float x, y;
		SwatchRect(i, x, y);

		if (SEASON3B::CheckMouseIn(NfX(x), NfY(y),
			NF_SWATCH_SZ, NF_SWATCH_SZ) == true)
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{
				m_fR = g_aColour[i].r;
				m_fG = g_aColour[i].g;
				m_fB = g_aColour[i].b;
				Apply();
				SaveConfig();
				PlayBuffer(SOUND_CLICK01);
			}
			return true;
		}
	}

	// --- the three channel sliders ---
	for (int c = 0; c < 3; ++c)
	{
		if (SEASON3B::CheckMouseIn(NfX(NF_SLIDER_X), NfY(SliderY(c)),
			NF_SLIDER_W, NF_SLIDER_H) == true)
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{
				m_iDragging = c;
				PlayBuffer(SOUND_CLICK01);
			}
			return true;
		}
	}

	// Anywhere else over the panel is still ours, so a click inside does not
	// fall through and walk the character.
	return SEASON3B::CheckMouseIn(NfX(0.f), NfY(0.f), NF_W, NF_H);
}

void CPegasusNameFx::Render()
{
	if (m_bShow == false)
		return;

	NfUpdateOrigin();

	const char* szName = (Hero != NULL && Hero->ID[0] != '\0') ? Hero->ID : "Name";

	// The client's own window handling, exactly as MenuCustom does it for the
	// game menu. gDrawWindowPega draws nothing itself - it carries the position,
	// the dragging and the hover, and it returns early unless the window is
	// marked OnShow in gInterface.Data. Missing that flag is what left this
	// window completely transparent when it was first wired up.
	g_pBCustomMenuInfo->gDrawWindowPega(&g_fNfX, &g_fNfY, NF_W, NF_H,
		ePG_NAMEFX_WINDOW, "Name Effect");

	// The frame itself: the same bronze panel the game menu wears, so this
	// window belongs to the set rather than looking like something bolted on.
	// 548x752 is the art's real extent inside TournamentMenu_I17.
	EnableAlphaTest();
	glColor3f(1.f, 1.f, 1.f);
	RenderImageFFF(SEASON3B::CNewUIBCustomMenuInfo::IMAGE_LIST::IMAGE_Frame_Menu,
		g_fNfX, g_fNfY, NF_W, NF_H, 0.f, 0.f, 548.f, 752.f);

	// Gold caption, in the house colour and the house call.
	gInterface.DrawFormat(eYellow, g_fNfX + 35.f, g_fNfY + 14.f, 100, 1,
		"Name Effect");

	// --- the page of effects ---
	// Each row draws the real name with that effect running. Reading a list of
	// names is the whole point; a list of words would tell you nothing.
	const int iFirst = m_iPage * NF_PER_PAGE;
	for (int k = 0; k < NF_PER_PAGE; ++k)
	{
		const int i = iFirst + k;
		if (i >= NF_COUNT)
			break;

		const float y = RowY(k);
		const bool  bSel = (i == m_iSelected);

		if (bSel)
			NfFill(NfX(14.f), NfY(y - 2.f), NF_W - 28.f, NF_ROW_H - 2.f,
				0.22f, 0.30f, 0.44f, 0.85f);
		else if (i == m_iHover)
			NfFill(NfX(14.f), NfY(y - 2.f), NF_W - 28.f, NF_ROW_H - 2.f,
				0.14f, 0.17f, 0.24f, 0.75f);

		PgScreenTextFx(NfX(62.f), NfY(y + NF_ROW_H * 0.5f - 2.f), g_aFx[i].szName,
			bSel ? 1.f : 0.62f, bSel ? 1.f : 0.66f, bSel ? 1.f : 0.74f,
			11.f, 1.f, PG_TFX_NONE);

		PgScreenTextFx(NfX(NF_W - 100.f), NfY(y + NF_ROW_H * 0.5f - 2.f), szName,
			m_fR, m_fG, m_fB, 13.f, 1.f, g_aFx[i].dwFx, (float)(i * 190));
	}

	// --- paging ---
	{
		const bool bPrev = (m_iPage > 0);
		const bool bNext = (m_iPage < PageCount() - 1);

		NfFill(NfX(18.f), NfY(NF_PAGE_Y), NF_ARROW_W, NF_ARROW_H,
			0.16f, 0.19f, 0.26f, bPrev ? 0.95f : 0.4f);
		NfFill(NfX(NF_W - 18.f - NF_ARROW_W), NfY(NF_PAGE_Y), NF_ARROW_W, NF_ARROW_H,
			0.16f, 0.19f, 0.26f, bNext ? 0.95f : 0.4f);

		PgScreenTextFx(NfX(18.f + NF_ARROW_W * 0.5f), NfY(NF_PAGE_Y + NF_ARROW_H * 0.5f),
			"<", bPrev ? 1.f : 0.35f, bPrev ? 1.f : 0.35f, bPrev ? 1.f : 0.35f,
			12.f, 1.f, PG_TFX_NONE);

		PgScreenTextFx(NfX(NF_W - 18.f - NF_ARROW_W * 0.5f), NfY(NF_PAGE_Y + NF_ARROW_H * 0.5f),
			">", bNext ? 1.f : 0.35f, bNext ? 1.f : 0.35f, bNext ? 1.f : 0.35f,
			12.f, 1.f, PG_TFX_NONE);

		char szPage[32];
		sprintf(szPage, "%d / %d", m_iPage + 1, PageCount());
		PgScreenTextFx(NfX(NF_W * 0.5f), NfY(NF_PAGE_Y + NF_ARROW_H * 0.5f), szPage,
			0.7f, 0.74f, 0.82f, 11.f, 1.f, PG_TFX_NONE);
	}

	// --- what the pick actually looks like, at the size it is worn ---
	PgScreenTextFx(NfX(NF_W * 0.5f), NfY(NF_PREVIEW_Y), szName,
		m_fR, m_fG, m_fB, 15.f, 1.f, g_aFx[m_iSelected].dwFx);

	// --- swatches ---
	for (int i = 0; i < NF_COLOURS; ++i)
	{
		float x, y;
		SwatchRect(i, x, y);
		NfFill(NfX(x), NfY(y), NF_SWATCH_SZ, NF_SWATCH_SZ,
			g_aColour[i].r, g_aColour[i].g, g_aColour[i].b, 1.f);
	}

	// The colour as it stands, beside the swatches.
	NfFill(NfX(NF_W - 78.f), NfY(NF_SWATCH_Y), 58.f,
		NF_SWATCH_SZ * 2 + NF_SWATCH_GAP, m_fR, m_fG, m_fB, 1.f);

	// --- the three channel sliders ---
	// Each strip is drawn as the gradient it controls, so the bar shows what
	// moving it will do rather than needing a number read off it.
	static const char* const aszLabel[3] = { "R", "G", "B" };

	for (int c = 0; c < 3; ++c)
	{
		const float y = SliderY(c);
		const float fStep = NF_SLIDER_W / (float)NF_SLIDER_STEPS;

		for (int k = 0; k < NF_SLIDER_STEPS; ++k)
		{
			const float t = (float)k / (float)(NF_SLIDER_STEPS - 1);

			float r = m_fR, g = m_fG, b = m_fB;
			if (c == 0)      r = t;
			else if (c == 1) g = t;
			else             b = t;

			NfFill(NfX(NF_SLIDER_X + fStep * k), NfY(y),
				fStep + 1.f, NF_SLIDER_H, r, g, b, 1.f);
		}

		const float v = (c == 0) ? m_fR : (c == 1) ? m_fG : m_fB;

		// The handle straddles the strip, so it reads against both a bright and
		// a dark end of it.
		NfFill(NfX(NF_SLIDER_X + NF_SLIDER_W * v - 2.f), NfY(y - 3.f),
			4.f, NF_SLIDER_H + 6.f, 1.f, 1.f, 1.f, 0.95f);
		NfFill(NfX(NF_SLIDER_X + NF_SLIDER_W * v - 1.f), NfY(y - 2.f),
			2.f, NF_SLIDER_H + 4.f, 0.1f, 0.1f, 0.12f, 0.9f);

		PgScreenTextFx(NfX(NF_SLIDER_X - 14.f), NfY(y + NF_SLIDER_H * 0.5f),
			aszLabel[c], 0.75f, 0.78f, 0.85f, 11.f, 1.f, PG_TFX_NONE);

		char szVal[16];
		sprintf(szVal, "%d", (int)(v * 255.f + 0.5f));
		PgScreenTextFx(NfX(NF_W - 24.f), NfY(y + NF_SLIDER_H * 0.5f),
			szVal, 0.72f, 0.76f, 0.84f, 10.f, 1.f, PG_TFX_NONE);
	}

	PgScreenTextFx(NfX(NF_W * 0.5f), NfY(NF_FOOT_Y),
		"Fire, Ice and Rainbow carry their own colour",
		0.5f, 0.53f, 0.6f, 10.f, 1.f, PG_TFX_NONE);
}
