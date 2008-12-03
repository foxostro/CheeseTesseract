#ifndef _VEC2_H_
#define _VEC2_H_

/**
Represents a mathematical vector in four dimensions.
@author Andrew Fox
@version August 2007
*/
class vec2
{
public:
	float x, y;

	/**	Default constructor. Sets all coordinates to zero */
	vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	/**
	Copy constructor
	@param vec The vector to copy
	*/
	vec2(const vec2 &vec)
	{
		set(vec);
	}

	/**
	Creates the vector directly from a series of coordinates
	@param x X-Coordinate
	@param y Y-Coordinate
	*/
	vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	/**
	Creates the vector directly from a series of coordinates
	@param x X-Coordinate
	@param y Y-Coordinate
	*/
	vec2(int x, int y)
	{
		this->x = (float)x;
		this->y = (float)y;
	}

	/**
	Calculates the magnitude of the vector, squared.
	|v|^2

	@return The magnitude of the vector, squared^2.
	*/
	inline float getMagnitudeSqr() const
	{
		return x*x + y*y;
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
	inline const vec2 getNormal() const
	{
		return scale(1.0f / getMagnitude());
	}

	/**
	Calculates the vector after scaling by a constant
	v = v1 * s

	@param scalar The scalar constant
	@return The scaled vector.
	*/
	inline const vec2 scale(float scalar) const
	{
		return vec2(x*scalar, y*scalar);
	}

	/**
	Calculates the vector after adding a constant to it.
	v = v1 + c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec2 add(float value) const
	{
		return vec2(x+value, y+value);
	}

	/**
	Calculates the vector after adding another vector to it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec2 add(const vec2& vec) const
	{
		return vec2(x+vec.x, y+vec.y);
	}

	/**
	Calculates the vector after subtracting a constant from it.
	v = v1 - c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec2 subtract(float value) const
	{
		return vec2(x-value, y-value);
	}

	/**
	Calculates the vector after subtracting another vector from it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec2 subtract(const vec2& vec) const
	{
		return vec2(x-vec.x, y-vec.y);
	}

	/**
	Calculates the value after performing a dot product operation with this vector and another vector.

	v1.v2 = |v1| |v2| cos theta

	@param vec The other vector
	@return The dot product
	*/
	inline float dot(const vec2 &vec) const
	{
		return (x * vec.x) + (y * vec.y);
	}

	/**
	Gets the distance between this vector and the other vector, squared.
	@param vec The other vector
	@return The distance, squared
	*/
	inline float distanceSqr(const vec2 &vec) const
	{
		float dx = x - vec.x;
		float dy = y - vec.y;

		return(dx*dx + dy*dy);
	}

	/**
	Gets the distance between this vector and the other vector.
	@param vec The other vector
	@return The distance
	*/
	inline float distance(const vec2 &vec) const
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
	vec2 operator=(const vec2 &rh)
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
		default: return 0.0f;
		};
	}

	float& operator[](size_t idx)
	{
		switch(idx)
		{
		case 0:  return x;
		case 1:  return y;
		default: return y;
		};
	}

	/**
	Subtracts the right-hand side from this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec2 operator - (vec2 rh) const
	{
		return subtract(rh);
	}

	/**
	Adds the right-hand side to this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec2 operator + (vec2 rh) const
	{
		return add(rh);
	}

	/**
	Scales this vector by the value on the right-hand side.
	@param scalar The right-hand side scalar
	@return The new vector
	*/
	vec2 operator * (float scalar) const
	{
		return scale(scalar);
	}

	/**
	Determines boolean equality between this vector and another vector on the right-hand side.
	@param rh The right-hand side@return true if equal
	*/
	bool operator == (const vec2 &rh) const
	{
		return isEqual(rh);
	}

	inline bool isEqual(const vec2 &rh) const
	{

		return fabsf(x-rh.x) < FLT_EPSILON && 
			   fabsf(y-rh.y) < FLT_EPSILON;
	}

	/**
	Negates this vector
	@return The new vector
	*/
	vec2 operator - () const
	{
		return vec2(-x, -y);
	}

public:
	/**
	Resets all coordinates in this vector to zero
	*/
	void zero()
	{
		x = 0.0f;
		y = 0.0f;
	}

	/**
	Sets the vector to the given value.
	@param vec The other value
	*/
	void set(const vec2 &vec)
	{
		x = vec.x;
		y = vec.y;
	}

	/**
	Sets the vector to the given value.
	@param x The new x-value
	@param y The new y-value
	*/
	void set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	/**
	Sets this vector to the unit-length version of itself.
	*/
	void normalize()
	{
		(*this) = getNormal();
	}
	
	static vec2 fromString(const string &_s)
	{
		vec2 vec;
		
		string s = toLowerCase(_s);

		char_separator<char> delimeters("(,)\t\n");
		tokenizer<char_separator<char> > tokens(s, delimeters);

		tokenizer<char_separator<char> >::const_iterator i = tokens.begin();

		if(i != tokens.end() && (string(*i)=="&vec2" || string(*i)=="&vec"))
		{
			vec.x = stof(*(++i));
			vec.y = stof(*(++i));
		}

		return vec;
	}

	static string toString(vec2 vec)
	{
		return string("&vec2(" + ftos(vec.x, 4) + ", " + ftos(vec.y, 4) + ")");
	}
};

#endif
