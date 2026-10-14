
#include "stdafx.h"
#include "ZzzOpenglUtil.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "ZzzTexture.h"
#include "ZzzAi.h"
#include "ZzzEffect.h"
#include "DSPlaySound.h"
#include "WSClient.h"
#include "NewUISystem.h"

PARTICLE  Points[MAX_POINTS];

int g_iLatestPoint = -1;

namespace
{
    // Hits in quick succession land on almost the same spot, and numbers that
    // rise straight up from it stack into one unreadable column. Each new
    // number is pushed to one side instead, alternating and widening, so a
    // burst opens into a fan. The counter resets once the hits stop, so a
    // single hit is never thrown off centre.
    const double PG_POINT_FAN_RESET_MS = 260.0;   // gap that ends a burst
    const float  PG_POINT_FAN_SPREAD   = 5.5f;    // sideways speed per fan step
    const int    PG_POINT_FAN_MAX      = 4;       // steps before the fan folds back
    const float  PG_POINT_FAN_DAMP     = 0.90f;   // drift eases off per frame

    // How fast the number climbs away and fades. Gravity doubles as the alpha
    // in RenderPoints, so lowering the decay both lengthens the float and holds
    // the number legible for longer before it starts to go.
    const float  PG_POINT_RISE_DECAY   = 0.24f;   // was 0.3 - about 0.7s of life

    int    s_iFan = 0;
    double s_dLastSpawn = 0.0;

    // The shout word sits above its number, sized off it so a big hit shouts
    // louder. pg_hitword.tga is 512 wide by 64 per row, so the tile is 8:1.
    const float PG_HITWORD_ASPECT = 8.f;
    const float PG_HITWORD_SIZE   = 0.85f;   // height, as a share of the number
    const float PG_HITWORD_LIFT   = 40.f;    // world units above the number

    // Drawn straight after the number it belongs to, in the same sprite pass and
    // with the same alpha, so the two rise and fade as one thing.
    void PgRenderHitWord(PARTICLE* o)
    {
        if (o->SubType < 0 || o->SubType >= PG_HITWORD_COUNT)
            return;

        float fH = o->Scale * PG_HITWORD_SIZE;
        float fW = fH * PG_HITWORD_ASPECT;

        // One row of the sheet. v runs downward, matching the digit sheet.
        float v0 = (float)o->SubType / (float)PG_HITWORD_COUNT;
        float v1 = (float)(o->SubType + 1) / (float)PG_HITWORD_COUNT;

        float UV[4][2];
        TEXCOORD(UV[0], 0.f, v1);
        TEXCOORD(UV[1], 1.f, v1);
        TEXCOORD(UV[2], 1.f, v0);
        TEXCOORD(UV[3], 0.f, v0);

        vec3_t Light[4];
        VectorCopy(o->Angle, Light[0]);
        VectorCopy(o->Angle, Light[1]);
        VectorCopy(o->Angle, Light[2]);
        VectorCopy(o->Angle, Light[3]);

        vec3_t p;
        VectorCopy(o->Position, p);
        p[2] += PG_HITWORD_LIFT;

        RenderSpriteUV(BITMAP_PG_HITWORD, p, fW, fH, UV, Light, o->Gravity * 0.4f);
    }
}

void CreatePoint(vec3_t Position, int Value, vec3_t Color, float scale, bool bMove, bool bRepeatedly, int iWord)
{

    for (int i = 0; i < MAX_POINTS; i++)
    {
        PARTICLE* o = &Points[i];
        if (!o->Live)
        {
            o->Live = true;
            o->Type = Value;
            VectorCopy(Position, o->Position);
            o->Position[2] += 140.f;
            VectorCopy(Color, o->Angle);
            o->bRepeatedly = bRepeatedly;
            o->fRepeatedlyHeight = RequestTerrainHeight(o->Position[0], o->Position[1]) + 140.0f;
            o->Gravity = 10.f;
            o->Scale = scale;
            o->LifeTime = 0;
            o->bEnableMove = bMove;
            o->SubType = iWord;   // shout word, PG_HITWORD_NONE for a plain number

            // Where in the fan this one sits. Order is centre, left, right,
            // further left, further right, then back to the centre.
            if (WorldTime - s_dLastSpawn > PG_POINT_FAN_RESET_MS)
                s_iFan = 0;
            s_dLastSpawn = WorldTime;

            int   iStep = (s_iFan + 1) / 2;
            float fSide = (s_iFan & 1) ? -1.f : 1.f;
            if (++s_iFan > PG_POINT_FAN_MAX * 2)
                s_iFan = 0;

            // Numbers that are pinned in place - the repeating ones - keep
            // their spot, so only the free-floating hits fan.
            float fPush = (bMove && !bRepeatedly)
                ? fSide * (float)iStep * PG_POINT_FAN_SPREAD : 0.f;

            o->Velocity[0] = fPush;
            o->Velocity[1] = fPush * 0.5f;
            o->Velocity[2] = 0.f;

            return;
        }
    }
}

void RenderPoints(BYTE byRenderOneMore)
{

    EnableAlphaTest();
    DisableDepthTest();
    for (int i = 0; i < MAX_POINTS; i++)
    {
        PARTICLE* o = &Points[i];
        if (o->Live)
        {
            if (byRenderOneMore == 1)
            {
                if (o->Position[2] > 350.f) continue;
            }
            else if (byRenderOneMore == 2)
            {
                if (o->Position[2] <= 300.f) continue;
            }
            else if (o->bRepeatedly)
            {
                if (o->Position[2] <= o->fRepeatedlyHeight) continue;
            }

            // -3 is a word on its own. A streak call-out is not attached to any
            // one hit, so there is no number for it to sit above.
            if (o->Type != -3)
                RenderNumber(o->Position, o->Type, o->Angle, o->Gravity * 0.4f, o->Scale);

            PgRenderHitWord(o);
        }
    }
}

void MovePoints()
{

    for (int i = 0; i < MAX_POINTS; i++)
    {
        PARTICLE* o = &Points[i];
        if (o->Live)
        {
            o->LifeTime -= FPS_ANIMATION_FACTOR;
            if (o->LifeTime < 0)
            {
                if (o->bRepeatedly && o->Position[2] > o->fRepeatedlyHeight)
                {
                    o->Gravity = 10.0f;
                    o->bRepeatedly = false;
                }
                if (o->bEnableMove)
                {
                    o->Position[2] += o->Gravity * FPS_ANIMATION_FACTOR;

                    // Sideways drift from the fan, easing off as it goes so the
                    // number opens away from its neighbours and then settles
                    // rather than sliding off across the ground.
                    o->Position[0] += o->Velocity[0] * FPS_ANIMATION_FACTOR;
                    o->Position[1] += o->Velocity[1] * FPS_ANIMATION_FACTOR;

                    float fDamp = 1.f - (1.f - PG_POINT_FAN_DAMP) * FPS_ANIMATION_FACTOR;
                    if (fDamp < 0.f) fDamp = 0.f;
                    o->Velocity[0] *= fDamp;
                    o->Velocity[1] *= fDamp;
                }
                o->Gravity -= PG_POINT_RISE_DECAY * FPS_ANIMATION_FACTOR;
                if (o->Gravity <= 0.f)
                    o->Live = false;
                if (o->Type != -2)
                {
                    o->Scale -= 5.f * FPS_ANIMATION_FACTOR;
                    if (o->Scale < 15.f)
                        o->Scale = 15.f;
                }
            }
        }
    }
}