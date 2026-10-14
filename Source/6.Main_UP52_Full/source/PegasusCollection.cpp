#include "stdafx.h"
#include "PegasusCollection.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "ZzzOpenglUtil.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "CBInterface.h"		// DisplayWin / DisplayHeight
#include "PegasusItemIcon.h"	// item models, drawn from inside the 2D pass
#include "Util.h"				// BGetItemName

CPegasusCollection gPegasusCollection;

namespace
{
	const char* const CO_DIR = "Interface\\Pegasus\\Pegasus_Collection\\";

	// Pegasus_Collections ships as one 2399x1507 sheet, well past the loader's
	// 1024 ceiling, so it never loaded at all. These are the pieces cut out of
	// it - see the sizes below, which are the art's real extent inside each
	// power-of-two texture. Asking for more than that samples the uninitialised
	// padding and comes out white.
	const float CO_SRC_PANEL_W = 1024.f, CO_SRC_PANEL_H = 878.f;
	const float CO_SRC_ALTAR   = 780.f;
	const float CO_SRC_SIDE_W  = 500.f,  CO_SRC_SIDE_H  = 392.f;
	// The pedestal frames are 172x170 on a 185px pitch. They were being cut on
	// a 194px one, which is how slot1 ended up with a sliver of its neighbour's
	// edge and slot3 with the blue navigation arrows sitting to the right of
	// the row - see _build_pg_col_assets.py.
	const float CO_SRC_SLOT_W  = 172.f,  CO_SRC_SLOT_H  = 170.f;

	// Window geometry, in its own space.
	const float CO_W = 700.f, CO_H = 600.f;

	const float CO_TITLE_Y = 18.f;

	// Set list down the left.
	const float CO_SET_X = 20.f, CO_SET_Y = 62.f;
	const float CO_SET_W = 150.f, CO_SET_H = 40.f, CO_SET_GAP = 6.f;

	// Showcase art, under the grid.
	const float CO_ALTAR_CX = 440.f, CO_ALTAR_CY = 250.f, CO_ALTAR_SZ = 300.f;

	// The 4 x 3 grid of pedestals.
	const float CO_GRID_X = 200.f, CO_GRID_Y = 70.f;
	const float CO_GRID_COLS = 4.f, CO_GRID_ROWS = 3.f;
	const float CO_CELL = 112.f, CO_CELL_GAP = 8.f;

	// Progress strip along the bottom.
	const float CO_PROG_X = 20.f, CO_PROG_Y = 508.f;
	const float CO_PROG_W = CO_W - 40.f, CO_PROG_H = 26.f;

	const float CO_CLOSE_SZ = 26.f;

	float g_fCoX = 0.f, g_fCoY = 0.f;

	inline float CoX(float x) { return g_fCoX + x; }
	inline float CoY(float y) { return g_fCoY + y; }

	void CoUpdateOrigin()
	{
		g_fCoX = ((float)DisplayWin - CO_W) * 0.5f;
		g_fCoY = ((float)DisplayHeight - CO_H) * 0.5f - 16.f;
	}

	void CoBlit(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, float fLevel = 1.f)
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(iImage);
		if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
			return;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		EnableAlphaTest();
		glColor4f(fLevel, fLevel, fLevel, 1.f);

		RenderBitmap(iImage, x, y, w, h, 0.f, 0.f,
			fSrcW / pTex->Width, fSrcH / pTex->Height);

		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	void CoText(float x, float y, float w, const char* szText, DWORD dwColour,
		int iSort = RT3_SORT_CENTER)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(dwColour);
		g_pRenderText->RenderText((int)x, (int)y, szText, (int)w, 0, iSort);
		g_pRenderText->SetTextColor(CLRDW_WHITE);
	}

	// Cell rect for a grid index, in window space.
	void CoCellRect(int i, float& x, float& y, float& sz)
	{
		int c = i % (int)CO_GRID_COLS;
		int r = i / (int)CO_GRID_COLS;

		x = CO_GRID_X + (CO_CELL + CO_CELL_GAP) * c;
		y = CO_GRID_Y + (CO_CELL + CO_CELL_GAP) * r;
		sz = CO_CELL;
	}
}


CPegasusCollection::CPegasusCollection()
	: m_bShow(false)
	, m_bLoaded(false)
	, m_iSelectedSet(0)
	, m_iHoverSlot(-1)
{
	FillMockSets();
}

CPegasusCollection::~CPegasusCollection()
{
}

// Stand-in data. Real item types go in so the pedestals show actual icons
// rather than empty frames.
void CPegasusCollection::FillMockSets()
{
	static const char* const apszName[SET_COUNT] =
	{
		"Weapons", "Armour", "Jewels", "Wings", "Pets", "Relics"
	};

	for (int s = 0; s < SET_COUNT; ++s)
	{
		m_Set[s].szName = apszName[s];
		m_Set[s].iBonus = 5 + s * 2;

		for (int i = 0; i < SLOT_COUNT; ++i)
		{
			SLOT& sl = m_Set[s].aSlot[i];

			sl.iItemType  = ITEM_POTION + (i % 12);
			sl.iItemLevel = 0;
			sl.iRarity    = i % RARITY_COUNT;
			// A scattered few already handed in, so the two states are visible.
			sl.bFilled    = ((i + s) % 3) == 0;
		}
	}
}

