#include "stdafx.h"
#include "Renderer.h"
#include "RenderMethod_TerrainPhong_CG.h"

RenderMethod_TerrainPhong_CG::RenderMethod_TerrainPhong_CG(class Renderer *r)
{
	ASSERT(r, "Null pointer: r");
	renderer = r;
	useCG = true;
}

bool RenderMethod_TerrainPhong_CG::isSupported() const
{
	return areShadersAvailable();
}

void RenderMethod_TerrainPhong_CG::setupShader(CGcontext &cg,
											   CGprofile &_cgVertexProfile,
											   CGprofile &_cgFragmentProfile)
{
	RenderMethod::setupShader(cg, _cgVertexProfile, _cgFragmentProfile);

	createVertexProgram(cg, FileName("Data/shaders/cg/terrain.vp.cg"));
	createFragmentProgram(cg, FileName("Data/shaders/cg/terrain.fp.cg"));

	// Vertex program parameters
	cgMVP             = getVertexProgramParameter  (cg, "MVP");
	cgView            = getVertexProgramParameter  (cg, "View");
	cgCameraPos       = getVertexProgramParameter  (cg, "CameraPos");
	cgLightDir        = getFragmentProgramParameter(cg, "LightDir");
	cgTexture         = getFragmentProgramParameter(cg, "tex0");
	cgKa              = getFragmentProgramParameter(cg, "Ka");
	cgKd              = getFragmentProgramParameter(cg, "Kd");
}

void RenderMethod_TerrainPhong_CG::renderPass(RENDER_PASS pass)
{
	switch(pass)
	{
	case OPAQUE_PASS: pass_opaque(); break;
	default: return;
	}
}

void RenderMethod_TerrainPhong_CG::pass_opaque()
{
	CHECK_GL_ERROR();

	// Setup state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4fv(white);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	// Setup client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Render the geometry chunks
	renderChunks();

	// Restore client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// Restore settings
	glPopAttrib();

	CHECK_GL_ERROR();
}

void RenderMethod_TerrainPhong_CG::setShaderData(const GeometryChunk &gc)
{
	ASSERT(renderer, "Null pointer");
	const Renderer::Light &light0 = renderer->getLight();

	ASSERT(light0.type == Renderer::Light::DIRECTIONAL, "dir lights only pls");

	mat4 MI = (gc.transformation).inverse(); // world -> object projection

	// Set the direction of the light (in object-space)
	vec3 LightDirObj = MI.transformVector(light0.direction.getNormal());
	cgGLSetParameter3fv(cgLightDir, LightDirObj);

	// Set the position of the camera (in object-space)
	vec3 CameraPosWld = renderer->getCamera().getPosition();
	vec3 CameraPosObj = MI.transformVector(CameraPosWld);
	cgGLSetParameter3fv(cgCameraPos, CameraPosObj);

	// Set the material properties
	cgGLSetParameter4fv(cgKa, gc.material.Ka);
	cgGLSetParameter4fv(cgKd, gc.material.Kd);

	// Everything else
	cgGLSetStateMatrixParameter(cgMVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(cgView, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}
