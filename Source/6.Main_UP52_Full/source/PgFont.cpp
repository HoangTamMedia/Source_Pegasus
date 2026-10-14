#include "stdafx.h"
#include "PgFont.h"
#include "PgFontData.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"

namespace
{
	// The world-space step between glyph centres for one full cell, taken from
	// RenderNumber so text laid out here lines up with the damage numbers.
	const float PG_FONT_STEP = 0.7071067f;

	bool s_bLoaded = false;

	// Anything outside the atlas draws as a space rather than as whatever
	// happens to sit at that offset in memory.
	inline int GlyphIndex(unsigned char c)
	{
		if (c < PG_FONT_FIRST_CHAR || c > PG_FONT_LAST_CHAR)
			return -1;
		return (int)c - PG_FONT_FIRST_CHAR;
	}

	inline float GlyphAdvance(unsigned char c)
	{
		int i = GlyphIndex(c);
		if (i < 0)
			return 0.26f;		// unknown byte, treated as a space
		return g_afPgFontAdvance[i];
	}

	// --- effect tuning -------------------------------------------------------
	const float PG_TFX_WAVE_RATE   = 0.0055f;	// radians per ms
	const float PG_TFX_WAVE_SPREAD = 0.85f;		// phase step between letters
	const float PG_TFX_WAVE_AMP    = 0.22f;		// as a share of the scale

	const float PG_TFX_SHEEN_RATE  = 0.0013f;	// how fast the band travels
	const float PG_TFX_SHEEN_WIDTH = 0.28f;		// of the string, at half height
	const float PG_TFX_SHEEN_GAIN  = 1.30f;		// brightness inside the band

	const float PG_TFX_RAINBOW_RATE   = 0.00022f;
	const float PG_TFX_RAINBOW_SPREAD = 0.085f;	// hue step between letters

	const float PG_TFX_PULSE_RATE  = 0.0032f;
	const float PG_TFX_PULSE_DEPTH = 0.10f;		// of the scale

	const float PG_TFX_SHAKE_RATE  = 0.021f;
	const float PG_TFX_SHAKE_AMP   = 0.055f;

	const float PG_TFX_GRADIENT_TOP = 1.18f;
	const float PG_TFX_GRADIENT_BOT = 0.62f;

	const double PG_TFX_POPIN_STEP = 70.0;		// ms between letters arriving
	const double PG_TFX_POPIN_TIME = 220.0;		// ms a letter takes to settle

	const float PG_TFX_BOUNCE_RATE   = 0.0060f;
	const float PG_TFX_BOUNCE_SPREAD = 0.95f;
	const float PG_TFX_BOUNCE_AMP    = 0.26f;

	const float PG_TFX_TREMBLE_RATE = 0.052f;	// fast, and small with it
	const float PG_TFX_TREMBLE_AMP  = 0.022f;

	const float PG_TFX_SWELL_RATE   = 0.0045f;
	const float PG_TFX_SWELL_SPREAD = 0.7f;
	const float PG_TFX_SWELL_DEPTH  = 0.20f;

	const float PG_TFX_FIRE_RATE = 0.0090f;
	const float PG_TFX_ICE_RATE  = 0.0026f;

	const float PG_TFX_FADE_RATE   = 0.0030f;
	const float PG_TFX_FADE_SPREAD = 3.1f;
	const float PG_TFX_FADE_FLOOR  = 0.45f;		// never fades out completely

	const double PG_TFX_GLITCH_HOLD  = 55.0;	// ms a displacement is held
	const float  PG_TFX_GLITCH_CHANCE = 0.16f;	// share of letters affected
	const float  PG_TFX_GLITCH_JUMP  = 0.30f;
	const float  PG_TFX_GLITCH_FLARE = 1.45f;

	const double PG_TFX_TYPE_STEP  = 55.0;		// ms between letters appearing
	const double PG_TFX_TYPE_CYCLE = 4200.0;	// ms before it types itself again

	// Hue to RGB at full saturation and value. Only the hue moves, so the
	// letters stay bright whatever colour they are passing through.
	void HueToRGB(float h, float& r, float& g, float& b)
	{
		h -= floorf(h);
		float s = h * 6.f;
		int   i = (int)s;
		float f = s - i;

		switch (i % 6)
		{
		case 0: r = 1.f;     g = f;       b = 0.f;     break;
		case 1: r = 1.f - f; g = 1.f;     b = 0.f;     break;
		case 2: r = 0.f;     g = 1.f;     b = f;       break;
		case 3: r = 0.f;     g = 1.f - f; b = 1.f;     break;
		case 4: r = f;       g = 0.f;     b = 1.f;     break;
		default:r = 1.f;     g = 0.f;     b = 1.f - f; break;
		}
	}

