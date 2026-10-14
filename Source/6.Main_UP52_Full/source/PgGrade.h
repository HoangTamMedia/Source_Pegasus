#pragma once

// Colour grading that knows what is happening in the game.
//
// QeffectsGL already grades the frame - saturation, lightness, emboss, vignette
// - but it only ever sees pixels. It cannot know that the map is a frozen one,
// or that the character is about to die. This layer sits between the world and
// the interface and grades from game state instead: the screen cools underground
// and in the ice maps, warms in the lava ones, and washes red as health runs
// out.
//
// It is deliberately not a framebuffer pass. QeffectsGL owns the final image,
// and a second system capturing the same frame is the kind of thing that breaks
// quietly on somebody else's driver. Two blended full-screen quads through the
// engine's own state wrappers get most of the effect and cannot fight anything.
//
// Every grade eases toward its target, so walking through a portal is a slide
// rather than a cut.

// Called once per frame from the scene, after the world and before the
// interface - see the call in ZzzScene.
void PgGradeRender();

// Force the current grade to its target immediately. For map changes, where a
// slide across a loading screen would arrive late and look like a fault.
void PgGradeSnap();

// A hit landed on the character. fFraction is the damage as a share of maximum
// health, so a scratch barely registers and something that takes a third of the
// bar is unmistakable. Decays on its own; nothing has to switch it off.
void PgGradeHit(float fFraction);

// A one-off wash of any colour - the general form of the hit flash. Snaps in
// and falls away rather than sliding, so it reads as an event.
void PgGradeBurst(float r, float g, float b, float fStrength);

// A monster died. The streak counter lives here because what it produces is
// screen feedback: the words at the thresholds, and the heat that builds while
// the killing keeps up and drains away when it stops.
void PgGradeKill();

// The character gained a level. Not the master-experience packet, which arrives
// on every kill - only the moment the level itself changes.
void PgGradeLevelUp();
