#include "stdafx.h"
#include "Renderer.h"
#include "RenderMethod_PhongPoint_CG.h"

RenderMethod_PhongPoint_CG::RenderMethod_PhongPoint_CG(class Renderer *r) {
	ASSERT(r, "Null pointer: r");
	renderer = r;
	useCG = true;
}

bool RenderMethod_PhongPoint_CG::isSupported() const {
	return areShadersAvailable();
}

void RenderMethod_PhongPoint_CG::setupShader(CGcontext &cg,
  CGprofile &_cgVertexProfile,
  CGprofile &_cgFragmentProfile) {
	RenderMethod::setupShader(cg, _cgVertexProfile, _cgFragmentProfile);
	
	createVertexProgram(cg, FileName("data/shaders/cg/phong_point.vp.cg"));
	createFragmentProgram(cg, FileName("data/shaders/cg/phong_point.fp.cg"));
	
	// Vertex program parameters
	cgMVP             = getVertexProgramParameter  (cg, "MVP");
	cgView            = getVertexProgramParameter  (cg, "View");
	cgLightPos        = getVertexProgramParameter  (cg, "LightPos");
	cgCameraPos       = getVertexProgramParameter  (cg, "CameraPos");
	cgTexture         = getFragmentProgramParameter(cg, "tex0");
	cgKa              = getFragmentProgramParameter(cg, "Ka");
	cgKd              = getFragmentProgramParameter(cg, "Kd");
	cgKs              = getFragmentProgramParameter(cg, "Ks");
	cgShininess       = getFragmentProgramParameter(cg, "shininess");
	cgkC              = getFragmentProgramParameter(cg, "kC");
	cgkL              = getFragmentProgramParameter(cg, "kL");
	cgkQ              = getFragmentProgramParameter(cg, "kQ");
}

void RenderMethod_PhongPoint_CG::renderPass(RENDER_PASS pass) {
	switch (pass) {
	case OPAQUE_PASS:
		pass_opaque();
		break;
	default:
		return;
	}
}

void RenderMethod_PhongPoint_CG::pass_opaque() {
	CHECK_GL_ERROR();
	
	// Setup state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4fv(white);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	
	// Setup client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	// Render the geometry chunks
	renderChunks();
	
	// Restore client state
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	// Restore settings
	glPopAttrib();
	
	CHECK_GL_ERROR();
}

void RenderMethod_PhongPoint_CG::setShaderData(const GeometryChunk &gc) {
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
	
	// Set the light properties
	cgGLSetParameter1fv(cgkC, &light0.kC);
	cgGLSetParameter1fv(cgkL, &light0.kL);
	cgGLSetParameter1fv(cgkQ, &light0.kQ);
	
	// Set the material properties
	cgGLSetParameter4fv(cgKa, gc.material.Ka);
	cgGLSetParameter4fv(cgKd, gc.material.Kd);
	cgGLSetParameter4fv(cgKs, gc.material.Ks);
	cgGLSetParameter1fv(cgShininess, &gc.material.shininess);
	
	// Everything else
	cgGLSetStateMatrixParameter(cgMVP, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(cgView, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}
