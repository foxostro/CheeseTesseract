#ifndef _RENDER_METHOD_GRASS_FFP_H_
#define _RENDER_METHOD_GRASS_FFP_H_

#include "RenderMethod.h"

/** Renders grass with the fixed-function pipeline */
class RenderMethod_GrassFFP : public RenderMethod {
public:
	/** Constructor */
	RenderMethod_GrassFFP() {
		// Do Nothing
	}
	
	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &, CGprofile &, CGprofile &) {
		// Do Nothing
	}
	
	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass);
	
	/** Indicates that the shader is supported on this hardware */
	virtual bool isSupported() const {
		return true; // so simple it uses OpenGL 1.0
	}
	
private:
	/** Pass for opaque, shaded objects */
	void pass_opaque();
};

#endif
