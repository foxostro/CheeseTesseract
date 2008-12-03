#ifndef _MAT4_
#define _MAT4_

#include "vec3.h"
#include "vec4.h"

/**
General purpose 4x4 matrix class.
The matrix may be used as a general transformation matrix
and can represent any position and orientation in space.

The front, up, and left vectors are used to represent an
orientation in space in the matrix and comprise the first
three elements of the first, second, and third columns,
respectively.  As long as three vectors are orthogonal, the
matrix remains a valid transformation matrix.  If the
vectors are allowed to become non-orthogonal, then the
resulting transformation will cause the transformed object
to be skewed.

It is important to note that although the mat4 class names
them the front, up, and left vectors, this is not
necessarily the directions they face.  Essentially, the
front vector represents the direction of the local-space
X-Axis after the transformation with this matrix, the up
vector represent the local space Y-Axis, and the left
vector represent the local-space Z-Axis.  Often, one may
prefer to use the Z-Axis to represent the forward or front
direction of an object, for example.

A position may be represented with the matrix as a 4D
vector comprised of the elements in the fourth row.  Often,
the w coordinate of this vector remains set to 1.0
*/
class mat4
{
public:
	/**	4x4 matrix */
	float m[16];

public:
	/**
	Constructs a transformation matrix.
	@param pos Position
	@param x Points up the X-Axis
	@param y Points up the Y-Axis
	@param z Points up the Z-Axis
	*/
	mat4(const vec3 &pos, const vec3 &x, const vec3 &y, const vec3 &z)
	{
		set(pos, x, y, z);
	}

	/**
	Default construtor. Constructs an identity matrix.
	*/
	mat4()
	{
		identity();
	}

	/**
	Copy constructor.
	@param mt mat4 to copy
	*/
	mat4(const mat4 &mt)
	{
		for(size_t ct=0; ct < sizeof(m) / sizeof(float); ct++)
		{
			m[ct] = mt.m[ct];
		}
	}

	/**
	Constructs the matrix from an array of floats
	@param mt 4x4 Float array, float[16]
	*/
	mat4(const float * const mt)
	{
		const int sz = sizeof(m) / sizeof(float);
		for(int ct=0; ct < sz; ct++)
		{
			m[ct] = mt[ct];
		}
	}

	/**
	Constructs a transformation matrix.
	@param pos		Position
	@param z		points up the Z-Axis
	@param y		points up the Y-Axis
	*/
	mat4(const vec3 &pos, const vec3 &z, const vec3 &y)
	{
		set(pos, z.cross(y), y, z);
	}

	inline static mat4 fromRotateX(float a)
	{
		mat4 r;
		r.rotateX(a);
		return r;
	}

	inline static mat4 fromRotateY(float a)
	{
		mat4 r;
		r.rotateY(a);
		return r;
	}

	inline static mat4 fromRotateZ(float a)
	{
		mat4 r;
		r.rotateZ(a);
		return r;
	}

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
	void set(const vec3 &pos, const vec3 &x, const vec3 &y, const vec3 &z)
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

