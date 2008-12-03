#include "stdafx.h"
#include "RenderMethod_Particles.h"

void RenderMethod_Particles::renderPass(RENDER_PASS pass)
{
	switch(pass)
	{
	case PARTICLE_PASS: pass_particles(); break;
	default: return;
	}
}

void RenderMethod_Particles::pass_particles()
{
	CHECK_GL_ERROR();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glDepthMask(GL_FALSE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Render
	for(vector<GeometryChunk>::const_iterator i = bucket.begin();
		i != bucket.end(); ++i)
	{
		renderParticleBatch(*i);
	}

	glPopClientAttrib();
	glPopAttrib();

	CHECK_GL_ERROR();
}

void RenderMethod_Particles::renderParticleBatch(const GeometryChunk &gc)
{
	gc.material.bind();
	glBlendFunc(GL_SRC_ALPHA, gc.material.glow ? GL_ONE
	                                           : GL_ONE_MINUS_SRC_ALPHA);

	GLsizei count = getPrimitiveCount(gc);

	// Bind vertex arrays
	gc.colorsArray->bind();
	glColorPointer(4, GL_FLOAT, 0, 0);

	gc.texCoordArray->bind();
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	
	gc.vertexArray->bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	// Draw the geometry without an element buffer
	glDrawArrays(gc.primitiveMode, 0, count);
}

GLsizei RenderMethod_Particles::getPrimitiveCount(const GeometryChunk &gc)
{
	GLsizei count=0;

	switch(gc.primitiveMode)
	{
	case GL_TRIANGLES:
		count = gc.vertexArray->getNumber() / 3;
		break;

	case GL_QUADS:
		count = gc.vertexArray->getNumber() / 4;
		break;

	case GL_POINTS: 
		// fall though
	default: 
		count = gc.vertexArray->getNumber();
		break;
	}

	return count;
}