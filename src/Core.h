#pragma once
#ifndef CORE_H
#define CORE_H

#define PROJECT_NAME "CheeseTesseract"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "msvc_rules.h" // Special directives for Microsoft Visual Studio#include <windows.h>
#include <direct.h>
#include <time.h>
#endif

/* Standard C Library */
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cfloat>
#include <cerrno>
#include <fstream>
#include <iostream>

/* STL */
#include "stl_wrapper.h"

/* Boost Library */
#include "boost_wrapper.h"

#include "macros.h"        // useful macros
#include "mytypes.h"       // types of specific sizes
#include "logger.h"        // Logger
#include "myassert.h"      // Custom assertion routine
#include "random.h"        // Various Random number routines
#include "tstring.h"       // functions to operate on strings
#include "FileName.h"      // File name wrapper
#include "FileFuncs.h"     // Useful file and file system utilities

#include "ivec2.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat3.h"
#include "mat4.h"

/** Convert a boost::any to a string */
string toString(const any &value);

/** Wraps an angle around so that it lies between 0 and 2*pi */
float angle_clamp(const float rads);

/**
Gets the radian angle between two 2D vectors
@param a a 2D vector that is different from b
@param b a 2D vector that is different from a
@return radian angle wrapped to [0, 2*pi]
*/
float getAngle(const vec2 &a, const vec2 &b);

struct Triangle
{
	vec3 a, b, c;
};

/** Calculate the normal of a triangle */
vec3 calcTriNorm(const vec3 &a, const vec3 &b, const vec3 &c);

/** Calculate the normal of a triangle */
vec3 calcTriNorm(const Triangle &tri);

/**
Creates billboard vertices that are aligned with the specified camera
@param matrix Model-view matrix
@param position Position of the billboard at its center
@param Size Size of the billboard object
@param A Returns one corner of the billboard
@param B Returns one corner of the billboard
@param C Returns one corner of the billboard
@param D Returns one corner of the billboard
*/
void buildBillboardVertices(const mat4 & matrix,
							const vec3 &position,
							float size,
							vec3 &A,
							vec3 &B,
							vec3 &C,
							vec3 &D);

#endif
