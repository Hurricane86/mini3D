
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

	// ---------- ABSTRACT OPENGL FUNCTIONS --------------------------------------
	// These functions preform opengl operations but they do not necessarily map 1-1 against any opengl functions.
	// what they do depends on the platform opengl implementation.
	
	virtual void PrepareWindow(const MINI3D_WINDOW hWindow) const = 0;
	virtual void SetRenderWindow(const MINI3D_WINDOW hWindow) const = 0;
	virtual void SetDefaultRenderWindow() const = 0;
	virtual void SwapWindowBuffers(const MINI3D_WINDOW hWindow) const = 0;

	// ---------- OPEN GL FUNCTIONS ----------------------------------------------

	// GENERAL
	virtual void GLSwapBuffers() const = 0;
	virtual void GLBindFramebuffer(GLenum target, GLuint framebuffer) const = 0;
	virtual void GLViewport(const unsigned int width, const unsigned int height) const = 0;

	virtual void GLShadeModel(GLenum mode) const = 0;
	virtual void GLEnable(GLenum cap) const = 0;
	virtual void GLDepthFunc(GLenum func) const = 0;

	//CAPS
	virtual void GLGetIntegerv(GLenum pname, GLint *params) const = 0;

	// TEXTURE
	virtual void GLActiveTexture(GLenum texture) const = 0;
	virtual void GLBindTexture(GLenum target, GLuint texture) const = 0;
	virtual void GLTexParameteri(GLenum target, GLenum pname, GLint params) const = 0;	

	virtual void GLGenRenderbuffers(GLsizei n, GLuint* renderbuffers) const = 0;
	virtual void GLBindRenderbuffer(GLenum target, GLuint renderbuffer) const = 0;
	virtual void GLRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) const = 0;

	virtual void GLDeleteRenderbuffers(GLsizei n, GLuint* renderbuffers) const = 0;
	virtual void GLDeleteFramebuffers(GLsizei n, GLuint* framebuffers) const= 0;

	virtual void GLGenFramebuffers(GLsizei n, GLuint* ids) const = 0;

	virtual void GLFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) const = 0;
	virtual void GLFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) const = 0;

	// SHADER FUNCTIONS
	virtual void GLUseProgram(GLuint program) const = 0; 
	
	virtual void GLEnableVertexAttribArray(GLuint index) const = 0;
	virtual void GLDisableVertexAttribArray(GLuint index) const = 0;
	virtual void GLVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const = 0;

	virtual GLboolean GLIsShader(GLuint shader) const = 0;

	virtual GLuint GLCreateShader(GLenum shaderType) const = 0;
	virtual void GLShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) const = 0;
	virtual void GLCompileShader(GLuint shader) const = 0;
	virtual void GLDeleteShader(GLuint shader) const = 0;

	virtual void GLGetShaderiv(GLuint shader, GLenum pname, GLint *params) const = 0;
	virtual void GLGetProgramiv(GLenum target, GLenum pname, GLint *params) const = 0;
	virtual void GLGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const = 0;
	virtual void GLGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const = 0;

	virtual void GLDeleteProgram(GLuint program) const = 0;
	virtual GLuint GLCreateProgram() const = 0;
	virtual void GLAttachShader(GLuint program, GLuint shader) const = 0;
	virtual void GLLinkProgram(GLuint program) const = 0;

	// SHADER PROGRAM FUNCTIONS
	virtual void GLGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) const = 0;
	virtual GLint GLGetAttribLocation(GLuint program, const GLchar* name) const = 0;

	virtual void GLGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) const = 0;
	virtual GLint GLGetUniformLocation(GLuint program, const GLchar* name) const = 0;

	// SHADER PARAMETERS
	virtual void GLUniform1f(GLint location, GLfloat v0) const = 0;
	virtual void GLUniform2f(GLint location, GLfloat v0, GLfloat v1) const = 0;
	virtual void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const = 0;
	virtual void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const = 0;

	virtual void GLUniform1i(GLint location, GLint  v0) const = 0;
	virtual void GLUniform2i(GLint location, GLint  v0, GLint  v1) const = 0;
	virtual void GLUniform3i(GLint location, GLint  v0, GLint  v1, GLint  v2) const = 0;
	virtual void GLUniform4i(GLint location, GLint  v0, GLint  v1, GLint  v2, GLint  v3) const = 0;

	virtual void GLUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) const = 0;
	
	// GEOMETRY
	virtual void GLBindBuffer(GLenum target, GLuint buffer) const = 0;
	virtual void* GLMapBuffer(GLenum target, GLenum access) const = 0;
	virtual void GLUnmapBuffer(GLenum target) const = 0;
	virtual void GLBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) const = 0;
	virtual void GLGenBuffers(GLsizei n, GLuint* buffers) const = 0;
	virtual void GLDeleteBuffers(GLsizei n, GLuint* buffers) const = 0;

	// DRAWING
	virtual void GLEnableClientState(GLenum target) const = 0;
	virtual void GLDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) const = 0;
    virtual void GLDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *   indices) const = 0;
	virtual void GLDisableClientState(GLenum array) const = 0;

	virtual void GLClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) const = 0;
	virtual void GLClearDepth(GLdouble depth) const = 0;
	virtual void GLClear(GLbitfield mask) const = 0;


};
}

#endif
