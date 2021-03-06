#include "stdafx.h"
#include "Frustum.h"
#include "Renderer.h"
#include "RenderMethodTags.h"
#include "RenderMethod.h"
#include "RenderMethod_RenderNormals_CG.h"
#include "RenderMethod_PhongDirectional_CG.h"
#include "RenderMethod_PhongPoint_CG.h"
#include "RenderMethod_Gouraud.h"
#include "RenderMethod_Particles.h"
#include "RenderMethod_GrassFFP.h"
#include "RenderMethod_Grass_CG.h"
#include "RenderMethod_CGTest.h"

#include <treelib-interface.h>

Frustum g_Frustum;

Renderer::Renderer(UID uid, ScopedEventHandler *parentScope)
		: cgVertexProfile(CG_PROFILE_ARBVP1),
		cgFragmentProfile(CG_PROFILE_ARBFP1),
		ScopedEventHandlerSubscriber(uid, parentScope) {
	REGISTER_HANDLER(Renderer::handleActionQueueRenderInstance);
	REGISTER_HANDLER(Renderer::handleActionQueueTreeForRender);
	
	initializeLights();
	
	initializeGraphicsDevice();
	initializeCG();
	initializeShaders();
	initializeTreeLib();
}

void Renderer::drawScene() {
	CHECK_GL_ERROR();
	
	camera.setCamera();
	
	// Updates our record of the camera frustum
	{
		mat4 modl, proj;
		glGetFloatv(GL_MODELVIEW_MATRIX, modl);
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		g_Frustum.set(modl, proj);
	}
	
	setLight(GL_LIGHT0, light0);
	
	for (int pass=0; pass < (int)NUM_PASSES; ++pass) {
		renderPass((RENDER_PASS)pass);
	}
	
	/*
	Render trees using TreeLib as the renderer because
	the library gives us no other choice.
	*/
	renderTrees();
	
	CHECK_GL_ERROR();
}

void Renderer::initializeGraphicsDevice() {
	bool fullscreen	= false;
	ivec2 windowSize(640, 480);
	GraphicsDevice *gd = new GraphicsDevice(windowSize, fullscreen);
	graphicsDevice = shared_ptr<GraphicsDevice>(gd);
	
	TRACE("Graphics device initialized");
}

void Renderer::handleActionQueueRenderInstance(const ActionQueueRenderInstance *action) {
	RenderInstance r = action->instance;
	
	// Does the instance request a general category of render methods?
	if (r.metaRenderMethod != TAG_UNSPECIFIED) {
		r.specificRenderMethod = getMethodFromTag(r.metaRenderMethod);
	}
	
	(renderMethods[r.specificRenderMethod])->acceptgc(r.gc);
}

void Renderer::handleActionQueueTreeForRender(const ActionQueueTreeForRender *action) {
	TreeData data;
	data.position = action->position;
	data.tree = getTreeFromPool(action->seed);
	tree_queue.push(data);
}

void Renderer::initializeShaders() {
	renderMethods[METHOD_CG_RENDER_NORMALS]    = shared_ptr<RenderMethod>(new RenderMethod_RenderNormals_CG());
	renderMethods[METHOD_CG_PHONG_DIRECTIONAL] = shared_ptr<RenderMethod>(new RenderMethod_PhongDirectional_CG(this));
	renderMethods[METHOD_CG_PHONG_POINT]       = shared_ptr<RenderMethod>(new RenderMethod_PhongPoint_CG(this));
	renderMethods[METHOD_DIFFUSE_TEX_FFP]      = shared_ptr<RenderMethod>(new RenderMethod_Gouraud());
	renderMethods[METHOD_PARTICLE]             = shared_ptr<RenderMethod>(new RenderMethod_Particles());
	renderMethods[METHOD_GRASS_FFP]            = shared_ptr<RenderMethod>(new RenderMethod_GrassFFP());
	renderMethods[METHOD_GRASS_CG]             = shared_ptr<RenderMethod>(new RenderMethod_Grass_CG(this));
	renderMethods[METHOD_CG_TEST]              = shared_ptr<RenderMethod>(new RenderMethod_CGTest());
	
	tagRenderMethod(TAG_DIFFUSE_TEX, METHOD_CG_PHONG_POINT,  2);
	tagRenderMethod(TAG_DIFFUSE_TEX, METHOD_DIFFUSE_TEX_FFP, 1);
	
	tagRenderMethod(TAG_GRASS,       METHOD_GRASS_CG,        2);
	tagRenderMethod(TAG_GRASS,       METHOD_GRASS_FFP,       1);
	
	setupShaders();
	
	TRACE("Shaders initialized");
}

