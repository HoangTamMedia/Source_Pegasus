#pragma once

// A batcher for the 2D layer.
//
// RenderBitmap draws one quad per call in immediate mode: a BindTexture, a
// glBegin, four vertices, a glEnd. What the UI costs is therefore the number of
// calls, not the number of pixels - the health bars alone put one of those
// through the pipe for every monster on screen, every frame, all of them from
// the same texture.
//
// This gathers quads into one vertex array and issues a single glDrawArrays per
// run of quads that share a texture. Thirty health bars become one draw call
// instead of thirty. The vertex maths is copied from RenderBitmap exactly, so
// the output is the same geometry in the same order.
//
// HOW TO USE IT SAFELY
//
//   PgBatchBegin();
//     ... PgBatchImageFFF(...) as many times as you like ...
//   PgBatchEnd();
//
// Inside a scope, draw NOTHING except through this. The quads are held back
// until a flush, so anything else that draws in between would come out
// underneath them instead of interleaved. A texture change flushes on its own,
// which is what keeps overlapping art in the right order.
//
// The batch never touches matrices, blending or the depth state. Set those
// before Begin the way you always would, and leave them alone until End - the
// batch inherits whatever is current, which is why the result matches the
// immediate-mode path it replaces.

// Call once after the GL context exists. Safe to call again; safe to skip, in
// which case the first Begin does it.
void PgBatchInit();

// Released on shutdown. Safe if Init never ran.
void PgBatchRelease();

void PgBatchBegin();
void PgBatchEnd();

// Sends everything gathered so far. Called for you on a texture change, when
// the buffer fills, and at End. Call it by hand if you have to break a scope
// open to draw something else.
void PgBatchFlush();

// Modulate colour for the quads that follow, matching a glColor4f you would
// otherwise make. Resets to opaque white at Begin.
void PgBatchSetColor(float r, float g, float b, float a);

// Same arguments and the same meaning as SEASON3B::RenderImageFFF: su, sv, uw
// and vh are in texels of the source art, not normalised.
void PgBatchImageFFF(unsigned int uiImage, float x, float y, float w, float h,
	float su, float sv, float uw, float vh);

// The kill switch. Off sends every quad straight to RenderBitmap, which is the
// behaviour this replaced - use it to A/B the batch against the original
// without rebuilding anything else.
void PgBatchSetEnabled(bool bEnable);
bool PgBatchIsEnabled();

// Draw calls saved since the last call to this. Diagnostics only.
int  PgBatchTakeSavedCalls();
