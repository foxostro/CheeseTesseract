#include "Core.h"
#include "mat4.h"

vec4 mat4::transformVector(const vec4 &v) const {
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

vec3 mat4::transformVector(const vec3 &v) const {
	// the return vec3
	vec3 temp;
	
	// temp = v * mt;
	temp.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12];
	temp.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13];
	temp.z = v.x*m[2] + v.y*m[6] + v.z*m[10]+ m[14];
	
	//temp = temp + getPos();
	
	// and return it
	return temp;
}

void mat4::rotate(float radians, const vec3 &axis) {
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

void mat4::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
	vec3 delta = center - eye;
	
	ASSERT(delta.getMagnitude() > FLT_EPSILON, "eye and center are too close together");
	
	const vec3 x = delta.getNormal();
	const vec3 y = up.cross(x);
	const vec3 z = up;
	
	identity();
	setAxisX(x);
	setAxisY(y);
	setAxisZ(z);
	setPos(eye);
}

mat4 mat4::inverse() const {
	mat4 MI, B;
	
	float det = determinant();
	
	ASSERT(det>FLT_EPSILON, "Cannot invert because the determinant is zero");
	
	const float a11 = m[0x0];
	const float a12 = m[0x1];
	const float a13 = m[0x2];
	const float a14 = m[0x3];
	
	const float a21 = m[0x4];
	const float a22 = m[0x5];
	const float a23 = m[0x6];
	const float a24 = m[0x7];
	
	const float a31 = m[0x8];
	const float a32 = m[0x9];
	const float a33 = m[0xA];
	const float a34 = m[0xB];
	
	const float a41 = m[0xC];
	const float a42 = m[0xD];
	const float a43 = m[0xE];
	const float a44 = m[0xF];
	
	// See http://www.cvl.iis.u-tokyo.ac.jp/~miyazaki/tech/teche23.html
	const float b11 = a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;
	const float b12 = a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43;
	const float b13 = a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42;
	const float b14 = a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33;
	const float b21 = a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a34*a41 - a24*a31*a43;
	const float b22 = a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41;
	const float b23 = a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43;
	const float b24 = a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31;
	const float b31 = a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41;
	const float b32 = a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42;
	const float b33 = a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41;
	const float b34 = a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32;
	const float b41 = a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42;
	const float b42 = a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41;
	const float b43 = a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42;
	const float b44 = a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31;
	
	B.m[0x0] = b11;
	B.m[0x1] = b12;
	B.m[0x2] = b13;
	B.m[0x3] = b14;
	
	B.m[0x4] = b21;
	B.m[0x5] = b22;
	B.m[0x6] = b23;
	B.m[0x7] = b24;
	
	B.m[0x8] = b31;
	B.m[0x9] = b32;
	B.m[0xA] = b33;
	B.m[0xB] = b34;
	
	B.m[0xC] = b41;
	B.m[0xD] = b42;
	B.m[0xE] = b43;
	B.m[0xF] = b44;
	
	MI = B * (1.0f / det);
	
#ifndef NDEBUG
	const mat4 &M = *this;
	mat4 I;
	I.identity();
	mat4 a = M * MI;
	ASSERT(a == I, "Not proper inverse: M * MI != I");
#endif
	
	return MI;
}

bool mat4::operator==( const mat4 &rh ) const {
	const float eps = 0.001f;
	
	for (int i=0; i<16; ++i) {
		if (fabs(m[i] - rh.m[i]) > eps)
			return false;
	}
	
	return true;
}

mat4 mat4::transpose() const {
	mat4 mt;
	
	for (int cty=0; cty<4; cty++) {
		for (int ctx=0; ctx<4; ctx++) {
			mt.m[INDEX(ctx,cty)]=m[INDEX(cty,ctx)];
		}
	}
	
	return mt;
}

float mat4::determinant() const {
	// See http://www.cvl.iis.u-tokyo.ac.jp/~miyazaki/tech/teche23.html
	
	const float a11 = m[0x0];
	const float a12 = m[0x1];
	const float a13 = m[0x2];
	const float a14 = m[0x3];
	
	const float a21 = m[0x4];
	const float a22 = m[0x5];
	const float a23 = m[0x6];
	const float a24 = m[0x7];
	
	const float a31 = m[0x8];
	const float a32 = m[0x9];
	const float a33 = m[0xA];
	const float a34 = m[0xB];
	
	const float a41 = m[0xC];
	const float a42 = m[0xD];
	const float a43 = m[0xE];
	const float a44 = m[0xF];
	
	const float det = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43
	                  + a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41
	                  + a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42
	                  + a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41
	                  - a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42
	                  - a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43
	                  - a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41
	                  - a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;
	                  
	return det;
}
