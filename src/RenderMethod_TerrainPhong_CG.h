#ifndef _RENDER_METHOD_TERRAIN_PHONG_DIRECTIONAL_CG_H_
#define _RENDER_METHOD_TERRAIN_PHONG_DIRECTIONAL_CG_H_

#include "RenderMethod.h"

/** Renders a model with diffuse gouraud shading and texturing */
class RenderMethod_TerrainPhong_CG : public RenderMethod
{
public:
	/** Constructor */
	RenderMethod_TerrainPhong_CG(class Renderer *r);

	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &cg,
		CGprofile &cgVertexProfile,
		CGprofile &cgFragmentProfile);

	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass);

	/** Indicates that the shader is supported on this hardware */
	virtual bool isSupported() const;

private:
	/** Allows a RenderMethod to set shader data per geometry chunk */
	virtual void setShaderData(const GeometryChunk &gc);

	/** Pass for opaque, shaded objects */
	void pass_opaque();

private:
	class Renderer *renderer;

	CGparameter cgMVP;
	CGparameter cgView;
	CGparameter cgLightDir;
	CGparameter cgCameraPos;
	CGparameter cgTexture;
	CGparameter cgKa;
	CGparameter cgKd;
};

#endif