	// --- one place where every effect is decided -----------------------------
	// Both renderers ask this, then map the offsets onto their own axes: the
	// world one along the camera's right and up, the interface one along x and
	// y. Written once so the two cannot drift apart as effects are added.
	struct GLYPH_FX
	{
		float fRight, fUp;		// offset, in multiples of the glyph size
		float fScale;			// multiplier on the size
		float fAlpha;			// multiplier on the alpha
		float r, g, b;			// final colour
		bool  bSkip;			// not drawn at all this frame
	};

	// A cheap repeatable hash, so "random" is the same every frame for a given
	// letter and step rather than boiling.
	inline float Hash01(int a, int b)
	{
		int h = a * 374761393 + b * 668265263;
		h = (h ^ (h >> 13)) * 1274126177;
		return (float)((h ^ (h >> 16)) & 0xFFFF) / 65535.f;
	}

	void EvalGlyphFx(GLYPH_FX& o, DWORD dwFx, double t, int iIndex, int iLen,
		float fAt, float fSheenAt, float r, float g, float b, float fAlpha)
	{
		o.fRight = 0.f;
		o.fUp    = 0.f;
		o.fScale = 1.f;
		o.fAlpha = fAlpha;
		o.r = r; o.g = g; o.b = b;
		o.bSkip = false;

		if (dwFx & PG_TFX_WAVE)
		{
			o.fUp += sinf((float)t * PG_TFX_WAVE_RATE
				+ iIndex * PG_TFX_WAVE_SPREAD) * PG_TFX_WAVE_AMP;
		}

		if (dwFx & PG_TFX_BOUNCE)
		{
			// abs of a sine gives the sharp turn at the bottom that makes this
			// read as weight rather than as another wave.
			float fB = sinf((float)t * PG_TFX_BOUNCE_RATE
				+ iIndex * PG_TFX_BOUNCE_SPREAD);
			if (fB < 0.f) fB = -fB;
			o.fUp += fB * PG_TFX_BOUNCE_AMP;
		}

		if (dwFx & PG_TFX_SHAKE)
		{
			o.fRight += sinf((float)t * PG_TFX_SHAKE_RATE + iIndex * 2.3f)
				* PG_TFX_SHAKE_AMP;
			o.fUp    += cosf((float)t * PG_TFX_SHAKE_RATE * 1.37f + iIndex * 1.7f)
				* PG_TFX_SHAKE_AMP;
		}

		if (dwFx & PG_TFX_TREMBLE)
		{
			o.fRight += sinf((float)t * PG_TFX_TREMBLE_RATE + iIndex * 5.1f)
				* PG_TFX_TREMBLE_AMP;
			o.fUp    += sinf((float)t * PG_TFX_TREMBLE_RATE * 1.61f + iIndex * 3.3f)
				* PG_TFX_TREMBLE_AMP;
		}

		if (dwFx & PG_TFX_SWELL)
		{
			o.fScale *= 1.f + PG_TFX_SWELL_DEPTH
				* sinf((float)t * PG_TFX_SWELL_RATE - iIndex * PG_TFX_SWELL_SPREAD);
		}

		if (dwFx & PG_TFX_RAINBOW)
		{
			HueToRGB((float)t * PG_TFX_RAINBOW_RATE
				+ iIndex * PG_TFX_RAINBOW_SPREAD, o.r, o.g, o.b);
		}

		if (dwFx & PG_TFX_FIRE)
		{
			// Held inside the ember band rather than the whole wheel, so it
			// stays fire instead of turning into a rainbow.
			float f = 0.5f + 0.5f * sinf((float)t * PG_TFX_FIRE_RATE
				+ iIndex * 1.9f + Hash01(iIndex, 7) * 6.28f);
			o.r = 1.f;
			o.g = 0.30f + 0.55f * f;
			o.b = 0.05f + 0.15f * f;
		}

		if (dwFx & PG_TFX_ICE)
		{
			float f = 0.5f + 0.5f * sinf((float)t * PG_TFX_ICE_RATE
				- iIndex * 0.7f);
			o.r = 0.62f + 0.34f * f;
			o.g = 0.84f + 0.16f * f;
			o.b = 1.f;
		}

		if (dwFx & PG_TFX_SHEEN)
		{
			float d = fAt - fSheenAt;
			if (d < 0.f) d = -d;
			if (d < PG_TFX_SHEEN_WIDTH)
			{
				float k = 1.f - (d / PG_TFX_SHEEN_WIDTH);
				k *= k;
				float fGain = 1.f + (PG_TFX_SHEEN_GAIN - 1.f) * k;
				o.r *= fGain; o.g *= fGain; o.b *= fGain;
			}
		}

		if (dwFx & PG_TFX_FADE)
		{
			// A dimming that travels, so the string reads as being lit from a
			// moving source rather than simply flashing.
			float f = 0.5f + 0.5f * sinf((float)t * PG_TFX_FADE_RATE
				- fAt * PG_TFX_FADE_SPREAD);
			o.fAlpha *= PG_TFX_FADE_FLOOR + (1.f - PG_TFX_FADE_FLOOR) * f;
		}

		if (dwFx & PG_TFX_GLITCH)
		{
			// Quantised time, so a letter holds its displacement for a moment
			// instead of vibrating - that hold is what makes it read as a fault
			// rather than as noise.
			int iStep = (int)(t / PG_TFX_GLITCH_HOLD);
			float h = Hash01(iIndex, iStep);
			if (h > 1.f - PG_TFX_GLITCH_CHANCE)
			{
				o.fRight += (Hash01(iIndex, iStep + 91) - 0.5f) * PG_TFX_GLITCH_JUMP;
				o.fUp    += (Hash01(iIndex, iStep + 57) - 0.5f) * PG_TFX_GLITCH_JUMP;
				o.r *= PG_TFX_GLITCH_FLARE;
				o.g *= PG_TFX_GLITCH_FLARE;
				o.b *= PG_TFX_GLITCH_FLARE;
			}
		}

		if (dwFx & PG_TFX_POPIN)
		{
			double dIn = t - iIndex * PG_TFX_POPIN_STEP;
			if (dIn < 0.0)
				o.bSkip = true;
			else if (dIn < PG_TFX_POPIN_TIME)
			{
				float k = (float)(dIn / PG_TFX_POPIN_TIME);
				o.fAlpha *= k;
				o.fScale *= 1.f + (1.f - k) * 0.9f;
			}
		}

		if (dwFx & PG_TFX_TYPE)
		{
			// Types out and then holds, looping on a long cycle so a label that
			// stays on screen types itself again rather than freezing.
			double dCycle = fmod(t, PG_TFX_TYPE_CYCLE);
			if (dCycle < iIndex * PG_TFX_TYPE_STEP)
				o.bSkip = true;
		}
	}

}

