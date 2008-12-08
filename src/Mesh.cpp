#include "stdafx.h"
#include "Mesh.h"

Mesh::~Mesh() {
	// Do Nothing
}

Mesh::Mesh()
		: vertexArray(new ResourceBufferVertices()),
		normalArray(new ResourceBufferVertices()),
		texCoordArray(new ResourceBufferTexCoords()),
		colorsArray(new ResourceBufferColors()),
		indexArray(new ResourceBufferIndices()),
		polygonWinding(GL_CW) {
	// Do Nothing
}

Mesh::Mesh(const Mesh *obj)
		: vertexArray(new ResourceBufferVertices()),
		normalArray(new ResourceBufferVertices()),
		texCoordArray(new ResourceBufferTexCoords()),
		colorsArray(new ResourceBufferColors()),
		indexArray(new ResourceBufferIndices()),
		polygonWinding(GL_CW) {
	copy(*obj);
}

Mesh::Mesh(const Mesh &obj) {
	copy(obj);
}

Mesh::Mesh(const vector<vec3> &verticesArray,
           const vector<vec3> &normalsArray,
           const vector<vec2> &texcoordsArray,
           const vector<Face> &facesArray,
           bool completelyStatic)
		: vertexArray(new ResourceBufferVertices()),
		normalArray(new ResourceBufferVertices()),
		texCoordArray(new ResourceBufferTexCoords()),
		colorsArray(new ResourceBufferColors()),
		indexArray(new ResourceBufferIndices()),
		polygonWinding(GL_CW) {
	ASSERT(verticesArray.size()>0,  "Too few vertices");
	ASSERT(normalsArray.size()>0,   "Too few normals");
	ASSERT(texcoordsArray.size()>0, "Too few tex-coords");
	ASSERT(facesArray.size()>0,     "Too few faces");
	
	const index_t numOfIndices = facesArray.size()*3;
	index_t el = 0;
	
	index_t *indices = new index_t[numOfIndices];
	vec3 *vertices = new vec3[numOfIndices];
	vec2 *texCoords = new vec2[numOfIndices];
	vec3 *normals = new vec3[numOfIndices];
	
	for (vector<Face>::const_iterator i=facesArray.begin();
	     i!=facesArray.end();
	     ++i) {
		const Face &face = *i;
		
		for (int j=0; j<3; ++j) {
			indices[el] = el;
			
			ASSERT(face.vertIndex[j] >= 0   && (size_t)face.vertIndex[j] < verticesArray.size(),   "Invalid vertex index: " + itos(face.vertIndex[j]));
			ASSERT(face.coordIndex[j] >= 0  && (size_t)face.coordIndex[j] < texcoordsArray.size(), "Invalid tex-coord index: " + itos(face.coordIndex[j]));
			ASSERT(face.normalIndex[j] >= 0 && (size_t)face.normalIndex[j] < normalsArray.size(),  "Invalid normal index: " + itos(face.normalIndex[j]));
			
			vertices[el] = verticesArray[face.vertIndex[j]];
			texCoords[el] = texcoordsArray[face.coordIndex[j]];
			normals[el] = normalsArray[face.normalIndex[j]];
			
			el++;
		}
	}
	
	if (completelyStatic) {
		vertexArray   -> recreate(numOfIndices, vertices,  STATIC_DRAW);
		normalArray   -> recreate(numOfIndices, normals,   STATIC_DRAW);
		texCoordArray -> recreate(numOfIndices, texCoords, STATIC_DRAW);
		indexArray    -> recreate(numOfIndices, indices,   STATIC_DRAW);
	} else {
		vertexArray   -> recreate(numOfIndices, vertices,  DYNAMIC_DRAW); // will be modified often in as mesh is animated
		normalArray   -> recreate(numOfIndices, normals,   DYNAMIC_DRAW); // will be modified often in as mesh is animated
		texCoordArray -> recreate(numOfIndices, texCoords, STATIC_DRAW);  // do not change as the mesh is animated
		indexArray    -> recreate(numOfIndices, indices,   STATIC_DRAW);  // do not change as the mesh is animated
	}
	
	delete [] indices;
	delete [] vertices;
	delete [] texCoords;
	delete [] normals;
}

