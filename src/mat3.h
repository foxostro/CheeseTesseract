#ifndef _MAT3_
#define _MAT3_

#include "vec3.h"
#include "vec4.h"

class mat3
{
public:
	/**	4x4 matrix */
	float m[16];

public:
	/**
	Constructs a transformation matrix.
	@param x Points up the X-Axis
	@param y Points up the Y-Axis
	@param z Points up the Z-Axis
	*/
	mat3(const vec3 &x, const vec3 &y, const vec3 &z)
	{
		set(x, y, z);
	}

	/** Constructs an identity matrix */
	mat3()
	{
		identity();
	}

	/**
	Copy constructor.
	@param mt mat3 to copy
	*/
	mat3(const mat3 &mt)
	{
		for(size_t ct=0; ct < sizeof(m) / sizeof(float); ct++)
		{
			m[ct] = mt.m[ct];
		}
	}

	static mat3 fromRotateX(float theta);

	static mat3 fromRotateY(float theta);

	static mat3 fromRotateZ(float theta);

	/**
	Converts from coordinates in the 2D matrix to the array index
	@param x Matrix Column
	@param y Matrix Row
	*/
	inline static int INDEX(int x, int y)
	{
		return (x*4)+y;
	}

	/**
	Constructs a transformation matrix.
	@param pos Position
	@param x Points up the X-Axis
	@param y Points up the Y-Axis
	@param z Points up the Z-Axis
	*/
	void set(const vec3 &x, const vec3 &y, const vec3 &z)
	{
		m[0]  = x.x;
		m[1]  = y.x;
		m[2]  = z.x;
		m[3]  = 0;

		m[4]  = x.y;
		m[5]  = y.y;
		m[6]  = z.y;
		m[7]  = 0;

		m[8]  = x.z;
		m[9]  = y.z;
		m[10] = z.z;
		m[11] = 0;

		m[12] = 0;
		m[13] = 0;
		m[14] = 0;
		m[15] = 1;
	}

	/** Constructs a zero'ed matrix */
	void zero()
	{
		for(size_t ct=0; ct < sizeof(m) / sizeof(float); ct++)
		{
			m[ct]=0.0f;
		}
	}

	/**	Constructs an identity matrix */
	void identity()
	{
		zero();
		m[0]=1.0f;
		m[5]=1.0f;
		m[10]=1.0f;
		m[15]=1.0f;
	}

	/**
	Sets the front vector of the matrix.
	This vector represents the direction of the local-space X-Axis.
	@param front Front vector
	*/
	void setAxisX(const vec3 &x)
	{
		m[0] = x.x;
		m[4] = x.y;
		m[8] = x.z;
	}

	/**
	Sets the up vector of the matrix.
	This vector represents the direction of the local-space Y-Axis.
	@param y up vector
	*/
	void setAxisY(const vec3 &y)
	{
		m[1] = y.x;
		m[5] = y.y;
		m[9] = y.z;
	}

	/**
	Sets the left vector of the matrix.
	This vector represents the direction of the local-space Z-Axis.
	@param left left vector
	*/
	void setAxisZ(const vec3 &z)
	{
		m[2]  = z.x;
		m[6]  = z.y;
		m[10] = z.z;
	}

	/**
	Adds this matrix with a scalar value.
	@param scalar The scalar value
	@return resultant matrix
	*/
	mat3 operator + (float scalar) const
	{
		mat3 temp = *this;

		for(size_t i=0; i < sizeof(m) / sizeof(float); ++i)
		{
			temp.m[i] += scalar;
		}

		return temp;
	}

	/**
	Multiplies this matrix with a scalar value.
	@param scalar The scalar value
	@return resultant matrix
	*/
	mat3 operator * (float scalar) const
	{
		mat3 temp = *this;

		for(size_t i = 0; i < sizeof(m) / sizeof(float); ++i)
		{
			temp.m[i] *= scalar;
		}

		return temp;
	}

	/**
	Subtracts this matrix to another matrix.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat3 operator - (const mat3 &mt) const
	{
		mat3 temp = *this;

		for(int cty=0; cty<4; cty++)
		{
			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)] -= mt.m[INDEX(ctx,cty)];
			}
		}

		return temp;
	}

	/**
	Adds this matrix to another matrix.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat3 operator + (const mat3 &mt) const
	{
		mat3 temp = *this;

		for(int cty=0; cty<4; cty++)
		{
			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)] += mt.m[INDEX(ctx,cty)];
			}
		}

		return temp;
	}

	/**
	Multiplies this matrix with another matrix.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat3 operator * (const mat3 &mt) const
	{
		mat3 temp;

		for(int cty=0; cty<4; cty++)
		{
			float row1=m[INDEX(0,cty)],
				  row2=m[INDEX(1,cty)],
				  row3=m[INDEX(2,cty)],
				  row4=m[INDEX(3,cty)];

			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)] = row1*mt.m[INDEX(ctx,0)] +
				                         row2*mt.m[INDEX(ctx,1)] +
										 row3*mt.m[INDEX(ctx,2)] +
										 row4*mt.m[INDEX(ctx,3)];
			}
		}

		return temp;
	}

	/**
	Transforms a vector by this matrix
	@param v The vector
	@return resultant vector
	*/
	vec4 transformVector(const vec4 &v) const;

	/**
	Transforms a vector by this matrix
	@param v The vector
	@return resultant vector
	*/
	vec3 transformVector(const vec3 &v) const;

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

