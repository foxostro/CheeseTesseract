#include "stdafx.h"
#include "Material.h"
#include "FileFuncs.h"
#include "Mesh.h"
#include "PhysicsEngine.h"
#include "devil_wrapper.h"
#include "ActionQueueRenderInstance.h"
#include "GrassLayer.h"
#include "TreeLayer.h"
#include "Terrain.h"
#include "RenderMethodTags.h"

dReal heightfieldCallback(void *_heightmapData, int y, int x) {
	ASSERT(_heightmapData, "Parameter \"_heightmapData\" was null");
	
	const HeightMapData &heightmapData = *reinterpret_cast<HeightMapData*>(_heightmapData);
	
	const int w = heightmapData.w;
	const float *heightMap = heightmapData.heightmap;
	
	ASSERT(heightMap, "\"heightMap\" was null");
	
	return heightMap[(y*w)+x];
}

Terrain::~Terrain() {
	delete [] heightmap.heightmap;
}

Terrain::Terrain(UID uid, ScopedEventHandler *parentScope)
		: geom(0),
		ScopedEventHandlerSubscriber(uid, parentScope) {
	clear();
}

void Terrain::clear() {
	if (geom) {
		dGeomDestroy(geom);
		geom=0;
	}
	
	mesh.reset();
	grassLayer.reset();
	treeLayer.reset();
	
	scaleXY=1.0f;
	scaleZ=1.0f;
	enableGrass=true;
	enableTrees=true;
}

HeightMapData Terrain::loadHeightMap(const FileName &fileName) {
	CHECK_GL_ERROR();
	
	unsigned int imageName = devil_loadImage(fileName);
	
	// Get image data
	unsigned char *imgData = ilGetData();
	int imgWidth		   = ilGetInteger(IL_IMAGE_WIDTH);
	int imgHeight		   = ilGetInteger(IL_IMAGE_HEIGHT);
	int bytesPerPixel	   = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	
	ASSERT(imgData!=0,       "Failed to load heightmap: " + fileName.str());
	ASSERT(imgWidth == imgHeight, "Heightmap is not square!");
	
	const size_t rowSize = imgWidth * bytesPerPixel;
	const size_t imageSize = imgHeight * rowSize;
	
	float *heightmap = new float[imgWidth * imgHeight];
	
	for (size_t y = 0; y < (size_t)(imgHeight); ++y) {
		unsigned char *row = imgData + (y*rowSize);
		
		for (size_t x = 0; x < rowSize; x += bytesPerPixel) {
			const unsigned char &red = row[x];
			
			// Height information is stored in the RED component
			heightmap[y*imgWidth + x] = red / 255.0f;
		}
	}
	
	// return
	HeightMapData data;
	data.heightmap = heightmap;
	data.w = imgWidth;
	data.h = imgHeight;
	return data;
}

