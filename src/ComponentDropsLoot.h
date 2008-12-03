#ifndef COMPONENT_DROPS_LOOT_H
#define COMPONENT_DROPS_LOOT_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventCharacterHasDied.h"
#include "EventPositionUpdate.h"
#include "EventRadiusUpdate.h"
#include "MessagePassWorld.h"

class World; // opaque

/** Feature functionality to drop some kind loot when the character dies */
class ComponentDropsLoot : public Component
{
public:
	virtual string getTypeString() const { return "DropsLoot"; }

	ComponentDropsLoot(UID uid, ScopedEventHandler *parentScope);

	/** Resets all object members to defaults */
	virtual void resetMembers();

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);

	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float) { /* Do Nothing */ }

private:
	void handleEventCharacterHasDied(const EventCharacterHasDied *message);
	void handleMessagePassWorld(const MessagePassWorld *message);
	void handleEventPositionUpdate(const EventPositionUpdate *message);
	void handleEventRadiusUpdate(const EventRadiusUpdate *message);

	/**
	Spawns a piece of loot dropped by the character.
	Chooses position and velocity automatically.
	@param lootFilename Filename of the loot's actor definition
	*/
	void spawnLoot(const FileName &lootFilename);

	/**
	Spawns a piece of loot dropped by the character
	@param lootFilename Filename of the loot's actor definition
	@param position Position of the loot as soon as spawned
	@param velocity Velocity of the loot as soon as spawned
	*/
	void spawnLoot(const FileName &lootFilename,
	               const vec3 &position,
				   const vec3 &velocity);

private:
	World *world;
	int maxLoots;
	ListBag<FileName> lootFileNames;

	vec3 lastReportedPosition;
	float lastReportedRadius;
};

#endif
