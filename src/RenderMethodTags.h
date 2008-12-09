#if defined(_MSC_VER)
#pragma once
#endif

#include "RenderInstance.h"

/*
Each of these tokens maps to a specific render method that should be applied.
If the method is not supported due on the current machine, then an error
is raised.
*/

static const RENDER_METHOD METHOD_UNSPECIFIED         =-1; // Do not use a specific method, use tags instead
static const RENDER_METHOD METHOD_DIFFUSE_TEX_FFP     = 0; // Diffuse texture with standard OpenGL lighting
static const RENDER_METHOD METHOD_CG_PHONG_DIRECTIONAL= 1; // Diffuse texture with per-pixel directional lighting (Cg shader)
static const RENDER_METHOD METHOD_CG_PHONG_POINT      = 2; // Diffuse texture with per-pixel point lighting (Cg shader)
static const RENDER_METHOD METHOD_PARTICLE            = 3; // Unlit particle billboard with texture
static const RENDER_METHOD METHOD_GRASS_FFP           = 4; // Fixed-function grass billboards
static const RENDER_METHOD METHOD_GRASS_CG            = 5; // Grass billboards on the GPU
static const RENDER_METHOD METHOD_CG_TEST             = 6; // CG test shader
static const RENDER_METHOD METHOD_CG_RENDER_NORMALS   = 7; // Displays normals in the color buffer

/*
Each of these tokens serves as a tag for general category of render methods
that can be applied to the object.  Chooses the exact method based on
hardware capabilities of the current machine.
*/

static const RENDER_METHOD_TAG TAG_UNSPECIFIED =-1; // Do not use tagged categories, use a specific method instead
static const RENDER_METHOD_TAG TAG_DIFFUSE_TEX = 0; // Diffuse texture with directional lighting
static const RENDER_METHOD_TAG TAG_GRASS       = 1; // Grass cover layer