void PgFontLoad()
{
	if (s_bLoaded)
		return;
	s_bLoaded = true;

	LoadBitmap("Interface\\Pegasus\\Pegasus_Main\\pg_font.tga",
		BITMAP_PG_FONT, GL_LINEAR);
}

float PgWorldTextWidth(const char* szText, float fScale)
{
	if (szText == NULL)
		return 0.f;

	float fTotal = 0.f;
	for (const unsigned char* p = (const unsigned char*)szText; *p; ++p)
		fTotal += GlyphAdvance(*p);

	return fTotal * fScale * PG_FONT_STEP;
}

void PgWorldText(vec3_t vPos, const char* szText, vec3_t vColour,
	float fScale, float fAlpha)
{
	PgWorldTextFx(vPos, szText, vColour, fScale, fAlpha, PG_TFX_NONE, 0.f);
}

void PgWorldTextFx(vec3_t vPos, const char* szText, vec3_t vColour,
	float fScale, float fAlpha, DWORD dwFx, float fPhase)
{
	if (szText == NULL || *szText == '\0' || fAlpha <= 0.f)
		return;

	PgFontLoad();

	BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_FONT);
	if (pTex == NULL || pTex->Width <= 0.f)
		return;

	// Text runs along the camera's right, the same as RenderNumber's digits, so
	// it stays readable however the view is turned.
	const float Rad = 0.01745329f;
	const float sinTh = sinf(Rad * CameraAngle[2]);
	const float cosTh = cosf(Rad * CameraAngle[2]);

	const double t = WorldTime + (double)fPhase;

	// The whole string breathing has to happen before the width is measured, or
	// the centring drifts as it pulses.
	if (dwFx & PG_TFX_PULSE)
		fScale *= 1.f + PG_TFX_PULSE_DEPTH * sinf((float)t * PG_TFX_PULSE_RATE);

	int iLen = 0;
	for (const unsigned char* q = (const unsigned char*)szText; *q; ++q)
		++iLen;

	const float fCellU = (float)PG_FONT_CELL / (float)PG_FONT_ATLAS_W;
	const float fCellV = (float)PG_FONT_CELL / (float)PG_FONT_ATLAS_H;

	// Start half the run to the left of centre, measured along the same axis
	// the text advances on.
	float fHalf = PgWorldTextWidth(szText, fScale) * 0.5f;

	vec3_t p;
	VectorCopy(vPos, p);
	p[0] -= fHalf * cosTh;
	p[1] += fHalf * sinTh;

	// Where the sheen band sits along the string this instant. It runs past both
	// ends before wrapping, so there is a pause between sweeps.
	float fSheenAt = 0.f;
	if (dwFx & PG_TFX_SHEEN)
		fSheenAt = (float)fmod(t * PG_TFX_SHEEN_RATE, 1.6) - 0.3f;

	int iIndex = 0;
	for (const unsigned char* s = (const unsigned char*)szText; *s; ++s, ++iIndex)
	{
		const float fAdv = GlyphAdvance(*s);
		const int   iGlyph = GlyphIndex(*s);

		// Half the advance to reach this glyph's centre, half again to leave it,
		// so the spacing between two glyphs is the sum of their own widths.
		const float fLead = fAdv * 0.5f * fScale * PG_FONT_STEP;

		p[0] += fLead * cosTh;
		p[1] -= fLead * sinTh;

		if (iGlyph >= 0 && *s != ' ')
		{
			GLYPH_FX fx;
			EvalGlyphFx(fx, dwFx, t, iIndex, iLen,
				(iLen > 1) ? (float)iIndex / (float)(iLen - 1) : 0.f, fSheenAt,
				vColour[0], vColour[1], vColour[2], fAlpha);

			// Right and up here are the camera's, the same axes the string runs
			// along, so an effect looks identical however the view is turned.
			vec3_t g;
			VectorCopy(p, g);
			g[0] += fx.fRight * fScale * cosTh;
			g[1] -= fx.fRight * fScale * sinTh;
			g[2] += fx.fUp * fScale;

			const float fGlyphScale = fScale * fx.fScale;
			const float fGlyphAlpha = fx.fAlpha;
			const float cr = fx.r, cg = fx.g, cb = fx.b;
			const bool  bSkip = fx.bSkip;

			if (bSkip == false)
			{
				vec3_t Light[4];
				if (dwFx & PG_TFX_GRADIENT)
				{
					// UV[0] and UV[1] are the bottom corners, [2] and [3] the
					// top, so this is a fall-off down the glyph itself.
					Vector(cr * PG_TFX_GRADIENT_BOT, cg * PG_TFX_GRADIENT_BOT,
						cb * PG_TFX_GRADIENT_BOT, Light[0]);
					VectorCopy(Light[0], Light[1]);
					Vector(cr * PG_TFX_GRADIENT_TOP, cg * PG_TFX_GRADIENT_TOP,
						cb * PG_TFX_GRADIENT_TOP, Light[2]);
					VectorCopy(Light[2], Light[3]);
				}
				else
				{
					Vector(cr, cg, cb, Light[0]);
					VectorCopy(Light[0], Light[1]);
					VectorCopy(Light[0], Light[2]);
					VectorCopy(Light[0], Light[3]);
				}

				const float u = (float)(iGlyph % PG_FONT_COLS) * fCellU;
				const float v = (float)(iGlyph / PG_FONT_COLS) * fCellV;

				// v grows downward through the atlas, matching the digit sheet.
				float UV[4][2];
				TEXCOORD(UV[0], u,          v + fCellV);
				TEXCOORD(UV[1], u + fCellU, v + fCellV);
				TEXCOORD(UV[2], u + fCellU, v);
				TEXCOORD(UV[3], u,          v);

				RenderSpriteUV(BITMAP_PG_FONT, g, fGlyphScale, fGlyphScale,
					UV, Light, fGlyphAlpha);
			}
		}

		p[0] += fLead * cosTh;
		p[1] -= fLead * sinTh;
	}
}

