#include "stdafx.h"
#include "PegasusDaily.h"
#include "Protocol.h"			// PSBMSG_HEAD, DataSend
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

CPegasusDaily gPegasusDaily;

// Wire format. These three have to match GameServer\PegasusDaily.h byte for
// byte; the header is four bytes and every DWORD sits on a four byte boundary
// on both sides, which is what the pad fields are there to guarantee.
struct PMSG_DAILY_SYNC			// C1:F3:60, server -> client
{
	PSBMSG_HEAD header;
	BYTE  Today;
	BYTE  Premium;
	BYTE  pad[2];
	DWORD ClaimedMask;
};

struct PMSG_DAILY_CLAIM_REQ		// C1:F3:61, client -> server
{
	PSBMSG_HEAD header;
	BYTE Day;
};

struct PMSG_DAILY_CLAIM_RESULT	// C1:F3:62, server -> client
{
	PSBMSG_HEAD header;
	BYTE  Result;
	BYTE  Day;
	BYTE  pad[2];
	DWORD ClaimedMask;
};

// Both live in ZzzOpenglUtil.cpp with no header to declare them. They carry the
// interface-space to pixel scale that every 2D primitive here has to be fed in,
// and the batched quad below talks to glVertex2f directly.
extern float ConvertX(float x);
extern float ConvertY(float y);

namespace
{
	const char* const DA_DIR = "Interface\\Pegasus\\Pegasus_Daily\\";

	// Real art sizes. TournamentDaily_I1 ships at 1104x852, past the loader's
	// 1024 ceiling, so it never loaded - pg_daily_panel is that art cut down.
	// Everything else is under the limit and used as-is.
	const float DA_SRC_PANEL_W = 1024.f, DA_SRC_PANEL_H = 790.f;
	const float DA_SRC_CARD_W  = 144.f,  DA_SRC_CARD_H  = 164.f;
	const float DA_SRC_BTN_W   = 124.f,  DA_SRC_BTN_H   = 32.f;
	const float DA_SRC_ICON    = 72.f;

	// Window geometry, in its own space. 720 x 556 is the panel art's own
	// 1024 x 790 to within a pixel, so none of the frame's corners are stretched.
	const float DA_W = 720.f, DA_H = 556.f;

	// Where the panel art's own bands land once it is drawn at that size,
	// measured off the file rather than guessed at. Every block below hangs off
	// one of these. The grid used to start at y 62 and run to 486, which put it
	// straight over the band up top and the one down at the bottom - that band
	// was never an empty box, it was a covered one.
	const float DA_PLAQUE_Y = 26.f,  DA_PLAQUE_H = 31.f;	// title plate
	const float DA_HEAD_Y   = 60.f,  DA_HEAD_H   = 57.f;	// the band up top
	const float DA_BODY_Y   = 117.f, DA_BODY_H   = 337.f;	// room for the grid
	const float DA_SIDE     = 46.f;							// furniture margin

	// The 7 x 4 grid of day cards. Four rows have to fit inside DA_BODY_H, and
	// that ceiling is what sets the card height - the art's 144x164 would want
	// 100 and there is only room for 77.
	const float DA_GRID_Y     = 121.f;
	const float DA_CARD_W     = 80.f, DA_CARD_H     = 77.f;
	const float DA_CARD_GAP_X = 10.f, DA_CARD_GAP_Y =  8.f;

	// Inside a card: a tab across the top for the milestone days, the reward in
	// the middle, the day number on a plate at the foot.
	const float DA_TAB_H  = 13.f;
	const float DA_NUM_H  = 16.f;
	const float DA_INSET  = 3.f;	// clear of the card art's own bevel

	// Header furniture: one line of text, then the month as a strip of cells.
	const float DA_HEAD_TEXT_Y = 70.f;
	const float DA_BAR_Y = 92.f, DA_BAR_H = 14.f, DA_BAR_GAP = 2.f;

	// The claim button, sitting on the lower band. Wide enough to carry a
	// sentence, tall enough to read as the thing the window is for.
	const float DA_CLAIM_W = 264.f, DA_CLAIM_H = 42.f;
	const float DA_CLAIM_X = (DA_W - DA_CLAIM_W) * 0.5f, DA_CLAIM_Y = 456.f;

	// The status strip along the bottom, on the panel's own footer.
	const float DA_FOOT_Y = 502.f, DA_FOOT_H = 26.f;
	const float DA_CHIP_W = 4.f;	// colour chip at its left edge

	const float DA_CLOSE_SZ = 26.f;

	// One clock for everything that moves, so today's card and the button
	// breathe together instead of drifting apart. WorldTime is milliseconds;
	// this rate puts a full breath at about a second and a half.
	const float DA_PULSE_RATE = 0.0042f;

	// The state palette. These five carry the whole read of the calendar and
	// are the first thing to reach for if it wants a different mood.
	const float DA_C_LOCKED [3] = { 0.34f, 0.42f, 0.56f };	// cool, quiet
	const float DA_C_READY  [3] = { 1.00f, 0.80f, 0.30f };	// gold, the focus
	const float DA_C_CLAIMED[3] = { 0.36f, 0.70f, 0.44f };	// green, collected
	const float DA_C_MISSED [3] = { 0.62f, 0.26f, 0.26f };	// dull red, gone
	const float DA_C_VIP    [3] = { 0.80f, 0.47f, 1.00f };	// orchid, milestone
	const float DA_C_INK    [3] = { 0.05f, 0.06f, 0.09f };	// plates and scrims
	const float DA_C_HILITE [3] = { 1.00f, 1.00f, 1.00f };	// hover rim