Mesh::Mesh(const vector<vec3> &verticesArray,
           const vector<vec3> &normalsArray,
           const vector<vec2> &texcoordsArray,
           const vector<color> &colorsVector,
           const vector<Face> &facesArray,
           bool completelyStatic)
		: vertexArray(new ResourceBufferVertices()),
		normalArray(new ResourceBufferVertices()),
		texCoordArray(new ResourceBufferTexCoords()),
		colorsArray(new ResourceBufferColors()),
		indexArray(new ResourceBufferIndices()),
		polygonWinding(GL_CW) {
	ASSERT(verticesArray.size()>0,  "Too few vertices");
	ASSERT(normalsArray.size()>0,   "Too few normals");
	ASSERT(texcoordsArray.size()>0, "Too few tex-coords");
	ASSERT(colorsVector.size()>0,   "Too few colors");
	ASSERT(facesArray.size()>0,     "Too few faces");
	
	const index_t numOfIndices = facesArray.size()*3;
	index_t el = 0;
	
	index_t *indices = new index_t[numOfIndices];
	vec3 *vertices = new vec3[numOfIndices];
	vec2 *texCoords = new vec2[numOfIndices];
	color *colors = new color[numOfIndices];
	vec3 *normals = new vec3[numOfIndices];
	
	for (vector<Face>::const_iterator i=facesArray.begin();
	     i!=facesArray.end();
	     ++i) {
		const Face &face = *i;
		
		for (int j=0; j<3; ++j) {
			indices[el] = el;
			
			ASSERT(face.vertIndex[j] >= 0   && (size_t)face.vertIndex[j] < verticesArray.size(),   "Invalid vertex index: " + itos(face.vertIndex[j]));
			ASSERT(face.coordIndex[j] >= 0  && (size_t)face.coordIndex[j] < texcoordsArray.size(), "Invalid tex-coord index: " + itos(face.coordIndex[j]));
			ASSERT(face.normalIndex[j] >= 0 && (size_t)face.normalIndex[j] < normalsArray.size(),  "Invalid normal index: " + itos(face.normalIndex[j]));
			
			vertices[el] = verticesArray[face.vertIndex[j]];
			texCoords[el] = texcoordsArray[face.coordIndex[j]];
			normals[el] = normalsArray[face.normalIndex[j]];
			colors[el] = colorsVector[face.vertIndex[j]];
			
			el++;
		}
	}
	
	if (completelyStatic) {
		vertexArray   -> recreate(numOfIndices, vertices,  STATIC_DRAW);
		normalArray   -> recreate(numOfIndices, normals,   STATIC_DRAW);
		texCoordArray -> recreate(numOfIndices, texCoords, STATIC_DRAW);
		colorsArray   -> recreate(numOfIndices, colors,    STATIC_DRAW);
		indexArray    -> recreate(numOfIndices, indices,   STATIC_DRAW);
	} else {
		vertexArray   -> recreate(numOfIndices, vertices,  DYNAMIC_DRAW); // will be modified often in as mesh is animated
		normalArray   -> recreate(numOfIndices, normals,   DYNAMIC_DRAW); // will be modified often in as mesh is animated
		texCoordArray -> recreate(numOfIndices, texCoords, STATIC_DRAW);  // do not change as the mesh is animated
		colorsArray   -> recreate(numOfIndices, colors,    STATIC_DRAW);  // do not change as the mesh is animated
		indexArray    -> recreate(numOfIndices, indices,   STATIC_DRAW);  // do not change as the mesh is animated
	}
	
	delete [] indices;
	delete [] vertices;
	delete [] texCoords;
	delete [] colors;
	delete [] normals;
}

Mesh& Mesh::operator=(const Mesh &obj) {
	copy(obj);
	return(*this);
}

void Mesh::copy(const Mesh &mesh) {
	material = mesh.material;
	vertexArray = mesh.vertexArray->clone();
	normalArray = mesh.normalArray->clone();
	texCoordArray = mesh.texCoordArray->clone();
	colorsArray = mesh.colorsArray->clone();
	indexArray = mesh.indexArray->clone();
	polygonWinding = mesh.polygonWinding;
}

float Mesh::calculateRadius(RadiusCalculator radius) const {
	const index_t numOfVerts = vertexArray->getNumber();
	const vec3 *vertices = (const vec3*)vertexArray->read_lock();
	
	vec3 a(0,0,0);
	for (index_t i=0; i<numOfVerts; ++i) {
		vec3 b = vertices[i];
		a = (radius(a) > radius(b)) ? a : b;
	}
	
	vertexArray->unlock();
	
	return radius(a);
}

float Mesh::calculateHeight() const {
	const index_t numOfVerts = vertexArray->getNumber();
	const vec3 *vertices = (const vec3*)vertexArray->read_lock();
	
	float maxZ=0, minZ=0;
	
	for (index_t i=0; i<numOfVerts; ++i) {
		const float &z = vertices[i].z;
		maxZ = max(maxZ, z);
		minZ = min(minZ, z);
	}
	
	vertexArray->unlock();
	
	return maxZ - minZ;
}

