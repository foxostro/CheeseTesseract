#pragma once
#ifndef _DEVIL_WRAPPER_H_
#define _DEVIL_WRAPPER_H_

/* DevIL Image Library */
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

/**
Loads an image and returns the DevIL image handle
@param fileName File name of the font image
@return DevIL image handle
*/
unsigned int devil_loadImage(const FileName &fileName);

/**
Calls DevIL's ilLoadImage, but allowing FileName type parameter
@param fileName File name of the font image
@return DevIL image handle
*/
ILboolean ilLoadImage(const FileName &fileName);

#endif