	float g_fDaX = 0.f, g_fDaY = 0.f;

	inline float DaX(float x) { return g_fDaX + x; }
	inline float DaY(float y) { return g_fDaY + y; }

	void DaUpdateOrigin()
	{
		g_fDaX = ((float)DisplayWin - DA_W) * 0.5f;
		g_fDaY = ((float)DisplayHeight - DA_H) * 0.5f - 16.f;
	}

	// RenderColor opens and tears down GL state around a single rectangle,
	// which is the wrong shape for the dozens of small quads a stroked mark
	// costs. This opens that state once and pours flat quads into one glBegin -
	// the same maths RenderColor uses, minus the per-rectangle teardown. A
	// window that redraws every frame cannot afford the other way round.
	void DaQuadBegin()
	{
		EnableAlphaTest();
		DisableTexture();
		glBegin(GL_QUADS);
	}

	void DaQuad(float x, float y, float w, float h, const float* c, float a)
	{
		if (a <= 0.f || w <= 0.f || h <= 0.f)
			return;

		x = ConvertX(x);
		w = ConvertX(w);
		h = ConvertY(h);
		y = (float)WindowHeight - ConvertY(y);

		glColor4f(c[0], c[1], c[2], a);

		glVertex2f(x,     y);
		glVertex2f(x,     y - h);
		glVertex2f(x + w, y - h);
		glVertex2f(x + w, y);
	}

	void DaQuadEnd()
	{
		glEnd();
		glColor4f(1.f, 1.f, 1.f, 1.f);

		// Hand back the state the rest of the window draws in. DisableTexture
		// turned off texturing and the alpha test on the way in, and neither
		// the blits nor CUIRenderText turn them back on for themselves - the
		// text path binds its atlas and trusts whatever the caller left. The
		// shared flag is set alongside so the next blit is not left guessing.
		TextureEnable = true;
		glEnable(GL_TEXTURE_2D);
		EnableAlphaTest();
	}

	void DaFill(float x, float y, float w, float h, const float* c, float a)
	{
		DaQuadBegin();
		DaQuad(x, y, w, h, c, a);
		DaQuadEnd();
	}

	// A hollow rectangle, as four bars, so whatever is already inside it stays.
	void DaFrame(float x, float y, float w, float h, float t,
		const float* c, float a)
	{
		DaQuadBegin();
		DaQuad(x,         y,         w, t,             c, a);
		DaQuad(x,         y + h - t, w, t,             c, a);
		DaQuad(x,         y + t,     t, h - t * 2.f,   c, a);
		DaQuad(x + w - t, y + t,     t, h - t * 2.f,   c, a);
		DaQuadEnd();
	}

	// A line at any angle, walked as a run of small squares. The only primitive
	// to hand draws axis-aligned rectangles, so a diagonal has to be stepped -
	// cheap at this size, and it buys every state mark a colour the window
	// picks rather than the one baked into the art. Call between Begin and End.
	void DaStrokeInto(float x0, float y0, float x1, float y1, float t,
		const float* c, float a)
	{
		const float dx = x1 - x0, dy = y1 - y0;
		const float fLen = sqrtf(dx * dx + dy * dy);

		if (fLen <= 0.f || t <= 0.f)
			return;

		int iSteps = (int)(fLen / (t * 0.55f));
		if (iSteps < 1)  iSteps = 1;
		if (iSteps > 64) iSteps = 64;

		for (int i = 0; i <= iSteps; ++i)
		{
			const float f = (float)i / (float)iSteps;
			DaQuad(x0 + dx * f - t * 0.5f, y0 + dy * f - t * 0.5f, t, t, c, a);
		}
	}

	void DaBlitC(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, float r, float g, float b, float a)
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
		glColor4f(r, g, b, a);

		RenderBitmap(iImage, x, y, w, h, 0.f, 0.f,
			fSrcW / pTex->Width, fSrcH / pTex->Height);

		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	void DaBlit(int iImage, float x, float y, float w, float h,
		float fSrcW, float fSrcH, float fLevel = 1.f)
	{
		DaBlitC(iImage, x, y, w, h, fSrcW, fSrcH, fLevel, fLevel, fLevel, 1.f);
	}