float Mesh::calculateHeightBelowGround() const {
	const index_t numOfVerts = vertexArray->getNumber();
	const vec3 *vertices = (const vec3*)vertexArray->read_lock();
	
	float minZ=0;
	
	for (index_t i=0; i<numOfVerts; ++i) {
		minZ = min(minZ, vertices[i].z);
	}
	
	vertexArray->unlock();
	
	return min(minZ, 0.0f); // Clamp pos values to 0, pass through neg values
}

void Mesh::interpolate(float bias, const Mesh &a, const Mesh &b) {
	const index_t numOfElements = a.indexArray->getNumber();
	
	const index_t *elA = (const index_t *)a.indexArray->read_lock();
	const vec3 *vertsA = (const vec3 *)a.vertexArray->read_lock();
	const vec3 *normsA = (const vec3 *)a.normalArray->read_lock();
	
	const index_t *elB = (const index_t *)b.indexArray->read_lock();
	const vec3 *vertsB = (const vec3 *)b.vertexArray->read_lock();
	const vec3 *normsB = (const vec3 *)b.normalArray->read_lock();
	
	index_t *elements = (index_t *)indexArray->lock();
	vec3 *vertices = (vec3 *)vertexArray->lock();
	vec3 *normals = (vec3 *)normalArray->lock();
	
	for (index_t i=0; i<numOfElements; ++i) {
		const index_t &a = elA[i];
		const index_t &b = elB[i];
		
		// Make sure connectivity is the same as one of the key frames
		const index_t &x = elements[i] = a;
		
		const vec3 &vertexA = vertsA[a];
		const vec3 &normalA = normsA[a];
		
		const vec3 &vertexB = vertsB[b];
		const vec3 &normalB = normsB[b];
		
		vertices[x] = vertexA + (vertexB - vertexA)*bias;
		normals[x] = normalA + (normalB - normalA)*bias;
	}
	
	indexArray->unlock();
	vertexArray->unlock();
	normalArray->unlock();
	
	b.indexArray->unlock();
	b.vertexArray->unlock();
	b.normalArray->unlock();
	
	a.indexArray->unlock();
	a.vertexArray->unlock();
	a.normalArray->unlock();
}

tuple<dGeomID,dTriMeshDataID> Mesh::createGeom(dSpaceID physicsSpace)const {
	dVector3 *triVert = new dVector3[vertexArray->getNumber()];
	
	// Convert vertices to a format that ODE can accept
	{
		const vec3 *vertices = (const vec3 *)vertexArray->lock();
		for (index_t i=0; i < (index_t)vertexArray->getNumber(); ++i) {
			triVert[i][0] = vertices[i].x;
			triVert[i][1] = vertices[i].y;
			triVert[i][2] = vertices[i].z;
		}
		vertexArray->unlock();
	}
	
	int *indexes = new int[indexArray->getNumber()];
	
	// Convert indices to a format that ODE can accept
	{
		const index_t *indices = (const index_t*)indexArray->lock();
		for (int i=0; i<indexArray->getNumber(); ++i) {
			indexes[i] = (int)indices[i];
		}
		indexArray->unlock();
	}
	
	return createGeom(physicsSpace,
	                  (const dVector3*)triVert,
	                  vertexArray->getNumber(),
	                  (const int*)indexes,
	                  indexArray->getNumber());
}

tuple<dGeomID,dTriMeshDataID> Mesh::createGeom(dSpaceID physicsSpace,
  const dVector3 *vertices,
  int numVertices,
  const int *indices, // in ODE format
  int numIndices) {
	dTriMeshDataID meshID = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(meshID,
	                            (const dReal*)vertices,
	                            numVertices,
	                            indices,
	                            numIndices);
	dGeomID geom = dCreateTriMesh(physicsSpace, meshID, 0, 0, 0);
	dGeomSetPosition(geom, 0.0, 0.0, 0.0);
	return make_tuple(geom, meshID);
}

void Mesh::getGeometryChunk(GeometryChunk &gc) const {
	gc.vertexArray = vertexArray;
	gc.normalArray = normalArray;
	gc.texCoordArray = texCoordArray;
	gc.indexArray = indexArray;
	gc.colorsArray = colorsArray;
	gc.material = material;
	gc.polygonWinding = polygonWinding;
	gc.primitiveMode = GL_TRIANGLES;
}

void Mesh::uniformScale( float scale ) {
	const index_t numOfVerts = vertexArray->getNumber();
	vec3 *vertices = (vec3*)vertexArray->lock();
	
	for (index_t i=0; i<numOfVerts; ++i) {
		vertices[i] = vertices[i] * scale;
	}
	
	vertexArray->unlock();
}
