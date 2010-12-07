
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>

#ifndef MINI3D_IOSFUNCTION_H
#define MINI3D_IOSFUNCTION_H

// Set the data type for the MINI3D_WINDOW macro
#include "../../mini3d/datatypes/DataTypes.h"

#include <GL/gl.h>
#include <GL/glext.h>

typedef char GLchar;

namespace mini3d
{
class IOGLWrapper
{
public:
	
	virtual ~IOGLWrapper() {};

	// Init the OS
	virtual void Init() = 0;

	// Device creation
	virtual void CreateDevice() = 0;
	virtual void CreateInternalWindow() = 0;

	// ---------- ABSTRACT OPENGL FUNCTIONS --------------------------------------
	// These functions preform opengl operations but they do not necessarily map 1-1 against any opengl functions.
	// what they do depends on the platform opengl implementation.
	
	virtual void PrepareWindow(const MINI3D_WINDOW hWindow) const = 0;
	virtual void SetRenderWindow(const MINI3D_WINDOW hWindow) const = 0;
	virtual void SetDefaultRenderWindow() const = 0;
	virtual void SwapWindowBuffers(const MINI3D_WINDOW hWindow) const = 0;

	// ---------- OPEN GL FUNCTIONS ----------------------------------------------

	// GENERAL
	void GLSwapBuffers() const {};
	void GLBindFramebuffer(GLenum target, GLuint framebuffer) const { glBindFramebufferEXT(target, framebuffer); };
	void GLViewport(const unsigned int width, const unsigned int height) const {};

	void GLShadeModel(GLenum mode) const { glShadeModel(mode); };
	void GLEnable(GLenum cap) const { glEnable(cap); };
	void GLDepthFunc(GLenum func) const { glDepthFunc(func); };

	//CAPS
	void GLGetIntegerv(GLenum pname, GLint *params) const { glGetIntegerv(pname, params); };

	// TEXTURE
	void GLActiveTexture(GLenum texture) const { glActiveTexture(texture); };
	void GLBindTexture(GLenum target, GLuint texture) const { glBindTexture(target, texture); };
	void GLTexParameteri(GLenum target, GLenum pname, GLint params) const { glTexParameteri(target, pname, params); };	

	void GLGenRenderbuffers(GLsizei n, GLuint* renderbuffers) const { glGenRenderbuffersEXT(n, renderbuffers); };
	void GLBindRenderbuffer(GLenum target, GLuint renderbuffer) const {glBindRenderbufferEXT(target, renderbuffer); };
	void GLRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) const { glRenderbufferStorageEXT(target, internalformat, width, height); };

	void GLDeleteRenderbuffers(GLsizei n, GLuint* renderbuffers) const { glDeleteRenderbuffersEXT(n, renderbuffers); };
	void GLDeleteFramebuffers(GLsizei n, GLuint* framebuffers) const{ glDeleteFramebuffersEXT(n, framebuffers); };

	void GLGenFramebuffers(GLsizei n, GLuint* ids) const { glGenFramebuffersEXT(n, ids); };

