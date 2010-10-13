
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_OSWINDOWS_H
#define MINI3D_OSWINDOWS_H

#include "../IOS.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

namespace mini3d
{
class OSWindows : public IOS
{
public:
	
	OSWindows();
	virtual ~OSWindows() {};

	virtual void Init();

	// Used by OPENGL for determining setting buffer and depth buffer format
	virtual unsigned int GetMonitorBitDepth() const;
	virtual void GetClientAreaSize(int windowHandle, unsigned int &width, unsigned int &height) const;
	virtual void Log(wchar_t* message) const;

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
	
	// SHADER FUNCTIONS
	virtual void GLUseProgram(GLuint program) const { glUseProgram(program); }; 
	
	virtual void GLEnableVertexAttribArray(GLuint index) const { glEnableVertexAttribArray(index); };
	virtual void GLDisableVertexAttribArray(GLuint index) const { };
	virtual void GLVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const { glVertexAttribPointer(index, size, type, normalized, stride, pointer); };

	virtual GLboolean GLIsShader(GLuint shader) const { return glIsShader(shader); };
	virtual void GLGetShaderiv(GLuint shader, GLenum pname, GLint *params) const { glGetShaderiv(shader, pname, params); };
	virtual void GLGetProgramiv(GLenum target, GLenum pname, GLint *params) const { glGetProgramiv(target,pname,params); };
	virtual void GLGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const { glGetShaderInfoLog(shader, bufSize, length, infoLog); };
	virtual void GLGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const { glGetProgramInfoLog(program, bufSize, length, infoLog); };

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

	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETPROGRAMIVNVPROC glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;

	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;

	PFNGLBINDBUFFERPROC glBindBuffer;

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
};
}

#endif