	void DaText(float x, float y, float w, const char* szText, DWORD dwColour,
		int iSort = RT3_SORT_CENTER)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(dwColour);
		g_pRenderText->RenderText((int)x, (int)y, szText, (int)w, 0, iSort);
		g_pRenderText->SetTextColor(CLRDW_WHITE);
	}

	const float* DaStateColour(CPegasusDaily::eDayState eState)
	{
		switch (eState)
		{
		case CPegasusDaily::DAY_READY:   return DA_C_READY;
		case CPegasusDaily::DAY_CLAIMED: return DA_C_CLAIMED;
		case CPegasusDaily::DAY_MISSED:  return DA_C_MISSED;
		default:                         break;
		}

		return DA_C_LOCKED;
	}

	// The reward each day pays out, mirroring Data\Custom\DailyReward.txt.
	// The sync packet carries state and not contents, so the cards have to get
	// their icons from somewhere - and shipping the table with the client is
	// what lets a locked day still show what it is worth waiting for.
	//
	// Item numbering is the same on both sides: type * 512 + index, which is
	// ITEM_POTION + index for the whole of this table.
	struct DA_REWARD { int iIndex; bool bPremium; };

	const DA_REWARD DA_REWARD_TABLE[CPegasusDaily::DAY_COUNT] =
	{
		{ 13, false }, { 13, false }, { 14, false }, { 14, false },
		{ 16, false }, { 16, false }, { 22, true  },

		{ 13, false }, { 13, false }, { 14, false }, { 14, false },
		{ 16, false }, { 16, false }, { 22, true  },

		{ 13, false }, { 13, false }, { 14, false }, { 14, false },
		{ 16, false }, { 16, false }, { 22, true  },

		{ 13, false }, { 13, false }, { 14, false }, { 14, false },
		{ 16, false }, { 16, false }, { 31, true  },
	};

	// What the footer says after a refused claim. The server decides, so the
	// window's job here is only to report the reason honestly.
	const char* DaResultText(int iResult)
	{
		switch (iResult)
		{
		case DAILY_OK:              return "Reward claimed";
		case DAILY_ERR_NOT_TODAY:   return "That day is not today's";
		case DAILY_ERR_ALREADY:     return "Already claimed";
		case DAILY_ERR_PREMIUM:     return "VIP accounts only";
		case DAILY_ERR_NO_ROOM:     return "No room in the inventory";
		case DAILY_ERR_DISABLED:    return "Daily rewards are unavailable";
		}

		return "";
	}

	// Card rect for a day index, in window space. The grid is centred.
	void DaCardRect(int i, float& x, float& y)
	{
		int c = i % CPegasusDaily::DAYS_PER_ROW;
		int r = i / CPegasusDaily::DAYS_PER_ROW;

		float fRowW = DA_CARD_W * CPegasusDaily::DAYS_PER_ROW
			+ DA_CARD_GAP_X * (CPegasusDaily::DAYS_PER_ROW - 1);

		x = (DA_W - fRowW) * 0.5f + (DA_CARD_W + DA_CARD_GAP_X) * c;
		y = DA_GRID_Y + (DA_CARD_H + DA_CARD_GAP_Y) * r;
	}
}


CPegasusDaily::CPegasusDaily()
	: m_bShow(false)
	, m_bLoaded(false)
	, m_bPremium(false)
	, m_bSynced(false)
	, m_iToday(0)
	, m_iHoverDay(-1)
	, m_bHoverClaim(false)
	, m_iLastResult(-1)
	, m_dwClaimedMask(0)
{
	BuildRewardTable();
	ApplyServerState();
}

CPegasusDaily::~CPegasusDaily()
{
}

// The contents of the calendar, which never change at runtime.
void CPegasusDaily::BuildRewardTable()
{
	for (int i = 0; i < DAY_COUNT; ++i)
	{
		DAY& d = m_Day[i];

		d.iItemType  = ITEM_POTION + DA_REWARD_TABLE[i].iIndex;
		d.iItemLevel = 0;
		d.iIcon      = i % 3;
		d.bPremium   = DA_REWARD_TABLE[i].bPremium;
		d.eState     = DAY_LOCKED;
	}
}

// The state of the calendar, which only the server sets.
//
// A day before today that was never taken reads as missed rather than locked -
// the player cannot go back for it, and saying so is the whole point of the
// streak. Today itself can already be claimed, in which case it stops being
// the button's target until the date rolls over.
void CPegasusDaily::ApplyServerState()
{
	for (int i = 0; i < DAY_COUNT; ++i)
	{
		bool bTaken = (m_dwClaimedMask & (1u << i)) != 0;

		if (bTaken == true)
			m_Day[i].eState = DAY_CLAIMED;
		else if (i < m_iToday)
			m_Day[i].eState = DAY_MISSED;
		else if (i == m_iToday)
			m_Day[i].eState = DAY_READY;
		else
			m_Day[i].eState = DAY_LOCKED;
	}
}

// 0xF3:0x60. The whole calendar in one message, so there is no incremental
// state to fall out of step.
void CPegasusDaily::OnSyncPacket(const BYTE* pRecv)
{
	if (pRecv == NULL)
		return;

	const PMSG_DAILY_SYNC* pMsg = (const PMSG_DAILY_SYNC*)pRecv;

	// A Today outside the calendar would index m_Day out of bounds further
	// down, and the window has no business trusting a number just because it
	// arrived over the wire.
	m_iToday = (int)pMsg->Today;
	if (m_iToday < 0 || m_iToday >= DAY_COUNT)
		m_iToday = 0;

	m_bPremium      = (pMsg->Premium != 0);
	m_dwClaimedMask = pMsg->ClaimedMask;
	m_bSynced       = true;
	m_iLastResult   = -1;

	ApplyServerState();
}

