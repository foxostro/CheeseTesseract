#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "ScopedEventHandlerSubscriber.h"
#include "GraphicsDevice.h"
#include "RenderInstance.h"
#include "RenderMethod.h"
#include "Camera.h"
#include "ActionQueueRenderInstance.h"

/** Renderer handles the bulk of render work for the client */
class Renderer : public ScopedEventHandlerSubscriber {
public:
	/** Bundles properties for a single light */
	struct Light {
		color ambient;
		color diffuse;
		color specular;
		float kC;
		float kL;
		float kQ;
		vec3 position;
		vec3 direction;
		enum LIGHT_TYPE { SPOT, POINT, DIRECTIONAL } type;
	};
	
public:
	virtual string getTypeString() const {
		return "Renderer";
	}
	
	/** Destructor */
	virtual ~Renderer();
	
	/** Constructor */
	Renderer(UID uid, ScopedEventHandler *parentScope);
	
	/** Changes the camera that will be used for the scene */
	void changeCamera(const Camera &_camera) {
		camera = _camera;
	}
	
	/** Gets the current camera settings */
	const Camera& getCamera() const {
		return camera;
	}
	
	/** Draws the scene this frame */
	void drawScene();
	
	/**
	Ticks the scene with some time step.  Usually this is unused, but some
	shaders will want to perform animation on the GPU and will need the
	current time.
	*/
	void tick(float timeStep);
	
	/**
	Resets the scene so objects can start emitting more geometry
	Called once per frame to set up the scene
	*/
	void setupScene();
	
	/** Gets light settings (only supports one light ATM) */
	const Light& getLight() const {
		return light0;
	}
	
	/**
	Given a render method tag, get the token identifying the method actually
	*/
	RENDER_METHOD getMethodFromTag(RENDER_METHOD_TAG tag);
	
private:
	/** Start OpenGL */
	void initializeGraphicsDevice();
	
	/** Initialize CG context */
	void initializeCG();
	
	/**
	Checks for errors reported by CG.
	If one is detected, print a message and exit the app
	*/
	static void checkForCgError(CGcontext cg, const string &situation);
	
	/** Collect geometry emitted by game objects */
	void handleActionQueueRenderInstance(const ActionQueueRenderInstance *a);
	
	/** Collect trees queued for render */
	void handleActionQueueTreeForRender(const ActionQueueTreeForRender *a);
	
	/** Called one time to set up the shader when the renderer starts */
	void initializeShaders();
	
	/** Initialize default light settings */
	void initializeLights();
	
	/**
	Create a new shader and bind it to the specified tag.
	If the quality of the shader is greater than others bound to the same tag,
	and the shader can be run on this machine, then it will be used when
	effects of the given tag are called for.
	*/
	void tagRenderMethod(RENDER_METHOD_TAG tag,
	                     RENDER_METHOD token,
	                     int quality);
	                     
	/** Called one time to set up the shader when the renderer starts */
	void setupShaders();
	
	/** Shaders perform work necessary for the given pass */
	void renderPass(RENDER_PASS pass);
	
	/** Sets the properties of the specified GL light */
	static void setLight( GLenum lightNum, const Light &light );
	
private:
	typedef map<RENDER_METHOD, shared_ptr<RenderMethod> > RenderMethods;
	typedef map<RENDER_METHOD_TAG, priority_queue<pair<int, RENDER_METHOD> > > MetaRenderMethods;
	
	RenderMethods renderMethods;
	MetaRenderMethods metaRenderMethods;
	
	shared_ptr<GraphicsDevice> graphicsDevice;
	
	CGprofile cgVertexProfile;
	CGprofile cgFragmentProfile;
	CGcontext cg;
	
	Light light0;
	
	Camera camera;
	
private:
	struct TreeData {
		void *tree;
		vec3 position;
	};
	
	/** Trees available for assigning to a tree instance */
	vector<void *> tree_pool;
	
	/** Trees to render persistently */
	queue<TreeData> tree_queue;
	
	/** Load, Link and Initialize the TreeLib library */
	void initializeTreeLib();
	
	/** Generates a tree using the TreeLib library */
	void* createTree();
	
	/**
	Generates a pool of trees using the TreeLib library
	@param treePoolSize Size of the tree pool to generate
	*/
	void initializeTreePool(size_t treePoolSize);
	
	/** Selects a tree at random (uniform distribution) from the tree pool */
	void* getTreeFromPool(int seed);
	
	/** Renders all trees iin the queue */
	void renderTrees();
	
	/**
	Prepares an individual tree for rendering by setting up
	matrices and adding it to the current TreeLib composition.
	*/
	void prepareTree(const TreeData &data);
};

#endif