	void GLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) const { glFramebufferTexture2DEXT(target, attachment, textarget, texture, level); };
	void GLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) const { glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer); };

	// SHADER FUNCTIONS
	void GLValidateProgram(GLuint program) const { glValidateProgram(program); };

	void GLUseProgram(GLuint program) const { glUseProgram(program); }; 

	void GLEnableVertexAttribArray(GLuint index) const { glEnableVertexAttribArray(index); };
	void GLDisableVertexAttribArray(GLuint index) const { glDisableVertexAttribArray(index); };
	void GLVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const { glVertexAttribPointer(index, size, type, normalized, stride, pointer); };

	GLboolean GLIsShader(GLuint shader) const { return glIsShader(shader); };

	GLuint GLCreateShader(GLenum shaderType) const { return glCreateShader(shaderType); };
	void GLShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) const { glShaderSource(shader, count, string, length); };
	void GLCompileShader(GLuint shader) const { glCompileShader(shader); };
	void GLDeleteShader(GLuint shader) const { glDeleteShader(shader); };

	void GLGetShaderiv(GLuint shader, GLenum pname, GLint *params) const { glGetShaderiv(shader, pname, params); };
	void GLGetProgramiv(GLenum target, GLenum pname, GLint *params) const { glGetProgramiv(target,pname,params); };
	void GLGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const { glGetShaderInfoLog(shader, bufSize, length, infoLog); };
	void GLGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const { glGetProgramInfoLog(program, bufSize, length, infoLog); };

	void GLDeleteProgram(GLuint program) const { glDeleteProgram(program); };
	GLuint GLCreateProgram() const { return glCreateProgram(); };
	void GLAttachShader(GLuint program, GLuint shader) const { glAttachShader(program, shader); };
	void GLLinkProgram(GLuint program) const { glLinkProgram(program); };

	void GLBindAttribLocation(GLuint program, GLuint index, const GLchar* name) const { glBindAttribLocation(program, index, name); }

	// SHADER PROGRAM FUNCTIONS
	void GLGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) const { glGetActiveAttrib(program, index, bufSize, length, size, type, name); };
	GLint GLGetAttribLocation(GLuint program, const GLchar* name) const { return glGetAttribLocation(program, name); };

	void GLGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) const { glGetActiveUniform(program, index, bufSize, length, size, type, name); };
	GLint GLGetUniformLocation(GLuint program, const GLchar* name) const { return glGetUniformLocation(program, name); };

	// SHADER PARAMETERS
	void GLUniform1f(GLint location, GLfloat v0) const { glUniform1f(location, v0); };
	void GLUniform2f(GLint location, GLfloat v0, GLfloat v1) const { glUniform2f(location, v0, v1); };
	void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const { glUniform3f(location, v0, v1, v2); };
	void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const { glUniform4f(location, v0, v1, v2, v3); };

	void GLUniform1i(GLint location, GLint  v0) const { glUniform1i(location, v0); };
	void GLUniform2i(GLint location, GLint  v0, GLint  v1) const { glUniform2i(location, v0, v1); };
	void GLUniform3i(GLint location, GLint  v0, GLint  v1, GLint  v2) const { glUniform3i(location, v0, v1, v2); };
	void GLUniform4i(GLint location, GLint  v0, GLint  v1, GLint  v2, GLint  v3) const { glUniform4i(location, v0, v1, v2, v3); };

	void GLUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) const { glUniformMatrix4fv(location, count, transpose, value); };

	// GEOMETRY
	void GLBindBuffer(GLenum target, GLuint buffer) const { glBindBuffer(target, buffer); };
	void* GLMapBuffer(GLenum target, GLenum access) const { return glMapBuffer(target, access); };
	void GLUnmapBuffer(GLenum target) const { glUnmapBuffer(target); };
	void GLBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) const { glBufferData(target, size, data, usage); };
	void GLGenBuffers(GLsizei n, GLuint* buffers) const { glGenBuffers(n, buffers); };
	void GLDeleteBuffers(GLsizei n, GLuint* buffers) const { glDeleteBuffers(n, buffers); };

	// DRAWING
	void GLEnableClientState(GLenum target) const { glEnableClientState(target); };
	void GLDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) const { glDrawElements(mode, count, type, indices); };
	void GLDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *   indices) const { glDrawRangeElements(mode, start, end, count, type, indices); };
	void GLDisableClientState(GLenum array) const { glDisableClientState(array); };

	void GLClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) const { glClearColor(red, green, blue, alpha); };
	void GLClearDepth(GLdouble depth) const { glClearDepth(depth); };
	void GLClear(GLbitfield mask) const { glClear(mask); };


protected:

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

	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

	PFNGLVALIDATEPROGRAMPROC glValidateProgram;

	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;

	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLACTIVETEXTUREPROC glActiveTexture;

	PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
	PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
	PFNGLBINDFRAMEBUFFERPROC glBindFramebufferEXT;
	PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;

	PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
	PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;

	PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;

	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
	PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;

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
};
}

#endif
