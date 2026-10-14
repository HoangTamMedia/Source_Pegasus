#include "stdafx.h"
#include "PegasusDungeon.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "ZzzOpenglUtil.h"
#include "ZzzInterface.h"
#include "UIControls.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "CBInterface.h"		// DisplayWin / DisplayHeight
#include "ZzzInventory.h"
#include "wsclientinline.h"	// SendRequestMoveToDevilSquare

CPegasusDungeon gPegasusDungeon;

namespace
{
	const char* const DG_DIR = "Interface\\Pegasus\\Pegasus_Dungeon\\";

	// Real art sizes. The loader rounds each up to a power of two and leaves
	// the extra rows uninitialised, so every source rect here has to be the
	// art's own size - ask for more and the quad samples heap and comes out
	// white.
	const float DG_SRC_PANEL   = 640.f, DG_SRC_PANEL_H = 412.f;	// I3
	const float DG_SRC_HEADER  = 640.f, DG_SRC_HEADER_H = 59.f;	// I21
	const float DG_SRC_ORB     = 572.f;							// I6
	const float DG_SRC_CREST   = 228.f;							// I14..I17
	const float DG_SRC_DOOR    = 60.f;							// I20
	const float DG_SRC_PARTY   = 32.f;							// I22 / I23
	const float DG_SRC_PLATE_W = 296.f, DG_SRC_PLATE_H = 80.f;	// I8

	// Window geometry, in its own space.
	const float DG_W = 640.f, DG_H = 412.f;

	const float DG_HEADER_H = 44.f;
	const float DG_TITLE_Y  = 14.f;

	// Dungeon tabs down the left.
	const float DG_TAB_X = 22.f, DG_TAB_Y = 70.f;
	const float DG_TAB_W = 170.f, DG_TAB_H = 46.f, DG_TAB_GAP = 8.f;

	// The portal, centred in the space right of the tab column (192..640).
	const float DG_ORB_CX = 416.f, DG_ORB_CY = 178.f, DG_ORB_SZ = 190.f;

	// Difficulty crests under the portal, on the same centre line.
	const float DG_DIFF_CY    = 306.f;
	const float DG_DIFF_SZ    = 58.f;
	const float DG_DIFF_PITCH = 70.f;
	const float DG_DIFF_CX    = 416.f;

	// Requirements block. The four tabs run from 70 to 286, so this has to
	// start below that or it lands on the last one.
	const float DG_REQ_X = 22.f, DG_REQ_Y = 296.f;
	const float DG_REQ_W = 170.f, DG_REQ_H = 50.f;

	const float DG_ENTER_X = 22.f, DG_ENTER_Y = 354.f;
	const float DG_ENTER_W = 170.f, DG_ENTER_H = 44.f;
	const float DG_DOOR_SZ = 30.f;

	const float DG_CLOSE_SZ = 26.f;

	// Filled by Render so the hit tests can't drift from the drawing.
	float g_fDgX = 0.f, g_fDgY = 0.f;

	inline float DgX(float x) { return g_fDgX + x; }
	inline float DgY(float y) { return g_fDgY + y; }

	void DgUpdateOrigin()
	{
		g_fDgX = ((float)DisplayWin - DG_W) * 0.5f;
		g_fDgY = ((float)DisplayHeight - DG_H) * 0.5f - 20.f;
	}

	void DgBlit(int iImage, float x, float y, float w, float h,
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

	// Additive pass, for the portal glow.
	void DgGlow(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, float fLevel)
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(iImage);
		if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
			return;

		if (TextureEnable == false)
		{
			TextureEnable = true;
			glEnable(GL_TEXTURE_2D);
		}

		EnableAlphaBlend();		// GL_ONE / GL_ONE
		glColor4f(fLevel, fLevel, fLevel, 1.f);

		RenderBitmap(iImage, x, y, w, h, 0.f, 0.f,
			fSrcW / pTex->Width, fSrcH / pTex->Height);

		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	void DgText(float x, float y, float w, const char* szText, DWORD dwColour,
		int iSort = RT3_SORT_CENTER)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(dwColour);
		g_pRenderText->RenderText((int)x, (int)y, szText, (int)w, 0, iSort);
		g_pRenderText->SetTextColor(CLRDW_WHITE);
	}

	const char* const s_apszDifficulty[CPegasusDungeon::DIFFICULTY_COUNT] =
	{
		"Normal", "Hard", "Nightmare", "Hell"
	};
}


