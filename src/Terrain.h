#ifndef _MAP_H_
#define _MAP_H_

#include "PropertyBag.h"
#include "Material.h"
#include "Mesh.h"
#include "ScopedEventHandlerSubscriber.h"
#include "RenderInstance.h"

class GrassLayer;
class TreeLayer;
class PhysicsEngine;

struct HeightMapData {
	float *heightmap;
	int w, h;
	
	HeightMapData() : heightmap(0), w(0), h(0) { /* Do Nothing */ }
};

/** Terrain */
class Terrain : public ScopedEventHandlerSubscriber {
private:
	shared_ptr<Mesh> mesh;
	dGeomID geom;
	HeightMapData heightmap;
	float scaleXY, scaleZ;
	RenderInstance renderInstance;
	shared_ptr<GrassLayer> grassLayer;
	shared_ptr<TreeLayer> treeLayer;
	bool enableGrass;
	bool enableTrees;
	shared_ptr<PhysicsEngine> physicsEngine; // keep this around to do ray casts with
	
public:
	~Terrain();
	
	Terrain(UID uid, ScopedEventHandler *parentScope);
	
	void clear();
	
	/**
	Creates a map from data
	@param data terrain data source
	@param textureFactory texture factory tracks loaded textures
	@param physicsEngine Physics engine
	*/
	void create(const PropertyBag &data,
	            shared_ptr<class Renderer> renderer,
	            TextureFactory &textureFactory,
	            shared_ptr<PhysicsEngine> physicsEngine);
	            
	/** emits a mesh to the renderer */
	void emitGeometry();
	
private:
	/** Emits geometry for the grass */
	void emitGrassGeometry();
	
	/** Emits geometry for the trees */
	void emitTreeGeometry();
	
	/**
	Loads a square heightmap from an image file
	@param fileName File name of the heightmap image
	@return tuple containing the heightmap buffer and the size of the square
	heightmap on a side. The length of the buffer is thus this size squared.
	*/
	HeightMapData loadHeightMap(const FileName &fileName);
	
	/** Generates geometry from heightmap data */
	static shared_ptr<Mesh> generateGeometry(const int heightmapSize,
	  const float *heightmap,
	  TextureFactory &textureFactory,
	  const FileName heightmapTex,
	  const float scaleXY,
	  const float scaleZ);
	  
	/** Generate vertices from heightmap data */
	static void generateVertices(vector<vec3> &verticesArray,
	                             vector<vec3> &normalsArray,
	                             vector<vec2> &texcoordsArray,
	                             const int heightmapSize,
	                             const float *heightmap,
	                             const float scaleXY,
	                             const float scaleZ);
	                             
	/** Generate indices for heightmap geometry */
	static vector<Face> generateIndices(const int heightmapSize);
	
	/** Adds two triangles the terrain making up a non-co-planar quad */
	static void addTerrainQuad(int x, int y,
	                           int heightmapSize,
	                           vector<Face> &facesArray,
	                           int &idx);
	                           
	void generateTreeLayer();
	
	void generateGrassLayer(shared_ptr<class Renderer> renderer,
	                        TextureFactory & textureFactory);
	                        
	/** Gets the elevation of the heightmap at point along the XY plane */
	tuple<float,vec3> getElevation(const HeightMapData &heightmap, vec2 p);
};

#endif
