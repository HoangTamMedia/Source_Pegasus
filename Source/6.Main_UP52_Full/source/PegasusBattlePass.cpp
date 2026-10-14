#include "stdafx.h"
#include "PegasusBattlePass.h"
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

CPegasusBattlePass gPegasusBattlePass;

namespace
{
	const char* const BP_DIR = "Interface\\Pegasus\\Pegasus_BattlePass\\";

	// Source rects, in texels. Every one of these is the art's real size, not
	// the padded texture's: OpenTga rounds up to a power of two and leaves the
	// extra rows uninitialised, so asking for more than the art is what draws
	// as a white block.
	const float BP_SRC_PANEL_W = 925.f, BP_SRC_PANEL_H = 706.f;
	const float BP_SRC_RAIL_W  = 902.f, BP_SRC_RAIL_H  = 39.f;
	const float BP_SRC_SEAL_W  = 104.f, BP_SRC_SEAL_H  = 100.f;
	const float BP_SRC_NODE    = 56.f;
	const float BP_SRC_BAR_W   = 296.f, BP_SRC_BAR_H   = 44.f;
	const float BP_SRC_SLOT    = 72.f;
	const float BP_SRC_BADGE   = 64.f;
	const float BP_SRC_ARROW_W = 32.f,  BP_SRC_ARROW_H = 28.f;

	// Window geometry, in its own space. The panel art is 925x706; drawing it
	// smaller keeps it clear of the HUD at 720-tall UI space.
	const float BP_W = 740.f, BP_H = 565.f;

	const float BP_TITLE_Y   = 26.f;
	const float BP_SEAL_X    = 40.f,  BP_SEAL_Y  = 52.f,  BP_SEAL_SZ = 62.f;
	const float BP_BAR_X     = 120.f, BP_BAR_Y   = 74.f;
	const float BP_BAR_W     = 480.f, BP_BAR_H   = 22.f;

	const float BP_RAIL_Y    = 300.f, BP_RAIL_H  = 26.f;
	const float BP_COL_PITCH = 128.f;
	const float BP_SLOT_SZ   = 74.f;
	const float BP_SLOT_GAP  = 34.f;	// from the rail to a slot's near edge
	const float BP_NODE_SZ   = 44.f;
	const float BP_BADGE_SZ  = 30.f;

	const float BP_ARROW_W = 34.f, BP_ARROW_H = 30.f;
	const float BP_ARROW_Y = 300.f;

	const float BP_CLOSE_SZ = 26.f;

	// Filled by Render, so the hit tests and the drawing can't drift apart.
	float g_fBpX = 0.f, g_fBpY = 0.f;

	inline float BpX(float x) { return g_fBpX + x; }
	inline float BpY(float y) { return g_fBpY + y; }

	void BpUpdateOrigin()
	{
		g_fBpX = ((float)DisplayWin - BP_W) * 0.5f;
		g_fBpY = ((float)DisplayHeight - BP_H) * 0.5f - 20.f;
	}

	// Same shape as the HUD's blit: RenderBitmap only binds, it never turns
	// texturing on, so the shared flag has to be set here or the quad comes out
	// as a flat block.
	void BpBlit(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, float fAlphaScale = 1.f)
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
		glColor4f(fAlphaScale, fAlphaScale, fAlphaScale, 1.f);

		RenderBitmap(iImage, x, y, w, h, 0.f, 0.f,
			fSrcW / pTex->Width, fSrcH / pTex->Height);

		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	void BpText(float x, float y, float w, const char* szText, DWORD dwColour,
		int iSort = RT3_SORT_CENTER)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(dwColour);
		g_pRenderText->RenderText((int)x, (int)y, szText, (int)w, 0, iSort);
		g_pRenderText->SetTextColor(CLRDW_WHITE);
	}
}


CPegasusBattlePass::CPegasusBattlePass()
	: m_bShow(false)
	, m_bLoaded(false)
	, m_iFirstOnView(0)
	, m_iLevel(0)
	, m_iPoints(0)
	, m_iPointsPerLevel(1000)
	, m_bPremium(false)
	, m_iHoverTier(-1)
	, m_bHoverPrem(false)
{
	FillMockSeason();
}

CPegasusBattlePass::~CPegasusBattlePass()
{
}

