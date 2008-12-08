#include "stdafx.h"
#include "Renderer.h"
#include "RenderMethod_Grass_CG.h"

RenderMethod_Grass_CG::RenderMethod_Grass_CG(class Renderer *r) {
	ASSERT(r, "Null pointer: r");
	renderer = r;
	useCG = true;
	angle=0.0f;
}

bool RenderMethod_Grass_CG::isSupported() const {
	return areShadersAvailable();
}

void RenderMethod_Grass_CG::setupShader(CGcontext &cg,
                                        CGprofile &_cgVertexProfile,
                                        CGprofile &_cgFragmentProfile) {
	RenderMethod::setupShader(cg, _cgVertexProfile, _cgFragmentProfile);
	
	createVertexProgram(cg, FileName("data/shaders/cg/grass.vp.cg"));
	createFragmentProgram(cg, FileName("data/shaders/cg/grass.fp.cg"));
	
	// Vertex program parameters
	cgMVP             = getVertexProgramParameter  (cg, "MVP");
	cgView            = getVertexProgramParameter  (cg, "View");
	cgViewI           = getVertexProgramParameter  (cg, "ViewI");
	cgLightPos        = getVertexProgramParameter  (cg, "LightPos");
	cgCameraPos       = getVertexProgramParameter  (cg, "CameraPos");
	cgCameraRight     = getVertexProgramParameter  (cg, "CameraRight");
	cgCameraUp        = getVertexProgramParameter  (cg, "CameraUp");
	cgTime            = getVertexProgramParameter  (cg, "time");
	
	// Fragment program parameters
	cgTexture         = getFragmentProgramParameter(cg, "tex0");
	cgKa              = getFragmentProgramParameter(cg, "Ka");
	cgKd              = getFragmentProgramParameter(cg, "Kd");
	cgkC              = getFragmentProgramParameter(cg, "kC");
	cgkL              = getFragmentProgramParameter(cg, "kL");
	cgkQ              = getFragmentProgramParameter(cg, "kQ");
}

void RenderMethod_Grass_CG::renderPass(RENDER_PASS pass) {
	switch (pass) {
	case OPAQUE_PASS:
		pass_opaque();
		break;
	default:
		return;
	}
}

void RenderMethod_Grass_CG::pass_opaque() {
	CHECK_GL_ERROR();
	
	// Setup state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4fv(white);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	
	// Setup client state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// Alpha-test pass (Handled by the fragment shader?)
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	
	// Render the geometry chunks
	renderChunks();
	
	// Restore client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	// Restore settings
	glPopAttrib();
	
	CHECK_GL_ERROR();
}

void RenderMethod_Grass_CG::setShaderData(const GeometryChunk &gc) {
	const Renderer::Light &light0 = renderer->getLight();
	
	ASSERT(light0.type == Renderer::Light::POINT, "point lights only pls");
	
	mat4 MI = (gc.transformation).inverse(); // world -> object projection
	
	// Set the position of the light (in object-space)
	vec3 LightPosObj = MI.transformVector(light0.position);
	cgGLSetParameter3fv(cgLightPos, LightPosObj);
	
	// Set the position of the camera (in object-space)
	vec3 CameraPosWld = renderer->getCamera().getPosition();
	vec3 CameraPosObj = MI.transformVector(CameraPosWld);
	cgGLSetParameter3fv(cgCameraPos, CameraPosObj);
	
	// Send the inverse view matrix
	mat4 ViewI = getViewI();
	cgGLSetMatrixParameterfr(cgViewI, ViewI);
	
	// Send the camera right vector (in object-space)
	vec3 CameraRightEye = vec3(1.0, 0.0, 0.0);
	vec3 CameraRightObj = ViewI.transformVector(CameraRightEye);
	cgGLSetParameter3fv(cgCameraRight, CameraRightObj);
	
	// Send the camera right vector (in object-space)
	vec3 CameraUpEye = vec3(0.0, 1.0, 0.0);
	vec3 CameraUpObj = ViewI.transformVector(CameraUpEye);
	cgGLSetParameter3fv(cgCameraUp, CameraUpObj);
	
	// Set the light properties
	cgGLSetParameter1fv(cgkC, &light0.kC);
	cgGLSetParameter1fv(cgkL, &light0.kL);
	cgGLSetParameter1fv(cgkQ, &light0.kQ);
	
	// Set the material properties
	cgGLSetParameter4fv(cgKa, gc.material.Ka);
	cgGLSetParameter4fv(cgKd, gc.material.Kd);
	
	// Set animation time
	cgGLSetParameter1fv(cgTime, &angle);
	
	// Everything else
	cgGLSetStateMatrixParameter(cgMVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(cgView, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void RenderMethod_Grass_CG::tick( float timeStep ) {
	float dAngle = (timeStep / 1000.0f) * (float)(M_PI / 9.0f);
	angle = angle_clamp(angle + dAngle);
}
