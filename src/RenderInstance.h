#ifndef _RENDER_INSTANCE_H_
#define _RENDER_INSTANCE_H_

#include "mat4.h"
#include "Material.h"
#include "ResourceBuffer.h"

typedef int RENDER_METHOD;
typedef int RENDER_METHOD_TAG;

/** Holds shader data */
class GeometryChunk {
public:
	ResourceBufferVerticesPtr_const vertexArray;
	ResourceBufferVerticesPtr_const normalArray;
	ResourceBufferTexCoordsPtr_const texCoordArray;
	ResourceBufferColorsPtr_const colorsArray;
	ResourceBufferIndicesPtr_const indexArray;
	
	mat4 transformation;
	Material material;
	GLenum polygonWinding;
	GLenum primitiveMode;
	
	GeometryChunk() {
		polygonWinding = GL_CW;
		primitiveMode = GL_TRIANGLES;
	}
};

/** Holds shader data and metadata on the shader and effect that is desired */
class RenderInstance {
public:
	GeometryChunk gc;
	RENDER_METHOD specificRenderMethod;
	RENDER_METHOD_TAG metaRenderMethod;
	bool canidateForShadowReceiver;
	bool canidateForShadowCaster;
	
	RenderInstance();
};

#endif