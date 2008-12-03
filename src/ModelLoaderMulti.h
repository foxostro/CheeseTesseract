#ifndef _MODEL_LOADER_MULTI_H_
#define _MODEL_LOADER_MULTI_H_

#include "ModelLoader.h"

/**
Model loader that expects each key frame of the animation
to be loaded from a separate file (e.g. 3DS, OBJ)
*/
class ModelLoaderMulti : public ModelLoader
{
protected:
	/**
	Loads a model from file
	@param fileName The file name of the model
	@param textureFactory Texture factory tracks loaded textures
	*/
	virtual AnimationController*
		loadFromFile(const FileName &fileName,
	                 TextureFactory &textureFactory) const;

    /**
    Loads a particular animation
    @param animation Animation data
    @param directory Directory where the model can be located
    @param skin File name of the skin to apply
	@param textureFactory Texture factory tracks loaded textures
    @return Key frames in the animation
    */
    AnimationSequence loadAnimation(const PropertyBag &animation,
                                    const FileName &directory,
									const FileName &skin,
									TextureFactory &textureFactory) const;

    /**
    Loads key frames for a particular animation
    @param animation Animation data
    @param directory Directory where the model can be located
    @return Key frames in the animation
    */
    vector<KeyFrame> loadKeyFrames(const PropertyBag &animation,
                                   const FileName &directory) const;

	/**
	Loads a single model from file
	@param fileName File name of the model
	@return vector<Mesh*>
	*/
	virtual vector<Mesh*> loadKeyFrame(const FileName &fileName) const = 0;

	/**
	Applies the skin to the specified model
	@param model The model to which the skin should be applied.
	@param material The skin
	*/
    static void applySkin(vector<KeyFrame> &kf, Material material);

	/**
	Takes a mesh and creates a key frame containing just that mesh
	@param mesh Mesh
	@return Key frame containing just that one mesh
	@todo Assign createKeyFrame to a fitting class?
	*/
	static vector<Mesh*> createKeyFrame(Mesh *mesh);
};

#endif
