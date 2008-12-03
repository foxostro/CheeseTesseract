#ifndef _RENDER_METHOD_H_
#define _RENDER_METHOD_H_

#include "RenderInstance.h"
#include "RenderPass.h"

#include <Cg/cg.h>
#include <Cg/cgGL.h>

/**
Responsible for rendering chunks of
geometry according to some visual
effect.
*/
class RenderMethod
{
public:
	/** Constructor */
	RenderMethod();

	/** Destructor */
	virtual ~RenderMethod();

	/**
	Ticks the scene with some time step.  Usually this is unused, but some
	shaders will want to perform animation on the GPU and will need the
	current time.
	*/
	virtual void tick(float timeStep) { /* Do Nothing */ }

	/** Accepts a chunk of geometry and adds it to the render bucket */
	virtual void acceptgc(const GeometryChunk &gc);

	/** Called one time to set up the shader when the renderer starts */
	virtual void setupShader(CGcontext &cg,
	                         CGprofile &cgVertexProfile,
							 CGprofile &cgFragmentProfile);

	/** Called once per frame to set up the scene */
	virtual void setupScene();

	/** Shader performs whatever work is necessary during the specified pass*/
	virtual void renderPass(RENDER_PASS pass)=0;

	/** Indicates that the shader is supported on this hardware */
	virtual bool isSupported() const = 0;

protected:
	/** Renders all geometry chunks using the active settings */
	virtual void renderChunks();

	/** Render a single geometry chunk using the active settings */
	virtual void renderChunk(const GeometryChunk &gc);

	/** Allows a RenderMethod to set shader data per geometry chunk */
	virtual void setShaderData(const GeometryChunk &)
	{
		// Do Nothing
	}

	/**
	Checks for errors reported by CG.
	If one is detected, print a message and exit the app
	*/
	static void checkForCgError(CGcontext cg, const string &situation);

	/**
	Creates a CG vertex program and bind to the vertex_program member
	@param cg CG context
	@param vp File name of the vertex program
	*/
	void createVertexProgram( CGcontext & cg, const FileName &vp );

	/**
	Creates a CG fragment program and bind to the fragment_program member
	@param cg CG context
	@param fp File name of the fragment program
	*/
	void createFragmentProgram( CGcontext & cg, const FileName &fp );

	/**
	Gets a named parameter of the vertex program.
	On error, reports the error and exits the application.
	@param cg CG context
	@param name Parameter name
	*/
	CGparameter getVertexProgramParameter(CGcontext &cg, const char *name);

	/**
	Gets a named parameter of the fragment program.
	On error, reports the error and exits the application.
	@param cg CG context
	@param name Parameter name
	*/
	CGparameter getFragmentProgramParameter(CGcontext &cg, const char *name);

	/** Indicates that CG support is available */
	static bool areShadersAvailable();

	/** Gets the inverse model-view matrix */
	static mat4 getView();

	/** Gets the inverse model-view matrix */
	static mat4 getViewI();

	/** Gets the projection matrix */
	static mat4 getProj();

protected:
	vector<GeometryChunk> bucket;

	CGprogram vertex_program;
	CGprogram fragment_program;

	CGprofile cgVertexProfile;
	CGprofile cgFragmentProfile;

	bool useCG;
};

#endif