float PgScreenTextWidth(const char* szText, float fSize)
{
	if (szText == NULL)
		return 0.f;

	float fTotal = 0.f;
	for (const unsigned char* p = (const unsigned char*)szText; *p; ++p)
		fTotal += GlyphAdvance(*p);

	return fTotal * fSize;
}

void PgScreenTextFx(float x, float y, const char* szText, float r, float g, float b,
	float fSize, float fAlpha, DWORD dwFx, float fPhase)
{
	if (szText == NULL || *szText == '\0' || fAlpha <= 0.f)
		return;

	PgFontLoad();

	BITMAP_t* pTex = Bitmaps.FindTexture(BITMAP_PG_FONT);
	if (pTex == NULL || pTex->Width <= 0.f)
		return;

	const double t = WorldTime + (double)fPhase;

	// Breathing changes the width, so it has to settle before anything is
	// measured or the line would slide as it pulses.
	if (dwFx & PG_TFX_PULSE)
		fSize *= 1.f + PG_TFX_PULSE_DEPTH * sinf((float)t * PG_TFX_PULSE_RATE);

	int iLen = 0;
	for (const unsigned char* q = (const unsigned char*)szText; *q; ++q)
		++iLen;

	const float fCellU = (float)PG_FONT_CELL / (float)PG_FONT_ATLAS_W;
	const float fCellV = (float)PG_FONT_CELL / (float)PG_FONT_ATLAS_H;

	float fPen = x - PgScreenTextWidth(szText, fSize) * 0.5f;

	float fSheenAt = 0.f;
	if (dwFx & PG_TFX_SHEEN)
		fSheenAt = (float)fmod(t * PG_TFX_SHEEN_RATE, 1.6) - 0.3f;

	if (TextureEnable == false)
	{
		TextureEnable = true;
		glEnable(GL_TEXTURE_2D);
	}
	EnableAlphaTest();

	int iIndex = 0;
	for (const unsigned char* s = (const unsigned char*)szText; *s; ++s, ++iIndex)
	{
		const float fAdv = GlyphAdvance(*s);
		const int   iGlyph = GlyphIndex(*s);

		if (iGlyph >= 0 && *s != ' ')
		{
			GLYPH_FX fx;
			EvalGlyphFx(fx, dwFx, t, iIndex, iLen,
				(iLen > 1) ? (float)iIndex / (float)(iLen - 1) : 0.f, fSheenAt,
				r, g, b, fAlpha);

			if (fx.bSkip)
			{
				fPen += fAdv * fSize;
				continue;
			}

			// y grows downward in interface space, so up is negative here - the
			// one place the two renderers genuinely differ.
			//
			// RenderBitmap takes the top-left corner while RenderSpriteUV takes
			// a centre, and the atlas cell is a full square with the glyph
			// centred inside it while the pen advances by the narrower ink
			// width. Dropping the cell straight on the pen therefore pushes
			// every letter right by half that difference, and the whole line
			// with it. Backing the cell off by that half centres the ink over
			// the span the pen actually walked.
			const float fInk = fAdv * fSize;
			const float gx = fPen + (fInk - fSize) * 0.5f + fx.fRight * fSize;
			const float gy = y - fx.fUp * fSize;
			const float fGlyph = fSize * fx.fScale;
			const float fA = fx.fAlpha;

			float cr = fx.r, cg = fx.g, cb = fx.b;

			// RenderBitmap has no per-corner colour, so the gradient becomes a
			// single darkening instead of a fall-off. It still lifts the glyph off
			// a bright background, which is what it is there for.
			if (dwFx & PG_TFX_GRADIENT)
			{
				cr *= 0.94f; cg *= 0.94f; cb *= 0.94f;
			}

			if (cr > 1.f) cr = 1.f;
			if (cg > 1.f) cg = 1.f;
			if (cb > 1.f) cb = 1.f;

			const float u = (float)(iGlyph % PG_FONT_COLS) * fCellU;
			const float v = (float)(iGlyph / PG_FONT_COLS) * fCellV;

			glColor4f(cr, cg, cb, fA);
			RenderBitmap(BITMAP_PG_FONT, gx, gy - fGlyph * 0.5f,
				fGlyph, fGlyph, u, v, fCellU, fCellV);
		}

		fPen += fAdv * fSize;
	}

	glColor4f(1.f, 1.f, 1.f, 1.f);
}