// Stand-in season until the server feeds this. Rewards cycle through a few
// real item types so the slots show actual icons rather than empty frames.
void CPegasusBattlePass::FillMockSeason()
{
	const int anFree[] = { ITEM_POTION + 0, ITEM_POTION + 1, ITEM_POTION + 2, ITEM_POTION + 3 };
	const int anPrem[] = { ITEM_POTION + 7, ITEM_POTION + 8, ITEM_POTION + 9, ITEM_POTION + 10 };

	for (int i = 0; i < TIER_COUNT; ++i)
	{
		m_Tier[i].iFreeType   = anFree[i % (sizeof(anFree) / sizeof(anFree[0]))];
		m_Tier[i].iFreeLevel  = 0;
		m_Tier[i].iPremType   = anPrem[i % (sizeof(anPrem) / sizeof(anPrem[0]))];
		m_Tier[i].iPremLevel  = 0;
		m_Tier[i].bFreeClaimed = false;
		m_Tier[i].bPremClaimed = false;
	}

	m_iLevel  = 7;
	m_iPoints = 430;

	for (int i = 0; i < m_iLevel && i < TIER_COUNT; ++i)
		m_Tier[i].bFreeClaimed = true;
}

void CPegasusBattlePass::LoadImages()
{
	if (m_bLoaded == true)
		return;

	char szPath[256];

	struct { const char* szFile; int iIndex; } aLoad[] =
	{
		{ "Pegasus_BattlePassUI01.tga", BITMAP_PG_BP_PANEL     },
		{ "Pegasus_BattlePassUI02.tga", BITMAP_PG_BP_RAIL      },
		{ "Pegasus_BattlePassUI03.tga", BITMAP_PG_BP_SEAL      },
		{ "Pegasus_BattlePassUI08.tga", BITMAP_PG_BP_NODE_OFF  },
		{ "Pegasus_BattlePassUI09.tga", BITMAP_PG_BP_NODE_ON   },
		{ "Pegasus_BattlePassUI36.tga", BITMAP_PG_BP_BAR_BACK  },
		{ "Pegasus_BattlePassUI35.tga", BITMAP_PG_BP_BAR_FILL  },
		{ "Pegasus_BattlePassUI37.tga", BITMAP_PG_BP_SLOT_FREE },
		{ "Pegasus_BattlePassUI38.tga", BITMAP_PG_BP_SLOT_PREM },
		{ "Pegasus_BattlePassUI41.tga", BITMAP_PG_BP_LOCK      },
		{ "Pegasus_BattlePassUI34.tga", BITMAP_PG_BP_CLAIMED   },
		{ "Pegasus_BattlePassUI33.tga", BITMAP_PG_BP_ARROW_L   },
		{ "Pegasus_BattlePassUI32.tga", BITMAP_PG_BP_ARROW_R   },
		{ "Pegasus_BattlePassUI29.tga", BITMAP_PG_BP_COMPLETE  },
	};

	for (int i = 0; i < sizeof(aLoad) / sizeof(aLoad[0]); ++i)
	{
		sprintf(szPath, "%s%s", BP_DIR, aLoad[i].szFile);
		LoadBitmap(szPath, aLoad[i].iIndex, GL_LINEAR);
	}

	m_bLoaded = true;
}

void CPegasusBattlePass::ReleaseImages()
{
	for (int i = BITMAP_PG_BP_BEGIN; i < BITMAP_PG_BP_END; ++i)
		DeleteBitmap(i);

	m_bLoaded = false;
}

void CPegasusBattlePass::Open()
{
	LoadImages();
	m_bShow = true;

	// Open on the page holding the current level rather than back at tier 1.
	m_iFirstOnView = m_iLevel - TIERS_ON_VIEW / 2;
	if (m_iFirstOnView < 0) m_iFirstOnView = 0;
	if (m_iFirstOnView > TIER_COUNT - TIERS_ON_VIEW)
		m_iFirstOnView = TIER_COUNT - TIERS_ON_VIEW;
}

void CPegasusBattlePass::Close()
{
	m_bShow = false;
}

void CPegasusBattlePass::Toggle()
{
	if (m_bShow) Close();
	else         Open();
}

