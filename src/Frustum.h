/*
Modified in 2005-2008 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Original Author:
Ben Humphrey (DigiBen)
E-Mail: mailto:DigiBen@GameTutorials.com
Co-Web Host of www.GameTutorials.com

Extensively modified over the years. Some resemblence to the original code can
still be seen in the Frustum::set method.
*/

#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "vec4.h"
#include "mat4.h"

/** This will allow us to create an object to keep track of our frustum */
class Frustum
{
public:
	typedef int Side;

	static const Side RIGHT   = 0;
	static const Side LEFT    = 1;
	static const Side BOTTOM  = 2;
	static const Side TOP     = 3;
	static const Side BACK    = 4;
	static const Side FRONT   = 5;

public:
	/** Default Constructor */
	Frustum();

	/** Copy Constructor */
	Frustum(const Frustum &frustum);

	/** Calculates the frustum from the given matrices */
	Frustum(const mat4 &modl, const mat4 &proj);

	/** Assignment operator */
	Frustum& operator=(const Frustum &o);

	/** Resets the frustum */
	inline void clear()
	{
		for(int i=0; i<6; ++i)
		{
			frustum[i] = vec4(0,0,0,0);
		}
	}

	/** Calculates the frustum from the given matrices */
	void set(const mat4 &modl, const mat4 &proj);

	/**
	Determines whether the point is withing the frustum
	@param point Point to test
	*/
	bool isPointWithin(const vec3 &point) const;

	/**
	Determines whether the cube is at last partially within the frustum.
	@param center Center point of the cube
	@param size Length of the cube on a side
	*/
	bool isCubeWithin(const vec3 &center, float size) const;

	/**
	Determines whether the sphere is at least partially within the frustum
	@param center Center point of the sphere
	@param diameter Diameter of the sphere
	*/
	bool isSphereWithin(const vec3 &sphereCenter, float diameter) const;

	/**
	Gets the plane equation of a single side of the frustum
	@param side The side of the frustum to return
	@return The plane equation of that side of the frustum
	*/
	inline const vec4& getPlane(Side side) const
	{
		return frustum[side];
	}

private:
	static vec4 normalizePlane(const vec4 &plane);

private:
	/** Plane equations for the 6 sides of the frustum */
	vec4 frustum[6];
};

#endif
