#include "stdafx.h"
#include "Application.h"
#include "GraphicsDevice.h"

#ifdef _WIN32
void GraphicsDevice::flushMessageQueue()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
#endif

GraphicsDevice::~GraphicsDevice()
{
	SDL_FreeSurface(windowSurface);
	windowSurface = 0;
}

GraphicsDevice::GraphicsDevice(const ivec2 &windowSize, bool fullscreen)
: windowSurface(0),
  dimensions(0,0),
  nearClip(0.1f),
  farClip(100.0f)
{
	attr = generateDefaultAttributes();

	setAttributes();

	SDL_WM_SetCaption(PROJECT_NAME, PROJECT_NAME);

	windowSurface = SDL_SetVideoMode(windowSize.x,
	                                 windowSize.y,
									 32,
		                             SDL_OPENGL |
									 SDL_HWSURFACE |
									 SDL_DOUBLEBUF |
									 (fullscreen?SDL_FULLSCREEN:0));

#ifdef _WIN32
	/*
	See:
	<http://listas.apesol.org/pipermail/sdl-libsdl.org/2002-July/028824.html>
	Sometimes, after calling SDL_SetVideoMode, MessageBox and standard assert
	(which calls MessageBox internally) will return immediately without
	displaying a message box window.
	*/
	flushMessageQueue();
#endif

	ASSERT(windowSurface, "Couldn't create window!");

	initializeOpenGLState();
	initializeOpenGLExtensions();
	resizeOpenGLViewport(windowSize, nearClip, farClip);

	SDL_ShowCursor(SDL_DISABLE);

	TRACE(getOpenGLInfo());
}

GraphicsDevice::GL_ATTRIBUTES GraphicsDevice::generateDefaultAttributes()
{
	int red = 8;
	int green = 8;
	int blue = 8;
	int alpha = 8;
	bool doublebuffer = true;
	int depth = 24;
	int fsaaBuffers = 1;
	int fsaaSamples = 4; // 4x FSAA is pretty standard; good default

	GL_ATTRIBUTES attr;

	attr.insert(make_pair(SDL_GL_RED_SIZE,           red));
	attr.insert(make_pair(SDL_GL_GREEN_SIZE,         green));
	attr.insert(make_pair(SDL_GL_BLUE_SIZE,          blue));
	attr.insert(make_pair(SDL_GL_ALPHA_SIZE,         alpha));
	attr.insert(make_pair(SDL_GL_DOUBLEBUFFER,       doublebuffer?1:0));
	attr.insert(make_pair(SDL_GL_DEPTH_SIZE,         depth));
	attr.insert(make_pair(SDL_GL_MULTISAMPLEBUFFERS, fsaaBuffers));
	attr.insert(make_pair(SDL_GL_MULTISAMPLESAMPLES, fsaaSamples));

	return attr;
}

void GraphicsDevice::setAttribute(pair<SDL_GLattr, int> attribute)
{
	SDL_GL_SetAttribute(attribute.first, attribute.second);
}

void GraphicsDevice::setAttributes()
{
	for_each(attr.begin(), attr.end(), GraphicsDevice::setAttribute);
}

mat4 GraphicsDevice::getProjectionMatrix()
{
	float m[16];
	glGetFloatv(GL_PROJECTION_MATRIX, m);
	return mat4(m);
}

mat4 GraphicsDevice::getModelViewMatrix()
{
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	return mat4(m);
}

Frustum GraphicsDevice::getCameraFrustum()
{
	return Frustum(getModelViewMatrix(), getProjectionMatrix());
}

mat3 GraphicsDevice::getCameraOrientation()
{
	const mat4 modl = getModelViewMatrix();
	mat3 orientation;
	orientation.setAxisX(modl.getAxisX());
	orientation.setAxisY(modl.getAxisY());
	orientation.setAxisZ(modl.getAxisZ());
	return orientation;
}

string GraphicsDevice::getOpenGLInfo()
{
	return string("OpenGL Specs:") +
	       "\nVendor: "     + (const char*)glGetString(GL_VENDOR) + 
		   "\nRenderer: "   + (const char*)glGetString(GL_RENDERER) +
		   "\nVersion: "    + (const char*)glGetString(GL_VERSION) +
		   "\nExtensions: " + (const char*)glGetString(GL_EXTENSIONS);
}

void GraphicsDevice::initializeOpenGLState()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	// Polygon color should be mixed with texture color
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Set pixel packing to be "tight," that is, with a 1 byte row alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Backface Culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	// Set up a default colored material
	setDefaultMaterial();
	
	// Initial lighting settings
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	// Initialize FFP specular highlights
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
}

void GraphicsDevice::initializeOpenGLExtensions()
{
	GLenum err = glewInit();
	VERIFY(GLEW_OK == err, (const char*)glewGetErrorString(err));
}

void GraphicsDevice::resizeOpenGLViewport(const ivec2 &_dimensions,
										  float _nearClip,
										  float _farClip)
{
	ASSERT(_dimensions.x!=0, "Window width equals zero");
	ASSERT(_dimensions.y!=0, "Window height equals zero");
	ASSERT(_nearClip > 0, "Near clip plan is too close");
	ASSERT(_farClip > _nearClip, "Far clip plane is too close");

	dimensions = _dimensions;
	nearClip = _nearClip;
	farClip = _farClip;

	const float aspectRatio = (GLfloat)dimensions.x/(GLfloat)dimensions.y;

	// Reset view port
	glViewport(0, 0, dimensions.x, dimensions.y);

	// Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, aspectRatio, nearClip, farClip);

	// Reset model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GraphicsDevice::setDefaultMaterial()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	color ambient = color(0.0f,0.0f,0.0f,1.0f);
	color diffuse = color(0.7f,0.7f,0.7f,1.0f);
	color specular = color(1.0f,1.0f,1.0f,1.0f);
	float shininess = 64.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
	glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
}
