#include "stdafx.h"
#include "Mesh.h"
#include "ModelLoaderMD3.h"

const S32 ModelLoaderMD3::MD3_MAX_FRAMES = 1024;
const S32 ModelLoaderMD3::MD3_MAX_SURFACES = 16;
const S32 ModelLoaderMD3::MD3_MAX_TAGS = 32;
const S32 ModelLoaderMD3::MD3_MAX_SHADERS = 256;
const S32 ModelLoaderMD3::MD3_MAX_VERTS = 4096;
const S32 ModelLoaderMD3::MD3_MAX_TRIANGLES = 8192;
const float ModelLoaderMD3::MD3_XYZ_SCALE = 1.0f / 64.0f;

ModelLoaderMD3::ModelLoaderMD3() {}

vector<KeyFrame>
ModelLoaderMD3::loadKeyFrames(const FileName &fileName,
                              const FileName &skinName,
                              TextureFactory &textureFactory) const {
	FILE *stream=0;
	
//	fopen_s(&stream, fileName.c_str(), "rb");
	stream = fopen(fileName.c_str(), "rb");
	
	VERIFY(stream && !ferror(stream), "MD3 failed to open: "+fileName.str());
	
	Header header = readHeader(stream);
	
	Frame *frames = readFrames(stream, header);
	
	Tag *tags = readTags(stream, header);
	
	Surface *surfaces = readSurfaces(stream, header);
	
	fclose(stream);
	
	vector<KeyFrame> keyFrames = buildKeyFrame(surfaces,
	                             fileName,
	                             skinName,
	                             header,
	                             textureFactory);
	                             
	delete [] frames;
	delete [] tags;
	delete [] surfaces;
	
	return keyFrames;
}

vector<KeyFrame> ModelLoaderMD3::buildKeyFrame(Surface * surfaces,
  const FileName &fileName,
  const FileName &skinName,
  const Header &header,
  TextureFactory &textureFactory) {
	vector<KeyFrame> keyFrames;
	
	const FileName directory = fileName.getPath();
	
	/*
	Only the first frame loads texture files.
	The other frames use a copy of the handles and that copy is stored in here.
	*/
	Material md3Material;
	
	// Take the all the surfaces and push each frame into the mesh manager
	for (int i=0; i<surfaces[0].header.numFrames; ++i) {
		string name = fileName.str() + "#" + itos(i);
		
		// Create a mesh from the surface
		Mesh *mesh = surfaces[0].getObject(i);
		
		// Load a material for the first mesh in the first model.
		// First model only! Ignored for subsequent models
		if (i==0) {
			if (header.numSurfaces > 0) {
				mesh->material.clear();
				
#if 0
				if (surfaces[0].header.numShaders > 0) {
					const char *shaderName=(char*)surfaces[0].shaders[0].name;
					FileName skin = directory.append(FileName(shaderName));
					mesh->material.setTexture(textureFactory.load(skin));
				} else {
					mesh->material.setTexture(textureFactory.load(skinName));
				}
#else
				mesh->material.setTexture(textureFactory.load(skinName));
#endif
				// Keep a copy of the material to propagate to the subsequent frames
				md3Material = mesh->material;
			}
		} else {
			mesh->material = md3Material; // shallow copy
		}
		
		keyFrames.push_back(KeyFrame(mesh));
	}
	
	return keyFrames;
}

ModelLoaderMD3::Surface* ModelLoaderMD3::readSurfaces(FILE* stream,
  const Header &header) {
	Surface *surfaces = 0;
	
	if (header.numSurfaces>0) {
		surfaces = new Surface[header.numSurfaces];
	}
	
	fseek(stream, header.surfacesOffset, SEEK_SET);
	
	for (int i=0; i<header.numSurfaces; ++i) {
		// Load the next surface
		surfaces[i].create(stream);
	}
	
	return surfaces;
}

ModelLoaderMD3::Tag* ModelLoaderMD3::readTags(FILE* stream,
  const Header &header) {
	Tag *tags = 0;
	
	if (header.numTags>0) {
		tags = new Tag[header.numTags];
	}
	
	fseek(stream, header.tagsOffset, SEEK_SET);
	fread(tags, sizeof(Tag), header.numTags, stream);
	
	return tags;
}

ModelLoaderMD3::Frame* ModelLoaderMD3::readFrames(FILE* stream,
  const Header &header) {
	ASSERT(header.numFrames>0 && header.numFrames<MD3_MAX_FRAMES,
	       "Invalid number of frames");
	       
	Frame *frames = new Frame[header.numFrames];
	
	fseek(stream, header.framesOffset, SEEK_SET);
	fread(frames, sizeof(Frame), header.numFrames, stream);
	
	return frames;
}

ModelLoaderMD3::Header ModelLoaderMD3::readHeader(FILE* stream) {
	Header header;
	
	fread(&header, sizeof(header), 1, stream);
	
	ASSERT(header.magicNumber[0] == 'I' &&
	       header.magicNumber[1] == 'D' &&
	       header.magicNumber[2] == 'P' &&
	       header.magicNumber[3] == '3',
	       "This file is not an MD3 file");
	       
	return header;
}

