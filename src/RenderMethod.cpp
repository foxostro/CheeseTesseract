#include "stdafx.h"
#include "RenderMethod.h"

RenderMethod::RenderMethod()
{
	useCG = false;
}

RenderMethod::~RenderMethod()
{
	cgDestroyProgram(fragment_program);
	cgDestroyProgram(vertex_program);
}

void RenderMethod::setupScene()
{
	bucket.clear();
}

void RenderMethod::acceptgc(const GeometryChunk &gc)
{
	bucket.push_back(gc);
}

void RenderMethod::renderChunk(const GeometryChunk &gc)
{
	CHECK_GL_ERROR();

	glPushMatrix();
	glMultMatrixf(gc.transformation);

	// Bind material settings
	gc.material.bind();
	setShaderData(gc);

	glFrontFace(gc.polygonWinding);

	// Bind vertex arrays
	gc.texCoordArray->bind();
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	gc.normalArray->bind();
	glNormalPointer(GL_FLOAT, 0, 0);

	gc.vertexArray->bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	gc.colorsArray->bind();
	glColorPointer(4, GL_FLOAT, 0, 0);

	// And actually render the batch using the element buffer object's indices
	GLsizei count = gc.indexArray->getNumber();
	gc.indexArray->bind();
	glDrawElements(gc.primitiveMode, count, MESH_INDEX_FORMAT, 0);

	glPopMatrix();

	FLUSH_GL_ERROR();
}

void RenderMethod::renderChunks()
{
	if(useCG)
	{		
		cgGLBindProgram(vertex_program);
		cgGLBindProgram(fragment_program);

		cgGLEnableProfile(cgVertexProfile);
		cgGLEnableProfile(cgFragmentProfile);
	}

	for(vector<GeometryChunk>::const_iterator i = bucket.begin();
		i != bucket.end(); ++i)
	{
		renderChunk(*i);
	}

	if(useCG)
	{		
		cgGLDisableProfile(cgVertexProfile);
		cgGLDisableProfile(cgFragmentProfile);

		cgGLUnbindProgram(cgFragmentProfile);
		cgGLUnbindProgram(cgVertexProfile);
	}
}

void RenderMethod::checkForCgError( CGcontext cg, const string &situation )
{
	CGerror error;

	const char *errstr= cgGetLastErrorString(&error);

	if(error != CG_NO_ERROR)
	{
		string message = "ERROR: " + situation + ": " + errstr;

		if (error == CG_COMPILER_ERROR)
		{
			message = message + "\n" + cgGetLastListing(cg);
		}

		FAIL(message);
	}
}

void RenderMethod::setupShader( CGcontext &cg, CGprofile &cgVertexProfile, CGprofile &cgFragmentProfile )
{
	this->cgVertexProfile = cgVertexProfile;
	this->cgFragmentProfile = cgFragmentProfile;
}

void RenderMethod::createVertexProgram( CGcontext & cg, const FileName &vp )
{
	TRACE(string("Creating vertex program: ") + vp.str());
	vertex_program = cgCreateProgramFromFile(cg, CG_SOURCE, vp.c_str(), cgVertexProfile, "main", NULL);
	checkForCgError(cg, "Create vertex program (" + vp.str() + ")");
	cgGLLoadProgram(vertex_program);
}

void RenderMethod::createFragmentProgram( CGcontext & cg, const FileName &fp )
{
	TRACE(string("Creating fragment program: ") + fp.str());
	fragment_program = cgCreateProgramFromFile(cg, CG_SOURCE, fp.c_str(), cgFragmentProfile, "main", NULL);
	checkForCgError(cg, "Create fragment program (" + fp.str() + ")");
	cgGLLoadProgram(fragment_program);
}

CGparameter RenderMethod::getVertexProgramParameter( CGcontext &cg, const char *name )
{
	CGparameter param = cgGetNamedParameter(vertex_program, name);
	checkForCgError(cg, string("Get vp parameter (") + name + ")");
	return param;
}

CGparameter RenderMethod::getFragmentProgramParameter( CGcontext &cg, const char *name )
{
	CGparameter param = cgGetNamedParameter(fragment_program, name);
	checkForCgError(cg, string("Get fp parameter (") + name + ")");
	return param;
}

bool RenderMethod::areShadersAvailable()
{
	return glewIsSupported("GL_ARB_vertex_program GL_ARB_fragment_program")!=0;
}

mat4 RenderMethod::getView()
{
	mat4 View;
	glGetFloatv(GL_MODELVIEW_MATRIX, View);
	return View;
}

mat4 RenderMethod::getProj()
{
	mat4 Proj;
	glGetFloatv(GL_PROJECTION_MATRIX, Proj);
	return Proj;
}

mat4 RenderMethod::getViewI()
{
	mat4 View, ViewI;
	glGetFloatv(GL_MODELVIEW_MATRIX, View);
	ViewI = View.transpose();
	return ViewI;
}
