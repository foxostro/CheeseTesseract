#ifndef _RENDER_METHOD_PARTICLE_H_
#define _RENDER_METHOD_PARTICLE_H_

#include "RenderMethod.h"

/** Renders particles with transparent, textured, and unlit quads */
class RenderMethod_Particles : public RenderMethod {
public:
	/** Constructor */
	RenderMethod_Particles() {
		// Do Nothing
	}
	
	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &, CGprofile &, CGprofile &) {
		// Do Nothing
	}
	
	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass);
	
	/** Indicates that the shadr is supported on this hardware */
	virtual bool isSupported() const {
		return true; // so simple it uses opengl 1.0
	}
	
private:
	/** Pass for opaque, shaded objects */
	void pass_particles();
	
	void renderParticleBatch(const GeometryChunk &gc );
	
	GLsizei getPrimitiveCount(const GeometryChunk &gc);
};

#endif
