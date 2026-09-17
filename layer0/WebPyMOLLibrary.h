/*
 * Open-source stand-in for the WebPyMOL support library header.
 *
 * `_WEBGL` builds include "WebPyMOLLibrary.h" (ShaderMgr.cpp, ShaderPrg.cpp,
 * RepDistDash.cpp), but the header which belongs to Schrodinger's WebPyMOL
 * product is not part of the open-source tree. This file provides the symbols
 * which the open-source code expects from it, without any JavaScript glue, so
 * that `_WEBGL` + `_PYMOL_WEBGL_OPEN` builds compile. It is never included by
 * non-`_WEBGL` builds.
 *
 * The complete list of such symbols is:
 *
 *   webpymol_get_fragdepth_support()   (via GET_FRAGDEPTH_SUPPORT, ShaderMgr.h)
 */

#pragma once

/**
 * Can fragment shaders write gl_FragDepth? If not, PyMOL does not load the
 * sphere and cylinder impostor shaders and falls back to triangle geometry.
 *
 * WebPyMOL presumably asks the WebGL 1 context for the EXT_frag_depth
 * extension. gl_FragDepth is a core feature of OpenGL ES 3.0 / WebGL 2
 * (GLSL ES 3.00), which is what the open-source ES build targets, so this is
 * a constant. Build with -DWEBPYMOL_FRAGDEPTH_SUPPORT=0 to disable the
 * impostor shaders.
 */
#ifndef WEBPYMOL_FRAGDEPTH_SUPPORT
#define WEBPYMOL_FRAGDEPTH_SUPPORT 1
#endif

inline int webpymol_get_fragdepth_support()
{
  return WEBPYMOL_FRAGDEPTH_SUPPORT;
}
