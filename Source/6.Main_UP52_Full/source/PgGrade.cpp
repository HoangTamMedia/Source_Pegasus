#include "stdafx.h"
#include "PgGrade.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "ZzzScene.h"
#include "MapManager.h"
#include "CBInterface.h"		// DisplayWin / DisplayHeight
#include "ZzzCharacter.h"
#include "ZzzEffect.h"		// CreatePoint, PG_HITWORD_*

namespace
{
	// A grade is a tint laid over the world and a warm wash added on top of it.
	// Both are full-screen quads through the engine's own blend wrappers, so
	// nothing here can leave the shared GL state cache out of step.
	struct GRADE
	{
		float r, g, b;		// tint colour
		float fTint;		// how much of it, 0 for none
		float fWarm;		// additive wash, for firelight and danger
	};

	// The neutral grade. Anything not named below plays exactly as it did.
	const GRADE PG_GRADE_NONE = { 1.f, 1.f, 1.f, 0.f, 0.f };

	// Per-map moods. These are deliberately gentle: the point is that a place
	// feels different, not that the player notices a filter. Raise fTint if a
	// map should read harder.
	GRADE GradeForMap(int iMap)
	{
		switch (iMap)
		{
		case WD_1DUNGEON:				// underground, cold and close
			{ GRADE g = { 0.62f, 0.70f, 1.00f, 0.17f, 0.00f }; return g; }

		case WD_2DEVIAS:				// snow
			{ GRADE g = { 0.78f, 0.88f, 1.00f, 0.14f, 0.00f }; return g; }

		case WD_4LOSTTOWER:				// grey stone, drained of colour
			{ GRADE g = { 0.80f, 0.82f, 0.92f, 0.12f, 0.00f }; return g; }

		case WD_7ATLANSE:				// underwater
			{ GRADE g = { 0.55f, 0.85f, 0.95f, 0.18f, 0.00f }; return g; }

		case WD_8TARKAN:				// desert heat
			{ GRADE g = { 1.00f, 0.86f, 0.60f, 0.13f, 0.03f }; return g; }

		case WD_9DEVILSQUARE:			// the arena, lit by something unpleasant
			{ GRADE g = { 1.00f, 0.72f, 0.62f, 0.14f, 0.04f }; return g; }

		case WD_10HEAVEN:				// bright and bleached
			{ GRADE g = { 1.00f, 1.00f, 0.95f, 0.08f, 0.06f }; return g; }

		case WD_18CHAOS_CASTLE:
			{ GRADE g = { 0.95f, 0.70f, 0.80f, 0.13f, 0.02f }; return g; }

		case WD_33AIDA:					// green, otherworldly
			{ GRADE g = { 0.72f, 0.95f, 0.80f, 0.13f, 0.00f }; return g; }

		case WD_34CRYWOLF_1ST:
		case WD_35CRYWOLF_2ND:
			{ GRADE g = { 0.85f, 0.78f, 1.00f, 0.12f, 0.00f }; return g; }

		case WD_37KANTURU_1ST:
		case WD_38KANTURU_2ND:
		case WD_39KANTURU_3RD:
			{ GRADE g = { 0.70f, 0.78f, 0.92f, 0.15f, 0.00f }; return g; }
		}

		return PG_GRADE_NONE;
	}

	// Health. Below the first threshold the screen starts to warm and redden,
	// and it keeps going as things get worse - the grade itself becomes the
	// warning, before any number is read.
	const float PG_DANGER_FROM = 0.35f;		// health fraction where it begins
	const float PG_DANGER_TINT = 0.34f;		// tint at zero health
	const float PG_DANGER_WARM = 0.16f;		// wash at zero health

	// Below this the danger wash beats instead of sitting still. The client
	// already plays SOUND_HEART under 20% health, so the screen is put on the
	// same rhythm - the pulse you see and the one you hear are one thing.
	const float PG_PULSE_FROM   = 0.20f;	// health fraction where the beat starts
	const float PG_PULSE_RATE   = 0.0072f;	// radians per millisecond, ~ 70 bpm
	const float PG_PULSE_DEPTH  = 0.55f;	// how much of the wash the beat swings

	// A hit landing on the character throws a short red flash. It decays on its
	// own, so nothing has to remember to switch it off, and it is capped so a
	// spot full of monsters cannot pin the screen red.
	const float PG_FLASH_PER_HIT = 0.42f;	// added per hit, scaled by its size
	const float PG_FLASH_MAX     = 0.30f;
	const float PG_FLASH_DECAY   = 0.055f;	// per frame

	// --- kill streak ---------------------------------------------------------
	// Kills inside a rolling window. Let the window lapse and the count starts
	// again, so a streak is something sustained rather than a running total.
	const double PG_STREAK_WINDOW = 2200.0;		// ms of quiet that ends a streak

