/*
Modified in 2005-2008 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Original Author:
Ben Humphrey (DigiBen)
DigiBen@GameTutorials.com
Co-Web Host of www.GameTutorials.com

Extensively modified over the years. Some resemblence to the original code can
still be seen in the Frustum::set method.
*/

#include "Core.h"
#include "Frustum.h"

Frustum::Frustum()
{
	clear();
}

Frustum::Frustum(const Frustum &o)
{
	for(int i=0; i<6; ++i)
	{
		frustum[i] = o.frustum[i];
	}
}

Frustum& Frustum::operator=(const Frustum &o)
{
	for(int i=0; i<6; ++i)
	{
		frustum[i] = o.frustum[i];
	}

	return(*this);
}

Frustum::Frustum(const mat4 &modl, const mat4 &proj)
{
	clear();
	set(modl, proj);
}

void Frustum::set(const mat4 &modl, const mat4 &proj)
{
	mat4 clip;

	// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
	// it will give us our clipping planes.  To combine 2 matrices, we multiply them.

	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	// Now we actually want to get the sides of the frustum.  To do this we take
	// the clipping planes we received above and extract the sides from them.

	// This will extract the RIGHT side of the frustum
	frustum[RIGHT].x = clip[ 3] - clip[ 0];
	frustum[RIGHT].y = clip[ 7] - clip[ 4];
	frustum[RIGHT].z = clip[11] - clip[ 8];
	frustum[RIGHT].w = clip[15] - clip[12];

	// This will extract the LEFT side of the frustum
	frustum[LEFT].x = clip[ 3] + clip[ 0];
	frustum[LEFT].y = clip[ 7] + clip[ 4];
	frustum[LEFT].z = clip[11] + clip[ 8];
	frustum[LEFT].w = clip[15] + clip[12];

	// This will extract the BOTTOM side of the frustum
	frustum[BOTTOM].x = clip[ 3] + clip[ 1];
	frustum[BOTTOM].y = clip[ 7] + clip[ 5];
	frustum[BOTTOM].z = clip[11] + clip[ 9];
	frustum[BOTTOM].w = clip[15] + clip[13];

	// This will extract the TOP side of the frustum
	frustum[TOP].x = clip[ 3] - clip[ 1];
	frustum[TOP].y = clip[ 7] - clip[ 5];
	frustum[TOP].z = clip[11] - clip[ 9];
	frustum[TOP].w = clip[15] - clip[13];

	// This will extract the BACK side of the frustum
	frustum[BACK].x = clip[ 3] - clip[ 2];
	frustum[BACK].y = clip[ 7] - clip[ 6];
	frustum[BACK].z = clip[11] - clip[10];
	frustum[BACK].w = clip[15] - clip[14];

	// This will extract the FRONT side of the frustum
	frustum[FRONT].x = clip[ 3] + clip[ 2];
	frustum[FRONT].y = clip[ 7] + clip[ 6];
	frustum[FRONT].z = clip[11] + clip[10];
	frustum[FRONT].w = clip[15] + clip[14];

	for(Side i=0; i<6; ++i)
	{
		frustum[i] = normalizePlane(frustum[i]);
	}
}

bool Frustum::isPointWithin(const vec3 &point) const
{
	for(Side i=0; i<6; ++i)
	{
		if(frustum[i].dot(vec4(point.x, point.y, point.z, 1.0f)) <= 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::isSphereWithin(const vec3 &center, float diameter) const
{
	for(Side i = 0; i < 6; i++)
	{
		if(frustum[i].dot(vec4(center.x, center.y, center.z, 1.0f)) <= -diameter)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::isCubeWithin(const vec3 &center, float size) const
{
	for(int i = 0; i < 6; i++ )
	{
		const vec4 &plane = frustum[i];

		if(plane.dot(vec4(center.x-size, center.y-size, center.z-size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x+size, center.y-size, center.z-size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x-size, center.y+size, center.z-size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x+size, center.y+size, center.z-size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x-size, center.y-size, center.z+size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x+size, center.y-size, center.z+size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x-size, center.y+size, center.z+size, 1)) > 0) continue;
		if(plane.dot(vec4(center.x+size, center.y+size, center.z+size, 1)) > 0) continue;

		return false;
	}

	return true;
}

vec4 Frustum::normalizePlane(const vec4 &plane)
{
	return plane * (1.0f / sqrtf(SQR(plane.x) + SQR(plane.y) + SQR(plane.z)));
}
