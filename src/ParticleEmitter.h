#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "ParticleElement.h"

/**
Emits particle for a ParticleSystem.
Creates particles in the parent ParticleSystem according to rules loaded
from the specified data source.
*/
class ParticleEmitter
{
private:
	/** Emitter shape chooses a point for an emitted partivle position */
	class EmitterShape
	{
	public:
		/** Loads emitters shape parameters */
		virtual void load(const PropertyBag &data) = 0;

		/** Generates a position for an emitted particle. */
		virtual vec3 generateParticlePosition(float percent) const = 0;
	};

	/**
	Emitter shape chooses a point at a random distance away from the emitter
	position, and a random direction. The random distance follows a Gaussian
	distribution as radius increases.
	*/
	class EmitterShape_Gaussian : public EmitterShape
	{
	public:
		/** Default Constructor */
		EmitterShape_Gaussian()
		{}

		/** Loads emitters shape parameters */
		virtual void load(const PropertyBag &data)
		{
			radius.reset();
			radius.load(data.getBag("radius"));
		}

		/** Generates a position for an emitted particle. */
		virtual vec3 generateParticlePosition(float percent) const
		{
			const float ratio = 1-powf((float)M_E,-SQR(FRAND_RANGE(0.01,2)));
			const float radius = this->radius.getValue(percent) * ratio;
			const vec3 position = GetRandomVector(radius);
			return position;
		}

	private:
		/**
		The probability of a particle being created drops to zero
		between the hot spot radius and the falloff radius.
		*/
		VaryingOverTime<float> radius;
	};

	/** Emitter shape chooses a random point within a specified sphere */
	class EmitterShape_Sphere : public EmitterShape
	{
	public:
		/** Default Constructor */
		EmitterShape_Sphere()
		{}

		/** Loads emitters shape parameters */
		virtual void load(const PropertyBag &data)
		{
			radius.reset();
			radius.load(data.getBag("radius"));
		}

		/** Generates a position for an emitted particle. */
		virtual vec3 generateParticlePosition(float percent) const
		{
			const vec3 position = GetRandomVector(FRAND_RANGE(0.01,
				                                   radius.getValue(percent)));
			return position;
		}

	private:
		/**
		The probability of a particle being created drops to zero
		between the hot spot radius and the falloff radius.
		*/
		VaryingOverTime<float> radius;
	};

	/** Emitter shape chooses a random point within a specified box */
	class EmitterShape_Box : public EmitterShape
	{
	public:
		/** Default Constructor */
		EmitterShape_Box()
		{}

		/** Loads emitters shape parameters */
		virtual void load(const PropertyBag &data)
		{
			minCorner.reset();
			maxCorner.reset();

			minCorner.load(data.getBag("minCorner"));
			maxCorner.load(data.getBag("maxCorner"));
		}

		/** Generates a position for an emitted particle. */
		virtual vec3 generateParticlePosition(float percent) const
		{
			vec3 a = minCorner.getValue(percent);
			vec3 b = maxCorner.getValue(percent);

			vec3 position(FRAND_RANGE(a.x, b.x),
			              FRAND_RANGE(a.y, b.y),
			              FRAND_RANGE(a.z, b.z));
			return position;
		}

	private:
		VaryingOverTime<vec3> minCorner;
		VaryingOverTime<vec3> maxCorner;
	};

	/** Emitter shape chooses a random point within a specified box */
	class EmitterShape_Cylinder : public EmitterShape
	{
	public:
		/** Default Constructor */
		EmitterShape_Cylinder()
		{}

		/** Loads emitters shape parameters */
		virtual void load(const PropertyBag &data)
		{
			minRadius.reset();
			maxRadius.reset();
			minZ.reset();
			maxZ.reset();

			minRadius.load(data.getBag("minRadius"));
			maxRadius.load(data.getBag("maxRadius"));
			minZ.load(data.getBag("minZ"));
			maxZ.load(data.getBag("maxZ"));
		}

