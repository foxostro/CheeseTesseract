#ifndef _RENDER_METHOD_RENDER_NORMALS_H_
#define _RENDER_METHOD_RENDER_NORMALS_H_

#include "RenderMethod.h"

/** Renders just the normals on a model */
class RenderMethod_RenderNormals_CG : public RenderMethod {
public:
	/** Constructor */
	RenderMethod_RenderNormals_CG();
	
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
	CGparameter cgMVP;
	CGparameter cgViewIT;
};

#endif
