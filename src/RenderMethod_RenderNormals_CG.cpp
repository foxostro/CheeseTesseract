#include "stdafx.h"
#include "FileText.h"
#include "RenderMethod_RenderNormals_CG.h"

RenderMethod_RenderNormals_CG::RenderMethod_RenderNormals_CG()
{
	useCG = true;
}

bool RenderMethod_RenderNormals_CG::isSupported() const
{
	return areShadersAvailable();
}

void RenderMethod_RenderNormals_CG::setupShader(CGcontext &cg,
												CGprofile &_cgVertexProfile,
												CGprofile &_cgFragmentProfile)
{
	RenderMethod::setupShader(cg, _cgVertexProfile, _cgFragmentProfile);

	createVertexProgram(cg, FileName("Data/shaders/cg/render_normals.vp.cg"));
	createFragmentProgram(cg, FileName("Data/shaders/cg/render_normals.fp.cg"));

	cgMVP    = getVertexProgramParameter(cg, "MVP");
	cgViewIT = getVertexProgramParameter(cg, "ViewIT");
}

void RenderMethod_RenderNormals_CG::renderPass(RENDER_PASS pass)
{
	switch(pass)
	{
	case OPAQUE_PASS: pass_opaque(); break;
	default: return;
	}
}

void RenderMethod_RenderNormals_CG::pass_opaque()
{
	CHECK_GL_ERROR();

	// Setup state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);

	// Setup client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Render the geometry chunks
	renderChunks();

	//Restore client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// Restore settings
	glPopAttrib();

	CHECK_GL_ERROR();
}

void RenderMethod_RenderNormals_CG::setShaderData( const GeometryChunk &gc )
{
	cgGLSetStateMatrixParameter(cgMVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	mat4 View, ViewIT, cgReturnsViewIT;

	glGetFloatv(GL_MODELVIEW_MATRIX, View);
	ViewIT = View.inverse().transpose();
	
#if 0
	cgGLSetStateMatrixParameter(cgViewIT, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);
	cgGLGetMatrixParameterfc(cgViewIT, cgReturnsViewIT);
	ASSERT(cgReturnsViewIT == ViewIT, "Cg returns an incorrect matrix");
#else
	cgGLSetMatrixParameterfc(cgViewIT, ViewIT);
#endif
}