		/** Generates a position for an emitted particle. */
		virtual vec3 generateParticlePosition(float percent) const
		{
			float minRadius = this->minRadius.getValue(percent);
			float maxRadius = this->maxRadius.getValue(percent);
			float minZ = this->minZ.getValue(percent);
			float maxZ = this->maxZ.getValue(percent);

			float theta = FRAND_RANGE(0.0f, (float)(2.0*M_PI));
			float r = FRAND_RANGE(minRadius, maxRadius);
			float z = FRAND_RANGE(minZ, maxZ);

			vec3 position = vec3(cosf(theta)*r, sinf(theta)*r, z);

			return position;
		}

	private:
		VaryingOverTime<float> minRadius;
		VaryingOverTime<float> maxRadius;
		VaryingOverTime<float> minZ;
		VaryingOverTime<float> maxZ;
	};

public:
	/**
	Constructor
	@param data Data to describe the behavior of the emitter
	@param owner ParticleSystem that owns the emitter
	*/
	ParticleEmitter(const PropertyBag &data, ParticleSystem *owner);

	/**
	Updates the emitter over time
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);

	/** Kills the emitter */
	void kill();

	/**
	Determines whether the emitter is dead or alive.
	@return true if the emitter is dead
	*/
	bool isDead() const;

private:
	/** Gets the percent of emitter completion */
	float getPercent() const
	{
		ASSERT(lifeSpan > 0.0f, "lifeSpan invalid: " + ftos(lifeSpan));
		return age / lifeSpan;
	}

	/** Emits a particle */
	void emitParticle();

	/** Gets the world-space location of the local coordinate system */
	vec3 getOrigin() const;

	/** Gets the local-space position of the emitter */
	const vec3& getLocalPosition() const
	{
		return position;
	}

	/** Gets the world-spae position of the emitter */
	vec3 getWorldPosition() const
	{
		return getOrigin() + getLocalPosition();
	}

	/**
	Creates an emitter shape object of a paticular type
	@param shape Type of the shape.  Valid values are
	             "gaussian", "sphere", "box", and "cylinder"
	@return New emitter shape object
	*/
	static EmitterShape* createShape(const string &shape);

	/**
	Creates an initializes the emitter shape object
	@param data Data specifying the emitter shape
	*/
	void createEmitterShape(const PropertyBag &data);

private:
	/** The particle system that owns this emitter */
	ParticleSystem *owner;

	/** Particle template for all Elements emitted by this object */
	ParticleElement particleTemplate;

	/** Rate of emission over time */
	VaryingOverTime<float> graphEmissionRate;

	/** Size multiplier for emitted particles */
	VaryingOverTime<float> graphSizeMultiplier;

	/**
	Elements are given a life span when they are spawned, however,
	this may change over time
	*/
	VaryingOverTime<float> graphLifeSpan;

	/**
	Elements may be given an initial velocity when they are spawned,
	however, this may change over time
	*/
	VaryingOverTime<vec3> graphInitialVelocity;

	/**
	Elements may be given an initial radial velocity when they are spawned.
	This affects the random variance between particles assigned velocities.
	*/
	VaryingOverTime<float> graphVelocityVariance;

	/** Age of the emitter */
	float age;

	/** Life span of the emitter */
	float lifeSpan;

	/** If true, then the emitter will repeat after its lifespan has expired*/
	bool looping;

	/**
	The number of times (remaining) to allow a looping emitter to repeat.
	If this is negative, then the emitter will continue to repeat forever.
	*/
	int numberOfLifeCycles;

	/** Local-space position of the emitter */
	vec3 position;

	/** Emitter shape */
	EmitterShape *shape;

	/**
	Selects whether to choose particle initial velocity with
	graphInitialVelocity (true) or with radial velocities controlled by
	graphRadialVelocityVariance (false)
	*/
	bool useDirectInitialVelocity;
};

#endif