void CPegasusBattlePass::RenderHeader(float x, float y, float w)
{
	char szBuf[128];

	BpText(x, BpY(BP_TITLE_Y), w, "BATTLE PASS", CLRDW_YELLOW);

	BpBlit(BITMAP_PG_BP_SEAL, BpX(BP_SEAL_X), BpY(BP_SEAL_Y),
		BP_SEAL_SZ, BP_SEAL_SZ, BP_SRC_SEAL_W, BP_SRC_SEAL_H);

	sprintf(szBuf, "%d", m_iLevel);
	BpText(BpX(BP_SEAL_X), BpY(BP_SEAL_Y + BP_SEAL_SZ * 0.32f), BP_SEAL_SZ,
		szBuf, CLRDW_WHITE);

	// Season bar. The fill is clipped rather than squashed so the gradient
	// stays put as it grows.
	BpBlit(BITMAP_PG_BP_BAR_BACK, BpX(BP_BAR_X), BpY(BP_BAR_Y),
		BP_BAR_W, BP_BAR_H, BP_SRC_BAR_W, BP_SRC_BAR_H);

	float fRatio = (m_iPointsPerLevel > 0)
		? (float)m_iPoints / (float)m_iPointsPerLevel : 0.f;
	if (fRatio < 0.f) fRatio = 0.f;
	if (fRatio > 1.f) fRatio = 1.f;

	if (fRatio > 0.f)
	{
		BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_BP_BAR_FILL);
		if (pTex != NULL && pTex->Width > 0.f)
		{
			if (TextureEnable == false)
			{
				TextureEnable = true;
				glEnable(GL_TEXTURE_2D);
			}
			EnableAlphaTest();
			glColor4f(1.f, 1.f, 1.f, 1.f);

			RenderBitmap(BITMAP_PG_BP_BAR_FILL,
				BpX(BP_BAR_X), BpY(BP_BAR_Y), BP_BAR_W * fRatio, BP_BAR_H,
				0.f, 0.f,
				BP_SRC_BAR_W * fRatio / pTex->Width,
				BP_SRC_BAR_H / pTex->Height);
		}
	}

	sprintf(szBuf, "%d / %d", m_iPoints, m_iPointsPerLevel);
	BpText(BpX(BP_BAR_X), BpY(BP_BAR_Y + 4.f), BP_BAR_W, szBuf, CLRDW_WHITE);

	BpText(x, BpY(BP_BAR_Y + BP_BAR_H + 8.f), w,
		m_bPremium ? "Premium track active" : "Free track", CLRDW_GRAY);
}