ModelLoaderMD3::Surface::Surface() {
	shaders = 0;
	triangles = 0;
	texCoords = 0;
	vertices = 0;
}

void ModelLoaderMD3::Surface::create(FILE *stream) {
	const S32 surfaceStart = (S32)ftell(stream);
	
	delete [] shaders;
	shaders = 0;
	
	delete [] triangles;
	triangles = 0;
	
	delete [] texCoords;
	texCoords = 0;
	
	delete [] vertices;
	vertices = 0;
	
	fread(&header, sizeof(header), 1, stream);
	
	ASSERT(header.magicNumber[0] == 'I' &&
	       header.magicNumber[1] == 'D' &&
	       header.magicNumber[2] == 'P' &&
	       header.magicNumber[3] == '3',
	       "This file is a corrupted MD3 file.");
	       
	if (header.numShaders>0) {
		shaders = new Shader[header.numShaders];
		fseek(stream, surfaceStart + header.shadersOffset, SEEK_SET);
		fread(shaders, sizeof(Shader), header.numShaders, stream);
	}
	
	if (header.numTris>0) {
		triangles = new Triangle[header.numTris];
		fseek(stream, surfaceStart + header.trianglesOffset, SEEK_SET);
		fread(triangles, sizeof(Triangle), header.numTris, stream);
	}
	
	if (header.numVerts>0) {
		texCoords = new TexCoord[header.numVerts];
		fseek(stream, surfaceStart + header.texCoordsOffset, SEEK_SET);
		fread(texCoords, sizeof(TexCoord), header.numVerts, stream);
	}
	
	if (header.numVerts>0 && header.numFrames>0) {
		const S32 totalNumberOfVertices = header.numVerts * header.numFrames;
		vertices = new Vertex[totalNumberOfVertices];
		fseek(stream, surfaceStart + header.vertexOffset, SEEK_SET);
		fread(vertices, sizeof(Vertex), totalNumberOfVertices, stream);
	}
}

ModelLoaderMD3::Surface::~Surface() {
	delete[] shaders;
	delete[] triangles;
	delete[] texCoords;
	delete[] vertices;
}

vec3 ModelLoaderMD3::Surface::transformVertex(const Vertex &v) {
	static const mat4 rot = mat4::fromRotateZ((float)M_PI / 2.0f);
	
	return rot.transformVector(vec3(v.x, v.y, v.z) * MD3_XYZ_SCALE);
}

vec3 ModelLoaderMD3::Surface::transformNormal(const Vertex &v) {
	const float normScale = (float)(2.0 * M_PI) / 255.0f;
	const float lat = ((v.encodedNormal >> 8) & 255) * normScale;
	const float lng = (v.encodedNormal & 255) * normScale;
	
	return vec3(cosf(lat) * sinf(lng),
	            sinf(lat) * sinf(lng),
	            cosf(lng));
}

vec2 ModelLoaderMD3::Surface::transformTexCoord(const TexCoord &uv) {
	return vec2(uv.st[0], -uv.st[1]);
}

Face ModelLoaderMD3::Surface::transformTriangle(const Triangle &triangle) {
	Face face;
	
	for (int i=0; i<3; ++i) {
		face.vertIndex[i] = triangle.indices[i];
		face.normalIndex[i] = triangle.indices[i];
		face.coordIndex[i] = triangle.indices[i];
	}
	
	return face;
}

vector<vec3> ModelLoaderMD3::Surface::getVertices(const Vertex *v,
  size_t count) {
	vector<vec3> result(count);
	
	for (size_t i=0; i<count; ++i) {
		result[i] = transformVertex(v[i]);
	}
	return result;
}

vector<vec3> ModelLoaderMD3::Surface::getNormals(const Vertex *v,
  size_t count) {
	vector<vec3> result(count);
	
	for (size_t i=0; i<count; ++i) {
		result[i] = transformNormal(v[i]);
	}
	return result;
}

vector<vec2> ModelLoaderMD3::Surface::getTexCoords(const TexCoord *uv,
  size_t count) {
	vector<vec2> result(count);
	
	for (size_t i=0; i<count; ++i) {
		result[i] = transformTexCoord(uv[i]);
	}
	return result;
}

vector<Face> ModelLoaderMD3::Surface::getFaces(const Triangle *tris,
  size_t count) {
	vector<Face> r(count);
	
	for (size_t i=0; i<count; ++i) {
		r[i] = transformTriangle(tris[i]);
	}
	return r;
}

Mesh* ModelLoaderMD3::Surface::getObject(size_t frame) const {
	const vector<vec3> verticesArray = getVertices(vertices + frame*header.numVerts, header.numVerts);
	const vector<vec3> normalsArray = getNormals(vertices + frame*header.numVerts, header.numVerts);
	const vector<vec2> texcoordsArray = getTexCoords(texCoords, header.numVerts);
	const vector<Face> facesArray = getFaces(triangles, header.numTris);
	
	return new Mesh(verticesArray, normalsArray, texcoordsArray, facesArray);
}
