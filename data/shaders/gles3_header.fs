#version 300 es
/*
 * GLSL ES 3.00 header for fragment shaders (OpenGL ES 3.0 / WebGL 2).
 *
 * Prepended by CShaderPrg::reload() in _PYMOL_GLES3 builds. The shader
 * sources are written in GLSL ES 1.00 syntax; these macros map them to
 * GLSL ES 3.00: `varying` becomes `in`, the built-in texture functions
 * are all called `texture`, gl_FragDepth is core (GL_EXT_frag_depth is
 * not offered as an extension any more), and gl_FragColor/gl_FragData
 * are replaced by a declared output array. With one draw buffer only
 * element 0 exists; the multi-render-target OIT path (no ONE_DRAW_BUFFER,
 * GL_EXT_draw_buffers on GLSL ES 1.00) writes gl_FragData[1] as well.
 * The C++ side sets the preprocessor variable PYMOL_GLES3 for the same
 * builds, so that shaders can leave out GLSL ES 1.00-only directives.
 */
#define varying in
#define texture2D texture
#define textureCube texture
#define gl_FragDepthEXT gl_FragDepth
precision highp float;
#ifdef ONE_DRAW_BUFFER
layout(location = 0) out highp vec4 pymol_FragData[1];
#else
layout(location = 0) out highp vec4 pymol_FragData[2];
#endif
#define gl_FragData pymol_FragData
#define gl_FragColor gl_FragData[0]
