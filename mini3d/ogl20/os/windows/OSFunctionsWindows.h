
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifdef _WIN32

#ifndef MINI3D_OSFUNCTIONWINDOWS_H
#define MINI3D_OSFUNCTIONWINDOWS_H

#include <windows.h>
#include "../IOSFunction.h"
#include <GL/wglext.h>

namespace mini3d
{
class OSFunctions : IOSFunctions
{
public:
	
	OSFunctions();
	virtual ~OSFunctions();

	virtual void Init();

	// Used by OPENGL for determining setting buffer and depth buffer format
	virtual unsigned int GetMonitorBitDepth() const;
	virtual void GetClientAreaSize(int windowHandle, unsigned int &width, unsigned int &height) const;
	virtual void Log(char* message) const;

	// Device creation
	void CreateDevice();
	void CreateInternalWindow();

	// ---------- ABSTRACT OPENGL FUNCTIONS --------------------------------------
	// These functions preform opengl operations but they do not map 1-1 against opengl functions.
	// what they do is platform dependent
	virtual void PrepareWindow(const int& hWindow) const;
	virtual void SetRenderWindow(const int& hWindow) const;
	virtual void SetFullscreenRenderWindow(const int& hWindow, const unsigned int& width, const unsigned int& height) const;
	virtual void SetDefaultRenderWindow() const;
	virtual void SwapWindowBuffers(const int& hWindow) const;

	// ---------- OPEN GL FUNCTIONS ----------------------------------------------

	// GENERAL
	virtual void GLSwapBuffers() const {};
	virtual void GLBindFramebuffer(GLenum target, GLuint framebuffer) const { glBindFramebuffer(target, framebuffer); };
	//virtual void GLMakeCurrent(const DisplayContext displayContext, const WindowContext windowContext, const GLRenderingContext renderingContext) const {};
	virtual void GLViewport(const unsigned int width, const unsigned int height) const {};

	virtual void GLShadeModel(GLenum mode) const { glShadeModel(mode); };
	virtual void GLEnable(GLenum cap) const { glEnable(cap); };
	virtual void GLDepthFunc(GLenum func) const { glDepthFunc(func); };

	//CAPS
	virtual void GLGetIntegerv(GLenum pname, GLint *params) const { glGetIntegerv(pname, params); };

	// TEXTURE
	virtual void GLActiveTexture(GLenum texture) const { glActiveTexture(texture); };
	virtual void GLBindTexture(GLenum target, GLuint texture) const { glBindTexture(target, texture); };
	virtual void GLTexParameteri(GLenum target, GLenum pname, GLint params) const { glTexParameteri(target, pname, params); };	

	virtual void GLGenRenderbuffers(GLsizei n, GLuint* renderbuffers) const { glGenRenderbuffers(n, renderbuffers); };
	virtual void GLBindRenderbuffer(GLenum target, GLuint renderbuffer) const {glBindRenderbuffer(target, renderbuffer); };
	virtual void GLRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) const { glRenderbufferStorage(target, internalformat, width, height); };

	virtual void GLDeleteRenderbuffers(GLsizei n, GLuint* renderbuffers) const { glDeleteRenderbuffers(n, renderbuffers); };
	virtual void GLDeleteFramebuffers(GLsizei n, GLuint* framebuffers) const{ glDeleteFramebuffers(n, framebuffers); };

	virtual void GLGenFramebuffers(GLsizei n, GLuint* ids) const { glGenFramebuffers(n, ids); };

