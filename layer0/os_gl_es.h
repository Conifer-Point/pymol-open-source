/*
 * OpenGL ES platform header for PURE_OPENGL_ES_2 builds.
 *
 * Included by os_gl.h instead of <GL/glew.h> + <GL/gl.h>. Never included by
 * desktop builds.
 *
 * PyMOL's ES build renders exclusively through its shader path (VBOs + GLSL
 * with PyMOL's own matrix uniforms). A lot of legacy code still has to
 * *compile*, though: most of the UI and several representations are written as
 * a run-time dual path,
 *
 *     if (orthoCGO) { CGOBegin(...); ... } else { glBegin(...); ... }
 *
 * and the fixed-function side of such code is not always wrapped in
 * `#ifndef PURE_OPENGL_ES_2`. This header supplies what the OpenGL ES 3.0
 * headers do not have so that this code builds:
 *
 *  1. desktop-only enums, with their real values (so they stay
 *     distinguishable in switch statements and lookup tables), and
 *  2. inline no-op functions for desktop-only entry points.
 *
 * Nothing in here emulates fixed-function OpenGL. If one of the no-ops is
 * reached at run time, then either the call is meaningless under a
 * shader-only pipeline (matrix stack, glShadeModel, ...), or the caller took
 * a fixed-function branch that should be unreachable in an ES build. To find
 * out which stubs are reached, build with -D_PYMOL_GLES_TRACE_STUBS: every
 * stub then reports its first call on stderr.
 */

#ifndef _H_os_gl_es
#define _H_os_gl_es

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#ifdef _PYMOL_GLES_TRACE_STUBS
#include <stdio.h>
#define PYMOL_GLES_STUB(name)                                                  \
  do {                                                                         \
    static int reported_;                                                      \
    if (!reported_) {                                                          \
      reported_ = 1;                                                           \
      fprintf(stderr, " os_gl_es.h: no-op stub reached: %s\n", name);          \
    }                                                                          \
  } while (0)
#else
#define PYMOL_GLES_STUB(name) ((void) 0)
#endif

/* ------------------------------------------------------------------------- */
/* Desktop-only enums (values from the Khronos <GL/gl.h> and <GL/glext.h>)   */
/* ------------------------------------------------------------------------- */

/*
 * NOTE: code shared with the desktop build passes some of these to real GL
 * entry points, e.g. glDisable(GL_LIGHTING) or
 * glPixelStorei(GL_PACK_SWAP_BYTES, ...). An ES implementation ignores such a
 * call and raises GL_INVALID_ENUM.
 */

/* primitives without an ES equivalent; only passed to glBegin() (a no-op
 * below) or to CGOBegin(), which converts them before anything is drawn */
#define GL_POLYGON                        0x0009

/* fixed-function state */
#define GL_POINT_SMOOTH                   0x0B10
#define GL_LINE_SMOOTH                    0x0B20
#define GL_POLYGON_SMOOTH                 0x0B41
#define GL_LIGHTING                       0x0B50
#define GL_COLOR_MATERIAL                 0x0B57
#define GL_FOG                            0x0B60
#define GL_NORMALIZE                      0x0BA1
#define GL_ALPHA_TEST                     0x0BC0
#define GL_FLAT                           0x1D00
#define GL_POINT_SPRITE                   0x8861
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642

/* matrix stack */
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701

/* texture environment */
#define GL_TEXTURE_ENV_MODE               0x2200
#define GL_TEXTURE_ENV                    0x2300

/* stereo / auxiliary color buffers of the default framebuffer */
#define GL_FRONT_LEFT                     0x0400
#define GL_FRONT_RIGHT                    0x0401
#define GL_BACK_LEFT                      0x0402
#define GL_BACK_RIGHT                     0x0403

/* pixel store modes which do not exist in ES */
#define GL_UNPACK_SWAP_BYTES              0x0CF0
#define GL_UNPACK_LSB_FIRST               0x0CF1
#define GL_PACK_SWAP_BYTES                0x0D00
#define GL_PACK_LSB_FIRST                 0x0D01

/* tessellation (the bezier shader is never loaded in an ES build) */
#define GL_PATCH_VERTICES                 0x8E72

/* ES has no GL_CLAMP wrap mode; GL_CLAMP_TO_EDGE is the closest match and what
 * the ES texture lookup table in GenericBuffer.cpp already uses for the
 * other clamping modes */
#define GL_CLAMP                          GL_CLAMP_TO_EDGE

/* ------------------------------------------------------------------------- */
/* Desktop-only functions                                                    */
/* ------------------------------------------------------------------------- */

/*
 * Immediate mode. Almost all call sites are the fixed-function side of a
 * run-time dual path (`if (orthoCGO) {...} else {<here>}`, `if (use_shaders)
 * {...} else {<here>}`). An ES build always renders with shaders, so these
 * are dead branches. The remaining callers only run in modes which an ES
 * build cannot support anyway and then draw nothing: Rep*RenderImmediate()
 * (defer_builds_mode=5), the OrthoBusyDraw() progress bar, and
 * ObjectVolume's slice polygons (volumes are disabled for ES).
 */
