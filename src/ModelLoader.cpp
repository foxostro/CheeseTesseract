#include "stdafx.h"
#include "ModelLoader.h"

FileNameToModel ModelLoader::cache;

void ModelLoader::insertInCache(const FileName &fileName, AnimationController *controller)
{
	ASSERT(controller!=0, "controller was null");
	cache.insert(make_pair(fileName.str(), controller));
}

AnimationController* ModelLoader::getFromCache(const FileName &fileName)
{
	FileNameToModel::const_iterator iter = cache.find(fileName.str());

	if(iter == cache.end())
	{
		return 0;
	}
	else
	{
		return(iter->second);
	}
}

AnimationController* ModelLoader::load(const FileName &fileName,
									   TextureFactory &textureFactory)
{
	AnimationController *controller = getFromCache(fileName);

	if(controller == 0)
	{
		// copy allocated for the cache alone
		controller = loadFromFile(fileName, textureFactory);

		if(!controller)
			return 0; // failed to load model

		insertInCache(fileName, controller);
	}

	ASSERT(controller!=0, "controller was null");

	return new AnimationController(*controller); // copy allocated for the client alone
}
