#include "stdafx.h"
#include "RenderMethod_GrassFFP.h"

void RenderMethod_GrassFFP::renderPass(RENDER_PASS pass)
{
	switch(pass)
	{
	case OPAQUE_PASS: pass_opaque(); break;
	default: return;
	}
}

void RenderMethod_GrassFFP::pass_opaque()
{
	CHECK_GL_ERROR();

	// Setup state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4fv(white);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	// Setup client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Alpha-test pass
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	renderChunks();

	//Restore client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// Restore settings
	glPopAttrib();

	CHECK_GL_ERROR();
}