// 0xF3:0x62. The result carries the new mask, so the calendar is rebuilt from
// authority instead of from an assumption about what the click did.
void CPegasusDaily::OnClaimResultPacket(const BYTE* pRecv)
{
	if (pRecv == NULL)
		return;

	const PMSG_DAILY_CLAIM_RESULT* pMsg = (const PMSG_DAILY_CLAIM_RESULT*)pRecv;

	m_iLastResult   = (int)pMsg->Result;
	m_dwClaimedMask = pMsg->ClaimedMask;
	m_bSynced       = true;

	ApplyServerState();

	if (pMsg->Result == DAILY_OK)
		PlayBuffer(SOUND_CLICK01);
}

// A dropped connection has to leave nothing behind, or the next account to log
// in on this client sees the previous one's calendar until its own sync lands.
void CPegasusDaily::Reset()
{
	m_bSynced       = false;
	m_bPremium      = false;
	m_iToday        = 0;
	m_iHoverDay     = -1;
	m_bHoverClaim   = false;
	m_iLastResult   = -1;
	m_dwClaimedMask = 0;

	ApplyServerState();
}

// How many of the 28 have been taken. Header furniture only - the states it
// counts are themselves derived from the server's mask, so this reports rather
// than decides.
int CPegasusDaily::ClaimedCount() const
{
	int iCount = 0;

	for (int i = 0; i < DAY_COUNT; ++i)
	{
		if (m_Day[i].eState == DAY_CLAIMED)
			++iCount;
	}

	return iCount;
}

void CPegasusDaily::LoadImages()
{
	if (m_bLoaded == true)
		return;

	char szPath[256];

	struct { const char* szFile; int iIndex; } aLoad[] =
	{
		{ "pg_daily_panel.tga",     BITMAP_PG_DA_PANEL     },
		{ "TournamentDaily_I2.tga", BITMAP_PG_DA_CARD      },
		{ "TournamentDaily_I3.tga", BITMAP_PG_DA_BTN_ON    },
		{ "TournamentDaily_I4.tga", BITMAP_PG_DA_BTN_HOT   },
		{ "TournamentDaily_I5.tga", BITMAP_PG_DA_BTN_OFF   },

		// These three are loaded and no longer drawn, which is deliberate
		// rather than an oversight. I6 is a diagonal "ONLY FOR PREMIUM MEMBERS"
		// ribbon that has nowhere to sit in this layout without covering a row
		// of cards - the button and the status strip say it in words instead.
		// I7 and I8 are the cross and the tick, and both are a dark red: since
		// glColor multiplies, a claimed day drawn with I8 can only ever look
		// like a day that went wrong. RenderCard draws both marks itself, in
		// the state's own colour. Kept here so restoring any of them is a
		// question of drawing it again rather than finding it again.
		{ "TournamentDaily_I6.tga", BITMAP_PG_DA_BANNER    },
		{ "TournamentDaily_I7.tga", BITMAP_PG_DA_MISSED    },
		{ "TournamentDaily_I8.tga", BITMAP_PG_DA_CLAIMED   },

		{ "TournamentDaily_I9.tga",  BITMAP_PG_DA_ICON     },
		{ "TournamentDaily_I10.tga", BITMAP_PG_DA_ICON + 1 },
		{ "TournamentDaily_I11.tga", BITMAP_PG_DA_ICON + 2 },
	};

	for (int i = 0; i < sizeof(aLoad) / sizeof(aLoad[0]); ++i)
	{
		sprintf(szPath, "%s%s", DA_DIR, aLoad[i].szFile);
		LoadBitmap(szPath, aLoad[i].iIndex, GL_LINEAR);
	}

	m_bLoaded = true;
}

void CPegasusDaily::ReleaseImages()
{
	for (int i = BITMAP_PG_DA_BEGIN; i < BITMAP_PG_DA_END; ++i)
		DeleteBitmap(i);

	m_bLoaded = false;
}

void CPegasusDaily::Open()
{
	LoadImages();
	m_bShow = true;
}

void CPegasusDaily::Close()
{
	m_bShow = false;
}

void CPegasusDaily::Toggle()
{
	if (m_bShow) Close();
	else         Open();
}

// Asks. It does not tell: the packet carries a day index and nothing else, and
// the server is free to refuse it. Nothing on this side is marked claimed here
// - that only happens when 0x62 comes back with a mask.
void CPegasusDaily::OnClaim(int iDay)
{
	if (iDay < 0 || iDay >= DAY_COUNT)
		return;

	if (m_bSynced == false)
		return;

	PMSG_DAILY_CLAIM_REQ pMsg;

	pMsg.header.set(0xF3, 0x61, sizeof(pMsg));

	pMsg.Day = (BYTE)iDay;

	DataSend((BYTE*)&pMsg, pMsg.header.size);

	PlayBuffer(SOUND_CLICK01);
}

