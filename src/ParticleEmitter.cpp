#include "stdafx.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

ParticleEmitter::ParticleEmitter(const PropertyBag &data,
								 ParticleSystem *_owner)
: owner(0),
  age(0.0f),
  lifeSpan(0.0f),
  looping(false),
  numberOfLifeCycles(0),
  particleTemplate(),
  shape(0)
{
	ASSERT(_owner, "Null Parameter: _owner");
	owner = _owner;

	particleTemplate = owner->getTemplate(data.getString("template"));

	graphEmissionRate.load(data.getBag("rate"));
	graphSizeMultiplier.load(data.getBag("sizemul"));
	graphLifeSpan.load(data.getBag("lifespan"));
	graphInitialVelocity.load(data.getBag("initialVelocity"));
	graphVelocityVariance.load(data.getBag("velocityVariance"));
	position = data.getVec3("position");
	lifeSpan = data.getFloat("length");
	looping = data.getBool("looping");
	numberOfLifeCycles = data.getInt("cycles");
	useDirectInitialVelocity = data.getBool("useDirectInitialVelocity");

	createEmitterShape(data);
}

void ParticleEmitter::update(float deltaTime)
{
	age += deltaTime;

	if(looping && age > lifeSpan)
	{
		age = 0.0f;

		if(numberOfLifeCycles > 0)
			numberOfLifeCycles--;
	}

	if(!isDead())
	{
		const float rate = graphEmissionRate.getValue(getPercent());
		const size_t numberOfEmittedParticles = (size_t)ceil(rate);

		for(size_t i = 0; i < numberOfEmittedParticles; ++i)
		{
			emitParticle();
		}
	}
}

void ParticleEmitter::emitParticle(void)
{
	ASSERT(owner!=0, "owner was null");
	ASSERT(shape!=0, "shape was null");

	// Copy the template
	ParticleElement particle = particleTemplate;

	particle.setSizeMultiplier(graphSizeMultiplier.getValue(getPercent()));

	particle.setLifeSpan(graphLifeSpan.getValue(getPercent()));

	vec3 origin = getWorldPosition();
	vec3 offset = shape->generateParticlePosition(getPercent());
	float variance = graphVelocityVariance.getValue(getPercent());

	if(useDirectInitialVelocity)
	{
		particle.setPosition(origin + offset);

		float rotation = owner->getRotation();
		float theta = FRAND_RANGE(rotation-variance, rotation+variance);
		mat3 rotZ = mat3::fromRotateZ(theta);
		vec3 velocity = graphInitialVelocity.getValue(getPercent());
		particle.initialVelocity = rotZ.transformVector(velocity);
	}
	else
	{
		particle.setPosition(origin + offset, origin, variance);
	}

	owner->spawn(particle);
}

bool ParticleEmitter::isDead(void) const
{
	return (!looping) ? (age>lifeSpan) : (numberOfLifeCycles==0);
}

void ParticleEmitter::kill(void)
{
	age = lifeSpan + 1.0f; // Run past the length of the animation
	numberOfLifeCycles = 0; // Do not loop
}

ParticleEmitter::EmitterShape* ParticleEmitter::createShape( const string &_shape )
{
	const string shape = toLowerCase(_shape);

	if(shape == "gaussian")
	{
		return new EmitterShape_Gaussian();
	}
	else if(shape == "sphere")
	{
		return new EmitterShape_Sphere();
	}
	else if(shape == "box")
	{
		return new EmitterShape_Box();
	}
	else if(shape == "cylinder")
	{
		return new EmitterShape_Cylinder();
	}
	else
	{
		FAIL("Unknown emitter shape: " + _shape);
		return 0;
	}
}

vec3 ParticleEmitter::getOrigin() const
{
	ASSERT(owner!=0, "Null pointer: owner");
	return owner->getPosition();
}

void ParticleEmitter::createEmitterShape( const PropertyBag &data )
{
	PropertyBag shapeData = data.getBag("shape");
	string shapeType = shapeData.getString("name");
	shape = createShape(shapeType);
	shape->load(shapeData);
}
