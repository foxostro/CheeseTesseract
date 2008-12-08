#include "stdafx.h"
#include "SkyBox.h"

SkyBox::SkyBox(TextureFactory &textureFactory,
               FileName _top,
               FileName _bottom,
               FileName _east,
               FileName _west,
               FileName _north,
               FileName _south) {
	// Texture clamp on these ones
	top.setTexture(textureFactory.load(_top, false));
	bottom.setTexture(textureFactory.load(_bottom, false));
	east.setTexture(textureFactory.load(_east, false));
	west.setTexture(textureFactory.load(_west, false));
	north.setTexture(textureFactory.load(_north, false));
	south.setTexture(textureFactory.load(_south, false));
}

void SkyBox::draw(const vec3 &cameraPosition) const {
	CHECK_GL_ERROR();
	
	// TODO: Rewrite the sky box code to work with the new render pipeline
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(0);
	glDisable(GL_CULL_FACE);
	
	static const float r = 1.01f;
	static const vec3 verts[8] = {
		vec3(-r,  r,  r),
		vec3( r,  r,  r),
		vec3(-r, -r,  r),
		vec3( r, -r,  r),
		vec3(-r,  r, -r),
		vec3( r,  r, -r),
		vec3(-r, -r, -r),
		vec3( r, -r, -r)
	};
	enum { A=0, B, C, D, E, F, G, H };
	
	// top
	top.bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3fv(verts[A]);
	glTexCoord2f(1,0);
	glVertex3fv(verts[B]);
	glTexCoord2f(1,1);
	glVertex3fv(verts[D]);
	glTexCoord2f(0,1);
	glVertex3fv(verts[C]);
	glEnd();
	
	// bottom
	bottom.bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3fv(verts[E]);
	glTexCoord2f(1,1);
	glVertex3fv(verts[F]);
	glTexCoord2f(1,0);
	glVertex3fv(verts[H]);
	glTexCoord2f(0,0);
	glVertex3fv(verts[G]);
	glEnd();
	
	// east
	east.bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3fv(verts[B]);
	glTexCoord2f(1,1);
	glVertex3fv(verts[D]);
	glTexCoord2f(1,0);
	glVertex3fv(verts[H]);
	glTexCoord2f(0,0);
	glVertex3fv(verts[F]);
	glEnd();
	
	// west
	west.bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3fv(verts[C]);
	glTexCoord2f(1,1);
	glVertex3fv(verts[A]);
	glTexCoord2f(1,0);
	glVertex3fv(verts[E]);
	glTexCoord2f(0,0);
	glVertex3fv(verts[G]);
	glEnd();
	
	// north
	north.bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3fv(verts[A]);
	glTexCoord2f(1,1);
	glVertex3fv(verts[B]);
	glTexCoord2f(1,0);
	glVertex3fv(verts[F]);
	glTexCoord2f(0,0);
	glVertex3fv(verts[E]);
	glEnd();
	
	// south
	south.bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3fv(verts[D]);
	glTexCoord2f(1,1);
	glVertex3fv(verts[C]);
	glTexCoord2f(1,0);
	glVertex3fv(verts[G]);
	glTexCoord2f(0,0);
	glVertex3fv(verts[H]);
	glEnd();
	
	glPopAttrib();
	
	glPopMatrix();
	
	CHECK_GL_ERROR();
}
