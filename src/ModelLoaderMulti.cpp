#include "stdafx.h"
#include "Mesh.h"
#include "ModelLoaderMulti.h"

AnimationController*
ModelLoaderMulti::loadFromFile(const FileName &fileName,
							   TextureFactory &textureFactory) const
{
	PropertyBag xml;
	if(!xml.loadFromFile(fileName))
		return 0;

    const FileName directory = fileName.getPath();
	const FileName skin = directory.append(xml.getFileName("skin"));

    AnimationController *controller = new AnimationController();

	for(size_t i=0, numAnimations=xml.getNumInstances("animation");
        i<numAnimations;
        ++i)
	{
        const PropertyBag animation = xml.getBag("animation", i);

        AnimationSequence sequence = loadAnimation(animation,
		                                           directory,
												   skin,
												   textureFactory);

		controller->addAnimation(sequence);
	}

	return controller;
}

AnimationSequence ModelLoaderMulti::loadAnimation(const PropertyBag &animation,
                                                  const FileName &directory,
												  const FileName &skin,
												  TextureFactory &texFactory)
                                                  const
{
    const string name = animation.getString("name");
    const float fps = animation.getFloat("fps");

    bool looping = false;
	animation.get("looping", looping); // optional tag

	float priority = 0.0f;
	animation.get("priority", priority); // optional tag

    vector<KeyFrame> keyFrames = loadKeyFrames(animation, directory);

	applySkin(keyFrames, Material(texFactory.load(skin)));

    return AnimationSequence(keyFrames,
                             name,
							 priority,
                             looping,
                             0,
                             keyFrames.size(),
                             fps);
}

vector<KeyFrame> ModelLoaderMulti::loadKeyFrames(const PropertyBag &animation,
                                                 const FileName &directory) const
{
    vector<KeyFrame> keyFrames;

    const size_t length = animation.getNumInstances("keyframe");

    for(size_t j = 0; j < length; ++j)
    {
        const string fileName = animation.getString("keyframe", j);
        const FileName pathedFileName = directory.append(FileName(fileName));

        keyFrames.push_back(loadKeyFrame(pathedFileName));
    }

    return keyFrames;
}

void ModelLoaderMulti::applySkin(vector<KeyFrame> &kf, Material material)
{
    for_each(kf.begin(), kf.end(), bind(&KeyFrame::applySkin, &_1, material));
}

vector<Mesh*> ModelLoaderMulti::createKeyFrame(Mesh *mesh)
{
	ASSERT(mesh!=0, "Parameter \'mesh\' was null");
	vector<Mesh*> kf;
	kf.push_back(mesh);
	return kf;
}
