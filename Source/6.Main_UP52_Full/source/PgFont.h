#pragma once

// Text drawn into the world rather than onto the interface.
//
// The client draws names and floating labels through GDI today. GDI is what
// cost this client a three hundred millisecond stall mid-fight once already,
// and it gives no control over colour or effect beyond what the font itself
// carries. This draws from a sprite atlas instead: one quad per glyph, tinted
// and faded by the caller, at the cost of any other sprite in the scene.
//
// The atlas and its metrics are generated together by
// Bin/Data/Interface/_build_pg_font.py - see PgFontData.h, which that script
// writes. Regenerate both or neither.
//
// Call from inside the 3D sprite pass, where RenderNumber is called from.

// Load the atlas. Safe to call more than once.
void PgFontLoad();

// Width the string will occupy, in the same units as fScale. For laying text
// out against something else, or deciding whether it will fit.
float PgWorldTextWidth(const char* szText, float fScale);

// Effects. Combine freely - they act on different things, so a wave can carry a
// sheen and still breathe. Everything is driven off the clock, so the same
// string looks the same at any frame rate.
enum
{
	PG_TFX_NONE      = 0x0000,
	PG_TFX_WAVE      = 0x0001,	// letters ride a travelling wave
	PG_TFX_SHEEN     = 0x0002,	// a bright band sweeps along the string
	PG_TFX_RAINBOW   = 0x0004,	// hue runs along the string and turns over time
	PG_TFX_PULSE     = 0x0008,	// the whole string breathes
	PG_TFX_SHAKE     = 0x0010,	// each letter jitters on its own
	PG_TFX_GRADIENT  = 0x0020,	// bright at the top, deeper at the bottom
	PG_TFX_POPIN     = 0x0040,	// letters arrive one after another

	PG_TFX_BOUNCE    = 0x0080,	// letters hop, with the weight of a bounce
	PG_TFX_FIRE      = 0x0100,	// each letter flickers through ember colours
	PG_TFX_ICE       = 0x0200,	// pale blue, with a slow shimmer crossing it
	PG_TFX_GLITCH    = 0x0400,	// letters jump and flare at random moments
	PG_TFX_FADE      = 0x0800,	// a dimming runs along the string
	PG_TFX_SWELL     = 0x1000,	// size travels along the letters
	PG_TFX_TYPE      = 0x2000,	// typed out once, then held
	PG_TFX_TREMBLE   = 0x4000,	// a fine, fast vibration - smaller than shake
};

// Draw centred on vPos, facing the camera the way every other world sprite
// does. vColour multiplies the glyphs, so it carries the whole look.
void PgWorldText(vec3_t vPos, const char* szText, vec3_t vColour,
	float fScale, float fAlpha);

// The same, with effects. dwFx is a combination of the PG_TFX_ flags above.
// fPhase shifts the animation - give two labels different phases and they stop
// moving in lockstep.
void PgWorldTextFx(vec3_t vPos, const char* szText, vec3_t vColour,
	float fScale, float fAlpha, DWORD dwFx, float fPhase = 0.f);

// The 2D form, for text that is laid out in interface space after the position
// has already been projected - names over health bars, and anything else drawn
// in the bitmap pass. x and y are the centre of the line. Same atlas, same
// effects; only the space differs.
void PgScreenTextFx(float x, float y, const char* szText, float r, float g, float b,
	float fSize, float fAlpha, DWORD dwFx, float fPhase = 0.f);

// Width in interface units, for centring against something else.
float PgScreenTextWidth(const char* szText, float fSize);