inline void glBegin(GLenum) { PYMOL_GLES_STUB("glBegin"); }
inline void glEnd() { PYMOL_GLES_STUB("glEnd"); }
inline void glVertex2i(GLint, GLint) { PYMOL_GLES_STUB("glVertex2i"); }
inline void glVertex2f(GLfloat, GLfloat) { PYMOL_GLES_STUB("glVertex2f"); }
inline void glVertex3i(GLint, GLint, GLint) { PYMOL_GLES_STUB("glVertex3i"); }
inline void glVertex3fv(const GLfloat*) { PYMOL_GLES_STUB("glVertex3fv"); }
inline void glNormal3fv(const GLfloat*) { PYMOL_GLES_STUB("glNormal3fv"); }
inline void glColor3f(GLfloat, GLfloat, GLfloat) { PYMOL_GLES_STUB("glColor3f"); }
inline void glColor3fv(const GLfloat*) { PYMOL_GLES_STUB("glColor3fv"); }
inline void glColor4f(GLfloat, GLfloat, GLfloat, GLfloat) { PYMOL_GLES_STUB("glColor4f"); }
inline void glColor4fv(const GLfloat*) { PYMOL_GLES_STUB("glColor4fv"); }
inline void glTexCoord3fv(const GLfloat*) { PYMOL_GLES_STUB("glTexCoord3fv"); }

/*
 * Raster position (bitmap font text; only used if labels are not textured,
 * i.e. without shaders) and pixel transfer to the framebuffer.
 *
 * NOTE: glDrawPixels() is how PyMOLDrawPixels() copies an image into the
 * viewport on desktop GL. The image overlay (e.g. the result of "ray") does
 * not go through it in an ES build: Scene.cpp draws the image as a textured
 * quad instead (SceneDrawPixelsES). Only ShaderMgr::drawPixelsTo, which has
 * no callers, still reaches this stub.
 */
inline void glRasterPos4fv(const GLfloat*) { PYMOL_GLES_STUB("glRasterPos4fv"); }
inline void glDrawPixels(GLsizei, GLsizei, GLenum, GLenum, const GLvoid*)
{
  PYMOL_GLES_STUB("glDrawPixels");
}

/*
 * Matrix stack. These ARE reached in an ES build (OrthoPushMatrix,
 * OrthoPopMatrix, ExecutiveDrawNow), but have no effect on rendering there: all
 * shaders take their matrices from PyMOL's own uniforms (g_ModelViewMatrix,
 * g_ProjectionMatrix, ...), which PyMOL tracks itself and not in GL state.
 */
inline void glMatrixMode(GLenum) { PYMOL_GLES_STUB("glMatrixMode"); }
inline void glPushMatrix() { PYMOL_GLES_STUB("glPushMatrix"); }
inline void glPopMatrix() { PYMOL_GLES_STUB("glPopMatrix"); }
inline void glLoadIdentity() { PYMOL_GLES_STUB("glLoadIdentity"); }
inline void glTranslatef(GLfloat, GLfloat, GLfloat) { PYMOL_GLES_STUB("glTranslatef"); }
inline void glOrtho(double, double, double, double, double, double)
{
  PYMOL_GLES_STUB("glOrtho");
}

/*
 * Fixed-function state without a shader-pipeline equivalent. These ARE
 * reached in an ES build. Point size is set with gl_PointSize in the vertex
 * shader (uniform g_PointSize); there is no flat shading or texture
 * environment.
 */
inline void glPointSize(GLfloat) { PYMOL_GLES_STUB("glPointSize"); }
inline void glShadeModel(GLenum) { PYMOL_GLES_STUB("glShadeModel"); }
inline void glTexEnvf(GLenum, GLenum, GLfloat) { PYMOL_GLES_STUB("glTexEnvf"); }

/*
 * Selecting a color buffer of the default framebuffer (stereo, front buffer
 * drawing). ES only has glDrawBuffers(), and the default framebuffer has
 * exactly one color buffer (GL_BACK). Only called from
 * CShaderMgr::drawPixelsTo(), which has no callers.
 */
inline void glDrawBuffer(GLenum) { PYMOL_GLES_STUB("glDrawBuffer"); }

/*
 * Per-draw-buffer color mask (desktop OpenGL 3.0; not in ES 3.0). Only called
 * if GLEW_VERSION_3_0, which is false in an ES build (gl_def.h).
 */
inline void glColorMaski(GLuint, GLboolean, GLboolean, GLboolean, GLboolean)
{
  PYMOL_GLES_STUB("glColorMaski");
}

/*
 * Tessellation. Only called after enabling the "bezier" program, which is
 * only created if GLEW_ARB_tessellation_shader (false in an ES build).
 */
inline void glPatchParameteri(GLenum, GLint) { PYMOL_GLES_STUB("glPatchParameteri"); }

#endif
