#ifndef _RENDER_METHOD_CG_TEST_H_
#define _RENDER_METHOD_CG_TEST_H_

#include "RenderMethod.h"

/** Renders a model with a test CG shader */
class RenderMethod_CGTest : public RenderMethod
{
public:	
	/** Constructor */
	RenderMethod_CGTest();

	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &cg,
	                         CGprofile &cgVertexProfile,
							 CGprofile &cgFragmentProfile);

	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass);

	/** Indicates that the shader is supported on this hardware */
	virtual bool isSupported() const;

private:
	/** Pass for opaque, shaded objects */
	void pass_opaque();

	/** Allows a RenderMethod to set shader data per geometry chunk */
	virtual void setShaderData(const GeometryChunk &gc);

private:
	CGparameter cgProj;
	CGparameter cgView;
	CGparameter cgTexture;
};

#endif