	// Where the words fire. Each one only fires once per streak, on the kill
	// that crosses it.
	const int PG_STREAK_SMASH   = 3;
	const int PG_STREAK_DEADLY  = 6;
	const int PG_STREAK_PERFECT = 10;

	// Heat builds with the streak and drains when it stops - the screen running
	// hot is the reward for keeping it going.
	const float PG_HEAT_PER_KILL = 0.020f;
	const float PG_HEAT_MAX      = 0.14f;
	const float PG_HEAT_DRAIN    = 0.0016f;		// per frame once the streak lapses

	// Levelling. Gold, and far stronger than anything else here, because it is
	// rare and it should land.
	const float PG_LEVELUP_BURST = 0.55f;

	int    s_iStreak = 0;
	double s_dLastKill = 0.0;
	float  s_fHeat = 0.f;

	// The burst colour, so one flash can serve red hits and gold level-ups.
	float s_fBurstR = 1.f, s_fBurstG = 0.16f, s_fBurstB = 0.12f;

	// Eased rather than snapped, so a portal is a slide. Per frame, scaled by
	// the frame-time factor so it takes the same real time at any rate.
	const float PG_GRADE_EASE = 0.045f;

	GRADE s_Current = { 1.f, 1.f, 1.f, 0.f, 0.f };
	bool  s_bHaveCurrent = false;
	float s_fFlash = 0.f;

	void Ease(float& v, float t, float f)
	{
		v += (t - v) * f;
	}

	GRADE TargetGrade()
	{
		GRADE g = GradeForMap(gMapManager.WorldActive);

		// Danger folds in on top of whatever the map is doing, so a low-health
		// moment in the snow still reads as the snow turning against you.
		if (Hero != NULL && CharacterAttribute != NULL)
		{
			DWORD dwMax = CharacterAttribute->PrintPlayer.ViewMaxHP;
			DWORD dwCur = CharacterAttribute->PrintPlayer.ViewCurHP;

			if (dwMax > 0)
			{
				float fLife = (float)dwCur / (float)dwMax;
				if (fLife < PG_DANGER_FROM)
				{
					float k = 1.f - (fLife / PG_DANGER_FROM);
					if (k < 0.f) k = 0.f;
					if (k > 1.f) k = 1.f;

					// Pull the tint toward red in proportion, and add the wash.
					float fD = PG_DANGER_TINT * k;
					g.r = g.r * (1.f - fD) + 1.00f * fD;
					g.g = g.g * (1.f - fD) + 0.28f * fD;
					g.b = g.b * (1.f - fD) + 0.24f * fD;
					g.fTint = g.fTint * (1.f - fD) + PG_DANGER_TINT * k;

					float fWash = PG_DANGER_WARM * k;

					// Once it is properly bad the wash beats rather than
					// holding, and the beat deepens as health falls further.
					if (fLife < PG_PULSE_FROM)
					{
						float p = 1.f - (fLife / PG_PULSE_FROM);
						if (p > 1.f) p = 1.f;

						// Kept off zero at the bottom of the swing, so it reads
						// as a pulse rather than a flicker.
						float fBeat = 0.5f + 0.5f * sinf((float)WorldTime * PG_PULSE_RATE);
						fWash *= 1.f - PG_PULSE_DEPTH * p * (1.f - fBeat);
					}

					g.fWarm += fWash;
				}
			}
		}

		return g;
	}
}

void PgGradeBurst(float r, float g, float b, float fStrength)
{
	if (fStrength <= 0.f)
		return;

	// A stronger burst takes the colour with it; a weaker one arriving while a
	// big one is still up does not get to repaint it.
	if (fStrength > s_fFlash)
	{
		s_fBurstR = r;
		s_fBurstG = g;
		s_fBurstB = b;
	}

	s_fFlash += fStrength;
	if (s_fFlash > PG_LEVELUP_BURST)
		s_fFlash = PG_LEVELUP_BURST;
}

void PgGradeHit(float fFraction)
{
	if (fFraction <= 0.f)
		return;
	if (fFraction > 1.f)
		fFraction = 1.f;

	float f = PG_FLASH_PER_HIT * fFraction;
	if (f > PG_FLASH_MAX)
		f = PG_FLASH_MAX;

	// Being hit is capped well below a level-up, so a busy spot never washes
	// the screen out.
	if (s_fFlash + f > PG_FLASH_MAX && s_fFlash < PG_FLASH_MAX)
		f = PG_FLASH_MAX - s_fFlash;
	else if (s_fFlash >= PG_FLASH_MAX)
		return;

	PgGradeBurst(1.f, 0.16f, 0.12f, f);
}

