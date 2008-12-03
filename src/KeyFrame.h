#include "PropertyBag.h"
#include "Mesh.h"

#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

/** Stores all data related to a particular key frame in an animation */
class KeyFrame
{
private:
	/** Reference to the meshes involved in the keyframe */
	vector<Mesh*> meshes;

public:
	/**
	Constructs the keyframe from a single mesh
	@param model The mesh to use for this keyframe
	*/
	KeyFrame(Mesh* mesh);

	/**
	Constructs the keyframe from a model
	@param model The mesh to use for this keyframe
	*/
	KeyFrame(vector<Mesh*> model);

	/**
	Copy costructor
	@param keyframe The key frame to copy
	*/
	KeyFrame(const KeyFrame &keyframe);

	/**
	Assignment operator
	@param keyframe The keyframe to copy
	@return The new value of this keyframe
	*/
	KeyFrame &operator=(const KeyFrame &keyframe);

	/**
	Gets the model associated with the key frame
	@param i Index of the mesh
	@return a pointer to the model associated with this key frame
	*/
	Mesh* getMesh(size_t i) const
	{
		return meshes[i];
	}

	/**
	Gets the models associated with the key frame
	@return a pointer to the model associated with this key frame
	*/
	vector<Mesh*> & getMeshes()
	{
		return meshes;
	}

	/**
	Gets the models associated with the key frame
	@return a pointer to the model associated with this key frame
	*/
	const vector<Mesh*> & getMeshes() const
	{
		return meshes;
	}

	/**
	Merges the keyframes with the given keyframes.
	Fails when the number of keyframes is mismatched.
	However, if this object is empty, then this acts to copy the keyframes over and nothing more.
	@param keyFrames extra meshes
	@return true if the merge was successful
	*/
	bool merge(const KeyFrame &o);

    /**
	Applies the skin to the specified model
	@param model The model to which the skin should be applied.
	@param material The skin
	*/
	static void applySkinToModel(vector<Mesh*> &model, const Material &material);

	/**
	Applies the skin to the specified model
	@param model The model to which the skin should be applied.
	@param material The skin
	*/
	void applySkin(const Material &material);
};

#endif