		m[12] = pos.x;
		m[13] = pos.y;
		m[14] = pos.z;
		m[15] = 1;
	}

	/**
	Constructs a zero'ed matrix
	*/
	void zero()
	{
		for(int ct=0; ct<16; ct++) m[ct]=0.0f;
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
	Sets the pos vector of the matrix
	@param pos pos vector
	*/
	void setPos(const vec3 &pos)
	{
		m[12] = pos.x;
		m[13] = pos.y;
		m[14] = pos.z;
	}

	/**
	Adds this matrix with a scalar value.
	@param scalar The scalar value
	@return resultant matrix
	*/
	mat4 operator + (float scalar) const
	{
		// the return matrix
		mat4 temp = *this;

		for(int i=0; i<16; ++i)
		{
			temp.m[i] += scalar;
		}

		// and return it
		return temp;
	}

	/**
	Multiplies this matrix with a scalar value.
	@param scalar The scalar value
	@return resultant matrix
	*/
	mat4 operator * (float scalar) const
	{
		// the return matrix
		mat4 temp = *this;

		for(int i=0; i<16; ++i)
		{
			temp.m[i] *= scalar;
		}

		// and return it
		return temp;
	}

	/**
	Subtracts this matrix to another matrix.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat4 operator - (const mat4 &mt) const
	{
		// the return matrix
		mat4 temp = *this;

		// first ys
		for(int cty=0; cty<4; cty++)
		{
			// then xs
			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)] -= mt.m[INDEX(ctx,cty)];
			}

		}

		// and return it
		return temp;
	}

	/**
	Adds this matrix to another matrix.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat4 operator + (const mat4 &mt) const
	{
		// the return matrix
		mat4 temp = *this;

		// first ys
		for(int cty=0; cty<4; cty++)
		{
			// then xs
			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)] += mt.m[INDEX(ctx,cty)];
			}

		}

		// and return it
		return temp;
	}

	/**
	Multiplies this matrix with another matrix.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat4 operator * (const mat4 &mt) const
	{
		// the return matrix
		mat4 temp;

		// first ys
		for(int cty=0; cty<4; cty++)
		{
			// get the present row
			float row1=m[INDEX(0,cty)],
				  row2=m[INDEX(1,cty)],
				  row3=m[INDEX(2,cty)],
				  row4=m[INDEX(3,cty)];

			// then xs
			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)]=(row1*mt.m[INDEX(ctx,0)])+(row2*mt.m[INDEX(ctx,1)])+
										(row3*mt.m[INDEX(ctx,2)])+(row4*mt.m[INDEX(ctx,3)]);
			}

		}

		// and return it
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
	Multiplies this matrix with another matrix and assigns this matrix to the result.
	@param mt The other matrix
	@return resultant matrix
	*/
	mat4 operator *= (mat4 &mt)
	{
		// the return matrix
		mat4 temp;

		// first ys
		for(int cty=0; cty<4; cty++)
		{
			// get the present row
			float row1=m[INDEX(0,cty)],
				  row2=m[INDEX(1,cty)],
				  row3=m[INDEX(2,cty)],
				  row4=m[INDEX(3,cty)];

			// then xs
			for(int ctx=0; ctx<4; ctx++)
			{
				temp.m[INDEX(ctx,cty)]=(row1*mt.m[INDEX(ctx,0)])+(row2*mt.m[INDEX(ctx,1)])+
										(row3*mt.m[INDEX(ctx,2)])+(row4*mt.m[INDEX(ctx,3)]);
			}

		}

		// copy back to ours
		for(int ct=0; ct<16; ct++)
		{
			m[ct]=temp.m[ct];
		}

		// and return
		return *this;
	}

	/**
	Transforms a vector by the matrix.
	*/
	vec4 operator * (vec4 &v) const
	{
		vec4 temp; // the return vector

		// run through the columns
		temp.x=v.x*m[INDEX(0,0)]+v.y*m[INDEX(0,1)]+v.z*m[INDEX(0,2)]+v.w*m[INDEX(0,3)];
		temp.y=v.x*m[INDEX(1,0)]+v.y*m[INDEX(1,1)]+v.z*m[INDEX(1,2)]+v.w*m[INDEX(1,3)];
		temp.z=v.x*m[INDEX(2,0)]+v.y*m[INDEX(2,1)]+v.z*m[INDEX(2,2)]+v.w*m[INDEX(2,3)];
		temp.w=v.x*m[INDEX(3,0)]+v.y*m[INDEX(3,1)]+v.z*m[INDEX(3,2)]+v.w*m[INDEX(3,3)];

		// and return it
		return temp;
	}

	/**
	Assignment operator
	@param rh The right-hand side
	@return The new matrix
	*/
	mat4 operator=(const mat4 &rh)
	{
		for(int i=0; i<16; ++i)
		{
			m[i] = rh.m[i];
		}

		return(*this);
	}

	/**
	Equality operator
	@param rh The right-hand side
	@return equal to within FLT_EPSILON?
	*/
	bool operator==(const mat4 &rh) const;

	/**
	Cast to floating point array
	@return array
	*/
	operator float* () const
	{
		return (float*)m;
	}

	/** Gets the determinant of the matrix */
	float determinant() const;

	/** Gets the transpose matrix */
	mat4 transpose() const;

	/** Gets the inverse matrix */
	mat4 inverse() const;

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
	Translates the transformation matrix.
	@param x Amount to translate on the X-Axis
	@param y Amount to translate on the Y-Axis
	@param z Amount to translate on the Z-Axis
	*/
	void translate(float x, float y, float z)
	{
		// clear to ID matrix
		identity();

		// and setup as translation
		m[12]=x;
		m[13]=y;
		m[14]=z;
	}

	/**
	Translates the transformation matrix.
	@param delta vec3 to translate by
	*/
	void translate(const vec3 &delta)
	{
		translate(delta.x, delta.y, delta.z);
	}

	/**
	Gets the left vector of the transformation matrix.
	This vector represents the direction of the local-space Z-Axis.
	@return left
	*/
    vec3 getAxisZ() const
	{
		return vec3(m[2], m[6], m[10]);
	}

	/**
	Gets the front vector of the transformation matrix.
	This vector represents the direction of the local-space X-Axis.
	@return front
	*/
    vec3 getAxisX() const
	{
		return vec3(m[0], m[4], m[8]);
		}

	/**
	Gets the up vector of the transformation matrix.
	This vector represents the direction of the local-space Y-Axis.
	@return up
	*/
    vec3 getAxisY() const
	{
		return vec3(m[1], m[5], m[9]);
	}

	/**
	Gets the position vector of the transformation matrix.
	@return position
	*/
    vec3 getPos() const
	{
		return vec3(m[12], m[13], m[14]);
	}

	static string toString(mat4 m)
	{
		return string("&mat4("
		             + vec3::toString(m.getPos()) + ", "
		             + vec3::toString(m.getAxisX()) + ", "
					 + vec3::toString(m.getAxisY()) + ", "
					 + vec3::toString(m.getAxisZ()) + ")");
	}
};

#endif