CPegasusDungeon::CPegasusDungeon()
	: m_bShow(false)
	, m_bLoaded(false)
	, m_iSelected(0)
	, m_iDifficulty(0)
{
	// Static until the server says which of these the character can actually
	// enter. Names match the art folders that ship alongside this one.
	const DUNGEON aStart[DUNGEON_COUNT] =
	{
		{ "Crywolf",   150, 0, true  },
		{ "Kanturu",   220, 2, true  },
		{ "Karutan",   300, 2, true  },
		{ "Nightmare", 380, 5, false },
	};

	for (int i = 0; i < DUNGEON_COUNT; ++i)
		m_Dungeon[i] = aStart[i];
}

CPegasusDungeon::~CPegasusDungeon()
{
}

void CPegasusDungeon::LoadImages()
{
	if (m_bLoaded == true)
		return;

	char szPath[256];

	struct { const char* szFile; int iIndex; } aLoad[] =
	{
		{ "Pegasus_Dungeon_I3.tga",  BITMAP_PG_DG_PANEL      },
		{ "Pegasus_Dungeon_I21.tga", BITMAP_PG_DG_HEADER     },
		{ "Pegasus_Dungeon_I6.tga",  BITMAP_PG_DG_ORB        },
		{ "Pegasus_Dungeon_I14.tga", BITMAP_PG_DG_CREST      },
		{ "Pegasus_Dungeon_I15.tga", BITMAP_PG_DG_CREST + 1  },
		{ "Pegasus_Dungeon_I16.tga", BITMAP_PG_DG_CREST + 2  },
		{ "Pegasus_Dungeon_I17.tga", BITMAP_PG_DG_CREST + 3  },
		{ "Pegasus_Dungeon_I20.tga", BITMAP_PG_DG_DOOR       },
		{ "Pegasus_Dungeon_I22.tga", BITMAP_PG_DG_PARTY_ON   },
		{ "Pegasus_Dungeon_I23.tga", BITMAP_PG_DG_PARTY_OFF  },
		{ "Pegasus_Dungeon_I8.tga",  BITMAP_PG_DG_PLATE      },
	};

	for (int i = 0; i < sizeof(aLoad) / sizeof(aLoad[0]); ++i)
	{
		sprintf(szPath, "%s%s", DG_DIR, aLoad[i].szFile);
		LoadBitmap(szPath, aLoad[i].iIndex, GL_LINEAR);
	}

	m_bLoaded = true;
}

void CPegasusDungeon::ReleaseImages()
{
	for (int i = BITMAP_PG_DG_BEGIN; i < BITMAP_PG_DG_END; ++i)
		DeleteBitmap(i);

	m_bLoaded = false;
}

void CPegasusDungeon::Open()
{
	LoadImages();
	m_bShow = true;
}

void CPegasusDungeon::Close()
{
	m_bShow = false;
}

void CPegasusDungeon::Toggle()
{
	if (m_bShow) Close();
	else         Open();
}

// Standard MU entry, the same shape CNewUIEnterDevilSquare uses: find the
// ticket in the inventory, then send its slot along with the chosen level.
// The macro adds the +12 equipment offset itself, so iItemIndex goes in raw.
void CPegasusDungeon::OnEnter()
{
	// Invitation matching this difficulty first, then a generic one, then the
	// Lucky Coin style pass - the same fallback order the stock window walks.
	int iItemIndex = g_pMyInventory->FindItemIndex(ITEM_POTION + 19, m_iDifficulty + 1);

	if (iItemIndex == -1)
		iItemIndex = g_pMyInventory->FindItemIndex(ITEM_POTION + 19, 0);

	if (iItemIndex == -1)
		iItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 46, -1);

	if (iItemIndex == -1)
	{
		// No ticket. Say so rather than sending a request the server will just
		// drop on the floor.
		g_pChatListBox->AddText("", "You need an entry ticket for this dungeon.",
			SEASON3B::TYPE_ERROR_MESSAGE);
		return;
	}

	SendRequestMoveToDevilSquare(m_iDifficulty, iItemIndex);
	PlayBuffer(SOUND_CLICK01);
	Close();
}

void CPegasusDungeon::RenderDungeonTab(int iIndex, float x, float y, float w, float h)
{
	const DUNGEON& d = m_Dungeon[iIndex];
	bool bSelected = (iIndex == m_iSelected);

	DgBlit(BITMAP_PG_DG_PLATE, x, y, w, h, DG_SRC_PLATE_W, DG_SRC_PLATE_H,
		bSelected ? 1.f : (d.bUnlocked ? 0.62f : 0.38f));

	DWORD dwColour = bSelected ? CLRDW_YELLOW : (d.bUnlocked ? CLRDW_WHITE : CLRDW_GRAY);
	DgText(x, y + h * 0.5f - 7.f, w, d.szName, dwColour);
}

