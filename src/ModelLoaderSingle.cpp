#include "stdafx.h"
#include "Mesh.h"
#include "ModelLoaderSingle.h"

AnimationController*
ModelLoaderSingle::loadFromFile(const FileName &fileName,
							    TextureFactory &textureFactory) const
{
	PropertyBag xml = PropertyBag::fromFile(fileName);

	const FileName directory = fileName.getPath();

	// Winding of polygons, either "CCW" or "CW"
	string polygonWinding = "CW";
	xml.get("polygonWinding", polygonWinding);

	// Allocate a blank animation controller
	AnimationController *controller = new AnimationController();

	// Get key frames from the first source
	PropertyBag fileBag = xml.getBag("model", 0);
	const FileName file = directory.append(fileBag.getFileName("file"));
	FileName skinName;
	if(fileBag.get("skin", skinName))
	{
		skinName = directory.append(skinName);
	}
	vector<KeyFrame> keyFrames = loadKeyFrames(file, skinName, textureFactory);

	// Get the rest of the key frames
	for(size_t i = 1, numMD3 = xml.getNumInstances("model"); i<numMD3; ++i)
	{
		PropertyBag fileBag = xml.getBag("model", i);
		const FileName file = directory.append(fileBag.getFileName("file"));
		FileName skinName;
		if(fileBag.get("skin", skinName))
		{
			skinName = directory.append(skinName);
		}

		vector<KeyFrame> k = loadKeyFrames(file, skinName, textureFactory);

		for(size_t i=0; i<k.size(); ++i)
		{
			KeyFrame &keyFrame = keyFrames[i];

			keyFrame.merge(k[i]);

			// set polygon windings according to data file
			setPolygonWinding(keyFrame, polygonWinding);
		}
	}

	// Build the animations from these keyframes
	for(size_t i = 0, numAnimations = xml.getNumInstances("animation");
		i<numAnimations;
		++i)
	{
		PropertyBag animation;
		string name;
		bool looping=false;
		int start=0;
		float priority=0;
		int length=0;
		float fps=0;

		xml.get("animation", animation, i);
		animation.get("name", name);
		animation.get("priority", priority);
		animation.get("looping", looping);
		animation.get("start", start);
		animation.get("length", length);
		animation.get("fps", fps);

		// Add it to the controller
		AnimationSequence animationSequence(keyFrames,
		                                    name,
											priority,
											looping,
											start,
											length,
											fps);
		controller->addAnimation(animationSequence);
	}

	return controller;
}

void ModelLoaderSingle::setPolygonWinding(KeyFrame &keyFrame,
										  const string &polygonWinding)
{
	vector<Mesh*> &meshes = keyFrame.getMeshes();
	for(vector<Mesh*>::iterator i=meshes.begin(); i!=meshes.end(); ++i)
	{
		(*i)->polygonWinding = polygonWinding=="CCW" ? GL_CCW : GL_CW;
	}
}
