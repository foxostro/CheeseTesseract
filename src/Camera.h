#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "PropertyBag.h"
#include "Frustum.h"

class Camera
{
public:
	vec3 eye, center, up;

public:
	/** Default constructor */
	Camera()
	{
		clear();
	}

	/** Resets the camera to a just-constructed state */
	void clear();

	/**
	The lookAt method creates a viewing matrix derived from an eye point,
	a reference point indicating the center of the scene, and an up vector.
	The matrix maps the reference point to the negative z-axis and the eye
	point to the origin, so that when you use a typical projection matrix,
	the center of the scene maps to the center of the viewport. Similarly,
	the direction described by the up vector projected onto the viewing plane
	is mapped to the positive y-axis so that it points upward in the viewport.
	The up vector must not be parallel to the line of sight from the eye to
	the reference point.

	@param eye    The position of the eye point.
	@param center The position of the reference point.
	@param up     The direction of the up vector.
	*/
	void lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);

	/** Sets the camera position and orientation */
	void setCamera() const;

	/**
	Gets the orientation of the camera
	@return orientation matrix
	*/
	mat3 getOrientation() const;

	/**
	Gets the position of the camera
	@return position
	*/
	inline vec3 getPosition() const
	{
		return eye;
	}
};

#endif