	@param eye		The position of the eye point.
	@param center	The position of the reference point.
	@param up		The direction of the up vector.
	*/
	void lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);

	/**
	Multiplies this matrix with another matrix and assigns this matrix to the result.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat3 operator *= (mat3 &mt)
	{
		mat3 temp;

		for(int cty=0; cty<4; cty++)
		{
			float row1=m[INDEX(0,cty)],
				  row2=m[INDEX(1,cty)],
				  row3=m[INDEX(2,cty)],
				  row4=m[INDEX(3,cty)];

			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)] = row1*mt.m[INDEX(ctx,0)] +
				                         row2*mt.m[INDEX(ctx,1)] +
										 row3*mt.m[INDEX(ctx,2)] +
										 row4*mt.m[INDEX(ctx,3)];
			}
		}

		for(size_t ct = 0; ct < sizeof(m) / sizeof(float); ct++)
		{
			m[ct] = temp.m[ct];
		}

		return *this;
	}

	/** Transforms a vector by the matrix */
	vec4 operator * (vec4 &v) const
	{
		vec4 temp; // the return vector

		// run through the columns
		temp.x=v.x*m[INDEX(0,0)]+v.y*m[INDEX(0,1)]+v.z*m[INDEX(0,2)]+v.w*m[INDEX(0,3)];
		temp.y=v.x*m[INDEX(1,0)]+v.y*m[INDEX(1,1)]+v.z*m[INDEX(1,2)]+v.w*m[INDEX(1,3)];
		temp.z=v.x*m[INDEX(2,0)]+v.y*m[INDEX(2,1)]+v.z*m[INDEX(2,2)]+v.w*m[INDEX(2,3)];
		temp.w=v.x*m[INDEX(3,0)]+v.y*m[INDEX(3,1)]+v.z*m[INDEX(3,2)]+v.w*m[INDEX(3,3)];

		return temp;
	}

	/**
	Assignment operator
	@param rh The right-hand side
	@return The new matrix
	*/
	mat3 operator=(const mat3 &rh)
	{
		for(size_t i = 0; i < sizeof(m) / sizeof(float); ++i)
		{
			m[i] = rh.m[i];
		}

		return(*this);
	}

	/**
	Rotate the transformation matrix about the X-Axis
	@param angle Angle to rotate the matrix by
	*/
	void rotateX(float angle)
	{
		// clear to identity matrix
		identity();

		// calculate sine and cosine of angle
		float msin=sinf(angle);
		float mcos=cosf(angle);

		// set up matrix
		m[5]=mcos;
		m[6]=-msin;
		m[9]=msin;
		m[10]=mcos;
	}

	/**
	Rotate the transformation matrix about the Y-Axis
	@param angle Angle to rotate the matrix by
	*/
	void rotateY(float angle)
	{
		// clear to ID matrix
		identity();

		// calculate sine and cosine of angle
		float msin=sinf(angle);
		float mcos=cosf(angle);

		// set up matrix
		m[0]=mcos;
		m[2]=msin;
		m[8]=-msin;
		m[10]=mcos;
	}

	/**
	Rotate the transformation matrix about the Z-Axis
	@param angle Angle to rotate the matrix by
	*/
	void rotateZ(float angle)
	{
		// clear to ID matrix
		identity();

		// calculate sine and cosine of angle
		float msin=sinf(angle);
		float mcos=cosf(angle);

		// set up matrix
		m[0]=mcos;
		m[1]=-msin;
		m[4]=msin;
		m[5]=mcos;
	}

	/**
	Rotate the transformation matrix about the X, Y, and Z-Axii
	@param x Angle to rotate the matrix by around the X-Axis
	@param y Angle to rotate the matrix by around the Y-Axis
	@param z Angle to rotate the matrix by around the Z-Axis
	*/
	void rotate(float x, float y, float z)
	{
		// clear to ID matrix
		identity();

		// get sine and cosine of angles
		float sinx=(float)sin((double)x);
		float cosx=(float)cos((double)x);
		float siny=(float)sin((double)y);
		float cosy=(float)cos((double)y);
		float sinz=(float)sin((double)z);
		float cosz=(float)cos((double)z);

		// set up the matrix
		m[0]=cosy*cosz;
		m[1]=sinx*siny*cosz-cosx*sinz;
		m[2]=cosx*siny*cosz+sinx*sinz;

		m[4]=cosy*sinz;
		m[5]=sinx*siny*sinz+cosx*cosz;
		m[6]=cosx*siny*sinz-sinx*cosz;

		m[8]=-siny;
		m[9]=sinx*cosy;
		m[10]=cosx*cosy;
	}

	/**
	Rotate the transformation matrix about an arbitrary axis.
	@param angle Angle to rotate the matrix by around the axis
	@param axis The axis of rotation
	@todo Implemented using OpenGL matrix functions. (Quite the hack.)
	*/
	void rotate(float angle, const vec3 &axis);

	/**
	Gets the left vector of the transformation matrix.
	This vector represents the direction of the local-space Z-Axis.
	@return left
	*/
    inline vec3 getAxisZ() const
	{
		return vec3(m[2], m[6], m[10]);
	}

	/**
	Gets the front vector of the transformation matrix.
	This vector represents the direction of the local-space X-Axis.
	@return front
	*/
    inline vec3 getAxisX() const
	{
		return vec3(m[0], m[4], m[8]);
	}

	/**
	Gets the up vector of the transformation matrix.
	This vector represents the direction of the local-space Y-Axis.
	@return up
	*/
    inline vec3 getAxisY() const
	{
		return vec3(m[1], m[5], m[9]);
	}

	static string toString(mat3 m)
	{
		return string("&mat3("
		             + vec3::toString(m.getAxisX()) + ", "
					 + vec3::toString(m.getAxisY()) + ", "
					 + vec3::toString(m.getAxisZ()) + ")");
	}
};

#endif
