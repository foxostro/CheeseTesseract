#ifndef SKYBOX_H
#define SKYBOX_H

#include "FileName.h"
#include "Material.h"

class TextureFactory;

class SkyBox
{
public:
	SkyBox(TextureFactory &textureFactory,
		   FileName top,
		   FileName bottom,
		   FileName east,
		   FileName west,
		   FileName north,
		   FileName south);

	void draw(const vec3 &cameraPosition) const;

private:
	Material top;
	Material bottom;
	Material east;
	Material west;
	Material north;
	Material south;
};

#endif