void PgGradeLevelUp()
{
	PgGradeBurst(1.f, 0.88f, 0.42f, PG_LEVELUP_BURST);
}

void PgGradeKill()
{
	// A gap longer than the window means the last streak is over.
	if (WorldTime - s_dLastKill > PG_STREAK_WINDOW)
		s_iStreak = 0;

	s_dLastKill = WorldTime;
	++s_iStreak;

	s_fHeat += PG_HEAT_PER_KILL;
	if (s_fHeat > PG_HEAT_MAX)
		s_fHeat = PG_HEAT_MAX;

	// One word per threshold, on the kill that crosses it.
	int iWord = PG_HITWORD_NONE;
	if (s_iStreak == PG_STREAK_SMASH)        iWord = PG_HITWORD_SMASH;
	else if (s_iStreak == PG_STREAK_DEADLY)  iWord = PG_HITWORD_DEADLY;
	else if (s_iStreak == PG_STREAK_PERFECT) iWord = PG_HITWORD_PERFECT;

	if (iWord != PG_HITWORD_NONE && Hero != NULL)
	{
		// Over the character rather than over a corpse: the streak is something
		// the player did, not something that happened to one monster.
		vec3_t p, c;
		VectorCopy(Hero->Object.Position, p);
		p[2] += 60.f;
		Vector(1.f, 0.86f, 0.35f, c);

		// Type -3 is the word on its own, with no number under it.
		CreatePoint(p, -3, c, 46.f, true, false, iWord);
	}
}

void PgGradeSnap()
{
	s_Current = TargetGrade();
	s_bHaveCurrent = true;
}

void PgGradeRender()
{
	if (SceneFlag != MAIN_SCENE)
		return;

	GRADE t = TargetGrade();

	// Streak heat rides on the warm wash, so a run of kills makes the scene
	// glow rather than putting a separate layer on top of it.
	t.fWarm += s_fHeat;

	if (s_bHaveCurrent == false)
	{
		s_Current = t;
		s_bHaveCurrent = true;
	}
	else
	{
		float f = PG_GRADE_EASE * FPS_ANIMATION_FACTOR;
		if (f > 1.f) f = 1.f;

		Ease(s_Current.r, t.r, f);
		Ease(s_Current.g, t.g, f);
		Ease(s_Current.b, t.b, f);
		Ease(s_Current.fTint, t.fTint, f);
		Ease(s_Current.fWarm, t.fWarm, f);
	}

	// Streak heat drains once the killing stops, and the streak itself lapses
	// with it so the next kill starts from one.
	if (WorldTime - s_dLastKill > PG_STREAK_WINDOW)
	{
		s_iStreak = 0;
		if (s_fHeat > 0.f)
		{
			s_fHeat -= PG_HEAT_DRAIN * FPS_ANIMATION_FACTOR;
			if (s_fHeat < 0.f) s_fHeat = 0.f;
		}
	}

	// The hit flash decays on its own clock, not the grade's, because it has to
	// snap in and fall away rather than slide.
	if (s_fFlash > 0.f)
	{
		s_fFlash -= PG_FLASH_DECAY * FPS_ANIMATION_FACTOR;
		if (s_fFlash < 0.f)
			s_fFlash = 0.f;
	}

	// Nothing to say, so say nothing - and skip the fill entirely.
	if (s_Current.fTint <= 0.002f && s_Current.fWarm <= 0.002f && s_fFlash <= 0.002f)
		return;

	const float w = (float)DisplayWin;
	const float h = (float)DisplayHeight;

	if (s_Current.fTint > 0.002f)
	{
		// Ordinary alpha blend, pulling the frame toward the grade colour.
		EnableAlphaTest();
		glColor4f(s_Current.r, s_Current.g, s_Current.b, s_Current.fTint);
		RenderColor(0.f, 0.f, w, h);
		EndRenderColor();
	}

	if (s_Current.fWarm > 0.002f)
	{
		// Additive, for the light the place throws back rather than the colour
		// it takes away.
		EnableAlphaBlend();
		glColor4f(s_Current.r * s_Current.fWarm,
			s_Current.g * s_Current.fWarm * 0.7f,
			s_Current.b * s_Current.fWarm * 0.55f, 1.f);
		RenderColor(0.f, 0.f, w, h);
		EndRenderColor();
	}

	// The hit flash goes last and on top: it is the loudest thing the grade has
	// to say, and it should not be tinted by the rest of it.
	if (s_fFlash > 0.002f)
	{
		EnableAlphaTest();
		glColor4f(s_fBurstR, s_fBurstG, s_fBurstB, s_fFlash);
		RenderColor(0.f, 0.f, w, h);
		EndRenderColor();
	}

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
}
