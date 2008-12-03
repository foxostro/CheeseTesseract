#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

#include "AnimationController.h"

typedef map< string, AnimationController* > FileNameToModel;

/** Generic model loader */
class ModelLoader
{
private:
	/** Stores previously loaded MD3 models */
	static FileNameToModel cache;

protected:
	/**
	Inserts the model into the cache
	@param fileName file name to identify the model
	@param controller animated model to cache
	*/
	static void insertInCache(const FileName &fileName, AnimationController *controller);

	/**
	Gets the model from the cache
	@param fileName file name to identify the model
	@return animated model retrieved from the cache
	*/
	static AnimationController* getFromCache(const FileName &fileName);

	/**
	Loads a model from file
	@param fileName The file name of the model
	@param textureFactory Texture factory tracks loaded textures
	*/
	virtual AnimationController*
	loadFromFile(const FileName &fileName,
	             TextureFactory &textureFactory) const = 0;

public:
	/** Destructor */
	virtual ~ModelLoader() {}

	/**
	Loads a model from file or the cache
	@param fileName The file name of the model
	@param textureFactory Texture factory tracks loaded textures
	*/
	AnimationController* load(const FileName &fileName,
	                          TextureFactory &textureFactory);
};

#endif
