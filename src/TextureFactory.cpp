#include "stdafx.h"
#include "TextureFactory.h"
#include "devil_wrapper.h"

TextureFactory::TextureFactory()
{}

TextureFactory::~TextureFactory()
{
	for(map<FileName, Handle>::iterator iter = textures.begin();
		iter != textures.end();
		++iter)
	{
		unsigned int id = (iter->second).getID();
		glDeleteTextures(1, &id);
	}
}

unsigned int TextureFactory::loadTexture(const FileName &fileName, bool repeat)
{
	CHECK_GL_ERROR();

	unsigned int imageName = devil_loadImage(fileName);
	unsigned char *pixels = ilGetData();
	int width		      = ilGetInteger(IL_IMAGE_WIDTH);
	int height		      = ilGetInteger(IL_IMAGE_HEIGHT);
	int bytesPerPixel	  = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	int wrapMode          = repeat ? GL_REPEAT : GL_CLAMP;

	unsigned int textureName=0;
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	gluBuild2DMipmaps(GL_TEXTURE_2D,
	                  bytesPerPixel,
	                  width,
	                  height,
	                  bytesPerPixel==4 ? GL_RGBA : GL_RGB,
	                  GL_UNSIGNED_BYTE,
	                  pixels);

	CHECK_GL_ERROR();

	ilDeleteImage(imageName);

	return textureName;
}

TextureFactory::Handle*
TextureFactory::load(const FileName &fileName, bool repeat)
{
    map<FileName, Handle>::iterator iter = textures.find(fileName);

    if(iter != textures.end())
    {
		return &(iter->second);
	}

	VERIFY(isFileOnDisk(fileName),
	       "Texture file not found: " + fileName.str());

	unsigned int id = loadTexture(fileName, repeat);
	textures.insert(make_pair(fileName, Handle(fileName, id)));

    return &textures.find(fileName)->second;
}
