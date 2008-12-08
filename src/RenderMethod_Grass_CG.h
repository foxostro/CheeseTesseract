#ifndef _RENDER_METHOD_GRASS_CG_H_
#define _RENDER_METHOD_GRASS_CG_H_

#include "RenderMethod.h"

/**
Renders grass with billboarding and movement on the GPU
*/
class RenderMethod_Grass_CG : public RenderMethod {
public:
	/** Constructor */
	RenderMethod_Grass_CG(class Renderer *r);
	
	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &cg,
	                         CGprofile &cgVertexProfile,
	                         CGprofile &cgFragmentProfile);
	                         
	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass);
	
	/** Indicates that the shader is supported on this hardware */
	virtual bool isSupported() const;
	
	/**
	Ticks the scene with some time step.  Usually this is unused, but the
	grass vertex program needs a time step to perform animation.
	*/
	virtual void tick(float timeStep);
	
private:
	/** Allows a RenderMethod to set shader data per geometry chunk */
	virtual void setShaderData(const GeometryChunk &gc);
	
	/** Pass for opaque, shaded objects */
	void pass_opaque();
	
private:
	class Renderer *renderer;
	
	CGparameter cgMVP;
	CGparameter cgView;
	CGparameter cgViewI;
	CGparameter cgLightPos;
	CGparameter cgTexture;
	
	CGparameter cgKa;
	CGparameter cgKd;
	CGparameter cgkC;
	
	CGparameter cgkL;
	CGparameter cgkQ;
	
	CGparameter cgCameraPos;
	CGparameter cgCameraRight;
	CGparameter cgCameraUp;
	
	CGparameter cgTime;
	float angle;
};

#endif