void CPegasusBattlePass::RenderTier(int iTier, float cx, float yRail)
{
	if (iTier < 0 || iTier >= TIER_COUNT)
		return;

	const TIER& t = m_Tier[iTier];
	bool bReached = (iTier < m_iLevel);

	char szBuf[64];

	// --- free reward, above the rail ---
	float fSlotX = cx - BP_SLOT_SZ * 0.5f;
	float fFreeY = yRail - BP_SLOT_GAP - BP_SLOT_SZ;

	BpBlit(BITMAP_PG_BP_SLOT_FREE, fSlotX, fFreeY, BP_SLOT_SZ, BP_SLOT_SZ,
		BP_SRC_SLOT, BP_SRC_SLOT, bReached ? 1.f : 0.55f);

	if (t.iFreeType >= 0)
	{
		PgRenderItemIcon(fSlotX + BP_SLOT_SZ * 0.18f, fFreeY + BP_SLOT_SZ * 0.18f,
			BP_SLOT_SZ * 0.64f, BP_SLOT_SZ * 0.64f, t.iFreeType, t.iFreeLevel);
	}

	if (t.bFreeClaimed == true)
	{
		BpBlit(BITMAP_PG_BP_CLAIMED,
			fSlotX + BP_SLOT_SZ - BP_BADGE_SZ * 0.7f,
			fFreeY + BP_SLOT_SZ - BP_BADGE_SZ * 0.7f,
			BP_BADGE_SZ, BP_BADGE_SZ, BP_SRC_BADGE, BP_SRC_BADGE);
	}

	// --- tier pip on the rail ---
	BpBlit(bReached ? BITMAP_PG_BP_NODE_ON : BITMAP_PG_BP_NODE_OFF,
		cx - BP_NODE_SZ * 0.5f, yRail + (BP_RAIL_H - BP_NODE_SZ) * 0.5f,
		BP_NODE_SZ, BP_NODE_SZ, BP_SRC_NODE, BP_SRC_NODE);

	sprintf(szBuf, "%d", iTier + 1);
	BpText(cx - BP_NODE_SZ * 0.5f, yRail + BP_RAIL_H * 0.5f - 7.f, BP_NODE_SZ,
		szBuf, bReached ? CLRDW_YELLOW : CLRDW_GRAY);

	// --- premium reward, below the rail ---
	float fPremY = yRail + BP_RAIL_H + BP_SLOT_GAP;

	BpBlit(BITMAP_PG_BP_SLOT_PREM, fSlotX, fPremY, BP_SLOT_SZ, BP_SLOT_SZ,
		BP_SRC_SLOT, BP_SRC_SLOT, (bReached && m_bPremium) ? 1.f : 0.55f);

	if (t.iPremType >= 0)
	{
		PgRenderItemIcon(fSlotX + BP_SLOT_SZ * 0.18f, fPremY + BP_SLOT_SZ * 0.18f,
			BP_SLOT_SZ * 0.64f, BP_SLOT_SZ * 0.64f, t.iPremType, t.iPremLevel);
	}

	// A padlock over the premium slot until the paid track is bought; once it
	// is, the tick takes over on anything already collected.
	if (m_bPremium == false)
	{
		BpBlit(BITMAP_PG_BP_LOCK,
			cx - BP_BADGE_SZ * 0.5f, fPremY + BP_SLOT_SZ * 0.5f - BP_BADGE_SZ * 0.5f,
			BP_BADGE_SZ, BP_BADGE_SZ, BP_SRC_BADGE, BP_SRC_BADGE);
	}
	else if (t.bPremClaimed == true)
	{
		BpBlit(BITMAP_PG_BP_CLAIMED,
			fSlotX + BP_SLOT_SZ - BP_BADGE_SZ * 0.7f,
			fPremY + BP_SLOT_SZ - BP_BADGE_SZ * 0.7f,
			BP_BADGE_SZ, BP_BADGE_SZ, BP_SRC_BADGE, BP_SRC_BADGE);
	}
}

void CPegasusBattlePass::Render()
{
	if (m_bShow == false)
		return;

	LoadImages();
	BpUpdateOrigin();

	BpBlit(BITMAP_PG_BP_PANEL, BpX(0.f), BpY(0.f), BP_W, BP_H,
		BP_SRC_PANEL_W, BP_SRC_PANEL_H);

	RenderHeader(BpX(0.f), BpY(0.f), BP_W);

	// Rail the tier pips sit on, spanning the columns.
	float yRail = BpY(BP_RAIL_Y);
	BpBlit(BITMAP_PG_BP_RAIL, BpX(30.f), yRail, BP_W - 60.f, BP_RAIL_H,
		BP_SRC_RAIL_W, BP_SRC_RAIL_H);

	float fFirstCx = BpX(BP_W * 0.5f) - BP_COL_PITCH * (TIERS_ON_VIEW - 1) * 0.5f;

	for (int i = 0; i < TIERS_ON_VIEW; ++i)
		RenderTier(m_iFirstOnView + i, fFirstCx + BP_COL_PITCH * i, yRail);

	// Paging arrows, dimmed at the ends of the season.
	bool bCanLeft  = (m_iFirstOnView > 0);
	bool bCanRight = (m_iFirstOnView < TIER_COUNT - TIERS_ON_VIEW);

	BpBlit(BITMAP_PG_BP_ARROW_L, BpX(18.f), BpY(BP_ARROW_Y),
		BP_ARROW_W, BP_ARROW_H, BP_SRC_ARROW_W, BP_SRC_ARROW_H,
		bCanLeft ? 1.f : 0.35f);

	BpBlit(BITMAP_PG_BP_ARROW_R, BpX(BP_W - 18.f - BP_ARROW_W), BpY(BP_ARROW_Y),
		BP_ARROW_W, BP_ARROW_H, BP_SRC_ARROW_W, BP_SRC_ARROW_H,
		bCanRight ? 1.f : 0.35f);

	// Season done? Stamp it.
	if (m_iLevel >= TIER_COUNT)
	{
		BpBlit(BITMAP_PG_BP_COMPLETE, BpX(BP_W * 0.5f - 90.f), BpY(BP_H - 190.f),
			180.f, 180.f, 400.f, 400.f);
	}

	BpText(BpX(0.f), BpY(BP_H - 34.f), BP_W,
		"Display only - rewards are not claimable yet", CLRDW_GRAY);

	// Last, so it sits over the columns. A slot shows a model and nothing else,
	// which is not enough to tell one jewel from another.
	if (m_iHoverTier >= 0 && m_iHoverTier < TIER_COUNT)
	{
		const TIER& t = m_Tier[m_iHoverTier];
		int iType  = m_bHoverPrem ? t.iPremType  : t.iFreeType;
		int iLevel = m_bHoverPrem ? t.iPremLevel : t.iFreeLevel;

		if (iType >= 0)
		{
			float cx = fFirstCx + BP_COL_PITCH * (m_iHoverTier - m_iFirstOnView);
			float y  = m_bHoverPrem
				? yRail + BP_RAIL_H + BP_SLOT_GAP
				: yRail - BP_SLOT_GAP - BP_SLOT_SZ;

			RenderTipText((int)cx, (int)(y - 14.f), BGetItemName(iType, iLevel));
		}
	}
}