// The band across the top of the panel. It was reading as a large empty box
// because the grid was drawn over the top of it; what it is actually for is the
// three things a wall of identical cards cannot say for itself - how far into
// the month the player is, how much of it was really taken, and where each of
// the 28 days stands.
void CPegasusDaily::RenderHeader(float fPulse)
{
	char szBuf[128];

	const float xL = DaX(DA_SIDE);
	const float fW = DA_W - DA_SIDE * 2.f;

	if (m_bSynced == false)
	{
		DaText(xL, DaY(DA_HEAD_Y + DA_HEAD_H * 0.5f - 7.f), fW,
			"Waiting for the server", CLRDW_GRAY);
		return;
	}

	// Left, where the streak stands. Right, what has actually been collected -
	// the two are not the same number, and the gap between them is the point.
	sprintf(szBuf, "DAY %d OF %d", m_iToday + 1, DAY_COUNT);
	DaText(xL, DaY(DA_HEAD_TEXT_Y), fW * 0.5f, szBuf,
		CLRDW_BR_ORANGE, RT3_SORT_LEFT);

	// RT3_SORT_RIGHT takes the box's right edge rather than its left, which is
	// what lands this hard against the same margin the strip below ends on.
	sprintf(szBuf, "%d OF %d CLAIMED", ClaimedCount(), DAY_COUNT);
	DaText(xL + fW, DaY(DA_HEAD_TEXT_Y), fW * 0.5f, szBuf,
		CLRDW_BR_GRAY, RT3_SORT_RIGHT);

	// The month as 28 cells, each in its own day's colour. A single filled bar
	// would have to pretend the claimed days run in one piece from the left,
	// and a mask with holes in it is exactly what this window exists to show.
	const float fCellW =
		(fW - DA_BAR_GAP * (float)(DAY_COUNT - 1)) / (float)DAY_COUNT;

	DaFill(xL - 2.f, DaY(DA_BAR_Y) - 2.f, fW + 4.f, DA_BAR_H + 4.f,
		DA_C_INK, 0.75f);

	DaQuadBegin();

	for (int i = 0; i < DAY_COUNT; ++i)
	{
		const float cx = xL + (fCellW + DA_BAR_GAP) * (float)i;
		const float* c = DaStateColour(m_Day[i].eState);

		float fA = 0.26f;						// locked, barely there
		if (m_Day[i].eState == DAY_CLAIMED)     fA = 0.95f;
		else if (m_Day[i].eState == DAY_READY)  fA = 0.55f + 0.45f * fPulse;
		else if (m_Day[i].eState == DAY_MISSED) fA = 0.55f;

		DaQuad(cx, DaY(DA_BAR_Y), fCellW, DA_BAR_H, c, fA);

		// The four milestones keep their colour even while the day is still
		// locked, so the eye can find them along the strip.
		if (m_Day[i].bPremium == true)
			DaQuad(cx, DaY(DA_BAR_Y), fCellW, 3.f, DA_C_VIP, 0.90f);
	}

	DaQuadEnd();

	DaFrame(xL - 2.f, DaY(DA_BAR_Y) - 2.f, fW + 4.f, DA_BAR_H + 4.f, 1.f,
		DA_C_LOCKED, 0.45f);
}

