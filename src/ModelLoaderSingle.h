#ifndef _MODEL_LOADER_SINGLE_H_
#define _MODEL_LOADER_SINGLE_H_

#include "ModelLoader.h"

/**
Model loader that expects all frames of animation
to be loaded from a single file (e.g. MD2, MD3)
*/
class ModelLoaderSingle : public ModelLoader
{
private:
	/**
	Loads a model from file
	@param fileName The file name of the model
	@param tex Texture factory tracks loaded textures
	*/
	virtual AnimationController* loadFromFile(const FileName &fileName,
	                                          TextureFactory &tex) const;

	/**
	Loads all keyframes from a single model file
	@param fileName File name of the model file
	@param skinName Name of the default skin
	@param tex Texture factory tracks loaded textures
	@return key frames
	*/
	virtual vector<KeyFrame> loadKeyFrames(const FileName &fileName,
	                                       const FileName &skinName,
	                                       TextureFactory &tex) const = 0;

	static void setPolygonWinding(KeyFrame &keyFrame, const string &w);
};

#endif
