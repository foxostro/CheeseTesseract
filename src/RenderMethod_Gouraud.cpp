#include "stdafx.h"
#include "RenderMethod_Gouraud.h"

void RenderMethod_Gouraud::renderPass(RENDER_PASS pass)
{
	switch(pass)
	{
	case OPAQUE_PASS: pass_opaque(); break;
	default: return;
	}
}

void RenderMethod_Gouraud::pass_opaque()
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
