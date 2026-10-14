#include "stdafx.h"
#include "PgSpriteBatch.h"
#include "PgGL.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "CBInterface.h"		// g_fScreenRate_x / _y
#include "./Utilities/Log/muConsoleDebug.h"

namespace
{
	// 2048 quads is 40KB of vertices. A frame never gets near it - the point of
	// the cap is only that a runaway caller flushes instead of growing.
	const int PG_BATCH_MAX_QUADS = 2048;

	struct PG_VERTEX
	{
		float x, y;
		float u, v;
		unsigned char r, g, b, a;
	};

	PG_VERTEX s_aVertex[PG_BATCH_MAX_QUADS * 4];

	int    s_iQuads     = 0;
	int    s_iTexture   = -1;		// bitmap index, not the GL name
	bool   s_bInScope   = false;
	bool   s_bEnabled   = true;
	bool   s_bInit      = false;
	GLuint s_uiVBO      = 0;
	int    s_iSavedCalls = 0;

	unsigned char s_ucR = 255, s_ucG = 255, s_ucB = 255, s_ucA = 255;

	unsigned char ToByte(float f)
	{
		if (f <= 0.f) return 0;
		if (f >= 1.f) return 255;
		return (unsigned char)(f * 255.f + 0.5f);
	}
}

void PgBatchInit()
{
	if (s_bInit)
		return;
	s_bInit = true;

	PgGLInit();

	if (PgGLHasVBO())
	{
		pgGenBuffers(1, &s_uiVBO);
		if (s_uiVBO == 0)
		{
			g_ConsoleDebug->Write(MCD_ERROR, "[PgBatch] glGenBuffers gave nothing, using client arrays");
		}
	}

	// Without a buffer object the same vertices go through client-side arrays
	// instead. That is core GL 1.1, so it is always there, and it still collapses
	// a run of quads into one draw call - the buffer object only saves the copy.
	g_ConsoleDebug->Write(MCD_NORMAL, "[PgBatch] ready | vbo=%s", s_uiVBO ? "YES" : "no");
}

void PgBatchRelease()
{
	if (s_uiVBO != 0 && pgDeleteBuffers != NULL)
	{
		pgDeleteBuffers(1, &s_uiVBO);
		s_uiVBO = 0;
	}
	s_bInit = false;
}

void PgBatchSetEnabled(bool bEnable)
{
	if (s_bInScope)
		PgBatchFlush();
	s_bEnabled = bEnable;
}

bool PgBatchIsEnabled()
{
	return s_bEnabled;
}

int PgBatchTakeSavedCalls()
{
	int n = s_iSavedCalls;
	s_iSavedCalls = 0;
	return n;
}

void PgBatchBegin()
{
	if (s_bInit == false)
		PgBatchInit();

	s_bInScope = true;
	s_iQuads   = 0;
	s_iTexture = -1;

	s_ucR = s_ucG = s_ucB = s_ucA = 255;
}

void PgBatchSetColor(float r, float g, float b, float a)
{
	// The colour rides on the vertices, so a change does not need a flush.
	s_ucR = ToByte(r);
	s_ucG = ToByte(g);
	s_ucB = ToByte(b);
	s_ucA = ToByte(a);
}

