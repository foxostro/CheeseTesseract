#ifndef _GRAPHICS_DEVICE_H_
#define _GRAPHICS_DEVICE_H_

#include "Frustum.h"

/**
Controls graphics device state.
Creates a window or fullscreen surface for hardware accelerated rendering.
*/
class GraphicsDevice {
private:
	typedef map<SDL_GLattr, int> GL_ATTRIBUTES;
	
	/** Attribute -> Value */
	GL_ATTRIBUTES attr;
	
	/** SDL Window */
	SDL_Surface *windowSurface;
	
	/** Window dimensions */
	ivec2 dimensions;
	
	/** Distances to the clip planes */
	float nearClip, farClip;
	
public:
	~GraphicsDevice();
	
	/** Creates a window for OpenGL rendering */
	GraphicsDevice(const ivec2 &windowSize, bool fullscreen);
	
	/** Gets the OpenGL projection matrix */
	static mat4 getProjectionMatrix();
	
	/** Gets the OpenGL modelview matrix */
	static mat4 getModelViewMatrix();
	
	/** Computes a frustum from the OpenGL modelview and projection matrices*/
	static Frustum getCameraFrustum();
	
	/** Computes the camera orientation from the OpenGL modelview matrix */
	static mat3 getCameraOrientation();
	
	/**
	Sets up the OpenGL viewport for the given window dimensions
	@param dimensions Window dimensions
	@param nearClip Distance of the near clip plane
	@param farClip Distance of the far clip plane
	*/
	void setClippingPlanes(float nearClip, float farClip) {
		resizeOpenGLViewport(dimensions, nearClip, farClip);
	}
	
	/** Gets window dimensions */
	ivec2 getDimensions() const {
		return dimensions;
	}
	
private:
	/** Do not call the assignment operator */
	GraphicsDevice operator=(const GraphicsDevice &rh);
	
	/** Do not call the copy constructor */
	GraphicsDevice(const GraphicsDevice &o);
	
	/** Gets OpenGL version and renderer information */
	static string getOpenGLInfo();
	
	/** Initializes OpenGL state settings */
	static void initializeOpenGLState();
	
	static void setDefaultMaterial();
	
	/** Initializes OpenGL extensions */
	static void initializeOpenGLExtensions();
	
	/**
	Sets up the OpenGL viewport for the given window dimensions
	@param dimensions Window dimensions
	@param nearClip Distance of the near clip plane
	@param farClip Distance of the far clip plane
	*/
	void resizeOpenGLViewport(const ivec2 &dimensions,
	                          float nearClip,
	                          float farClip);
	                          
	/**
	Generates default attributes for the SDL video mode
	@return attribute set
	*/
	static GL_ATTRIBUTES generateDefaultAttributes();
	
	/**
	Sets video mode attributes.
	These attributes are only bound on the next call to setVideoMode.
	@param attribute single SDL attribute
	*/
	static void setAttribute(pair<SDL_GLattr, int> attribute);
	
	/** Set all the "video mode" attributes that are recorded right now */
	void setAttributes();
	
#ifdef _WIN32
	/**
	Flushes all messages from the windows message queue.
	See:
	<http://listas.apesol.org/pipermail/sdl-libsdl.org/2002-July/028824.html>
	Sometimes, after calling SDL_SetVideoMode, MessageBox and standard assert
	(which calls MessageBox internally) will return immediately without
	displaying a message box window.
	*/
	static void flushMessageQueue();
#endif
};

#endif