void Terrain::generateVertices(vector<vec3> &verticesArray,
                               vector<vec3> &normalsArray,
                               vector<vec2> &texcoordsArray,
                               const int heightmapSize,
                               const float *heightmap,
                               const float scaleXY,
                               const float scaleZ) {
	const int numVertices = heightmapSize * heightmapSize;
	
	verticesArray.resize(numVertices);
	normalsArray.resize(numVertices);
	texcoordsArray.resize(numVertices);
	
	for (int y=0; y<heightmapSize; ++y) {
		for (int x=0; x<heightmapSize; ++x) {
			const size_t idx = y*heightmapSize + x;
			
			verticesArray[idx] = vec3((float)x*scaleXY,
			                          (float)y*scaleXY,
			                          heightmap[idx]*scaleZ);
			                          
			normalsArray[idx] = vec3(0,0,1); // fake
			
			texcoordsArray[idx] = vec2((float)x, (float)y);
		}
	}
	
	// Generate face normals (except for edge vertices, don't care about them)
	for (int y=1; y<heightmapSize-1; ++y) {
		for (int x=1; x<heightmapSize-1; ++x) {
			Triangle t1 = {verticesArray[(y+0)*heightmapSize + (x+0)],
			               verticesArray[(y+1)*heightmapSize + (x-1)],
			               verticesArray[(y+0)*heightmapSize + (x-1)]
			              };
			              
			Triangle t2 = {verticesArray[(y+0)*heightmapSize + (x+0)],
			               verticesArray[(y+1)*heightmapSize + (x+0)],
			               verticesArray[(y+1)*heightmapSize + (x-1)]
			              };
			              
			Triangle t3 = {verticesArray[(y+0)*heightmapSize + (x+0)],
			               verticesArray[(y+0)*heightmapSize + (x-1)],
			               verticesArray[(y-1)*heightmapSize + (x+0)]
			              };
			              
			Triangle t4 = {verticesArray[(y+0)*heightmapSize + (x+0)],
			               verticesArray[(y+0)*heightmapSize + (x+1)],
			               verticesArray[(y+1)*heightmapSize + (x+0)]
			              };
			              
			Triangle t5 = {verticesArray[(y+0)*heightmapSize + (x+0)],
			               verticesArray[(y-1)*heightmapSize + (x+0)],
			               verticesArray[(y-1)*heightmapSize + (x+1)]
			              };
			              
			Triangle t6 = {verticesArray[(y+0)*heightmapSize + (x+0)],
			               verticesArray[(y-1)*heightmapSize + (x+1)],
			               verticesArray[(y+0)*heightmapSize + (x+1)]
			              };
			              
			vec3 n1 = calcTriNorm(t1);
			vec3 n2 = calcTriNorm(t2);
			vec3 n3 = calcTriNorm(t3);
			vec3 n4 = calcTriNorm(t4);
			vec3 n5 = calcTriNorm(t5);
			vec3 n6 = calcTriNorm(t6);
			
			// Take the smooth, averaged normal of adjacent faces
			vec3 n = (n1 + n2 + n3 + n4 + n5 + n6) * (1.0f/6.0f);
			
			normalsArray[y*heightmapSize + x] = n;
		}
	}
}

vector<Face> Terrain::generateIndices(const int heightmapSize) {
	const int numFaces = (heightmapSize-1) * (heightmapSize-1) * 2;
	vector<Face> facesArray;
	facesArray.resize(numFaces);
	int idx=0;
	for (int y=0; y<heightmapSize-1; ++y) {
		for (int x=0; x<heightmapSize-1; ++x) {
			addTerrainQuad(x, y, heightmapSize, facesArray, idx);
		}
	}
	
	return facesArray;
}

shared_ptr<Mesh> Terrain::generateGeometry(const int heightmapSize,
  const float *heightmap,
  TextureFactory &textureFactory,
  const FileName heightmapTex,
  const float scaleXY,
  const float scaleZ) {
	vector<vec3> verticesArray;
	vector<vec3> normalsArray;
	vector<vec2> texcoordsArray;
	
	// Generate geometry
	generateVertices(verticesArray,
	                 normalsArray,
	                 texcoordsArray,
	                 heightmapSize,
	                 heightmap,
	                 scaleXY,
	                 scaleZ);
	                 
	vector<Face> facesArray = generateIndices(heightmapSize);
	
	shared_ptr<Mesh> mesh(new Mesh(verticesArray,
	                               normalsArray,
	                               texcoordsArray,
	                               facesArray,
	                               true)); // completely static mesh
	                               
	mesh->material = Material(textureFactory.load(heightmapTex));
	mesh->material.shininess = 64.0f;
	mesh->polygonWinding = GL_CCW;
	
	return mesh;
}

void Terrain::create(const PropertyBag &data,
                     shared_ptr<class Renderer> renderer,
                     TextureFactory &textureFactory,
                     shared_ptr<PhysicsEngine> _physicsEngine) {
	ASSERT(_physicsEngine, "Physics engine was null");
	physicsEngine = _physicsEngine;
	
	scaleXY = data.getFloat("scaleXY");
	scaleZ = data.getFloat("scaleZ");
	
	const FileName heightmapFile = data.getFileName("heightmap");
	const FileName heightmapTex = data.getFileName("material");
	const FileName grassMaterial = data.getFileName("grassMaterial");
	
	// Loads heightmap
	this->heightmap = loadHeightMap(heightmapFile);
	
	// Generate heightmap geometry
	mesh = generateGeometry(heightmap.w,
	                        heightmap.heightmap,
	                        textureFactory,
	                        heightmapTex,
	                        scaleXY, scaleZ);
	                        
	// Send heightmap to physics engine
	{
		tuple<dGeomID,dTriMeshDataID> result = mesh->createGeom(physicsEngine->getSpace());
		geom = result.get<0>();
		dGeomSetPosition(geom, 0.0f, 0.0f, 0.0f);
	}
	
	// Prepare a static render instance for future rendering
	{
		mesh->getGeometryChunk(renderInstance.gc);
		renderInstance.gc.transformation.identity();
		// Stick with default render method and material settings
	}
	
	generateGrassLayer(renderer, textureFactory);
	generateTreeLayer();
}

