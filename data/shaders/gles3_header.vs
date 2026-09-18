#version 300 es
/*
 * GLSL ES 3.00 header for vertex shaders (OpenGL ES 3.0 / WebGL 2).
 *
 * Prepended by CShaderPrg::reload() in _PYMOL_GLES3 builds. The shader
 * sources are written in GLSL ES 1.00 syntax; these macros map them to
 * GLSL ES 3.00, where the old keywords and functions no longer exist.
 * The C++ side sets the preprocessor variable PYMOL_GLES3 for the same
 * builds, so that shaders can leave out GLSL ES 1.00-only directives.
 */
#define attribute in
#define varying out
#define texture2D texture
#define textureCube texture
