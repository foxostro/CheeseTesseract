#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "vec4.h"

/**
Bounding boxes are used to represent the
volume of an object and test for collisions
@author Andrew Fox
*/
class BoundingBox
{
public:
	/** Position of the box */
	vec3 m_Pos;

	/** Minimum vertex of the box relative to m_Pos */
	vec3 m_Min;

	/** Maximum vertex of the box relative to m_Pos*/
	vec3 m_Max;

	/*
	Determines whether a collision between two boxes has occurred
	@param box The bounding box to test against
	@return Returns true if this bounding box intersects the given bounding box, otherwise, returns false.
	*/
	bool Collision(BoundingBox &box);

	/**
	Determines whether the point is within the vertical projection of the box
	@param p The point to test
	@return Returns true if the point resides within the vertical projection of the box and false otherwise
	*/
	bool Collision(vec2 p)
	{
		return Collision(p.x, p.y);
	}
	/**
	Determines whether the point is within the vertical projection of the box
	@param x The x-coordinate of the point to test
	@param z The y-coordinate of the point to test
	@return Returns true if the point resides within the vertical projection of the box and false otherwise
	*/
	bool Collision(float x, float z);

	/**
	Tests whether a sphere intersects the bounding box
	@param center The center of the sphere
	@param radius The radius of the sphere
	@return true if the two intersect
	*/
	bool testBoxVersusSphere(const vec3 &center, float radius) const;
};

#endif