bool CPegasusBattlePass::UpdateMouseEvent()
{
	if (m_bShow == false)
		return false;

	BpUpdateOrigin();
	m_iHoverTier = -1;

	// Reward slots. Nothing to click yet - this only feeds the name Render puts
	// over the slot - so it is tested first and does not consume the press.
	{
		float yRail   = BpY(BP_RAIL_Y);
		float fFirstCx = BpX(BP_W * 0.5f) - BP_COL_PITCH * (TIERS_ON_VIEW - 1) * 0.5f;
		float fFreeY  = yRail - BP_SLOT_GAP - BP_SLOT_SZ;
		float fPremY  = yRail + BP_RAIL_H + BP_SLOT_GAP;

		for (int i = 0; i < TIERS_ON_VIEW; ++i)
		{
			float fSlotX = fFirstCx + BP_COL_PITCH * i - BP_SLOT_SZ * 0.5f;

			if (SEASON3B::CheckMouseIn(fSlotX, fFreeY, BP_SLOT_SZ, BP_SLOT_SZ))
			{
				m_iHoverTier = m_iFirstOnView + i;
				m_bHoverPrem = false;
				break;
			}
			if (SEASON3B::CheckMouseIn(fSlotX, fPremY, BP_SLOT_SZ, BP_SLOT_SZ))
			{
				m_iHoverTier = m_iFirstOnView + i;
				m_bHoverPrem = true;
				break;
			}
		}
	}

	// Close box, top-right corner of the panel.
	if (SEASON3B::CheckMouseIn(BpX(BP_W - 46.f), BpY(16.f),
		BP_CLOSE_SZ, BP_CLOSE_SZ) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			Close();
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	if (SEASON3B::CheckMouseIn(BpX(18.f), BpY(BP_ARROW_Y),
		BP_ARROW_W, BP_ARROW_H) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON) && m_iFirstOnView > 0)
		{
			--m_iFirstOnView;
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	if (SEASON3B::CheckMouseIn(BpX(BP_W - 18.f - BP_ARROW_W), BpY(BP_ARROW_Y),
		BP_ARROW_W, BP_ARROW_H) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON)
			&& m_iFirstOnView < TIER_COUNT - TIERS_ON_VIEW)
		{
			++m_iFirstOnView;
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	// Anywhere else over the panel still counts as ours, so clicks don't fall
	// through and move the character.
	return SEASON3B::CheckMouseIn(BpX(0.f), BpY(0.f), BP_W, BP_H);
}

// Whether the cursor is over the panel at all. The world's click handling
// runs in the move phase, well before this window draws, so the answer has
// to be available there - see PegasusMouseOnWindow in CBInterface.cpp.
bool CPegasusBattlePass::IsMouseOver()
{
	if (m_bShow == false)
		return false;

	BpUpdateOrigin();
	return SEASON3B::CheckMouseIn(BpX(0.f), BpY(0.f), BP_W, BP_H);
}