int CPegasusCollection::CountFilled(int iSet) const
{
	int n = 0;
	for (int i = 0; i < SLOT_COUNT; ++i)
		if (m_Set[iSet].aSlot[i].bFilled)
			++n;
	return n;
}

void CPegasusCollection::LoadImages()
{
	if (m_bLoaded == true)
		return;

	char szPath[256];

	struct { const char* szFile; int iIndex; } aLoad[] =
	{
		{ "pg_col_panel.tga", BITMAP_PG_CO_PANEL     },
		{ "pg_col_altar.tga", BITMAP_PG_CO_ALTAR     },
		{ "pg_col_side.tga",  BITMAP_PG_CO_SIDE      },
		{ "pg_col_slot0.tga", BITMAP_PG_CO_SLOT      },
		{ "pg_col_slot1.tga", BITMAP_PG_CO_SLOT + 1  },
		{ "pg_col_slot2.tga", BITMAP_PG_CO_SLOT + 2  },
		{ "pg_col_slot3.tga", BITMAP_PG_CO_SLOT + 3  },
	};

	for (int i = 0; i < sizeof(aLoad) / sizeof(aLoad[0]); ++i)
	{
		sprintf(szPath, "%s%s", CO_DIR, aLoad[i].szFile);
		LoadBitmap(szPath, aLoad[i].iIndex, GL_LINEAR);
	}

	m_bLoaded = true;
}

void CPegasusCollection::ReleaseImages()
{
	for (int i = BITMAP_PG_CO_BEGIN; i < BITMAP_PG_CO_END; ++i)
		DeleteBitmap(i);

	m_bLoaded = false;
}

void CPegasusCollection::Open()
{
	LoadImages();
	m_bShow = true;
}

void CPegasusCollection::Close()
{
	m_bShow = false;
}

void CPegasusCollection::Toggle()
{
	if (m_bShow) Close();
	else         Open();
}

// Where handing an item in will go.
void CPegasusCollection::OnDonate(int iSlot)
{
	PlayBuffer(SOUND_CLICK01);
}

