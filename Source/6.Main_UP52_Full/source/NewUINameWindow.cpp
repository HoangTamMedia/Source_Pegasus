

#include "stdafx.h"
#include "NewUINameWindow.h"
#include "ZzzBmd.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "CSChaosCastle.h"
#include "PersonalShopTitleImp.h"
#include "PgSpriteBatch.h"
#include "MatchEvent.h"
#include "MapManager.h"
#include "ZzzTexture.h"		// LoadBitmap - without it windows.h's LoadBitmapA wins

extern int DisplayWinCDepthBox;
extern int DisplayWin;
extern int DisplayWinMid;
extern int DisplayWinExt;
extern int DisplayWinReal;
extern int DisplayHeight;
using namespace SEASON3B;

SEASON3B::CNewUINameWindow::CNewUINameWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;

	m_bShowItemName = false;
}

SEASON3B::CNewUINameWindow::~CNewUINameWindow()
{
	Release();
}

bool SEASON3B::CNewUINameWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_NAME_WINDOW, this);

	SetPos(x, y);

	Show(true);

	return true;
}

void SEASON3B::CNewUINameWindow::Release()
{
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUINameWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUINameWindow::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CNewUINameWindow::UpdateKeyEvent()
{
	if(SEASON3B::IsPress(VK_MENU) == true)
	{
		m_bShowItemName = !m_bShowItemName;
	}

	return true;
}

bool SEASON3B::CNewUINameWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUINameWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderName();
	RenderNpcMarkers();
    RenderTimes();
    matchEvent::RenderMatchTimes ();
    RenderBooleans();
	DrawPersonalShopTitleImp();
	DisableAlphaBlend();
	return true;
}

// ---- Overhead NPC markers ------------------------------------------------
// Each kind of NPC gets its own symbol - chest for the vault, anvil for the
// smith, and so on. The exclamation mark is reserved for quest givers.
// Art is Pegasus_Minimaps, 32x32 each.
namespace
{
	const int PG_MARK_SIZE = 32;		// source art is 32x32
	const int PG_MARK_DRAW = 30;		// drawn size, in UI units

	// Halo behind the marker. The flare art is 136x136, which the loader pads
	// to 256, so the UV has to stop at 136/256 or it samples the uninitialised
	// padding and comes out as a white square.
	const float PG_FLARE_SRC     = 136.f;
	const float PG_FLARE_DRAW    = 66.f;	// a bit over twice the icon
	const float PG_FLARE_PEAK    = 0.40f;	// brightness of the innermost pass
	const float PG_FLARE_SWELL   = 0.14f;	// how much the size breathes
	const int   PG_FLARE_BLUR    = 3;		// passes per flare, each softer
	const float PG_FLARE_BLUR_STEP = 0.22f;	// how much wider each pass gets

	// How far above the NPC's bounding box the marker floats, in world units.
	// Raise to lift the markers, lower to bring them down onto the heads.
	const float PG_MARK_LIFT = 40.f;

	// --- cost control -------------------------------------------------------
	// RenderBitmap is immediate mode: every call is a BindTexture plus its own
	// glBegin/glEnd, so what this loop costs is simply how many quads it emits.
	// A marker at full quality is BITMAP_PG_FLARE_COUNT * PG_FLARE_BLUR additive
	// passes plus the icon - ten draws and around forty matrix operations - and
	// the loop walks all MAX_CHARACTERS_CLIENT slots every frame. A town full of
	// NPCs was putting several hundred of those through the pipe.
	//
	// Nothing below changes how a marker looks. The only thing skipped is a
	// marker the player cannot see: off the edges of the screen, or behind the
	// camera, where the quads were being built and submitted anyway.
	//
	// Half the marker's drawn extent, with the breathe and the widest blur ring
	// already in it, so the bounds test is exact rather than generous.
	const float PG_FLARE_MAX_HALF = 66.f * 1.15f * (1.f + 0.22f * 2) * 0.5f;

	// Icon slots, in load order. Art is Pegasus_Minimaps.
	enum
	{
		PG_ICON_CHEST = 0,		// I3   storage / vault
		PG_ICON_QUEST,			// I18  exclamation - quests ONLY
		PG_ICON_ANVIL,			// I15  smith / repair
		PG_ICON_SWORDS,			// I13  weapon and armour shops
		PG_ICON_POTION,			// I5   general goods
		PG_ICON_GEM,			// I6   chaos machine / mixing
		PG_ICON_HORSE,			// I10  pet trainer
		PG_ICON_SCROLL,			// I16  wizard / skills
		PG_ICON_CROWN,			// I27  guild master
		PG_ICON_COUNT
	};