// One day card, built up in layers. The card art is opaque and near-black, so
// glColor can only ever take brightness away from it - which is why every state
// here is laid over the top of the art rather than pushed through it, and why
// twenty-eight of them used to look like one.
void CPegasusDaily::RenderCard(int iDay, float fPulse)
{
	if (iDay < 0 || iDay >= DAY_COUNT)
		return;

	const DAY&   d       = m_Day[iDay];
	const bool   bHover  = (iDay == m_iHoverDay);
	const float* c       = DaStateColour(d.eState);

	char szBuf[32];

	float x, y;
	DaCardRect(iDay, x, y);
	x = DaX(x);
	y = DaY(y);

	// Locked is the quiet default; the other three each take it somewhere.
	float fLevel = 0.50f;	// how much of the card art survives
	float fWash  = 0.10f;	// the state's colour, laid over it
	float fScrim = 0.30f;	// ink over the reward model
	float fEdge  = 1.f;		// frame thickness
	float fEdgeA = 0.45f;

	switch (d.eState)
	{
	case DAY_READY:
		fLevel = 1.00f; fWash = 0.14f; fScrim = 0.00f;
		fEdge  = 2.00f; fEdgeA = 0.80f + 0.20f * fPulse;
		break;

	case DAY_CLAIMED:
		fLevel = 0.62f; fWash = 0.13f; fScrim = 0.22f;
		fEdge  = 2.00f; fEdgeA = 0.70f;
		break;

	case DAY_MISSED:
		fLevel = 0.38f; fWash = 0.12f; fScrim = 0.48f;
		fEdge  = 2.00f; fEdgeA = 0.60f;
		break;

	default:
		break;
	}

	if (bHover == true)
	{
		fLevel += 0.22f; if (fLevel > 1.f) fLevel = 1.f;
		fWash  += 0.07f;
		fScrim -= 0.12f; if (fScrim < 0.f) fScrim = 0.f;
		fEdgeA += 0.25f; if (fEdgeA > 1.f) fEdgeA = 1.f;
	}

	// Today's halo, drawn outside the card so it costs the card nothing. Three
	// rings falling away, breathing on the window's one clock. A milestone
	// already owns the first ring out, so the halo starts beyond it.
	if (d.eState == DAY_READY)
	{
		const float fBase = (d.bPremium == true) ? 5.f : 2.f;

		for (int k = 0; k < 3; ++k)
		{
			const float f = fBase + 3.f * (float)k;

			DaFrame(x - f, y - f, DA_CARD_W + f * 2.f, DA_CARD_H + f * 2.f, 2.f,
				c, (0.30f - 0.08f * (float)k) * (0.40f + 0.60f * fPulse));
		}
	}

	DaBlit(BITMAP_PG_DA_CARD, x, y, DA_CARD_W, DA_CARD_H,
		DA_SRC_CARD_W, DA_SRC_CARD_H, fLevel);

	DaFill(x + DA_INSET, y + DA_INSET,
		DA_CARD_W - DA_INSET * 2.f, DA_CARD_H - DA_INSET * 2.f, c, fWash);

	// The reward. A real item model, with the calendar glyph as the fallback
	// for a day that has nothing attached to it.
	const float fIconW = DA_CARD_W * 0.62f;
	const float fIconH = 38.f;
	const float fIconX = x + (DA_CARD_W - fIconW) * 0.5f;
	const float fIconY = y + DA_INSET + DA_TAB_H + 1.f;

	if (d.iItemType >= 0)
	{
		PgRenderItemIcon(fIconX, fIconY, fIconW, fIconH, d.iItemType, d.iItemLevel);
	}
	else
	{
		DaBlit(BITMAP_PG_DA_ICON + d.iIcon, fIconX, fIconY, fIconW, fIconH,
			DA_SRC_ICON, DA_SRC_ICON, fLevel);
	}

	// RenderItem3D forces its own white, so a spent day can only be dimmed by
	// laying something over the model after the fact.
	if (fScrim > 0.f)
	{
		DaFill(x + DA_INSET, y + DA_INSET,
			DA_CARD_W - DA_INSET * 2.f, DA_CARD_H - DA_INSET * 2.f,
			DA_C_INK, fScrim);
	}

	// State marks, drawn rather than blitted. Both mark files are a dark red
	// and glColor multiplies, so neither could ever have been made to read as
	// anything but a refusal - a claimed day wants to look collected, not
	// wrong. Drawing them buys the colour back.
	const float mx = x + DA_CARD_W * 0.5f;
	const float my = fIconY + fIconH * 0.5f;

	if (d.eState == DAY_CLAIMED)
	{
		const float s = DA_CARD_W * 0.16f;

		DaQuadBegin();

		// A dark pass first, so the tick holds its shape over a bright reward.
		DaStrokeInto(mx - s * 1.30f, my, mx - s * 0.35f, my + s * 0.95f,
			6.f, DA_C_INK, 0.80f);
		DaStrokeInto(mx - s * 0.35f, my + s * 0.95f, mx + s * 1.35f, my - s,
			6.f, DA_C_INK, 0.80f);

		DaStrokeInto(mx - s * 1.30f, my, mx - s * 0.35f, my + s * 0.95f,
			3.f, c, 1.f);
		DaStrokeInto(mx - s * 0.35f, my + s * 0.95f, mx + s * 1.35f, my - s,
			3.f, c, 1.f);

		DaQuadEnd();
	}
	else if (d.eState == DAY_MISSED)
	{
		// Struck corner to corner. A cross in the middle only says something is
		// wrong; a line through the whole card says the day is not coming back.
		DaQuadBegin();

		DaStrokeInto(x + 5.f, y + 5.f,
			x + DA_CARD_W - 5.f, y + DA_CARD_H - 5.f, 3.f, c, 0.85f);
		DaStrokeInto(x + DA_CARD_W - 5.f, y + 5.f,
			x + 5.f, y + DA_CARD_H - 5.f, 3.f, c, 0.85f);

		DaQuadEnd();
	}

	// Day number, on a plate of its own so it reads over whatever is behind it.
	DaFill(x + DA_INSET, y + DA_CARD_H - DA_INSET - DA_NUM_H,
		DA_CARD_W - DA_INSET * 2.f, DA_NUM_H, DA_C_INK, 0.72f);

	sprintf(szBuf, "%d", iDay + 1);
	DaText(x, y + DA_CARD_H - DA_INSET - DA_NUM_H + 1.f, DA_CARD_W, szBuf,
		(d.eState == DAY_READY) ? CLRDW_BR_ORANGE
		: (d.eState == DAY_CLAIMED) ? CLRDW_BR_GRAY : CLRDW_GRAY);

	// Premium days are the month's milestones, so they get a tier of their own
	// rather than a word: a second frame outside the state's own, in a colour
	// used nowhere else in the window, and a tab across the head of the card.
	if (d.bPremium == true)
	{
		DaFrame(x - 3.f, y - 3.f, DA_CARD_W + 6.f, DA_CARD_H + 6.f, 2.f,
			DA_C_VIP, bHover ? 0.95f : 0.75f);

		DaFill(x + DA_INSET, y + DA_INSET, DA_CARD_W - DA_INSET * 2.f, DA_TAB_H,
			DA_C_VIP, m_bPremium ? 0.85f : 0.55f);

		DaText(x, y + DA_INSET - 1.f, DA_CARD_W,
			m_bPremium ? "VIP" : "VIP ONLY", CLRDW_WHITE);
	}

	// The frame last, so it closes over every layer under it.
	DaFrame(x, y, DA_CARD_W, DA_CARD_H, fEdge, c, fEdgeA);

	// Hover puts a thin light rim just inside that frame, which reads as a lift
	// without arguing with the colour the card is already saying.
	if (bHover == true)
	{
		DaFrame(x + fEdge, y + fEdge, DA_CARD_W - fEdge * 2.f,
			DA_CARD_H - fEdge * 2.f, 1.f, DA_C_HILITE, 0.55f);
	}
}

