#ifndef _H_os_gl
#define _H_os_gl

#include"os_predef.h"
#include"os_proprietary.h"

/*
 * Build macros which select the OpenGL flavor:
 *
 * PURE_OPENGL_ES_2   OpenGL ES: shader-only rendering, no GLEW (os_gl_es.h)
 * _WEBGL             with PURE_OPENGL_ES_2: WebGL restrictions (no client-side
 *                    vertex arrays, no glLineWidth, no threads, ...)
 * _PYMOL_WEBGL_OPEN  with _WEBGL: build without the parts of the WebPyMOL
 *                    product which are not in the open-source tree (its
 *                    Feedback macros, its Python-less iterate/alter, and the
 *                    embind lookup of Module.ONEBUFFER)
 */

// hardcode either true, or (x)
#define ALWAYS_IMMEDIATE_OR(x) true

#if 1
  #define _PYMOL_NO_AA_SHADERS
#endif

#if !defined(GL_GLEXT_PROTOTYPES) && !defined(_WIN32)
#define GL_GLEXT_PROTOTYPES
#endif

#ifndef GLEW_NO_GLU
#define GLEW_NO_GLU
#endif

#ifndef PURE_OPENGL_ES_2
#include <GL/glew.h>
#endif

#ifdef PURE_OPENGL_ES_2
#include "os_gl_es.h"
#elif defined(_PYMOL_OSX)
#import <OpenGL/gl.h>
#import <OpenGL/glext.h>
#else
#include <GL/gl.h>
#endif

#include "os_gl_glut.h"
#include "Spatial.h"

struct GLFramebufferConfig {
  std::uint32_t framebuffer{};
  GLenum drawBuffer{};
};

void PyMOLReadPixels(GLint x,
                     GLint y,
                     GLsizei width,
                     GLsizei height, GLenum format, GLenum type, GLvoid * pixels);

void PyMOLDrawPixels(GLsizei width,
                     GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);

int PyMOLCheckOpenGLErr(const char *pos);

#define VertexIndex_t std::uint32_t
#define VertexIndex_GL_ENUM GL_UNSIGNED_INT
#define SceneGLClearColor(red,green,blue,alpha) glClearColor(red,green,blue,alpha);

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#define hasFrameBufferBinding() false

#ifndef PURE_OPENGL_ES_2
#define GL_DEBUG_PUSH(title) \
  GLEW_KHR_debug ? glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, title) : (void)0

#define GL_DEBUG_POP() \
  GLEW_KHR_debug ? glPopDebugGroup() : (void)0

#ifdef __cplusplus

class glDebugBlock {
public:
  explicit glDebugBlock(char const* title) {
    GL_DEBUG_PUSH(title);
  }
  ~glDebugBlock() {
    GL_DEBUG_POP();
  }
};

#define GL_DEBUG_FUN() \
  glDebugBlock glDebugBlockVariable(__FUNCTION__)

#endif /* __cplusplus */
#else
#define GL_DEBUG_FUN()
#endif

#endif