void CPegasusDungeon::RenderDifficulty(int iIndex, float cx, float cy, float sz)
{
	bool bSelected = (iIndex == m_iDifficulty);

	// The crest gets bigger and brighter when it is the one chosen, so the
	// selection reads without needing a separate highlight frame.
	float fSize = sz * (bSelected ? 1.22f : 1.f);

	DgBlit(BITMAP_PG_DG_CREST + iIndex,
		cx - fSize * 0.5f, cy - fSize * 0.5f, fSize, fSize,
		DG_SRC_CREST, DG_SRC_CREST, bSelected ? 1.f : 0.5f);
}

void CPegasusDungeon::Render()
{
	if (m_bShow == false)
		return;

	LoadImages();
	DgUpdateOrigin();

	const DUNGEON& d = m_Dungeon[m_iSelected];
	char szBuf[128];

	// The panel art is a frame with an open middle - on its own the world shows
	// straight through the window. Lay an opaque ground down first, then the
	// frame over it.
	EnableAlphaTest();
	glColor4f(0.05f, 0.06f, 0.09f, 0.94f);
	RenderColor(DgX(6.f), DgY(6.f), DG_W - 12.f, DG_H - 12.f);
	EndRenderColor();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	DgBlit(BITMAP_PG_DG_PANEL, DgX(0.f), DgY(0.f), DG_W, DG_H,
		DG_SRC_PANEL, DG_SRC_PANEL_H);

	DgBlit(BITMAP_PG_DG_HEADER, DgX(0.f), DgY(0.f), DG_W, DG_HEADER_H,
		DG_SRC_HEADER, DG_SRC_HEADER_H);

	DgText(DgX(0.f), DgY(DG_TITLE_Y), DG_W, "DUNGEON GATEWAY", CLRDW_YELLOW);

	// --- portal, behind everything on the right ---
	// Two additive passes at slightly different sizes and rates: the sprite is
	// a flat ring, and layering it out of phase makes it read as something with
	// depth that is still turning over.
	{
		float fPulse = 1.f + 0.06f * sinf(WorldTime * 0.0016f);
		float fSpin  = 0.55f + 0.18f * sinf(WorldTime * 0.0011f);

		float fSize = DG_ORB_SZ * fPulse;
		DgGlow(BITMAP_PG_DG_ORB,
			DgX(DG_ORB_CX) - fSize * 0.5f, DgY(DG_ORB_CY) - fSize * 0.5f,
			fSize, fSize, DG_SRC_ORB, DG_SRC_ORB, fSpin);

		fSize = DG_ORB_SZ * (2.f - fPulse) * 0.82f;
		DgGlow(BITMAP_PG_DG_ORB,
			DgX(DG_ORB_CX) - fSize * 0.5f, DgY(DG_ORB_CY) - fSize * 0.5f,
			fSize, fSize, DG_SRC_ORB, DG_SRC_ORB, 0.9f - fSpin * 0.5f);
	}

	// --- dungeon list ---
	for (int i = 0; i < DUNGEON_COUNT; ++i)
	{
		RenderDungeonTab(i, DgX(DG_TAB_X),
			DgY(DG_TAB_Y + (DG_TAB_H + DG_TAB_GAP) * i), DG_TAB_W, DG_TAB_H);
	}

	// --- difficulty crests ---
	float fFirstCx = DgX(DG_DIFF_CX) - DG_DIFF_PITCH * (DIFFICULTY_COUNT - 1) * 0.5f;

	for (int i = 0; i < DIFFICULTY_COUNT; ++i)
		RenderDifficulty(i, fFirstCx + DG_DIFF_PITCH * i, DgY(DG_DIFF_CY), DG_DIFF_SZ);

	DgText(DgX(DG_DIFF_CX - 100.f), DgY(DG_DIFF_CY + DG_DIFF_SZ * 0.5f + 10.f),
		200.f, s_apszDifficulty[m_iDifficulty], CLRDW_YELLOW);

	// --- requirements ---
	DgBlit(BITMAP_PG_DG_PLATE, DgX(DG_REQ_X), DgY(DG_REQ_Y), DG_REQ_W, DG_REQ_H,
		DG_SRC_PLATE_W, DG_SRC_PLATE_H, 0.7f);

	sprintf(szBuf, "Level %d", d.iMinLevel);
	DgText(DgX(DG_REQ_X), DgY(DG_REQ_Y + 8.f), DG_REQ_W, szBuf, CLRDW_WHITE);

	// Party requirement, drawn as filled and empty figures.
	if (d.iPartyNeeded > 0)
	{
		float fIcon = 16.f;
		float fRowW = fIcon * d.iPartyNeeded;
		float fRowX = DgX(DG_REQ_X + DG_REQ_W * 0.5f) - fRowW * 0.5f;

		for (int i = 0; i < d.iPartyNeeded; ++i)
		{
			// Nothing knows the real party size yet, so the first slot - the
			// player - is the only one filled.
			DgBlit((i == 0) ? BITMAP_PG_DG_PARTY_ON : BITMAP_PG_DG_PARTY_OFF,
				fRowX + fIcon * i, DgY(DG_REQ_Y + 28.f), fIcon, fIcon,
				DG_SRC_PARTY, DG_SRC_PARTY);
		}
	}
	else
	{
		DgText(DgX(DG_REQ_X), DgY(DG_REQ_Y + 28.f), DG_REQ_W, "Solo", CLRDW_GRAY);
	}

	// --- enter ---
	bool bCanEnter = d.bUnlocked;

	DgBlit(BITMAP_PG_DG_PLATE, DgX(DG_ENTER_X), DgY(DG_ENTER_Y),
		DG_ENTER_W, DG_ENTER_H, DG_SRC_PLATE_W, DG_SRC_PLATE_H,
		bCanEnter ? 1.f : 0.4f);

	DgBlit(BITMAP_PG_DG_DOOR, DgX(DG_ENTER_X + 10.f),
		DgY(DG_ENTER_Y + (DG_ENTER_H - DG_DOOR_SZ) * 0.5f),
		DG_DOOR_SZ, DG_DOOR_SZ, DG_SRC_DOOR, DG_SRC_DOOR,
		bCanEnter ? 1.f : 0.4f);

	DgText(DgX(DG_ENTER_X + 24.f), DgY(DG_ENTER_Y + DG_ENTER_H * 0.5f - 7.f),
		DG_ENTER_W, bCanEnter ? "ENTER" : "LOCKED",
		bCanEnter ? CLRDW_YELLOW : CLRDW_GRAY);

	DgText(DgX(0.f), DgY(DG_H - 26.f), DG_W,
		"Display only - entering is not wired up yet", CLRDW_GRAY);
}