// The button greys itself out on the same conditions the server enforces, which
// is a courtesy and not a check: pressing it anyway only produces a refusal,
// because the decision is not made here. What is new is that the four outcomes
// no longer share one look and differ only in their wording.
void CPegasusDaily::RenderClaimButton(float fPulse)
{
	const DAY& dToday = m_Day[m_iToday];

	const bool bVipLock  = (dToday.bPremium == true && m_bPremium == false);
	const bool bDone     = (m_bSynced == true && dToday.eState == DAY_CLAIMED);
	const bool bCanClaim = (m_bSynced == true)
		&& (dToday.eState == DAY_READY)
		&& (bVipLock == false);

	const float x = DaX(DA_CLAIM_X), y = DaY(DA_CLAIM_Y);

	char szLabel[64];
	const float* c = DA_C_LOCKED;
	DWORD dwInk = CLRDW_GRAY;

	if (m_bSynced == false)
	{
		strcpy(szLabel, "WAITING FOR SERVER");
	}
	else if (bCanClaim == true)
	{
		c = DA_C_READY;
		dwInk = CLRDW_WHITE;
		sprintf(szLabel, "CLAIM DAY %d", m_iToday + 1);
	}
	else if (bVipLock == true)
	{
		c = DA_C_VIP;
		dwInk = CLRDW_BR_YELLOW;
		strcpy(szLabel, "VIP ACCOUNTS ONLY");
	}
	else if (bDone == true)
	{
		c = DA_C_CLAIMED;
		dwInk = CLRDW_BR_GRAY;
		strcpy(szLabel, "CLAIMED - BACK TOMORROW");
	}
	else
	{
		strcpy(szLabel, "COME BACK TOMORROW");
	}

	// Only the claimable state glows. That is what separates "you can press
	// this" from "this is a label shaped like a button".
	if (bCanClaim == true)
	{
		for (int k = 0; k < 3; ++k)
		{
			const float f = 3.f + 3.f * (float)k;

			DaFrame(x - f, y - f, DA_CLAIM_W + f * 2.f, DA_CLAIM_H + f * 2.f,
				2.f, c, (0.26f - 0.07f * (float)k) * (0.35f + 0.65f * fPulse));
		}
	}

	// I4 is the same plate as I3 in a brighter red, which is what it was drawn
	// for and what nothing has ever used it as until now.
	int iArt = BITMAP_PG_DA_BTN_OFF;

	if (bCanClaim == true)
		iArt = (m_bHoverClaim == true) ? BITMAP_PG_DA_BTN_HOT : BITMAP_PG_DA_BTN_ON;

	DaBlit(iArt, x, y, DA_CLAIM_W, DA_CLAIM_H, DA_SRC_BTN_W, DA_SRC_BTN_H,
		bCanClaim ? 1.f : 0.70f);

	DaFill(x + 2.f, y + 2.f, DA_CLAIM_W - 4.f, DA_CLAIM_H - 4.f, c,
		bCanClaim ? (0.10f + 0.06f * fPulse) : 0.13f);

	DaFrame(x, y, DA_CLAIM_W, DA_CLAIM_H, 2.f, c,
		(m_bHoverClaim == true) ? 0.95f : 0.72f);

	if (m_bHoverClaim == true)
	{
		DaFrame(x + 2.f, y + 2.f, DA_CLAIM_W - 4.f, DA_CLAIM_H - 4.f, 1.f,
			DA_C_HILITE, 0.45f);
	}

	DaText(x, y + DA_CLAIM_H * 0.5f - 7.f, DA_CLAIM_W, szLabel, dwInk);
}

