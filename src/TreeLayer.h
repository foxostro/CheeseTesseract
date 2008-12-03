#ifndef TREE_LAYER_H
#define TREE_LAYER_H

#include "ActionQueueRenderInstance.h"

/** Tree layer of the terrain */
class TreeLayer
{
private:
	/** Information pertaining to a single grass quad */
	struct TreeData
	{
		/** Position of the tree */
		vec3 position;

		/** Seed for random selection from the pool of generated trees */
		int seed; 
	};

	/** Collection of grass quads  */
	struct TreeCell
	{
		/** Grass cells are square. This is the length on each side */
		float size;

		/** Position of the grass cell */
		vec2 center;

		/**	Trees contained within the cell */
		vector<TreeData> trees;
	};

	vec2 spacing;
	float displacement;

	int numCellColumns;
	int numCellRows;
	TreeCell *cells;

public:
	/** Destructor */
	~TreeLayer();

	/** Constructor */
	TreeLayer();

	/**
	Generates trees for the entire layer given a position within the
	2D projection of the map onto the XY plane.  The heighmap is consulted
	through the elevationFunc function to determine the height of the terrain
	at the point specified.
	@param center Position of the layer
	@param size Length of the entire layer on each side (its a square)
	@param elevationFunc Given the coordinates (x,y) of a region within the
	bounding rectangle, return the elevation (z) of that
	point and the normal at that point.
	*/
	void generate(const vec2 &center,
	              float size,
	              function<tuple<float,vec3>(vec2)> elevationFunc);

	/**
	Generates tree render requests.
	Employs frustum culling to reduce batches.
	*/
	void queueForRender(vector<ActionQueueTreeForRender> &actions) const;

private:
	/**
	Generates a single tree cell.
	@param center Position of the center of the grass cell
	@param size Length of the cell square on it side
	@param elevationFunc Given the coordinates (x,y) of a region within the
	bounding rectangle, return the elevation (z) of that
	point and the normal at that point.
	*/
	void generateCell(struct TreeCell &cell,
	                  const vec2 &center,
	                  float size,
	                  function<tuple<float,vec3>(vec2)> elevationFunc);
};

#endif
