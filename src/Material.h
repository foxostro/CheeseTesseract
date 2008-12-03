#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "TextureFactory.h"

/** Encapsulates texture and material settings */
class Material
{
public:
	/** Default Constructor */
	Material();

	/** Copy Constructor */
	Material(const Material &mat);

	/**
	Sets a texture for the material and leaves all else at default values.
	@param fileName Texture file name
	*/
	Material(TextureFactory::Handle *texture);

	/** Clears the state of the Material and resets it */
	void clear();

	/**
	Adds a texture to the list of textures to apply during multitexturing
	@param textureHandle A handle to the texture
	*/
	void setTexture(TextureFactory::Handle *textureHandle);

	/**	Passes Material information to the currently bound Effect. */
	void bind() const;

    /** Gets the file name of the texture */
    inline FileName getFileName() const
    {
        return texture->getFileName();
    }

private:
	/**
	Copies across from another material
	@param mat The object to copy
	*/
	void copy(const Material &mat);
	
	/** Applies texture filter settings */
	static void setTextureFilters();

public:
	bool glow;
	color Ka;
	color Kd;
	color Ks;
	float shininess;

    /**
    Handle to the texture resource.  Texture resource may be reloaded or
    altered behind the scenes by the texture manager.
    */
	TextureFactory::Handle *texture;
};

#endif

