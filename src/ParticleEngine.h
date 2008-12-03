#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include "ScopedEventHandlerSubscriber.h"

class ParticleSystem; // So we can use opaque pointers in this header

/** Collection of particle systems */
class ParticleEngine : public ScopedEventHandlerSubscriber
{
public:
	typedef list<ParticleSystem*>::iterator handle;

public:
	/** Constructor */
	ParticleEngine(UID uid, ScopedEventHandler *parentScope)
	: ScopedEventHandlerSubscriber(uid, parentScope)
	{
		// Do nothing
	}

	/** Emit geometry to the renderer */
	void emitGeometry();

	/**
	Updates all particles, particle systems, and particle emitters
	@param milliseconds Milliseconds since the last update
	@param camera Camera to align particle billboards against
	*/
	void update(float milliseconds, Camera &camera);

	/**
	Adds another particle system under engine control
	@param fileName File where the system is described
	@param position Position where the system is initially placed
	@param rotation system's initial rotation about the Z axis
	@param textureFactory Texture loader / cache subsystem
	@return handle to the particle system
	*/
	handle add(const FileName &fileName,
	           const vec3 &position,
			   float rotation,
			   TextureFactory &textureFactory);

	/**
	Removes a particle system from the game
	@param h handle to the particle system
	*/
	void remove(handle h);

private:
	list<ParticleSystem*> particles;
};

#endif