// The strip along the bottom. It carries the server's last word on a claim, so
// a refusal has somewhere to land instead of the click just appearing to do
// nothing - and when there is no last word yet it is worth more as a note of
// what is waiting today than as an empty bar.
void CPegasusDaily::RenderStatus()
{
	const float x = DaX(DA_SIDE), y = DaY(DA_FOOT_Y);
	const float w = DA_W - DA_SIDE * 2.f;

	char szBuf[192];

	const float* c = DA_C_LOCKED;
	DWORD dwInk = CLRDW_BR_GRAY;

	if (m_bSynced == false)
	{
		strcpy(szBuf, "Waiting for the server");
		dwInk = CLRDW_GRAY;
	}
	else if (m_iLastResult >= 0)
	{
		strcpy(szBuf, DaResultText(m_iLastResult));

		if (m_iLastResult == DAILY_OK)
		{
			c = DA_C_CLAIMED;
			dwInk = CLRDW_BR_YELLOW;
		}
		else
		{
			c = DA_C_MISSED;
			dwInk = CLRDW_BR_GRAY;
		}
	}
	else if (m_Day[m_iToday].bPremium == true && m_bPremium == false)
	{
		c = DA_C_VIP;
		strcpy(szBuf, "Today is a milestone day - VIP accounts only");
	}
	else if (m_Day[m_iToday].iItemType >= 0)
	{
		c = (m_Day[m_iToday].eState == DAY_READY) ? DA_C_READY : DA_C_CLAIMED;

		sprintf(szBuf, "Today's reward: %s",
			BGetItemName(m_Day[m_iToday].iItemType, m_Day[m_iToday].iItemLevel));
	}
	else
	{
		szBuf[0] = '\0';
	}

	DaFill(x, y, w, DA_FOOT_H, DA_C_INK, 0.70f);
	DaFrame(x, y, w, DA_FOOT_H, 1.f, c, 0.45f);
	DaFill(x, y, DA_CHIP_W, DA_FOOT_H, c, 0.95f);

	if (szBuf[0] != '\0')
	{
		DaText(x + DA_CHIP_W + 10.f, y + DA_FOOT_H * 0.5f - 7.f,
			w - DA_CHIP_W - 20.f, szBuf, dwInk, RT3_SORT_LEFT);
	}
}

void CPegasusDaily::Render()
{
	if (m_bShow == false)
		return;

	LoadImages();
	DaUpdateOrigin();

	// One breath for the whole window. WorldTime is in milliseconds, and this
	// lands in 0..1 so every caller can lean on it however it likes.
	const float fPulse = 0.5f + 0.5f * sinf((float)WorldTime * DA_PULSE_RATE);

	// The frame art has an open middle, so lay an opaque ground first or the
	// world shows straight through the window.
	DaFill(DaX(8.f), DaY(8.f), DA_W - 16.f, DA_H - 16.f, DA_C_INK, 0.95f);

	DaBlit(BITMAP_PG_DA_PANEL, DaX(0.f), DaY(0.f), DA_W, DA_H,
		DA_SRC_PANEL_W, DA_SRC_PANEL_H);

	// The plaque in the frame is the title's own place; the day count it used
	// to carry has moved down to the header, which has room to say it properly.
	DaText(DaX(0.f), DaY(DA_PLAQUE_Y + DA_PLAQUE_H * 0.5f - 7.f), DA_W,
		"DAILY REWARDS", CLRDW_BR_ORANGE);

	RenderHeader(fPulse);

	for (int i = 0; i < DAY_COUNT; ++i)
		RenderCard(i, fPulse);

	RenderClaimButton(fPulse);
	RenderStatus();

	// Last, so it sits over the calendar. The card shows a model and a day
	// number, neither of which names the reward.
	if (m_iHoverDay >= 0 && m_iHoverDay < DAY_COUNT
		&& m_Day[m_iHoverDay].iItemType >= 0)
	{
		float x, y;
		DaCardRect(m_iHoverDay, x, y);
		RenderTipText((int)DaX(x + DA_CARD_W * 0.5f), (int)DaY(y - 12.f),
			BGetItemName(m_Day[m_iHoverDay].iItemType, m_Day[m_iHoverDay].iItemLevel));
	}
}

bool CPegasusDaily::UpdateMouseEvent()
{
	if (m_bShow == false)
		return false;

	DaUpdateOrigin();
	m_iHoverDay   = -1;
	m_bHoverClaim = false;

	if (SEASON3B::CheckMouseIn(DaX(DA_W - 42.f), DaY(12.f),
		DA_CLOSE_SZ, DA_CLOSE_SZ) == true)
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			Close();
			PlayBuffer(SOUND_CLICK01);
		}
		return true;
	}

	for (int i = 0; i < DAY_COUNT; ++i)
	{
		float x, y;
		DaCardRect(i, x, y);

		if (SEASON3B::CheckMouseIn(DaX(x), DaY(y), DA_CARD_W, DA_CARD_H) == true)
		{
			m_iHoverDay = i;

			if (SEASON3B::IsPress(VK_LBUTTON) && m_Day[i].eState == DAY_READY)
				OnClaim(i);

			return true;
		}
	}

	if (SEASON3B::CheckMouseIn(DaX(DA_CLAIM_X), DaY(DA_CLAIM_Y),
		DA_CLAIM_W, DA_CLAIM_H) == true)
	{
		m_bHoverClaim = true;

		if (SEASON3B::IsPress(VK_LBUTTON)
			&& m_Day[m_iToday].eState == DAY_READY)
		{
			OnClaim(m_iToday);
		}
		return true;
	}

	// Anything else over the panel is still ours, so a click inside doesn't
	// fall through and walk the character.
	return SEASON3B::CheckMouseIn(DaX(0.f), DaY(0.f), DA_W, DA_H);
}

// Whether the cursor is over the panel at all. The world's click handling
// runs in the move phase, well before this window draws, so the answer has
// to be available there - see PegasusMouseOnWindow in CBInterface.cpp.
bool CPegasusDaily::IsMouseOver()
{
	if (m_bShow == false)
		return false;

	DaUpdateOrigin();
	return SEASON3B::CheckMouseIn(DaX(0.f), DaY(0.f), DA_W, DA_H);
}
