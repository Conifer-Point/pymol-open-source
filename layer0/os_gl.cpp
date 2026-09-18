#include"os_predef.h"
#include"os_gl.h"

#include<stdio.h>

/* GL_PACK_SWAP_BYTES / GL_PACK_LSB_FIRST (and the UNPACK ones) do not exist in
 * OpenGL ES: glGetIntegerv leaves the output untouched and glPixelStorei
 * raises GL_INVALID_ENUM. Byte order is not selectable there anyway. */

void PyMOLReadPixels(GLint x,
                     GLint y,
                     GLsizei width,
                     GLsizei height, GLenum format, GLenum type, GLvoid * pixels)
{

  /* special "safe" version of glReadPixels for buggy OpenGL implementations */

  GLint rowlength;
  GLint skiprows, skippixels, alignment;
#ifndef PURE_OPENGL_ES_2
  GLint swapbytes, lsbfirst;
#endif

  /* Save current pixel store state. */
#ifndef PURE_OPENGL_ES_2
  glGetIntegerv(GL_PACK_SWAP_BYTES, &swapbytes);
  glGetIntegerv(GL_PACK_LSB_FIRST, &lsbfirst);
#endif
  glGetIntegerv(GL_PACK_ROW_LENGTH, &rowlength);
  glGetIntegerv(GL_PACK_SKIP_ROWS, &skiprows);
  glGetIntegerv(GL_PACK_SKIP_PIXELS, &skippixels);
  glGetIntegerv(GL_PACK_ALIGNMENT, &alignment);

  /* Set desired pixel store state. */
#ifndef PURE_OPENGL_ES_2
  glPixelStorei(GL_PACK_SWAP_BYTES, GL_FALSE);
  glPixelStorei(GL_PACK_LSB_FIRST, GL_FALSE);
#endif
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  /* call glFlush & glFinish first to avoid full system crash on buggy
   * ATI Radeon drivers, such as Radeon 7000 & 9000 series.  (calling
   * both is probably redundant and ultra-paranoid, but so what?) */
  glFlush();
  glFinish();

  /* now get the pixels */
  glReadPixels(x, y, width, height, format, type, pixels);

  /* now flush once again just to be extra sure that we don't encounter
   * the dreaded ATI driver bug system freeze-up */
  glFlush();
  glFinish();

  /* and then estore current pixel store state. */
#ifndef PURE_OPENGL_ES_2
  glPixelStorei(GL_PACK_SWAP_BYTES, swapbytes);
  glPixelStorei(GL_PACK_LSB_FIRST, lsbfirst);
#endif
  glPixelStorei(GL_PACK_ROW_LENGTH, rowlength);
  glPixelStorei(GL_PACK_SKIP_ROWS, skiprows);
  glPixelStorei(GL_PACK_SKIP_PIXELS, skippixels);
  glPixelStorei(GL_PACK_ALIGNMENT, alignment);

}

void PyMOLDrawPixels(GLsizei width,
                     GLsizei height, GLenum format, GLenum type, const GLvoid * pixels)
{

  /* special "safe" version of glDrawPixels for buggy OpenGL implementations */

  GLint rowlength;
  GLint skiprows, skippixels, alignment;
#ifndef PURE_OPENGL_ES_2
  GLint swapbytes, lsbfirst;
#endif

  /* Save current pixel store state. */
#ifndef PURE_OPENGL_ES_2
  glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
  glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
#endif
  glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
  glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
  glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

  /* Set desired pixel store state. */
#ifndef PURE_OPENGL_ES_2
  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
  glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
#endif
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glDrawPixels(width, height, format, type, pixels);

  /* Restore current pixel store state. */
#ifndef PURE_OPENGL_ES_2
  glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
  glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
#endif
  glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
  glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

}

int PyMOLCheckOpenGLErr(const char *pos)
{
  int flag = 0;
  GLenum glerr = glGetError();
  while(glerr != GL_NO_ERROR) {
    printf("OpenGL-Error: Where? %s: glerr=%d\n", pos, glerr);
    glerr = glGetError();
    flag = 1;
  }
  return flag;
}
