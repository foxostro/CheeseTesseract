#ifndef _VEC3_H_
#define _VEC3_H_

#include "vec2.h"

/**
Represents a mathematical vector in four dimensions.
@author Andrew Fox
@version August 2007
*/
class vec3
{
public:
	float x, y, z;

	/**	Default constructor. Sets all coordinates to zero */
	vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	/**
	Copy constructor
	@param vec The vector to copy
	*/
	vec3(const vec3 &vec)
	{
		set(vec);
	}

	/**
	Creates the vector directly from a series of coordinates
	@param x X-Coordinate
	@param y Y-Coordinate
	@param z Z-Coordinate
	*/
	vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/**
	Creates the vector directly from a 2D vector and a z-coordinate
	@param xy X and Y-Coordinates
	@param z Z-Coordinate
	*/
	vec3(const vec2 &xy, float z)
	{
		this->x = xy.x;
		this->y = xy.y;
		this->z = z;
	}

	/**	Gets a 2D vector from the X and Y coordinates of this one */
	vec2 xy() const
	{
		return vec2(x, y);
	}

	/**
	Calculates the magnitude of the vector, squared.
	|v|^2

	@return The magnitude of the vector, squared^2.
	*/
	inline float getMagnitudeSqr() const
	{
		return x*x + y*y + z*z;
	}

	/**
	Calculates the magnitude of the vector.
	L = |v|

	@return The magnitude of the vector.
	*/
	inline float getMagnitude() const
	{
		return sqrtf(getMagnitudeSqr());
	}

	/**
	Calculates the unit-length vector.
	v = v1 / |v1|

	@return The unit-length vector.
	*/
	inline const vec3 getNormal() const
	{
		return scale(1.0f / getMagnitude());
	}

	/**
	Calculates the vector after scaling by a constant
	v = v1 * s

	@param scalar The scalar constant
	@return The scaled vector.
	*/
	inline const vec3 scale(float scalar) const
	{
		return vec3(x*scalar, y*scalar, z*scalar);
	}

	/**
	Calculates the vector after adding a constant to it.
	v = v1 + c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec3 add(float value) const
	{
		return vec3(x+value, y+value, z+value);
	}

	/**
	Calculates the vector after adding another vector to it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec3 add(const vec3& vec) const
	{
		return vec3(x+vec.x, y+vec.y, z+vec.z);
	}

	/**
	Calculates the vector after subtracting a constant from it.
	v = v1 - c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec3 subtract(float value) const
	{
		return vec3(x-value, y-value, z-value);
	}

	/**
	Calculates the vector after subtracting another vector from it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec3 subtract(const vec3& vec) const
	{
		return vec3(x-vec.x, y-vec.y, z-vec.z);
	}

	/**
	Calculates the vector after performing a cross product operation with it and another vector.
	Determines the vector orthogonal to this vector and the given vector.

	v = < (v1.y * v2.z) - (v1.z * v2.y), (v1.z * V2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x) >

	@param vec The other vector
	@return The resultant vector.
	*/
	inline vec3 cross(const vec3 &vec) const
	{
		vec3 crossProduct;

		crossProduct.x = (y * vec.z) - (z * vec.y);
		crossProduct.y = (z * vec.x) - (x * vec.z);
		crossProduct.z = (x * vec.y) - (y * vec.x);

		return crossProduct;
	}

	/**
	Calculates the value after performing a dot product operation with this vector and another vector.

	v1.v2 = |v1| |v2| cos theta

	@param vec The other vector
	@return The dot product
	*/
	inline float dot(const vec3 &vec) const
	{
		return (x * vec.x) + (y * vec.y) + (z * vec.z);
	}

	/**
	Gets the distance between this vector and the other vector, squared.
	@param vec The other vector
	@return The distance, squared
	*/
	inline float distanceSqr(const vec3 &vec) const
	{
		float dx = x - vec.x;
		float dy = y - vec.y;
		float dz = z - vec.z;

		return(dx*dx + dy*dy + dz*dz);
	}

	/**
	Gets the distance between this vector and the other vector.
	@param vec The other vector
	@return The distance
	*/
	inline float distance(const vec3 &vec) const
	{
		return sqrtf(distanceSqr(vec));
	}

public:
	/**
	Cast to floating point array
	@return array
	*/
	operator float* () const
	{
		return (float*)(this);
	}

	/**
	Cast to floating point array
	@return array
	*/
	operator const float* () const
	{
		return (const float*)(this);
	}

	/**
	Sets the vector to the value on the right-hand side
	@param rh The right-hand side
	@return The new vector
	*/
	vec3 operator=(const vec3 &rh)
	{
		set(rh);
		return(*this);
	}

	float operator[](size_t idx) const
	{
		switch(idx)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return 0.0f;
		};
	}

	float& operator[](size_t idx)
	{
		switch(idx)
		{
		case 0:  return x;
		case 1:  return y;
		case 2:  return z;
		default: return z;
		};
	}

	/**
	Subtracts the right-hand side from this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec3 operator - (vec3 rh) const
	{
		return subtract(rh);
	}

	/**
	Adds the right-hand side to this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec3 operator + (vec3 rh) const
	{
		return add(rh);
	}

	/**
	Scales this vector by the value on the right-hand side.
	@param scalar The right-hand side scalar
	@return The new vector
	*/
	vec3 operator * (float scalar) const
	{
		return scale(scalar);
	}

	/**
	Determines boolean equality between this vector and another vector on the right-hand side.
	@param rh The right-hand side@return true if equal
	*/
	bool operator == (const vec3 &rh) const
	{
		return isEqual(rh);
	}

	inline bool isEqual(const vec3 &rh) const
	{

		return fabsf(x-rh.x) < FLT_EPSILON && 
			   fabsf(y-rh.y) < FLT_EPSILON &&
			   fabsf(z-rh.z) < FLT_EPSILON;
	}

	/**
	Negates this vector
	@return The new vector
	*/
	vec3 operator - () const
	{
		return vec3(-x, -y, -z);
	}

public:
	/**
	Resets all coordinates in this vector to zero
	*/
	void zero()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	/**
	Sets the vector to the given value.
	@param vec The other value
	*/
	void set(const vec3 &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	/**
	Sets the vector to the given value.
	@param x The new x-value
	@param y The new y-value
	@param z The new z-value
	*/
	void set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/**
	Sets this vector to the unit-length version of itself.
	*/
	void normalize()
	{
		(*this) = getNormal();
	}

	static vec3 fromString(const string &_s)
	{
		vec3 vec;
		
		string s = toLowerCase(_s);

		char_separator<char> delimeters("(,)\t\n");
		tokenizer<char_separator<char> > tokens(s, delimeters);

		tokenizer<char_separator<char> >::const_iterator i = tokens.begin();

		if(i != tokens.end() && (string(*i)=="&vec3" || string(*i)=="&vec"))
		{
			vec.x = stof(*(++i));
			vec.y = stof(*(++i));
			vec.z = stof(*(++i));
		}

		return vec;
	}

	static string toString(vec3 vec)
	{
		return string("&vec3(" + ftos(vec.x, 4) + ", " + ftos(vec.y, 4) + ", " + ftos(vec.z, 4) + ")");
	}
};

#endif
