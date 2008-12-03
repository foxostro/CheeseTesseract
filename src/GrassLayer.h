#ifndef GRASS_LAYER_H
#define GRASS_LAYER_H

#include "RenderInstance.h"
#include "TextureFactory.h"
#include "Mesh.h"

/**
Grass layer of the terrain.
@todo Grass parameters are loaded from data
@todo Grass color modulation map over the terrain
@todo Grass size modulation map over the terrain
@todo Grass coverage map over the terrain
*/
class GrassLayer
{
private:
	/** Collection of grass quads  */
	struct GrassCell
	{
		/** Grass cells are square. This is the length on each side */
		float size;

		/** Position of the grass cell */
		vec2 center;

		/** Geometry of the grass cell*/
		RenderInstance renderInstance;
	};

	/** Information pertaining to a single grass quad */
	struct GrassTuft
	{
		vec3 position;
		float size;

		/** Normal of the terrain beneath the tuft*/
		vec3 terrainNormal;

		/** Texture atlas index */
		int type; 
	};

	vec2 spacing;
	float displacement;
	float size_mu;
	float size_sigma;
	float size_constant_offset;
	float size_multiplier;
	Material grassMaterial;
	bool useShader;

	int numCellColumns;
	int numCellRows;
	GrassCell *cells;

public:
	/** Destructor */
	~GrassLayer();

	/**
	Constructor
	@param renderer The grass layer may need to generate different geometry
	                based on the shader that the renderer has selected.
	@param textureFactory Loads textures for the grass layer
	*/
	GrassLayer(shared_ptr<class Renderer> renderer, TextureFactory &textureFactory);

	/**
	Generates grass elements and turns them into geometry.
	@param center Position of the grass layer
	@param size Length of the square grass layer on each side
	@param elevationFunc Given the coordinates (x,y) of a region within the
	                     bounding rectangle, return the elevation (z) of that
						 point and the normal at that point.
	*/
	void generate(const vec2 &center,
	              float size,
	              function<tuple<float,vec3>(vec2)> elevationFunc);

	/**
	Generates batches for the grass layer.
	Employs frustum culling to reduce batches
	*/
	void emitGeometry(vector<class ActionQueueRenderInstance> &batches) const;

private:
	/**
	Generates a single grass cell and associated geometry.
	@param center Position of the center of the grass cell
	@param size Length of the grass cell square on it side
	@param elevationFunc Given the coordinates (x,y) of a region within the
	                     bounding rectangle, return the elevation (z) of that
						 point and the normal at that point.
	*/
	void generateCell(struct GrassCell &cell,
	                  const vec2 &center,
	                  float size,
					  function<tuple<float,vec3>(vec2)> elevationFunc);

	/**
	Generates geometry from grass elements.
	@param elements Grass elements in the grass layer
	*/
	shared_ptr<Mesh> generateMesh(const vector<GrassTuft> &elements);

	/**
	Generates grass elements.
	@param bounds Grass will be generated within in a rectangular region
	              parallel to the XY plane with corners here and at (0,0)
	@param z Given the coordinates (x,y) of a region within the
	         bounding rectangle, return the elevation (z) of that
	         point.
	@param n Given the coordinates (x,y) of a region within the
			 bounding rectangle, return the terrain normal of that point.
	@param spacing Tufts will be placed on a grid with this spacing
	@param displacement Tufts will be displaced from the standard grid by a
	                    random offset vector having a Gaussian distribution
						in magnitude.  This is the standard deviation of that
						offset.
	*/
	vector<GrassTuft>
	generateTufts(const vec2 &bounds_min,
	              const vec2 &bounds_max,
				  function<tuple<float,vec3>(vec2)> zn);

	float generateTuftSize();

	void addQuad(const GrassTuft &tuft,
	             int &idx,
				 float angle,
				 vector<vec3> &verticesArray,
				 vector<vec3> &normalsArray,
				 vector<vec2> &texcoordsArray,
				 vector<color> &colorsArray,
				 vector<Face> &facesArray);
};

#endif