void Renderer::setupShaders() {
	// Flush CG errors
	while (cgGetError() != CG_NO_ERROR);
	
	for (RenderMethods::iterator i=renderMethods.begin();
	     i!=renderMethods.end(); ++i) {
		shared_ptr<RenderMethod> &renderMethod = i->second;
		renderMethod->setupShader(cg, cgVertexProfile, cgFragmentProfile);
		checkForCgError(cg, "Shader setup caused an error");
	}
}

void Renderer::checkForCgError( CGcontext cg, const string &situation ) {
	CGerror error;
	
	const char *errstr= cgGetLastErrorString(&error);
	
	if (error != CG_NO_ERROR) {
		string message = "ERROR: " + situation + ": " + errstr;
		
		if (error == CG_COMPILER_ERROR) {
			message = message + "\n" + cgGetLastListing(cg);
		}
		
		FAIL(message);
	}
}

void Renderer::setupScene() {
	for (RenderMethods::iterator i=renderMethods.begin();
	     i!=renderMethods.end(); ++i) {
		shared_ptr<RenderMethod> &renderMethod = (i->second);
		renderMethod->setupScene();
	}
}

void Renderer::renderPass( RENDER_PASS pass ) {
	for (RenderMethods::iterator i=renderMethods.begin();
	     i!=renderMethods.end(); ++i) {
		shared_ptr<RenderMethod> &renderMethod = i->second;
		renderMethod->renderPass(pass);
	}
}

Renderer::~Renderer() {
	cgDestroyContext(cg);
}

void Renderer::initializeCG() {
	TRACE("Initializing CG runtime...");
	
	cg = cgCreateContext();
	checkForCgError(cg, "cgCreateContext");
	
	cgSetAutoCompile(cg, CG_COMPILE_LAZY);
	
	cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgGLSetOptimalOptions(cgVertexProfile);
	checkForCgError(cg, "cgGLSetOptimalOptions(cgVertexProfile)");
	
	cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(cgFragmentProfile);
	checkForCgError(cg, "cgGLSetOptimalOptions(cgFragmentProfile)");
	
	TRACE("..done (CG runtime initialized)");
}

void Renderer::tagRenderMethod(RENDER_METHOD_TAG tag,
                               RENDER_METHOD token,
                               int quality) {
	shared_ptr<RenderMethod> renderMethod = renderMethods[token];
	
	if (renderMethod->isSupported()) {
		metaRenderMethods[tag].push(make_pair(quality, token));
	}
}