	const char* const s_apszIconFile[PG_ICON_COUNT] =
	{
		"TournamentMaps_I3.tga",
		"TournamentMaps_I18.tga",
		"TournamentMaps_I15.tga",
		"TournamentMaps_I13.tga",
		"TournamentMaps_I5.tga",
		"TournamentMaps_I6.tga",
		"TournamentMaps_I10.tga",
		"TournamentMaps_I16.tga",
		"TournamentMaps_I27.tga",
	};

	// NPCs are matched on o->Type, the MODEL_* they were spawned with -
	// c->Class is left at 0 for them. Add a row to flag another NPC.
	struct PG_NPC_MARK { int iModel; int iIcon; };

	const PG_NPC_MARK s_aNpcMark[] =
	{
		{ MODEL_STORAGE,			PG_ICON_CHEST  },

		{ MODEL_NPC_SEVINA,			PG_ICON_QUEST  },
		{ MODEL_NPC_DEVIN,			PG_ICON_QUEST  },

		{ MODEL_SMITH,				PG_ICON_ANVIL  },
		{ MODEL_SNOW_SMITH,			PG_ICON_ANVIL  },
		{ MODEL_REFINERY_NPC,		PG_ICON_ANVIL  },
		{ MODEL_SMELTING_NPC,		PG_ICON_ANVIL  },

		{ MODEL_MERCHANT_MAN,		PG_ICON_SWORDS },
		{ MODEL_MERCHANT_FEMALE,	PG_ICON_SWORDS },
		{ MODEL_ELF_MERCHANT,		PG_ICON_SWORDS },
		{ MODEL_DEVIAS_TRADER,		PG_ICON_SWORDS },
		{ MODEL_KALIMA_SHOP,		PG_ICON_SWORDS },

		{ MODEL_MERCHANT_GIRL,		PG_ICON_POTION },
		{ MODEL_SNOW_MERCHANT,		PG_ICON_POTION },
		{ MODEL_RECOVERY_NPC,		PG_ICON_POTION },

		{ MODEL_MIX_NPC,			PG_ICON_GEM    },

		{ MODEL_NPC_BREEDER,		PG_ICON_HORSE  },

		{ MODEL_SCIENTIST,			PG_ICON_SCROLL },
		{ MODEL_ELF_WIZARD,			PG_ICON_SCROLL },
		{ MODEL_SNOW_WIZARD,		PG_ICON_SCROLL },

		{ MODEL_MASTER,				PG_ICON_CROWN  },
	};

	int FindNpcIcon(int iModel)
	{
		const int iCount = sizeof(s_aNpcMark) / sizeof(s_aNpcMark[0]);
		for (int i = 0; i < iCount; ++i)
			if (s_aNpcMark[i].iModel == iModel)
				return s_aNpcMark[i].iIcon;
		return -1;
	}
}

