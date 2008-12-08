#include "stdafx.h"
#include "Mesh.h"
#include "ModelLoaderMD2.h"

const VEC3 ModelLoaderMD2::anorms_table[162] = {
#include "anorms.h"
};

const S32 ModelLoaderMD2::MD2_MAX_TRIANGLES = 4096;
const S32 ModelLoaderMD2::MD2_MAX_VERTS = 2048;
const S32 ModelLoaderMD2::MD2_MAX_TEX_VERTS = 2048;
const S32 ModelLoaderMD2::MD2_MAX_FRAMES = 512;
const S32 ModelLoaderMD2::MD2_MAX_SKINS = 32;

ModelLoaderMD2::ModelLoaderMD2() {
	// Do Nothing
}

vector<KeyFrame>
ModelLoaderMD2::loadKeyFrames(const FileName &fileName,
                              const FileName &skinName,
                              TextureFactory &textureFactory) const {
	FILE *stream=0;
	
//	fopen_s(&stream, fileName.c_str(), "rb");
	stream = fopen(fileName.c_str(), "rb");
	
	VERIFY(stream && !ferror(stream), "MD2 failed to open: "+fileName.str());
	
	Header header = readHeader(stream);
	Skin *skins = readSkins(stream, header);
	TexCoord *texCoords = readTexCoords(stream, header);
	Triangle *triangles = readTriangles(stream, header);
	Frame *frames = readFrames(stream, header);
	
	fclose(stream);
	
	Material skin;
	
	if (header.numOfSkins>0) {
		skin.setTexture(textureFactory.load(FileName((const char*)skins[0].name)));
	} else {
		skin.setTexture(textureFactory.load(skinName));
	}
	
	vector<KeyFrame> keyFrames = buildKeyFrames(fileName,
	                             skin,
	                             header,
	                             texCoords,
	                             triangles,
	                             frames);
	                             
	delete [] skins;
	delete [] texCoords;
	delete [] triangles;
	delete [] frames;
	
	return keyFrames;
}

vector<KeyFrame> ModelLoaderMD2::buildKeyFrames(const FileName &fileName,
  const Material &skin,
  const Header &header,
  TexCoord *texCoords,
  Triangle *triangles,
  Frame *frames) {
  
	vector<KeyFrame> keyFrames;
	
	float heightBelowGround=0;
	
	// Take the all the frames and push each frame into the mesh manager
	for (int i=0; i<header.numOfFrames; ++i) {
		string name = fileName.str() + "#" + itos(i);
		
		Mesh *mesh = buildKeyFrame(skin,
		                           header,
		                           texCoords,
		                           triangles,
		                           frames[i]);
		                           
		if (i==0) {
			heightBelowGround = mesh->calculateHeightBelowGround();
		}
		
		// Place the feet of the mesh on the ground
		vec3 *vertices = (vec3*)mesh->vertexArray->lock();
		for (int i=0; i<mesh->vertexArray->getNumber(); ++i) {
			vertices[i].y -= heightBelowGround;
		}
		mesh->vertexArray->unlock();
		
		keyFrames.push_back(KeyFrame(mesh));
	}
	
	return keyFrames;
}

Mesh* ModelLoaderMD2::buildKeyFrame(const Material &skin,
                                    const Header &header,
                                    TexCoord *texCoords,
                                    Triangle *triangles,
                                    const Frame &frame) {
	const vector<vec3> verticesArray = getVertices(frame.vertices, header.numOfVertices, frame.scale, frame.translate);
	const vector<vec3> normalsArray = getNormals(frame.vertices, header.numOfVertices);
	const vector<vec2> texcoordsArray = getTexCoords(texCoords, header.numOfSt, header.skinWidth, header.skinHeight);
	const vector<Face> facesArray = getFaces(triangles, header.numOfTris);
	
	Mesh *mesh = new Mesh(verticesArray, normalsArray, texcoordsArray, facesArray);
	
	mesh->material = skin;
	
	return mesh;
}

vec3 ModelLoaderMD2::transformVertex(const Vertex &v,
                                     VEC3 *scale,
                                     VEC3 *translate) {
	static mat3 rotX = mat3::fromRotateX((float)M_PI / 2.0f);
	static mat3 rotY = mat3::fromRotateY((float)M_PI / 2.0f);
	
	return (rotY * rotX).transformVector(vec3(v.x*scale->x + translate->x,
	                                     v.y*scale->y + translate->y,
	                                     v.z*scale->z + translate->z));
}

vec3 ModelLoaderMD2::transformNormal(const Vertex &v) {
	const VEC3 &n = anorms_table[v.encodedNormal];
	return vec3(n.x, n.y, n.z);
}

vec2 ModelLoaderMD2::transformTexCoord(const TexCoord &uv, S32 w, S32 h) {
	return vec2((float)uv.s / w, (float)(uv.t) / -h);
}

Face ModelLoaderMD2::transformTriangle(const Triangle &triangle) {
	Face face;
	
	for (int i=0; i<3; ++i) {
		face.vertIndex[i] = triangle.indices[i];
		face.normalIndex[i] = triangle.indices[i];
		face.coordIndex[i] = triangle.st[i];
	}
	
	return face;
}

