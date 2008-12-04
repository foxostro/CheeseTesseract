#include "stdafx.h"
#include "devil_wrapper.h"

ILboolean ilLoadImage(const FileName &fileName)
{
	char *pszFileName = strdup(fileName.str());
	ILboolean r = ::ilLoadImage(pszFileName);
	delete[] pszFileName;
	return r;
}

unsigned int devil_loadImage(const FileName &fileName)
{
	unsigned int imageName = 0;
	ilGenImages(1, &imageName);
	ilBindImage(imageName);

	ilLoadImage(fileName);

	ILenum error = ilGetError();

	if(error != IL_NO_ERROR)
	{
		ILstring errstring = iluErrorString(error);
		TRACE(errstring);
		return 0;
	}
	else
	{
		if(ilGetInteger(IL_ORIGIN_MODE) == IL_ORIGIN_LOWER_LEFT &&
		   fileName.getExtension() != ".tif")
		{
			iluFlipImage();
		}

		return imageName;
	}
}
