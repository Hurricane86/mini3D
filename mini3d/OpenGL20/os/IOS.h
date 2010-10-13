
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_IOS_H
#define MINI3D_IOS_H

#ifndef GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
#endif

#include <windows.h>
#include <GL/gl.h>

typedef char GLchar;

//#include <GL/wglext.h>
//#include <GL/glext.h>

namespace mini3d
{
class IOS
{
public:
	virtual ~IOS() { };

	virtual void Init() = 0;

	// Used by OPENGL for determining setting buffer and depth buffer format
	virtual unsigned int GetMonitorBitDepth() const = 0;
	virtual void GetClientAreaSize(int windowHandle, unsigned int &width, unsigned int &height) const = 0;
	virtual void Log(wchar_t* message) const = 0;



	// ---------- OPEN GL FUNCTIONS ----------------------------------------------

	// GENERAL
	virtual void GLSwapBuffers() const = 0;
	virtual void GLBindFramebuffer(GLenum target, GLuint framebuffer) const = 0;
	//virtual void GLMakeCurrent(const DisplayContext displayContext, const WindowContext windowContext, const GLRenderingContext renderingContext) const = 0;
	virtual void GLViewport(const unsigned int width, const unsigned int height) const = 0;

	virtual void GLShadeModel(GLenum mode) const = 0;
	virtual void GLEnable(GLenum cap) const = 0;	// Enables Depth Testing
	virtual void GLDepthFunc(GLenum func) const = 0; // The Type Of Depth Test To Do

	//CAPS
	virtual void GLGetIntegerv(GLenum pname, GLint *params) const = 0;

	// TEXTURE
	virtual void GLActiveTexture(GLenum texture) const = 0;
	virtual void GLBindTexture(GLenum target, GLuint texture) const = 0;
	virtual void GLTexParameteri(GLenum target, GLenum pname, GLint params) const = 0;	
	
	// SHADER FUNCTIONS
	virtual void GLUseProgram(GLuint program) const = 0;
	
	virtual void GLEnableVertexAttribArray(GLuint index) const = 0;
	virtual void GLDisableVertexAttribArray(GLuint index) const = 0;
	virtual void GLVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const = 0;

	virtual GLboolean GLIsShader(GLuint shader) const = 0;
	virtual void GLGetShaderiv(GLuint shader, GLenum pname, GLint *params) const = 0;
	virtual void GLGetProgramiv(GLenum target, GLenum pname, GLint *params) const = 0;
	virtual void GLGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const = 0;
	virtual void GLGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) const = 0;

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