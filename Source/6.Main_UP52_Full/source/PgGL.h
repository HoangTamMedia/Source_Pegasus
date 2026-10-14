#pragma once

// Entry points for the buffer-object and shader extensions.
//
// stdafx.h pulls in gl/glew.h, but glewInit is never called and glew32.lib is
// not linked - GLEW is here for its constants and typedefs only, and touching
// any glew function symbol would fail to link. glprocs.lib is linked instead,
// but it reaches its entry points through a TLS proc table whose macros collide
// with the GLEW ones. So the pointers are loaded here by hand, the same way
// ZzzOpenglUtil already loads wglSwapIntervalEXT, which keeps this self
// contained: no new library, no loader to initialise, nothing to collide with.
//
// Every pointer may legitimately be NULL. PgGLHasVBO and PgGLHasShaders say
// whether the whole set a feature needs came back, and callers must fall back
// rather than assume.

#include <windows.h>
#include <gl/GL.h>

// GL 1.5 / ARB_vertex_buffer_object. The typedefs are spelled out rather than
// taken from glext.h so this header does not depend on include order.
typedef ptrdiff_t PgGLsizeiptr;
typedef ptrdiff_t PgGLintptr;

typedef void   (APIENTRY* PFN_PG_GENBUFFERS)(GLsizei, GLuint*);
typedef void   (APIENTRY* PFN_PG_DELETEBUFFERS)(GLsizei, const GLuint*);
typedef void   (APIENTRY* PFN_PG_BINDBUFFER)(GLenum, GLuint);
typedef void   (APIENTRY* PFN_PG_BUFFERDATA)(GLenum, PgGLsizeiptr, const void*, GLenum);
typedef void   (APIENTRY* PFN_PG_BUFFERSUBDATA)(GLenum, PgGLintptr, PgGLsizeiptr, const void*);

// GL 2.0 shader objects.
typedef GLuint (APIENTRY* PFN_PG_CREATESHADER)(GLenum);
typedef void   (APIENTRY* PFN_PG_DELETESHADER)(GLuint);
typedef void   (APIENTRY* PFN_PG_SHADERSOURCE)(GLuint, GLsizei, const char* const*, const GLint*);
typedef void   (APIENTRY* PFN_PG_COMPILESHADER)(GLuint);
typedef void   (APIENTRY* PFN_PG_GETSHADERIV)(GLuint, GLenum, GLint*);
typedef void   (APIENTRY* PFN_PG_GETSHADERINFOLOG)(GLuint, GLsizei, GLsizei*, char*);
typedef GLuint (APIENTRY* PFN_PG_CREATEPROGRAM)(void);
typedef void   (APIENTRY* PFN_PG_DELETEPROGRAM)(GLuint);
typedef void   (APIENTRY* PFN_PG_ATTACHSHADER)(GLuint, GLuint);
typedef void   (APIENTRY* PFN_PG_LINKPROGRAM)(GLuint);
typedef void   (APIENTRY* PFN_PG_GETPROGRAMIV)(GLuint, GLenum, GLint*);
typedef void   (APIENTRY* PFN_PG_GETPROGRAMINFOLOG)(GLuint, GLsizei, GLsizei*, char*);
typedef void   (APIENTRY* PFN_PG_USEPROGRAM)(GLuint);
typedef GLint  (APIENTRY* PFN_PG_GETUNIFORMLOCATION)(GLuint, const char*);
typedef void   (APIENTRY* PFN_PG_UNIFORM1I)(GLint, GLint);
typedef void   (APIENTRY* PFN_PG_UNIFORM1F)(GLint, GLfloat);
typedef void   (APIENTRY* PFN_PG_UNIFORMMATRIX4FV)(GLint, GLsizei, GLboolean, const GLfloat*);

extern PFN_PG_GENBUFFERS         pgGenBuffers;
extern PFN_PG_DELETEBUFFERS      pgDeleteBuffers;
extern PFN_PG_BINDBUFFER         pgBindBuffer;
extern PFN_PG_BUFFERDATA         pgBufferData;
extern PFN_PG_BUFFERSUBDATA      pgBufferSubData;

extern PFN_PG_CREATESHADER       pgCreateShader;
extern PFN_PG_DELETESHADER       pgDeleteShader;
extern PFN_PG_SHADERSOURCE       pgShaderSource;
extern PFN_PG_COMPILESHADER      pgCompileShader;
extern PFN_PG_GETSHADERIV        pgGetShaderiv;
extern PFN_PG_GETSHADERINFOLOG   pgGetShaderInfoLog;
extern PFN_PG_CREATEPROGRAM      pgCreateProgram;
extern PFN_PG_DELETEPROGRAM      pgDeleteProgram;
extern PFN_PG_ATTACHSHADER       pgAttachShader;
extern PFN_PG_LINKPROGRAM        pgLinkProgram;
extern PFN_PG_GETPROGRAMIV       pgGetProgramiv;
extern PFN_PG_GETPROGRAMINFOLOG  pgGetProgramInfoLog;
extern PFN_PG_USEPROGRAM         pgUseProgram;
extern PFN_PG_GETUNIFORMLOCATION pgGetUniformLocation;
extern PFN_PG_UNIFORM1I          pgUniform1i;
extern PFN_PG_UNIFORM1F          pgUniform1f;
extern PFN_PG_UNIFORMMATRIX4FV   pgUniformMatrix4fv;

// Constants, in case the GL headers in scope predate them.
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER          0x8892
#endif
#ifndef GL_STREAM_DRAW
#define GL_STREAM_DRAW           0x88E0
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER       0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER         0x8B31
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS        0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS           0x8B82
#endif

// Call once, after the rendering context is current. Safe to call again.
void PgGLInit();

bool PgGLHasVBO();
bool PgGLHasShaders();