vector<vec3> ModelLoaderMD2::getVertices(const Vertex *v,
  const size_t count,
  VEC3 scale,
  VEC3 translate) {
	vector<vec3> result(count);
	//transform(v, v+count, result.begin(), bind(transformVertex, _1, &scale, &translate));
	for (size_t i=0; i<count; ++i) {
		result[i] = transformVertex(v[i], &scale, &translate);
	}
	return result;
}

vector<vec3> ModelLoaderMD2::getNormals(const Vertex *v,
                                        const size_t count) {
	vector<vec3> result(count);
	//transform(v, v+count, result.begin(), transformNormal);
	for (size_t i=0; i<count; ++i) {
		result[i] = transformNormal(v[i]);
	}
	return result;
}

vector<vec2> ModelLoaderMD2::getTexCoords(const TexCoord *uv,
  const size_t count,
  S32 w, S32 h) {
	vector<vec2> result(count);
	//transform(uv, uv+count, result.begin(), bind(transformTexCoord, _1, w, h));
	for (size_t i=0; i<count; ++i) {
		result[i] = transformTexCoord(uv[i], w, h);
	}
	return result;
}

vector<Face> ModelLoaderMD2::getFaces(const Triangle *tris,
                                      const size_t count) {
	vector<Face> r(count);
	//transform(tris, tris+count, r.begin(), transformTriangle);
	for (size_t i=0; i<count; ++i) {
		r[i] = transformTriangle(tris[i]);
	}
	return r;
}

ModelLoaderMD2::Header ModelLoaderMD2::readHeader(FILE* stream) {
	Header header;
	
	fread(&header, sizeof(header), 1, stream);
	
	ASSERT(header.magicNumber == (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I'),
	       "This file is not an MD2 file!");
	       
	ASSERT(header.version == 8, "Can only load version 8 MD2 models!");
	
	ASSERT(header.numOfFrames >= 0 && header.numOfFrames < MD2_MAX_FRAMES,
	       "Invalid number of frames: " + header.numOfFrames);
	       
	ASSERT(header.numOfSkins >= 0 && header.numOfSkins < MD2_MAX_SKINS,
	       "Invalid number of skins: " + header.numOfSkins);
	       
	ASSERT(header.numOfSt >= 0 && header.numOfSt < MD2_MAX_TEX_VERTS,
	       "Invalid number of tex-coords: " + header.numOfSt);
	       
	ASSERT(header.numOfTris >= 0 && header.numOfTris < MD2_MAX_TRIANGLES,
	       "Invalid number of triangles: " + header.numOfTris);
	       
	ASSERT(header.numOfVertices >= 0 && header.numOfVertices < MD2_MAX_VERTS,
	       "Invalid number of vertices: " + header.numOfVertices);
	       
	return header;
}

ModelLoaderMD2::Skin* ModelLoaderMD2::readSkins(FILE* stream,
  const Header &header) {
	if (header.numOfSkins==0)
		return 0;
		
	ASSERT(header.numOfSkins>=0 && header.numOfSkins<MD2_MAX_SKINS,
	       "Invalid number of skins");
	       
	Skin *skins = new Skin[header.numOfSkins];
	
	fseek(stream, header.offsetSkins, SEEK_SET);
	fread(skins, sizeof(Skin), header.numOfSkins, stream);
	
	return skins;
}

ModelLoaderMD2::TexCoord* ModelLoaderMD2::readTexCoords(FILE* stream,
  const Header &header) {
	ASSERT(header.numOfSt>0 && header.numOfSt<MD2_MAX_TEX_VERTS,
	       "Invalid number of tex-verts");
	       
	TexCoord *texCoords = new TexCoord[header.numOfSt];
	
	fseek(stream, header.offsetTexCoords, SEEK_SET);
	fread(texCoords, sizeof(TexCoord), header.numOfSt, stream);
	
	return texCoords;
}

ModelLoaderMD2::Triangle* ModelLoaderMD2::readTriangles(FILE* stream,
  const Header &header) {
	ASSERT(header.offsetTriangles>0 && header.offsetTriangles<MD2_MAX_TRIANGLES,
	       "Invalid number of triangles");
	       
	Triangle *triangles = new Triangle[header.numOfTris];
	
	fseek(stream, header.offsetTriangles, SEEK_SET);
	fread(triangles, sizeof(Triangle), header.numOfTris, stream);
	
	return triangles;
}

ModelLoaderMD2::Frame* ModelLoaderMD2::readFrames(FILE* stream,
  const Header &header) {
	ASSERT(header.numOfFrames>0 && header.numOfFrames<MD2_MAX_FRAMES,
	       "Invalid number of frames");
	       
	Frame *frames = new Frame[header.numOfFrames];
	
	fseek(stream, header.offsetFrames, SEEK_SET);
	
	for (S32 i=0; i<header.numOfFrames; ++i) {
		Frame &frame = frames[i];
		
		frame.vertices = new Vertex[header.numOfVertices];
		
		fread(&frame.scale, sizeof(VEC3), 1, stream);
		fread(&frame.translate, sizeof(VEC3), 1, stream);
		fread(&frame.name, sizeof(U8), 16, stream);
		fread(frame.vertices, sizeof(Vertex), header.numOfVertices, stream);
	}
	
	return frames;
}
