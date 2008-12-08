#ifndef PICKUPS_H
#define PICKUPS_H

#include "PropertyBag.h"
#include "World.h"

/** Defines an interface for interacting with powerup objects */
class PickupItem {
public:
	/**
	Constructor
	@param world Game world
	*/
	PickupItem(World *world);
	
	/** Performs the effect associated with the pickup of this item */
	virtual void executeEffect(ActorID id) = 0;
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data) {
		sound = data.getFileName("sound");
	}
	
protected:
	/** Gets the game world */
	const World& getWorld() const {
		ASSERT(world, "Null member: world");
		return *world;
	}
	
	/** Gets the game world */
	World& getWorld() {
		ASSERT(world, "Null member: world");
		return *world;
	}
	
	/** Gets the sfx to play when the item is picked up */
	const FileName& getSound() const {
		return sound;
	}
	
	/**
	Plays a sound file
	@param fileName Sound file name
	*/
	void playSound(const FileName &fileName);
	
	/**
	Creates a particle system
	@param fileName Particle system filename
	@param position Particle system position
	*/
	void spawnParticleSystem(const FileName &fileName, const vec3 &position);
	
	/**
	Relays a message to the actor specified
	@param id ID of the actor to direct the message towards
	@param message Message to relay
	*/
	void sendMessage(ActorID id, Event *event);
	
private:
	/** Game World*/
	World *world;
	
	/** Sound to play when the item is picked up */
	FileName sound;
};

#endif