void SEASON3B::CNewUINameWindow::RenderNpcMarkers()
{
	static bool s_bLoaded = false;
	if (s_bLoaded == false)
	{
		unicode::t_char szPath[256];
		for (int i = 0; i < PG_ICON_COUNT; ++i)
		{
			unicode::_sprintf(szPath, "Interface\\Pegasus\\Pegasus_Minimaps\\%s",
				s_apszIconFile[i]);
			LoadBitmap(szPath, BITMAP_PG_NPCMARK_BEGIN + i, GL_LINEAR);
		}

		// The three flare sprites the halo cross-fades between.
		static const char* const apszFlare[BITMAP_PG_FLARE_COUNT] =
		{
			"TournamentMain_I137.tga",	// gold
			"TournamentMain_I135.tga",	// green
			"TournamentMain_I136.tga",	// blue
		};

		for (int i = 0; i < BITMAP_PG_FLARE_COUNT; ++i)
		{
			unicode::_sprintf(szPath, "Interface\\Pegasus\\Pegasus_Main\\%s",
				apszFlare[i]);
			LoadBitmap(szPath, BITMAP_PG_FLARE_BEGIN + i, GL_LINEAR);
		}

		s_bLoaded = true;
	}

	// Gentle bob so the marker reads as an overlay, not part of the scene.
	float fBob = sinf(WorldTime * 0.004f) * 4.f;

	// These depend on the clock alone, not on the NPC, so they came out of the
	// loop. They were being recomputed - three sinf and three multiplies - for
	// every marker on screen.
	float fBreathe = 1.f + PG_FLARE_SWELL * sinf(WorldTime * 0.0018f);
	float afAngle[BITMAP_PG_FLARE_COUNT];
	{
		// Different speeds, and the middle one reversed.
		static const float afSpin[BITMAP_PG_FLARE_COUNT] = { 0.020f, -0.013f, 0.031f };
		for (int f = 0; f < BITMAP_PG_FLARE_COUNT; ++f)
			afAngle[f] = WorldTime * afSpin[f];
	}

	for (int i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
	{
		CHARACTER* c = &CharactersClient[i];
		OBJECT* o = &c->Object;

		if (o->Live == false || o->Kind != KIND_NPC)
			continue;

		int iIcon = FindNpcIcon(o->Type);
		if (iIcon == -1)
			continue;

		int iImage = BITMAP_PG_NPCMARK_BEGIN + iIcon;

		vec3_t vPos;
		Vector(o->Position[0], o->Position[1],
			o->Position[2] + o->BoundingBoxMax[2] + PG_MARK_LIFT, vPos);

		// Behind the camera. Projection divides through by this and hands back a
		// mirrored point that can land anywhere on screen, so without the test a
		// marker at your back draws a full halo somewhere in front of you.
		vec3_t vView;
		VectorTransform(vPos, CameraMatrix, vView);
		if (vView[2] >= 0.f)
			continue;

		int sx = 0, sy = 0;
		Projection(vPos, &sx, &sy);

		// Off the edges. The halo is the widest thing drawn, so it sets the
		// margin; anything further out than that cannot put a pixel on screen.
		if ((float)sx + PG_FLARE_MAX_HALF < 0.f
			|| (float)sx - PG_FLARE_MAX_HALF > (float)DisplayWin
			|| (float)sy + PG_FLARE_MAX_HALF < 0.f
			|| (float)sy - PG_FLARE_MAX_HALF - PG_MARK_DRAW > (float)DisplayHeight)
			continue;

		// Projection already scales into DisplayWin x DisplayHeight, which is the
		// same space RenderImage draws in - do not convert again.
		float fX = (float)sx - PG_MARK_DRAW * 0.5f;
		float fY = (float)sy - PG_MARK_DRAW + fBob;

		// --- halo, behind the icon ---
		// All three flares at once rather than cross-faded: gold, green and
		// blue stacked additively give a white-hot core with coloured fringes.
		// Each turns at its own rate, one of them backwards, so the arms never
		// settle into a fixed shape.
		//
		// Softness comes from drawing each flare a few times at growing size
		// and falling brightness - a cheap bloom. The sprite has hard arms, and
		// one pass of it looks like a cut-out sticker.
		{
			float fCX = (float)sx;
			float fCY = (float)sy - PG_MARK_DRAW * 0.5f + fBob;

			EnableAlphaBlend();		// GL_ONE / GL_ONE

			// The pivot has to be in the space the vertices are actually
			// emitted in, not the UI space the draw call takes arguments in:
			// x and y go through ConvertX/ConvertY and then y is flipped with
			// WindowHeight - y. Rotating around the UI point swung the quads
			// clear across the screen. ConvertX/ConvertY are just the screen
			// rate multiply and they aren't exported - inline them.
			float fPivotX = fCX * g_fScreenRate_x;
			float fPivotY = (float)WindowHeight - fCY * g_fScreenRate_y;

			for (int f = 0; f < BITMAP_PG_FLARE_COUNT; ++f)
			{
				// All the blur rings of one flare turn by the same angle, so the
				// matrix is set up once for the group instead of being pushed,
				// rotated and popped again for each of them.
				glPushMatrix();
				glTranslatef(fPivotX, fPivotY, 0.f);
				glRotatef(afAngle[f], 0.f, 0.f, 1.f);
				glTranslatef(-fPivotX, -fPivotY, 0.f);

				// Same texture and now the same matrix across the group, so the
				// rings go out as one draw call rather than one apiece.
				PgBatchBegin();

				for (int b = 0; b < PG_FLARE_BLUR; ++b)
				{
					// Each blur ring is wider and dimmer than the last.
					float fGrow  = 1.f + PG_FLARE_BLUR_STEP * b;
					float fLevel = PG_FLARE_PEAK / (1.f + b * 1.7f);

					float fSize = PG_FLARE_DRAW * fBreathe * fGrow;
					float fX0 = fCX - fSize * 0.5f;
					float fY0 = fCY - fSize * 0.5f;

					PgBatchSetColor(fLevel, fLevel, fLevel, 1.f);
					PgBatchImageFFF(BITMAP_PG_FLARE_BEGIN + f, fX0, fY0, fSize, fSize,
						0.f, 0.f, PG_FLARE_SRC, PG_FLARE_SRC);
				}

				PgBatchEnd();

				glPopMatrix();
			}

			EnableAlphaTest();
			glColor4f(1.f, 1.f, 1.f, 1.f);
		}

		// u/v/uw/vh here are normalised UV, not pixels. The art is 32x32, which
		// is already a power of two, so the whole texture is 0..1.
		RenderImage(iImage, fX, fY, (float)PG_MARK_DRAW, (float)PG_MARK_DRAW,
			0.f, 0.f, 1.f, 1.f);
	}
}

void SEASON3B::CNewUINameWindow::RenderName()
{
	// Names over heads are drawn from the sprite atlas now, alongside the health
	// bars - see the PgScreenTextFx call in RenderHPBar. This loop used to raise
	// an empty chat balloon for every player purely to get a name out of the
	// balloon renderer, and with both running the two copies landed a few pixels
	// apart and read as one garbled word.
	//
	// Balloons carrying actual text are created elsewhere and are untouched, so
	// speech still works. Only the name-only ones are gone.
	if (false && g_bGMObservation == true)
	{
		for(int i=0; i<MAX_CHARACTERS_CLIENT; i++)
		{
			CHARACTER *c = &CharactersClient[i];
			OBJECT *o = &c->Object;
			if(o->Live && o->Kind == KIND_PLAYER)
			{
				if(IsShopTitleVisible(c) == false)
				{
					CreateChat(c->ID,"",c);
				}
			}
		}
	}

    if(gMapManager.InChaosCastle() == true)
    {
        if(FindText(Hero->ID, "webzen") == false)
        {
            if(SelectedNpc != -1 || SelectedCharacter != -1 )
			{
				return;
			}
        }
    }

	if(SelectedItem != -1 || SelectedNpc != -1 || SelectedCharacter != -1)
	{
		if(SelectedNpc != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedNpc];
			OBJECT *o = &c->Object;
			CreateChat(c->ID,"",c);
		}
		else if(SelectedCharacter != -1)
		{
			CHARACTER *c = &CharactersClient[SelectedCharacter];

				OBJECT *o = &c->Object;
				if(o->Kind == KIND_MONSTER)
				{

				}
				else
				{
					if(IsShopTitleVisible(c) == false)
					{
						CreateChat(c->ID,"",c);
					}
				}
		}
		else if(SelectedItem != -1)
		{
			RenderItemName(SelectedItem,&Items[SelectedItem].Object,Items[SelectedItem].Item.Level,Items[SelectedItem].Item.Option1,Items[SelectedItem].Item.ExtOption,false);

			if (gProtect.m_MainInfo.ShowInfoItemDrop)
			{

				if (Items[SelectedItem].Item.Type != 7183)
				{
					RenderItemInfo(MouseX + 120, MouseY - 50, &Items[SelectedItem].Item, 0, 0, false, true);
				}
			}

		}
	}

	if(m_bShowItemName || SEASON3B::IsRepeat(VK_MENU))
	{
		for(int i=0;i<MAX_ITEMS;i++)
		{
			OBJECT *o = &Items[i].Object;
			if(o->Live)
			{
				if(o->Visible && i!=SelectedItem)
				{

					RenderItemName(i,o,Items[i].Item.Level,Items[i].Item.Option1,Items[i].Item.ExtOption,true);
				}
			}
		}
	}
}

float SEASON3B::CNewUINameWindow::GetLayerDepth()
{
	return 1.0f;
}