void Renderer::setLight( GLenum lightNum, const Light &light ) {
	CHECK_GL_ERROR();
	glLightfv(lightNum, GL_AMBIENT,  light.ambient);
	glLightfv(lightNum, GL_DIFFUSE,  light.diffuse);
	glLightfv(lightNum, GL_SPECULAR, light.specular);
	
	switch (light.type) {
	case Light::SPOT:
		FAIL("Spot lights not implented");
		// TODO: Set spot cutoff
		// TODO: Set spot exponent
		
		// Fall through here
		
	case Light::POINT:
		glLightf(lightNum, GL_CONSTANT_ATTENUATION,  light.kC);
		glLightf(lightNum, GL_LINEAR_ATTENUATION,    light.kL);
		glLightf(lightNum, GL_QUADRATIC_ATTENUATION, light.kQ);
		glLightfv(lightNum, GL_POSITION, vec4(light.position.x,
		                                      light.position.y,
		                                      light.position.z,
		                                      1.0f));
		break;
		
	case Light::DIRECTIONAL:
		glLightfv(lightNum, GL_POSITION, vec4(light.direction.x,
		                                      light.direction.y,
		                                      light.direction.z,
		                                      0.0f));
		break;
	}
	
	glEnable (lightNum);
	CHECK_GL_ERROR();
}

void Renderer::initializeLights() {
	light0.ambient = color(0.1f, 0.1f, 0.1f, 1.0f);
	light0.diffuse = white;
	light0.specular = white;
	light0.kC = 0.00f;
	light0.kL = 0.00f;
	light0.kQ = 0.01f;
	light0.position = vec3(20, 15, 5);
	light0.type = Light::POINT;
}

void Renderer::tick( float timeStep ) {
	for (RenderMethods::iterator i=renderMethods.begin();
	     i!=renderMethods.end(); ++i) {
		shared_ptr<RenderMethod> &renderMethod = i->second;
		renderMethod->tick(timeStep);
	}
}

RENDER_METHOD Renderer::getMethodFromTag( RENDER_METHOD_TAG tag ) {
	return metaRenderMethods[tag].top().second;
}

void Renderer::initializeTreeLib() {
#ifdef _WIN32
	FileName path_to_treelib = getApplicationDirectory().append(FileName("treelib.dll"));
	if (!TreeLib::load(path_to_treelib.c_str())) {
		FAIL(string("Error while linking: ") + TreeLib::error());
	}
	
	initializeTreePool(10);
#else
	TRACE("TreeLib is not available for Linux. TODO: Replacement for TreeLib");
#endif
}

void Renderer::renderTrees() {
#ifdef _WIN32
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	
	/*
	Full white diffuse, no ambient, and no specular
	*/
	color ambient = color(0.0f,0.0f,0.0f,1.0f);
	color diffuse = color(1.0f,1.0f,1.0f,1.0f);
	color specular = color(0.0f,0.0f,0.0f,1.0f);
	float shininess = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	// Prepare a composition and render (and flush the queue)
	while (!tree_queue.empty()) {
		prepareTree(tree_queue.front());
		tree_queue.pop();
	}
	TreeLib::renderComposition();
	
	glPopAttrib();
#endif
}

void Renderer::prepareTree( const TreeData &data ) {
#ifdef _WIN32
	glPushMatrix();
	glTranslatef(data.position.x, data.position.y, data.position.z);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotate so Z-axis is UP
	TreeLib::putTree(data.tree);
	glPopMatrix();
#endif
}

void* Renderer::createTree() {
#ifdef _WIN32
	TreeLib::Tree tree = TreeLib::createTree();
	
	if (!tree) {
		FAIL(string("Error while creating a tree: ") + TreeLib::error());
	}
	
	TreeLib::setAttributei(tree, TL_SHADOW, TL_NO_SHADOW);
	TreeLib::setAttributei(tree, TL_AUTO_BALANCE, 1);
	TreeLib::setAttributef(tree, TL_BASE_LENGTH, 0.9f); // Default = 1.25
	TreeLib::setAttributef(tree, TL_BASE_RADIUS, 0.3f); // Default = 0.25
	
	TreeLib::rebuildTree(tree);
	
	return tree;
#else
	return 0;
#endif
}

void Renderer::initializeTreePool( size_t treePoolSize ) {
	for (size_t i=0; i<treePoolSize; ++i) {
		tree_pool.push_back(createTree());
	}
}

void* Renderer::getTreeFromPool(int seed) {
	return tree_pool[abs(seed) % tree_pool.size()];
}