void PgBatchFlush()
{
	if (s_iQuads <= 0)
	{
		s_iTexture = -1;
		return;
	}

	const int iVerts = s_iQuads * 4;

	// One bind for the whole run - the immediate path did one per quad.
	BindTexture(s_iTexture);

	// glColorPointer overrides the current colour for these vertices, so the
	// caller's glColor4f does not reach them. That is why the modulate is
	// carried per vertex instead.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	const GLsizei iStride = sizeof(PG_VERTEX);

	if (s_uiVBO != 0)
	{
		pgBindBuffer(GL_ARRAY_BUFFER, s_uiVBO);

		// Orphan first: handing the driver a fresh store means it never has to
		// stall waiting for the previous frame's draw to finish reading.
		pgBufferData(GL_ARRAY_BUFFER,
			(PgGLsizeiptr)(sizeof(PG_VERTEX) * PG_BATCH_MAX_QUADS * 4),
			NULL, GL_STREAM_DRAW);
		pgBufferData(GL_ARRAY_BUFFER,
			(PgGLsizeiptr)(sizeof(PG_VERTEX) * iVerts),
			s_aVertex, GL_STREAM_DRAW);

		glVertexPointer  (2, GL_FLOAT,         iStride, (const void*)0);
		glTexCoordPointer(2, GL_FLOAT,         iStride, (const void*)8);
		glColorPointer   (4, GL_UNSIGNED_BYTE, iStride, (const void*)16);
	}
	else
	{
		glVertexPointer  (2, GL_FLOAT,         iStride, &s_aVertex[0].x);
		glTexCoordPointer(2, GL_FLOAT,         iStride, &s_aVertex[0].u);
		glColorPointer   (4, GL_UNSIGNED_BYTE, iStride, &s_aVertex[0].r);
	}

	// The corners go round in the order RenderBitmap emitted them, so a quad
	// covers exactly the same triangles it did before.
	glDrawArrays(GL_QUADS, 0, iVerts);

	if (s_uiVBO != 0)
		pgBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// A colour array leaves the current colour undefined on some drivers. Put it
	// back so the next immediate-mode caller is not drawing with our last tint.
	glColor4f(1.f, 1.f, 1.f, 1.f);

	s_iSavedCalls += (s_iQuads - 1);

	s_iQuads   = 0;
	s_iTexture = -1;
}

void PgBatchEnd()
{
	PgBatchFlush();
	s_bInScope = false;
}

void PgBatchImageFFF(unsigned int uiImage, float x, float y, float w, float h,
	float su, float sv, float uw, float vh)
{
	BITMAP_t* pTex = Bitmaps.FindTexture(uiImage);
	if (pTex == NULL || pTex->Width <= 0.f || pTex->Height <= 0.f)
		return;

	// Normalised the same way RenderImageFFF does it.
	const float u  = su / pTex->Width;
	const float v  = sv / pTex->Height;
	const float uW = uw / pTex->Width;
	const float vH = vh / pTex->Height;

	if (s_bEnabled == false || s_bInScope == false)
	{
		// Straight through, so a caller outside a scope - or with the batch
		// switched off - behaves exactly as it did before.
		glColor4f(s_ucR / 255.f, s_ucG / 255.f, s_ucB / 255.f, s_ucA / 255.f);
		RenderBitmap(uiImage, x, y, w, h, u, v, uW, vH, 1, 1, 0.f);
		return;
	}

	if (s_iTexture != (int)uiImage || s_iQuads >= PG_BATCH_MAX_QUADS)
	{
		PgBatchFlush();
		s_iTexture = (int)uiImage;
	}

	// Straight out of RenderBitmap: interface units scale up by the screen rate,
	// and y is measured from the bottom.
	// ConvertX and ConvertY are file-local to ZzzOpenglUtil and are only the
	// screen-rate multiply, so they are inlined here the same way the flare code
	// has to inline them.
	x *= g_fScreenRate_x;
	y *= g_fScreenRate_y;
	w *= g_fScreenRate_x;
	h *= g_fScreenRate_y;

	y = (float)WindowHeight - y;

	PG_VERTEX* p = &s_aVertex[s_iQuads * 4];

	p[0].x = x;     p[0].y = y;     p[0].u = u;      p[0].v = v;
	p[1].x = x;     p[1].y = y - h; p[1].u = u;      p[1].v = v + vH;
	p[2].x = x + w; p[2].y = y - h; p[2].u = u + uW; p[2].v = v + vH;
	p[3].x = x + w; p[3].y = y;     p[3].u = u + uW; p[3].v = v;

	for (int i = 0; i < 4; ++i)
	{
		p[i].r = s_ucR;
		p[i].g = s_ucG;
		p[i].b = s_ucB;
		p[i].a = s_ucA;
	}

	++s_iQuads;
}
