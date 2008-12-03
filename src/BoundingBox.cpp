#include "Core.h"
#include "BoundingBox.h"

bool BoundingBox::testBoxVersusSphere(const vec3 &center, float radius) const
{
	float s, d = 0;

	const vec3 pos = m_Pos;
	const vec3 min = m_Min + pos;
	const vec3 max = m_Max + pos;

	//find the square of the distance
	//from the sphere to the box

	// X
	if( center.x < min.x )
	{
		s = center.x - min.x;
		d += s*s;
	}
	else if( center.x > max.x )
	{
		s = center.x - max.x;
		d += s*s;
	}
	
	// Z
	if( center.z < min.z )
	{
		s = center.z - min.z;
		d += s*s;
	}
	else if( center.z > max.z )
	{
		s = center.z - max.z;
		d += s*s;
	}

	return(d <= SQR(radius));
}

bool BoundingBox::Collision(BoundingBox &box)
{
	float left1   = m_Min.x + m_Pos.x;
	float right1  = m_Max.x + m_Pos.x;
	float top1    = m_Max.x + m_Pos.x;
	float bottom1 = m_Min.x + m_Pos.x;
	float left2   = box.m_Min.x +box. m_Pos.x;
	float right2  = box.m_Max.x + box.m_Pos.x;
	float top2    = box.m_Max.z + box.m_Pos.z;
	float bottom2 = box.m_Min.z + box.m_Pos.z;

	return
	(
		!(bottom1 < top2)
			&&
		!(top1 > bottom2)
			&&
		!(right1 < left2)
			&&
		!(left1 > right2)
	);
}

bool BoundingBox::Collision(float x, float y)
{
	float left   = m_Min.x + m_Pos.x;
	float bottom = m_Min.z + m_Pos.z;
	float width  = m_Max.x - m_Min.x;
	float height = m_Max.z - m_Min.z;
	float right  = left + width;
	float top    = bottom + height;

	return(x > left && x < right && y > top && y < bottom);
}