void Terrain::emitGeometry() {
	ActionQueueRenderInstance action1(renderInstance);
	sendGlobalAction(&action1);
	
	if (enableGrass) {
		emitGrassGeometry();
	}
	
	if (enableTrees) {
		emitTreeGeometry();
	}
}

tuple<float,vec3> Terrain::getElevation(const HeightMapData &heightmap,
                                        vec2 p) {
	// Ray cast to get all contact points with the terrain
	float rayLength = 100.0f;
	vec3 rayOrigin = vec3(p.x, p.y, rayLength);
	vec3 rayDirection = vec3(0,0,-1);
	
	dContactGeom contact;
	if (physicsEngine->rayCast(rayOrigin, rayDirection, rayLength, geom, contact)) {
		// Pack the results
		float z = (float)contact.pos[2];
		vec3 n = vec3((float)contact.normal[0],
		              (float)contact.normal[1],
		              (float)contact.normal[2]);
		              
		return make_tuple(z,n);
	} else {
		float z = 0.0f;
		vec3 n = vec3(0.0f, 0.0f, 1.0f);
		return make_tuple(z,n);
	}
}

void Terrain::addTerrainQuad(int x, int y,
                             int heightmapSize,
                             vector<Face> &facesArray,
                             int &idx) {
	Face a;
	a.vertIndex[0] = a.coordIndex[0] = a.normalIndex[0] = (y+0)*heightmapSize + (x+0);
	a.vertIndex[1] = a.coordIndex[1] = a.normalIndex[1] = (y+0)*heightmapSize + (x+1);
	a.vertIndex[2] = a.coordIndex[2] = a.normalIndex[2] = (y+1)*heightmapSize + (x+0);
	facesArray[idx++] = a;
	
	Face b;
	b.vertIndex[0] = b.coordIndex[0] = b.normalIndex[0] = (y+0)*heightmapSize + (x+1);
	b.vertIndex[1] = b.coordIndex[1] = b.normalIndex[1] = (y+1)*heightmapSize + (x+1);
	b.vertIndex[2] = b.coordIndex[2] = b.normalIndex[2] = (y+1)*heightmapSize + (x+0);
	facesArray[idx++] = b;
}

void Terrain::emitGrassGeometry() {
	vector<ActionQueueRenderInstance> batches;
	vector<ActionQueueRenderInstance>::const_iterator i;
	
	ASSERT(grassLayer, "Null pointer: grassLayer");
	
	// Get the grass batches
	grassLayer->emitGeometry(batches);
	
	// Send batches to the renderer
	for (i=batches.begin(); i!=batches.end(); ++i) {
		const ActionQueueRenderInstance &action = *i;
		sendGlobalAction(&action);
	}
}

void Terrain::emitTreeGeometry() {
	vector<ActionQueueTreeForRender> actions;
	vector<ActionQueueTreeForRender>::const_iterator i;
	
	ASSERT(treeLayer, "Null pointer: treeLayer");
	
	// Get the tree instance data
	treeLayer->queueForRender(actions);
	
	// Send trees to the renderer
	for (i=actions.begin(); i!=actions.end(); ++i) {
		const ActionQueueTreeForRender &action = *i;
		sendGlobalAction(&action);
	}
}

void Terrain::generateTreeLayer() {
	treeLayer = shared_ptr<TreeLayer>(new TreeLayer());
	vec2 center = vec2(heightmap.w, heightmap.h) * (scaleXY / 2);
	float size = heightmap.h * scaleXY;
	treeLayer->generate(center, size,
	                    bind(&Terrain::getElevation, this,
	                         heightmap,
	                         _1));
}

void Terrain::generateGrassLayer(shared_ptr<class Renderer> renderer,
                                 TextureFactory & textureFactory) {
	grassLayer = shared_ptr<GrassLayer>(new GrassLayer(renderer, textureFactory));
	vec2 center = vec2(heightmap.w, heightmap.h) * (scaleXY / 2);
	float size = heightmap.h * scaleXY;
	grassLayer->generate(center, size,
	                     bind(&Terrain::getElevation, this,
	                          heightmap,
	                          _1));
}
