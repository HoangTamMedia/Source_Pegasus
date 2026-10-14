#pragma once

// Item art in this client is a BMD model, never a sprite, so drawing one means
// leaving the 2D bitmap pass for a perspective camera and coming back again -
// the same matrix swap CNewUISystem::RenderItem3DFree performs. Every Pegasus
// window draws out of CBInterface's 2D pass, so calling RenderItem3D straight
// from Render() binds the model against the ortho matrix and nothing reaches
// the screen: the reward slots come out empty. This wraps the swap once so the
// windows themselves can stay 2D.
//
// RenderItem3DFree is not usable in their place. It carries a per-type table
// of y nudges (+20 to +35 px) measured for the 13x13 slots of the item bank,
// and those push an icon clean out of a 74px reward slot.
//
// Two things about RenderItem3D are worth knowing before touching this:
//
//  - the width and height it takes do NOT size the model. Scale is baked per
//    type inside RenderObjectScreen; the rect only feeds the per-type centring
//    table and the hover test. So the rect handed over has to be the item's
//    natural inventory footprint - Width x Height cells at INVENTORY_SCALE -
//    or the model sits off centre in its slot.
//
//  - screen size comes from the field of view instead, and inversely.
//    CreateScreenVector parks the model at RENDER_ITEMVIEW_FAR and derives its
//    placement from the same PerspectiveX/Y that gluPerspective2 has just
//    written, so the screen position is fov independent while the size goes as
//    1/fov. Fov 1.0 is what the inventory uses, i.e. the natural footprint at
//    1:1. Fitting an item to a slot is therefore fov = natural / target.

#include "ZzzOpenglUtil.h"
#include "ZzzInventory.h"		// RenderItem3D
#include "ZzzInfomation.h"		// ItemAttribute
#include "_define.h"			// MAX_ITEM, INVENTORY_SCALE, RENDER_ITEMVIEW_*

extern int   m_Resolution;
extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

// Air between the model and the slot frame, and a ceiling so a 1x1 potion in a
// large reward slot is not blown up until it reads as some other item.
const float PG_ICON_FIT_MARGIN = 0.9f;
const float PG_ICON_FIT_MAX    = 2.4f;

// x, y, w, h describe the slot in 2D interface space - the same space the
// window's own blits use. The item is fitted inside that rect and centred on
// it. iLevel is the raw level field, i.e. already shifted the way the caller's
// data holds it.
inline void PgRenderItemIcon(float x, float y, float w, float h, int iType, int iLevel)
{
	if (iType < 0 || iType >= MAX_ITEM || ItemAttribute == NULL)
		return;

	const ITEM_ATTRIBUTE* pAttr = &ItemAttribute[iType];

	float fNatW = (float)pAttr->Width  * INVENTORY_SCALE;
	float fNatH = (float)pAttr->Height * INVENTORY_SCALE;

	// An item the client has no attribute row for still has to draw as
	// something rather than divide by zero.
	if (fNatW <= 0.f) fNatW = (float)INVENTORY_SCALE;
	if (fNatH <= 0.f) fNatH = (float)INVENTORY_SCALE;

	float fFit = min(w / fNatW, h / fNatH) * PG_ICON_FIT_MARGIN;
	if (fFit > PG_ICON_FIT_MAX) fFit = PG_ICON_FIT_MAX;
	if (fFit < 0.1f)            fFit = 0.1f;

	float fFov = 1.f / fFit;
	if (m_Resolution >= 3)
		fFov *= 1.30f;		// matching the correction the other two item paths make

	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(fFov, (float)WindowWidth / (float)WindowHeight,
		RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);

	EnableDepthTest();
	EnableDepthMask();

	// The world's depth is still in the buffer, and the item view has a near
	// and far of its own that put the model out by the far plane - so without a
	// clear the terrain wins every pixel and the model never appears. The
	// camera pass in NewUI3DRenderMng clears for the same reason, once for the
	// whole pass; here it is per icon, so the clear is scissored to the slot
	// rather than costing a full screen of fill on every one of them.
	const float fPadX = w * 0.5f, fPadY = h * 0.5f;
	int iSx = (int)((x - fPadX)        * g_fScreenRate_x);
	int iSw = (int)((w + fPadX * 2.f)  * g_fScreenRate_x);
	int iSh = (int)((h + fPadY * 2.f)  * g_fScreenRate_y);
	int iSy = (int)WindowHeight - (int)((y - fPadY) * g_fScreenRate_y) - iSh;

	glEnable(GL_SCISSOR_TEST);
	glScissor(iSx, iSy, iSw, iSh);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderItem3D(x + (w - fNatW) * 0.5f, y + (h - fNatH) * 0.5f,
		fNatW, fNatH, iType, iLevel, 0, 0, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();

	// BeginBitmap leaves depth test off again, but the colour and the blend
	// mode are whatever the model left behind, so the caller's next blit gets a
	// known state.
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
}
