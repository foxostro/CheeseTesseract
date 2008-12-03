#ifndef _COLOR_H_
#define _COLOR_H_

/** Represents an RGBA color */
class color
{
public:
	float r;
	float g;
	float b;
	float a;

	/**	Default constructor. Sets to black */
	color()
	{
		r = g = b = a = 0.0f;
	}

	/**
	Copy constructor
	@param c The color to copy
	*/
	color(const color &c)
	{
		set(c);
	}

	/**
	Creates the color directly from components
	@param r Red
	@param g Green
	@param b Blue
	@param a Alpha
	*/
	color(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

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
	Sets the color to the value on the right-hand side
	@param rh The right-hand side
	@return The new color
	*/
	color operator=(const color &rh)
	{
		set(rh);
		return(*this);
	}

	/**
	Gets an element of the color as if it were layed out as an array
	@throws Throws std:invalid_argument when the parameter idx is not within 
	        the bounds of the array
	*/
	float operator[](size_t idx) const
	{
		switch(idx)
		{
		case 0: return r;
		case 1: return g;
		case 2: return b;
		case 3: return a;
		default:
			throw std::invalid_argument("Invalid argument \'idx\'");
			return -1.0f;
		};
	}

	/**
	Sets the color to the given value.
	@param col The other color
	*/
	void set(const color &col)
	{
		r = col.r;
		g = col.g;
		b = col.b;
		a = col.a;
	}

	/**
	Sets the color to the given value.
	@param r Red
	@param g Green
	@param b Blue
	@param a Alpha
	*/
	void set(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	void clamp()
	{
		r = min(1.0f, max(0.0f, r));
		g = min(1.0f, max(0.0f, g));
		b = min(1.0f, max(0.0f, b));
		a = min(1.0f, max(0.0f, a));
	}

	color operator - (color rh) const
	{
		return subtract(rh);
	}

	color operator + (color rh) const
	{
		return add(rh);
	}
	
	color operator*(const float rhs) const
	{
		return color(r*rhs, g*rhs, b*rhs, a*rhs);
	}

	inline const color add(const color& c) const
	{
		return color(r+c.r, g+c.g, b+c.b, a+c.a);
	}

	inline const color subtract(float c) const
	{
		return color(r-c, g-c, b-c, a-c);
	}

	inline const color subtract(const color& vec) const
	{
		return color(r-vec.r, g-vec.g, b-vec.b, a-vec.a);
	}

	inline static color interpolate(const color &a,
	                                const color &b,
	                                float bias)
	{
		return color(lerp(a.r, b.r, bias),
			         lerp(a.g, b.g, bias),
					 lerp(a.b, b.b, bias),
					 lerp(a.a, b.a, bias));
	}

	static color fromString(const string &_s)
	{
		color vec;

		string s = toLowerCase(_s);

		char_separator<char> delimeters("(,)\t\n");
		tokenizer<char_separator<char> > tokens(s, delimeters);

		tokenizer<char_separator<char> >::const_iterator i = tokens.begin();

		if(i != tokens.end() && string(*i)=="&color")
		{
			vec.r = stof(*(++i));
			vec.g = stof(*(++i));
			vec.b = stof(*(++i));
			vec.a = stof(*(++i));
		}

		return vec;
	}

	static string toString(color c)
	{
		return string("&color(" + ftos(c.r, 4) + ", " +
			                      ftos(c.g, 4) + ", " +
								  ftos(c.b, 4) + ", " +
								  ftos(c.a, 4) + ")");
	}

private:
	template<typename T> static inline
	T lerp(T a, T b, float bias)
	{
		return a*bias + b*(1-bias);
	}
};

const color white(1.0f, 1.0f, 1.0f, 1.0f);
const color black(0.0f, 0.0f, 0.0f, 0.0f);

const color red(1.0f, 0.0f, 0.0f, 1.0f);
const color green(0.0f, 1.0f, 0.0f, 1.0f);
const color blue(0.0f, 0.0f, 1.0f, 1.0f);

const color cyan(0.0f, 1.0f, 1.0f, 1.0f);
const color magenta(1.0f, 0.0f, 1.0f, 1.0f);
const color yellow(1.0f, 1.0f, 0.0f, 1.0f);

const color cornflower(0.392f, 0.584f, 0.929f, 1.0f);
const color dark_cyan(0.0f, 0.545f, 0.545f, 1.0f);

#endif
