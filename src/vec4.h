#ifndef _VEC4_H_
#define _VEC4_H_

#include "vec3.h"

/**
Represents a mathematical vector in four dimensions.
@author Andrew Fox
@version August 2007
*/
class vec4
{
public:
	float x, y, z, w;

	/**	Default constructor. Sets all coordinates to zero */
	vec4()
	{
	    x = 0.0f;
	    y = 0.0f;
	    z = 0.0f;
	    w = 0.0f;
    }

	/**
	Copy constructor
	@param vec The vector to copy
	*/
	vec4(const vec4 &vec)
	{
		set(vec);
	}

	/**
	Constructor from a vec3 and a q component
	@param vec The vector to copy
	*/
	vec4(const vec3 &xyz, float _w)
	{
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
		w = _w;
	}

	/**
	Creates the vector directly from a series of coordinates
	@param x X-Coordinate
	@param y Y-Coordinate
	@param z Z-Coordinate
	@param w W-Coordinate
	*/
	vec4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	Calculates the magnitude of the vector, squared.
	|v|^2

	@return The magnitude of the vector, squared^2.
	*/
	inline float getMagnitudeSqr() const
	{
		return x*x + y*y + z*z + w*w;
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
	inline const vec4 getNormal() const
	{
		return scale(1.0f / getMagnitude());
	}

	/**
	Calculates the vector after scaling by a constant
	v = v1 * s

	@param scalar The scalar constant
	@return The scaled vector.
	*/
	inline const vec4 scale(float scalar) const
	{
		return vec4(x*scalar, y*scalar, z*scalar, w*scalar);
	}

	/**
	Calculates the vector after adding a constant to it.
	v = v1 + c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec4 add(float value) const
	{
		return vec4(x+value, y+value, z+value, w+value);
	}

	/**
	Calculates the vector after adding another vector to it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec4 add(const vec4& vec) const
	{
		return vec4(x+vec.x, y+vec.y, z+vec.z, w+vec.w);
	}

	/**
	Calculates the vector after subtracting a constant from it.
	v = v1 - c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec4 subtract(float value) const
	{
		return vec4(x-value, y-value, z-value, w-value);
	}

	/**
	Calculates the vector after subtracting another vector from it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec4 subtract(const vec4& vec) const
	{
		return vec4(x-vec.x, y-vec.y, z-vec.z, w-vec.w);
	}

	/**
	Calculates the value after performing a dot product operation with this vector and another vector.

	v1.v2 = |v1| |v2| cos theta

	@param vec The other vector
	@return The dot product
	*/
	inline float dot(const vec4 &vec) const
	{
		return (x * vec.x) + (y * vec.y) + (z * vec.z) + (w * vec.w);
	}

	/**
	Gets the distance between this vector and the other vector, squared.
	@param vec The other vector
	@return The distance, squared
	*/
	inline float distanceSqr(const vec4 &vec) const
	{
		float dx = x - vec.x;
		float dy = y - vec.y;
		float dz = z - vec.z;
		float dw = w - vec.w;

		return(dx*dx + dy*dy + dz*dz + dw*dw);
	}

	/**
	Gets the distance between this vector and the other vector.
	@param vec The other vector
	@return The distance
	*/
	inline float distance(const vec4 &vec) const
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
	vec4 operator=(const vec4 &rh)
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
		case 3: return w;
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
		case 3:  return w;
		default: return w;
		};
	}

	/**
	Subtracts the right-hand side from this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator - (vec4 rh) const
	{
		return subtract(rh);
	}

	/**
	Adds the right-hand side to this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator + (vec4 rh) const
	{
		return add(rh);
	}

	/**
	Scales this vector by the value on the right-hand side.
	@param scalar The right-hand side scalar
	@return The new vector
	*/
	vec4 operator * (float scalar) const
	{
		return scale(scalar);
	}

	/**
	Determines boolean equality between this vector and another vector on the right-hand side.
	@param rh The right-hand side
	@return true if equal
	*/
	bool operator == (const vec4 &rh) const
	{
		return isEqual(rh);
	}

	inline bool isEqual(const vec4 &rh) const
	{

		return fabsf(x-rh.x) < FLT_EPSILON && 
			   fabsf(y-rh.y) < FLT_EPSILON &&
			   fabsf(z-rh.z) < FLT_EPSILON &&
			   fabsf(w-rh.w) < FLT_EPSILON;
	}

	/**
	Negates this vector
	@return The new vector
	*/
	vec4 operator - () const
	{
		return vec4(-x, -y, -z, -w);
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
		w = 1.0f;
	}

	/**
	Sets the vector to the given value.
	@param vec The other value
	*/
	void set(const vec4 &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	/**
	Sets the vector to the given value.
	@param x The new x-value
	@param y The new y-value
	@param z The new z-value
	@param w The new w-value
	*/
	void set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	Sets this vector to the unit-length version of itself.
	*/
	void normalize()
	{
		(*this) = getNormal();
	}
	
	static vec4 fromString(const string &_s)
	{
		vec4 vec;
		
		string s = toLowerCase(_s);

		char_separator<char> delimeters("(,)\t\n");
		tokenizer<char_separator<char> > tokens(s, delimeters);

		tokenizer<char_separator<char> >::const_iterator i = tokens.begin();

		if(i != tokens.end() && (string(*i)=="&vec4" || string(*i)=="&vec"))
		{
			vec.x = stof(*(++i));
			vec.y = stof(*(++i));
			vec.z = stof(*(++i));
			vec.w = stof(*(++i));
		}

		return vec;
	}

	static string toString(vec4 vec)
	{
		return string("&vec4(" + ftos(vec.x, 4) + ", " + ftos(vec.y, 4) + ", " + ftos(vec.z, 4) + ", " + ftos(vec.w, 4) + ")");
	}
};

#endif
