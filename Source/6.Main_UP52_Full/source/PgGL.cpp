#include "stdafx.h"
#include "PgGL.h"
#include "./Utilities/Log/muConsoleDebug.h"

PFN_PG_GENBUFFERS         pgGenBuffers = NULL;
PFN_PG_DELETEBUFFERS      pgDeleteBuffers = NULL;
PFN_PG_BINDBUFFER         pgBindBuffer = NULL;
PFN_PG_BUFFERDATA         pgBufferData = NULL;
PFN_PG_BUFFERSUBDATA      pgBufferSubData = NULL;

PFN_PG_CREATESHADER       pgCreateShader = NULL;
PFN_PG_DELETESHADER       pgDeleteShader = NULL;
PFN_PG_SHADERSOURCE       pgShaderSource = NULL;
PFN_PG_COMPILESHADER      pgCompileShader = NULL;
PFN_PG_GETSHADERIV        pgGetShaderiv = NULL;
PFN_PG_GETSHADERINFOLOG   pgGetShaderInfoLog = NULL;
PFN_PG_CREATEPROGRAM      pgCreateProgram = NULL;
PFN_PG_DELETEPROGRAM      pgDeleteProgram = NULL;
PFN_PG_ATTACHSHADER       pgAttachShader = NULL;
PFN_PG_LINKPROGRAM        pgLinkProgram = NULL;
PFN_PG_GETPROGRAMIV       pgGetProgramiv = NULL;
PFN_PG_GETPROGRAMINFOLOG  pgGetProgramInfoLog = NULL;
PFN_PG_USEPROGRAM         pgUseProgram = NULL;
PFN_PG_GETUNIFORMLOCATION pgGetUniformLocation = NULL;
PFN_PG_UNIFORM1I          pgUniform1i = NULL;
PFN_PG_UNIFORM1F          pgUniform1f = NULL;
PFN_PG_UNIFORMMATRIX4FV   pgUniformMatrix4fv = NULL;

namespace
{
	bool s_bInit = false;
	bool s_bVBO = false;
	bool s_bShaders = false;

	// wglGetProcAddress only answers for the context that is current, and it
	// returns NULL - and on some drivers 1, 2, 3 or -1 - when the entry point is
	// not there. The odd values have to be screened out or they get called.
	void* Get(const char* szName)
	{
		PROC p = wglGetProcAddress(szName);
		if (p == NULL
			|| p == (PROC)0x1 || p == (PROC)0x2 || p == (PROC)0x3
			|| p == (PROC)-1)
		{
			return NULL;
		}
		return (void*)p;
	}
}

void PgGLInit()
{
	if (s_bInit)
		return;
	s_bInit = true;

	if (wglGetCurrentContext() == NULL)
	{
		// No context yet, so nothing can be resolved. Leave s_bInit set anyway:
		// a caller that asks this early gets the fixed-function path, which is
		// always correct, just slower.
		g_ConsoleDebug->Write(MCD_ERROR, "[PgGL] no current context, buffer objects off");
		return;
	}

	pgGenBuffers    = (PFN_PG_GENBUFFERS)   Get("glGenBuffers");
	pgDeleteBuffers = (PFN_PG_DELETEBUFFERS)Get("glDeleteBuffers");
	pgBindBuffer    = (PFN_PG_BINDBUFFER)   Get("glBindBuffer");
	pgBufferData    = (PFN_PG_BUFFERDATA)   Get("glBufferData");
	pgBufferSubData = (PFN_PG_BUFFERSUBDATA)Get("glBufferSubData");

	// ARB_vertex_buffer_object spells them with a suffix. Drivers that expose
	// only the extension and not core 1.5 still work through these.
	if (pgGenBuffers == NULL)
	{
		pgGenBuffers    = (PFN_PG_GENBUFFERS)   Get("glGenBuffersARB");
		pgDeleteBuffers = (PFN_PG_DELETEBUFFERS)Get("glDeleteBuffersARB");
		pgBindBuffer    = (PFN_PG_BINDBUFFER)   Get("glBindBufferARB");
		pgBufferData    = (PFN_PG_BUFFERDATA)   Get("glBufferDataARB");
		pgBufferSubData = (PFN_PG_BUFFERSUBDATA)Get("glBufferSubDataARB");
	}

	s_bVBO = (pgGenBuffers != NULL && pgBindBuffer != NULL
		&& pgBufferData != NULL && pgDeleteBuffers != NULL);

	pgCreateShader       = (PFN_PG_CREATESHADER)      Get("glCreateShader");
	pgDeleteShader       = (PFN_PG_DELETESHADER)      Get("glDeleteShader");
	pgShaderSource       = (PFN_PG_SHADERSOURCE)      Get("glShaderSource");
	pgCompileShader      = (PFN_PG_COMPILESHADER)     Get("glCompileShader");
	pgGetShaderiv        = (PFN_PG_GETSHADERIV)       Get("glGetShaderiv");
	pgGetShaderInfoLog   = (PFN_PG_GETSHADERINFOLOG)  Get("glGetShaderInfoLog");
	pgCreateProgram      = (PFN_PG_CREATEPROGRAM)     Get("glCreateProgram");
	pgDeleteProgram      = (PFN_PG_DELETEPROGRAM)     Get("glDeleteProgram");
	pgAttachShader       = (PFN_PG_ATTACHSHADER)      Get("glAttachShader");
	pgLinkProgram        = (PFN_PG_LINKPROGRAM)       Get("glLinkProgram");
	pgGetProgramiv       = (PFN_PG_GETPROGRAMIV)      Get("glGetProgramiv");
	pgGetProgramInfoLog  = (PFN_PG_GETPROGRAMINFOLOG) Get("glGetProgramInfoLog");
	pgUseProgram         = (PFN_PG_USEPROGRAM)        Get("glUseProgram");
	pgGetUniformLocation = (PFN_PG_GETUNIFORMLOCATION)Get("glGetUniformLocation");
	pgUniform1i          = (PFN_PG_UNIFORM1I)         Get("glUniform1i");
	pgUniform1f          = (PFN_PG_UNIFORM1F)         Get("glUniform1f");
	pgUniformMatrix4fv   = (PFN_PG_UNIFORMMATRIX4FV)  Get("glUniformMatrix4fv");

	s_bShaders = (pgCreateShader != NULL && pgShaderSource != NULL
		&& pgCompileShader != NULL && pgCreateProgram != NULL
		&& pgAttachShader != NULL && pgLinkProgram != NULL
		&& pgUseProgram != NULL && pgGetUniformLocation != NULL
		&& pgUniform1i != NULL && pgUniformMatrix4fv != NULL);

	const char* szVendor = (const char*)glGetString(GL_VENDOR);
	const char* szVersion = (const char*)glGetString(GL_VERSION);

	g_ConsoleDebug->Write(MCD_NORMAL, "[PgGL] vendor=%s | version=%s | vbo=%s | shaders=%s",
		szVendor ? szVendor : "?", szVersion ? szVersion : "?",
		s_bVBO ? "YES" : "no", s_bShaders ? "YES" : "no");
}

bool PgGLHasVBO()
{
	return s_bVBO;
}

bool PgGLHasShaders()
{
	return s_bShaders;
}
