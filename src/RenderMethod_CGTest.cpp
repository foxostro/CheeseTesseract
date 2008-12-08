#include "stdafx.h"
#include "Renderer.h"
#include "RenderMethod_CGTest.h"

RenderMethod_CGTest::RenderMethod_CGTest()
{
	useCG = true;
}

void RenderMethod_CGTest::setupShader(CGcontext &cg,
									  CGprofile &_cgVertexProfile,
									  CGprofile &_cgFragmentProfile)
{
	RenderMethod::setupShader(cg, _cgVertexProfile, _cgFragmentProfile);

	createVertexProgram(cg, FileName("data/shaders/cg/test.vp.cg"));
	createFragmentProgram(cg, FileName("data/shaders/cg/test.fp.cg"));
	
	// get handles shader program parameters
	cgProj    = getVertexProgramParameter  (cg, "Proj");
	cgView    = getVertexProgramParameter  (cg, "View");
	cgTexture = getFragmentProgramParameter(cg, "tex0");

	// setup shader constants
	cgGLSetStateMatrixParameter(cgProj, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgSetParameterVariability(cgProj, CG_LITERAL); // make it a constant
}

bool RenderMethod_CGTest::isSupported() const
{
	return RenderMethod::areShadersAvailable();
}

void RenderMethod_CGTest::renderPass(RENDER_PASS pass)
{
	switch(pass)
	{
	case OPAQUE_PASS: pass_opaque(); break;
	default: return;
	}
}

void RenderMethod_CGTest::pass_opaque()
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

	//Restore client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// Restore settings
	glPopAttrib();

	CHECK_GL_ERROR();
}

void RenderMethod_CGTest::setShaderData(const GeometryChunk &)
{
	// send model-view matrix to the vertex program
	cgGLSetStateMatrixParameter(cgView, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}