bool CPegasusDungeon::UpdateMouseEvent()
{
	if (m_bShow == false)
		return false;

	DgUpdateOrigin();

	if (SEASON3B::CheckMouseIn(DgX(DG_W - 40.f), DgY(10.f),
		DG_CLOSE_SZ, DG_CLOSE_SZ) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			Close();
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	for (int i = 0; i < DUNGEON_COUNT; ++i)
	{
		if (SEASON3B::CheckMouseIn(DgX(DG_TAB_X),
			DgY(DG_TAB_Y + (DG_TAB_H + DG_TAB_GAP) * i),
			DG_TAB_W, DG_TAB_H) == true)
		{
			if (SEASON3B::IsPress(VK_LBUTTON) && m_Dungeon[i].bUnlocked)
			{
				m_iSelected = i;
				PlayBuffer(SOUND_CLICK01);
			}
			return true;
		}
	}

	float fFirstCx = DgX(DG_DIFF_CX) - DG_DIFF_PITCH * (DIFFICULTY_COUNT - 1) * 0.5f;

	for (int i = 0; i < DIFFICULTY_COUNT; ++i)
	{
		float cx = fFirstCx + DG_DIFF_PITCH * i;

		if (SEASON3B::CheckMouseIn(cx - DG_DIFF_SZ * 0.5f,
			DgY(DG_DIFF_CY) - DG_DIFF_SZ * 0.5f, DG_DIFF_SZ, DG_DIFF_SZ) == true)
		{
			if (SEASON3B::IsPress(VK_LBUTTON))
			{
				m_iDifficulty = i;
				PlayBuffer(SOUND_CLICK01);
			}
			return true;
		}
	}

	if (SEASON3B::CheckMouseIn(DgX(DG_ENTER_X), DgY(DG_ENTER_Y),
		DG_ENTER_W, DG_ENTER_H) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && m_Dungeon[m_iSelected].bUnlocked)
			OnEnter();

		return true;
	}

	// Everything else over the panel is still ours, so a click inside doesn't
	// fall through and walk the character.
	return SEASON3B::CheckMouseIn(DgX(0.f), DgY(0.f), DG_W, DG_H);
}

// Whether the cursor is over the panel at all. The world's click handling
// runs in the move phase, well before this window draws, so the answer has
// to be available there - see PegasusMouseOnWindow in CBInterface.cpp.
bool CPegasusDungeon::IsMouseOver()
{
	if (m_bShow == false)
		return false;

	DgUpdateOrigin();
	return SEASON3B::CheckMouseIn(DgX(0.f), DgY(0.f), DG_W, DG_H);
}
