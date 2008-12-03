#include "stdafx.h"
#include "color.h"
#include "Dimmer.h"

float Dimmer::alphaBlur = 1.0f;

Dimmer::Dimmer()
{}

void Dimmer::draw() const
{
	if(alphaBlur < FLT_EPSILON)
	{
		alphaBlur = 0.0f;
		return;
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor4f(0, 0, 0, alphaBlur);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*glBlendFunc(GL_SRC_ALPHA, GL_ONE);*/
	
	// Save the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Set up the projection matrix for 2D
	glLoadIdentity();
	glOrtho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f);

	// Save the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set up the model view matrix
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -0.2f);

	// Render a quad over the screen
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1024.0f,   0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1024.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,   0.0f, 0.0f);
	glEnd();
	
	// Restore the model view matrix
	glPopMatrix();

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Use modelview mode
	glMatrixMode(GL_MODELVIEW);
	
	// Restore settings
	glPopAttrib();
}
