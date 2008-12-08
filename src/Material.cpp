#include "stdafx.h"
#include "TextureFactory.h"
#include "Material.h"

Material::Material() {
	clear();
}

Material::Material(const Material &mat) {
	copy(mat);
}

Material::Material(TextureFactory::Handle *texture) {
	clear();
	setTexture(texture);
}

void Material::copy(const Material &mat) {
	Ka = mat.Ka;
	Kd = mat.Kd;
	Ks = mat.Ks;
	shininess = mat.shininess;
	glow = mat.glow;
	texture = mat.texture;
}

void Material::clear() {
	Ka = color(0.1f, 0.1f, 0.1f, 1.0f);
	Kd = color(0.8f, 0.8f, 0.8f, 1.0f);
	Ks = color(1.0f, 1.0f, 1.0f, 1.0f);
	shininess = 64.0f;
	glow = false;
	texture = 0;
}

void Material::setTexture(TextureFactory::Handle *handle) {
	ASSERT(handle!=0, "handle was null");
	texture = handle;
}

void Material::bind() const {
	CHECK_GL_ERROR();
	
	if (texture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		setTextureFilters();
		glEnable(GL_TEXTURE_2D);
	}
	
	glMaterialfv(GL_FRONT, GL_AMBIENT,   Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION,  color(0.0f, 0.0f, 0.0f, 1.0f));
	glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	CHECK_GL_ERROR();
}

void Material::setTextureFilters() {
	// trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}
