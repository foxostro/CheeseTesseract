#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "tstring.h"
#include "vec3.h"

// Random Number Macros
#define IRAND (rand())                                 // Get a random integer
#define FRAND ( (float)rand() / (float)RAND_MAX )      // Get a random float between 0.0 and 1.0
#define DRAND ( (double)rand() / (double)RAND_MAX )    // Get a random double between 0.0 and 1.0

#define RANGE(low, high) ((high) - (low))
#define IRAND_RANGE(low, high) (int)(  IRAND % (int)RANGE(low,high) + (int)(low)  )
#define FRAND_RANGE(low, high) (float)(  (float)RANGE(low,high)  * FRAND + (float)(low)  )
#define DRAND_RANGE(low, high) (double)( (double)RANGE(low,high) * DRAND + (double)(low) )

/**
Get a vector pointing in a random direction
@param length The length of the vector
@return The random vector
*/
vec3 GetRandomVector(float length);

float SampleNormal(float mean, float sigma);

float SampleLogNormal(float mean, float sigma);

#endif
