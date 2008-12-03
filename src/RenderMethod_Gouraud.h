#ifndef _RENDER_METHOD_DIFFUSE_TEXTURE_H_
#define _RENDER_METHOD_DIFFUSE_TEXTURE_H_

#include "RenderMethod.h"

/** Renders a model with diffuse gouraud shading and texturing */
class RenderMethod_Gouraud : public RenderMethod
{
public:
	/** Constructor */
	RenderMethod_Gouraud()
	{
		// Do Nothing
	}

	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &, CGprofile &, CGprofile &)
	{
		// Do Nothing
	}

	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass);

	/** Indicates that the shadr is supported on this hardware */
	virtual bool isSupported() const
	{
		return true; // so simple it uses opengl 1.0
	}

private:
	/** Pass for opaque, shaded objects */
	void pass_opaque();
};

#endif
