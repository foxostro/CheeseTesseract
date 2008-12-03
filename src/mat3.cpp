#include "Core.h"
#include "mat3.h"

vec4 mat3::transformVector(const vec4 &v) const
{
	// the return vec4
	vec4 temp;

	// temp = v * mt;
	temp.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12];
	temp.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13];
	temp.z = v.x*m[2] + v.y*m[6] + v.z*m[10]+ m[14];
	temp.w = v.x*m[3] + v.y*m[7] + v.z*m[11]+ m[15];
	
	//temp = temp + getPos();

	// and return it
	return temp;
}

vec3 mat3::transformVector(const vec3 &v) const
{
	vec3 temp;

	temp.x = v.dot(getAxisX());
	temp.y = v.dot(getAxisY());
	temp.z = v.dot(getAxisZ());

	return temp;
}

void mat3::rotate(float radians, const vec3 &axis)
{
	FAIL("STUB - Maths needed");
	identity();
	/*
	identity();
	float degrees = radians * 180.0f / (float)M_PI;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(degrees, axis.x, axis.y, axis.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();
*/
}

void mat3::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 delta = center - eye;

	ASSERT(delta.getMagnitude() > FLT_EPSILON, "eye and center are too close together");

	const vec3 x = delta.getNormal();
	const vec3 y = up.cross(x);
	const vec3 z = up;

	setAxisX(x);
	setAxisY(y);
	setAxisZ(z);
}

mat3 mat3::fromRotateX( float theta )
{
	mat3 r;
	r.identity();
	r.setAxisX(vec3(1.0f,        0.0f,         0.0f));
	r.setAxisY(vec3(0.0f, cosf(theta), -sinf(theta)));
	r.setAxisZ(vec3(0.0f, sinf(theta),  cosf(theta)));
	return r;
}

mat3 mat3::fromRotateY( float theta )
{
	mat3 r;
	r.identity();
	r.setAxisX(vec3(cosf(theta), 0.0f, sinf(theta)));
	r.setAxisY(vec3(       0.0f, 1.0f,        0.0f));
	r.setAxisZ(vec3(sinf(theta), 0.0f, cosf(theta)));
	return r;
}

mat3 mat3::fromRotateZ( float theta )
{
	mat3 r;
	r.identity();
	r.setAxisX(vec3(cosf(theta), -sinf(theta), 0.0f));
	r.setAxisY(vec3(sinf(theta),  cosf(theta), 0.0f));
	r.setAxisZ(vec3(       0.0f,         0.0f, 1.0f));
	return r;
}