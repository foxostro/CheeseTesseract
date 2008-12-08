#ifndef PARTICLE_ELEMENT_H
#define PARTICLE_ELEMENT_H

#include "PropertyBag.h"
#include "VaryingOverTime.h"
#include "color.h"
#include "Material.h"
#include "Camera.h"

class ParticleSystem; // opaque pointers FTW

/** A single particle element. */
class ParticleElement {
private:
	/** Position of the body */
	vec3 position;
	
	/** Milliseconds since creation*/
	float age;
	
	/** Points to the material to use */
	Material *material;
	
	/** Milliseconds that this element will be alive for */
	float lifeSpan;
	
	/** Particle emitter assigns a size multiplier on creation. */
	float sizeMultiplier;
	
	/** Size in meters over time */
	VaryingOverTime<float> graphSize;
	
	/** color over time */
	VaryingOverTime<color> graphColor;
	
public:
	/** Initial position of the body */
	vec3 initialPosition;
	
	/** Initial velocity of the body (meters per second) */
	vec3 initialVelocity;
	
	/** Initial velocity of the particle outwards from the center point */
	float initialRadialVelocity;
	
	/** The acceleration of the body */
	VaryingOverTime<vec3> graphAcceleration;
	
public:
	/** Default Constructor */
	ParticleElement();
	
	/**
	Constructor
	@param data Data to describe the element
	@param material Material to apply to the particle
	*/
	ParticleElement(const PropertyBag &data,
	                Material *material);
	                
	/** Copy Constructor */
	ParticleElement(const ParticleElement &element);
	
	/**
	Loads particle data
	@param data Data to describe the element
	@param material Material to apply to the particle element
	*/
	void load(const PropertyBag &data, Material *material);
	
	/** Reset all data members */
	void resetMembers();
	
	/**
	Updates the particle element
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);
	
	/**
	Sets the position of the body
	@param position Position of the body
	@param center Center of the body
	@param variance Randomly varies particle velocity within some bounds
	*/
	void setPosition(const vec3 &position, const vec3 &center, float variance);
	
	/**
	Sets the position of the body
	@param position Position of the body
	*/
	void setPosition(const vec3 &position);
	
	float setSizeMultiplier(float x) {
		return(sizeMultiplier = x);
	}
	
	float setLifeSpan(float x) {
		return(lifeSpan = x);
	}
	
	/**
	Determines whether the particle is dead or not
	@return true if the particle is now dead
	*/
	bool isDead() const {
		return age>lifeSpan;
	}
	
	/**
	Gets the position of the Body
	@return position of the Body
	*/
	inline const vec3& getPosition() const {
		return position;
	}
	
	/** Gets a pointer to the material used by this particle */
	inline Material* getMaterial() const {
		return material;
	}
	
	/** Sort by distance from the camera */
	bool operator<(const ParticleElement& o) const;
	
	/**
	Creates billboard vertices that are aligned with the specified camera
	@param matrix Model-view matrix
	@param A Returns one corner of the billboard
	@param B Returns one corner of the billboard
	@param C Returns one corner of the billboard
	@param D Returns one corner of the billboard
	*/
	void buildBillboardVertices(const mat4 & matrix,
	                            vec3 &A,
	                            vec3 &B,
	                            vec3 &C,
	                            vec3 &D) const;
	                            
	inline color getColor() const {
		return graphColor.getValue(age / lifeSpan);
	}
	
private:
	inline float getSizeMultiplier() const {
		return sizeMultiplier;
	}
	
	inline float getAge() const {
		return age;
	}
	
	inline float getSize() const {
		return graphSize.getValue(age / lifeSpan);
	}
	
	inline vec3 getAcceleration() const {
		return graphAcceleration.getValue(age / lifeSpan);
	}
};

#endif
