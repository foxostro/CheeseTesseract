#ifndef _TEXTURE_FACTORY_H_
#define _TEXTURE_FACTORY_H_

class TextureFactory
{
public:
	/** Handle to a texture object */
	class Handle
	{
	private:
		FileName fileName; /** file name of the texture */
		unsigned int id;   /** the texture ID */

	public:
		/**
		Constructor
		@param fileName The name of the image file
		@param dimemsions Dimensions of the texture
		@param alpha Indicates the texture has an alpha component
		@param id
		*/
		Handle(const FileName &_fileName, unsigned int _id)
			: fileName(_fileName),
			id(_id)
		{}

		/** Gets the file name of the texture */
		inline const FileName& getFileName() const
		{
			return fileName;
		}

		/** Gets the OpenGL texture ID */
		inline unsigned int getID() const
		{
			return id;
		}
	};

private:
	map<FileName, Handle> textures;

public:
	~TextureFactory();
	TextureFactory();

	/**
	Load a texture
	@param fileName texture to load
	@param repeat Indicates that we want GL_REPEAT, as opposed to GL_CLAMP
	@return TextureHandle
	*/
	Handle* load(const FileName &fileName, bool repeat = true);private:
	
private:
	/**
	Loads an OpenGL texture
	@param fileName Image file name
	@param repeat Indicates GL_REPEAT when true, GL_CLAMP otherwise
	@return OpenGL texture handle
	*/
	static unsigned int loadTexture(const FileName &fileName, bool repeat);
};

#endif
