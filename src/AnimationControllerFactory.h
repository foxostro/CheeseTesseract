#ifndef _ANIMATION_CONTROLLER_FACTORY_H_
#define _ANIMATION_CONTROLLER_FACTORY_H_

#include "AnimationController.h"

class ModelLoader; // class prototype

/** Creates AnimationController objects from file */
class AnimationControllerFactory
{
public:
	/**
	Constructor
	@param textureFactory Loads textures for the model loaders
	*/
    AnimationControllerFactory(TextureFactory &textureFactory);

    /**
    Creates an animated model from file
    @param fileName Name of the file containing model data
    @return New animation controller; ownership passes to the caller.
    */
    AnimationController* createFromFile(const FileName &fileName);

    /**
    Creates an instance of the "default" model.
    @return New animation controller; ownership passes to the caller.
    */
    AnimationController* createDefault();

private:
	/** Do not call the assignment operator */
	AnimationControllerFactory operator=(const AnimationControllerFactory &rh);

	/** Do not call the copy constructor */
	AnimationControllerFactory(const AnimationControllerFactory &factory);

private:
    typedef map<string, shared_ptr<ModelLoader> > MAP;
    MAP loader;
	TextureFactory &textureFactory;
};

#endif