void CPegasusCollection::Render()
{
	if (m_bShow == false)
		return;

	LoadImages();
	CoUpdateOrigin();

	char szBuf[128];
	const SET& set = m_Set[m_iSelectedSet];

	// This panel, unlike the other Pegasus frames, is opaque across its whole
	// middle, so the ground below is only a fallback for the frame failing to
	// load - without it the window would be a hole onto the world.
	EnableAlphaTest();
	glColor4f(0.04f, 0.05f, 0.08f, 0.95f);
	RenderColor(CoX(8.f), CoY(8.f), CO_W - 16.f, CO_H - 16.f);
	EndRenderColor();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	CoBlit(BITMAP_PG_CO_PANEL, CoX(0.f), CoY(0.f), CO_W, CO_H,
		CO_SRC_PANEL_W, CO_SRC_PANEL_H);

	// Showcase art, over the panel and under the grid. It used to be drawn
	// first, which is why it never appeared: this panel is not one of the open
	// centred frames, it is opaque right across the middle, so anything behind
	// it is simply covered. The shrine is authored at about a third alpha on
	// the sheet, so ordinary blending at full strength already leaves it as the
	// faint backdrop it was drawn to be - no extra dimming needed.
	CoBlit(BITMAP_PG_CO_ALTAR,
		CoX(CO_ALTAR_CX) - CO_ALTAR_SZ * 0.5f, CoY(CO_ALTAR_CY) - CO_ALTAR_SZ * 0.5f,
		CO_ALTAR_SZ, CO_ALTAR_SZ, CO_SRC_ALTAR, CO_SRC_ALTAR);

	CoText(CoX(0.f), CoY(CO_TITLE_Y), CO_W, "COLLECTION", CLRDW_YELLOW);

	// --- set list ---
	for (int s = 0; s < SET_COUNT; ++s)
	{
		float y = CO_SET_Y + (CO_SET_H + CO_SET_GAP) * s;
		bool bSel = (s == m_iSelectedSet);

		CoBlit(BITMAP_PG_CO_SIDE, CoX(CO_SET_X), CoY(y), CO_SET_W, CO_SET_H,
			CO_SRC_SIDE_W, CO_SRC_SIDE_H, bSel ? 1.f : 0.55f);

		sprintf(szBuf, "%s  %d/%d", m_Set[s].szName, CountFilled(s), SLOT_COUNT);
		CoText(CoX(CO_SET_X), CoY(y + CO_SET_H * 0.5f - 7.f), CO_SET_W, szBuf,
			bSel ? CLRDW_YELLOW : CLRDW_WHITE);
	}

	// --- pedestal grid ---
	for (int i = 0; i < SLOT_COUNT; ++i)
	{
		float x, y, sz;
		CoCellRect(i, x, y, sz);

		const SLOT& sl = set.aSlot[i];
		bool bHover = (i == m_iHoverSlot);

		CoBlit(BITMAP_PG_CO_SLOT + sl.iRarity, CoX(x), CoY(y), sz, sz,
			CO_SRC_SLOT_W, CO_SRC_SLOT_H,
			sl.bFilled ? (bHover ? 1.f : 0.92f) : (bHover ? 0.75f : 0.5f));

		if (sl.iItemType >= 0)
		{
			// An unfilled pedestal shows the item it wants, greyed by the
			// frame's own dimming rather than a separate silhouette.
			PgRenderItemIcon(CoX(x + sz * 0.22f), CoY(y + sz * 0.22f),
				sz * 0.56f, sz * 0.56f, sl.iItemType, sl.iItemLevel);
		}
	}

	// --- progress ---
	int iFilled = CountFilled(m_iSelectedSet);

	EnableAlphaTest();
	glColor4f(0.10f, 0.12f, 0.18f, 0.9f);
	RenderColor(CoX(CO_PROG_X), CoY(CO_PROG_Y), CO_PROG_W, CO_PROG_H);
	EndRenderColor();

	glColor4f(0.85f, 0.68f, 0.22f, 0.9f);
	RenderColor(CoX(CO_PROG_X), CoY(CO_PROG_Y),
		CO_PROG_W * ((float)iFilled / (float)SLOT_COUNT), CO_PROG_H);
	EndRenderColor();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	sprintf(szBuf, "%s   %d / %d collected   -   set bonus +%d%%",
		set.szName, iFilled, SLOT_COUNT, set.iBonus);
	CoText(CoX(CO_PROG_X), CoY(CO_PROG_Y + 6.f), CO_PROG_W, szBuf, CLRDW_WHITE);

	CoText(CoX(0.f), CoY(CO_H - 30.f), CO_W,
		"Display only - handing items in is not wired up yet", CLRDW_GRAY);

	// Last, so it sits over the grid. A pedestal only shows a model, and the
	// model alone does not say which of the four Blesses or Souls it wants.
	if (m_iHoverSlot >= 0 && m_iHoverSlot < SLOT_COUNT)
	{
		const SLOT& sl = set.aSlot[m_iHoverSlot];
		if (sl.iItemType >= 0)
		{
			float x, y, sz;
			CoCellRect(m_iHoverSlot, x, y, sz);
			RenderTipText((int)CoX(x + sz * 0.5f), (int)CoY(y - 14.f),
				BGetItemName(sl.iItemType, sl.iItemLevel));
		}
	}
}

bool CPegasusCollection::UpdateMouseEvent()
{
	if (m_bShow == false)
		return false;

	CoUpdateOrigin();
	m_iHoverSlot = -1;

	if (SEASON3B::CheckMouseIn(CoX(CO_W - 42.f), CoY(12.f),
		CO_CLOSE_SZ, CO_CLOSE_SZ) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			Close();
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	for (int s = 0; s < SET_COUNT; ++s)
	{
		float y = CO_SET_Y + (CO_SET_H + CO_SET_GAP) * s;

		if (SEASON3B::CheckMouseIn(CoX(CO_SET_X), CoY(y),
			CO_SET_W, CO_SET_H) == true)
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{
				m_iSelectedSet = s;
				PlayBuffer(SOUND_CLICK01);
			}
			return true;
		}
	}

	for (int i = 0; i < SLOT_COUNT; ++i)
	{
		float x, y, sz;
		CoCellRect(i, x, y, sz);

		if (SEASON3B::CheckMouseIn(CoX(x), CoY(y), sz, sz) == true)
		{
			m_iHoverSlot = i;

			if (SEASON3B::IsPress(VK_LBUTTON)
				&& m_Set[m_iSelectedSet].aSlot[i].bFilled == false)
			{
				OnDonate(i);
			}
			return true;
		}
	}

	// Anything else over the panel is still ours, so a click inside doesn't
	// fall through and walk the character.
	return SEASON3B::CheckMouseIn(CoX(0.f), CoY(0.f), CO_W, CO_H);
}

// Whether the cursor is over the panel at all. The world's click handling
// runs in the move phase, well before this window draws, so the answer has
// to be available there - see PegasusMouseOnWindow in CBInterface.cpp.
bool CPegasusCollection::IsMouseOver()
{
	if (m_bShow == false)
		return false;

	CoUpdateOrigin();
	return SEASON3B::CheckMouseIn(CoX(0.f), CoY(0.f), CO_W, CO_H);
}