	virtual void GLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) const { glFramebufferTexture2D(target, attachment, textarget, texture, level); };
	virtual void GLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) const { glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer); };

	// SHADER FUNCTIONS
	virtual void GLUseProgram(GLuint program) const { glUseProgram(program); }; 
	
	virtual void GLEnableVertexAttribArray(GLuint index) const { glEnableVertexAttribArray(index); };
	virtual void GLDisableVertexAttribArray(GLuint index) const { glDisableVertexAttribArray(index); };
	virtual void GLVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const { glVertexAttribPointer(index, size, type, normalized, stride, pointer); };

	virtual GLboolean GLIsShader(GLuint shader) const { return glIsShader(shader); };

	virtual GLuint GLCreateShader(GLenum shaderType) const { return glCreateShader(shaderType); };
	virtual void GLShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) const { glShaderSource(shader, count, string, length); };
	virtual void GLCompileShader(GLuint shader) const { glCompileShader(shader); };
	virtual void GLDeleteShader(GLuint shader) const { glDeleteShader(shader); };

	virtual void GLGetShaderiv(GLuint shader, GLenum pname, GLint *params) const { glGetShaderiv(shader, pname, params); };
	virtual void GLGetProgramiv(GLenum target, GLenum pname, GLint *params) const { glGetProgramiv(target,pname,params); };
	virtual void GLGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const { glGetShaderInfoLog(shader, bufSize, length, infoLog); };
	virtual void GLGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const { glGetProgramInfoLog(program, bufSize, length, infoLog); };

	virtual void GLDeleteProgram(GLuint program) const { glDeleteProgram(program); };
	virtual GLuint GLCreateProgram() const { return glCreateProgram(); };
	virtual void GLAttachShader(GLuint program, GLuint shader) const { glAttachShader(program, shader); };
	virtual void GLLinkProgram(GLuint program) const { glLinkProgram(program); };

	// SHADER PROGRAM FUNCTIONS
	virtual void GLGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) const { glGetActiveAttrib(program, index, bufSize, length, size, type, name); };
	virtual GLint GLGetAttribLocation(GLuint program, const GLchar* name) const { return glGetAttribLocation(program, name); };

	virtual void GLGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) const { glGetActiveUniform(program, index, bufSize, length, size, type, name); };
	virtual GLint GLGetUniformLocation(GLuint program, const GLchar* name) const { return glGetUniformLocation(program, name); };

	// SHADER PARAMETERS
	virtual void GLUniform1f(GLint location, GLfloat v0) const { glUniform1f(location, v0); };
	virtual void GLUniform2f(GLint location, GLfloat v0, GLfloat v1) const { glUniform2f(location, v0, v1); };
	virtual void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const { glUniform3f(location, v0, v1, v2); };
	virtual void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const { glUniform4f(location, v0, v1, v2, v3); };

	virtual void GLUniform1i(GLint location, GLint  v0) const { glUniform1i(location, v0); };
	virtual void GLUniform2i(GLint location, GLint  v0, GLint  v1) const { glUniform2i(location, v0, v1); };
	virtual void GLUniform3i(GLint location, GLint  v0, GLint  v1, GLint  v2) const { glUniform3i(location, v0, v1, v2); };
	virtual void GLUniform4i(GLint location, GLint  v0, GLint  v1, GLint  v2, GLint  v3) const { glUniform4i(location, v0, v1, v2, v3); };

	virtual void GLUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) const { glUniformMatrix4fv(location, count, transpose, value); };
	
	// GEOMETRY
	virtual void GLBindBuffer(GLenum target, GLuint buffer) const { glBindBuffer(target, buffer); };
	virtual void* GLMapBuffer(GLenum target, GLenum access) const { return glMapBuffer(target, access); };
	virtual void GLUnmapBuffer(GLenum target) const { glUnmapBuffer(target); };
	virtual void GLBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) const { glBufferData(target, size, data, usage); };
	virtual void GLGenBuffers(GLsizei n, GLuint* buffers) const { glGenBuffers(n, buffers); };
	virtual void GLDeleteBuffers(GLsizei n, GLuint* buffers) const { glDeleteBuffers(n, buffers); };

	// DRAWING
	virtual void GLEnableClientState(GLenum target) const { glEnableClientState(target); };
	virtual void GLDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) const { glDrawElements(mode, count, type, indices); };
    virtual void GLDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *   indices) const { glDrawRangeElements(mode, start, end, count, type, indices); };
	virtual void GLDisableClientState(GLenum array) const { glDisableClientState(array); };

	virtual void GLClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) const { glClearColor(red, green, blue, alpha); };
	virtual void GLClearDepth(GLdouble depth) const { glClearDepth(depth); };
	virtual void GLClear(GLbitfield mask) const { glClear(mask); };


	// --------- Private variables -----------------------------------------------

	PFNGLISSHADERPROC glIsShader;

	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLDELETESHADERPROC glDeleteShader;

	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETPROGRAMIVNVPROC glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLLINKPROGRAMPROC glLinkProgram;

	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;

	PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
	PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
	PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;

	PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
	PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;

	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
	
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLMAPBUFFERPROC glMapBuffer;
	PFNGLUNMAPBUFFERPROC glUnmapBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;

	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM2FPROC glUniform2f;
	PFNGLUNIFORM3FPROC glUniform3f;
	PFNGLUNIFORM4FPROC glUniform4f;

	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM2IPROC glUniform2i;
	PFNGLUNIFORM3IPROC glUniform3i;
	PFNGLUNIFORM4IPROC glUniform4i;
	
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

	PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;

	// Default window and render context
	HWND hWindow;
	HDC hDeviceContext;
	HGLRC hRenderContext;
};
}

#endif
#endif