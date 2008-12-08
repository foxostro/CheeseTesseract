#include "Core.h"
#include "random.h"
#include <boost/random.hpp>
#include <ctime>

float angle_clamp(const float rads) {
	const float P = float(2.0*M_PI);
	float clamped = rads;
	
	while (clamped < 0.0f) {
		clamped += P;
	}
	
	while (clamped > 2*P) {
		clamped -= P;
	}
	
	return clamped;
}

float getAngle( const vec2 &a, const vec2 &b ) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float rads = atan2f(dy, dx);
	float clamped = angle_clamp(rads);
	return clamped;
}

vec3 GetRandomVector(float length) {
	return vec3(FRAND_RANGE(-1.0f,+1.0f),
	            FRAND_RANGE(-1.0f,+1.0f),
	            FRAND_RANGE(-1.0f,+1.0f)).getNormal() * length;
}

string toString(const any &value) {
	string typeName = value.type().name();
	
	if (typeName == typeid(vec2).name()) {
		return vec2::toString(any_cast<vec2>(value));
	} else if (typeName == typeid(vec3).name()) {
		return vec3::toString(any_cast<vec3>(value));
	} else if (typeName == typeid(vec4).name()) {
		return vec4::toString(any_cast<vec4>(value));
	} else if (typeName == typeid(mat4).name()) {
		return mat4::toString(any_cast<mat4>(value));
	} else if (typeName == typeid(mat3).name()) {
		return mat3::toString(any_cast<mat3>(value));
	} else if (typeName == typeid(float).name()) {
		return ftos(any_cast<float>(value));
	} else if (typeName == typeid(int).name()) {
		return itos(any_cast<int>(value));
	} else if (typeName == typeid(size_t).name()) {
		return sizet_to_string(any_cast<size_t>(value));
	} else if (typeName == typeid(bool).name()) {
		return any_cast<bool>(value) ? "true" : "false";
	} else {
		return "???";
	}
}

vec3 calcTriNorm(const vec3 &a, const vec3 &b, const vec3 &c) {
	vec3 v1 = a - b;
	vec3 v2 = b - c;
	vec3 n = v1.cross(v2);
	return n.getNormal();
}

vec3 calcTriNorm(const Triangle &tri) {
	return calcTriNorm(tri.a, tri.b, tri.c);
}

float SampleLogNormal(float mean, float sigma) {
	// Create a Mersenne twister random number generator
	// that is seeded once with #seconds since 1970
	static mt19937 rng(static_cast<unsigned> (std::time(0)));
	
	// select gamma probability distribution
	lognormal_distribution<float> dist(mean, sigma);
	
	// bind random number generator to distribution, forming a function
	variate_generator<mt19937&, lognormal_distribution<float> >  sampler(rng, dist);
	
	// sample from the distribution
	return sampler();
}

float SampleNormal(float mean, float sigma) {
	// Create a Mersenne twister random number generator
	// that is seeded once with #seconds since 1970
	static mt19937 rng(static_cast<unsigned> (std::time(0)));
	
	// select Gaussian probability distribution
	normal_distribution<float> norm_dist(mean, sigma);
	
	// bind random number generator to distribution, forming a function
	variate_generator<mt19937&, normal_distribution<float> >  normal_sampler(rng, norm_dist);
	
	// sample from the distribution
	return normal_sampler();
}
