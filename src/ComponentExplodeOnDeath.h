#ifndef COMPONENT_EXPLODE_ON_DEATH_H
#define COMPONENT_EXPLODE_ON_DEATH_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventCharacterHasDied.h"
#include "EventOrientationUpdate.h"
#include "EventPositionUpdate.h"
#include "MessagePassWorld.h"

class World;

/**
When the actor dies (assuming it can die), create an explosion effect
with splash damage and
*/
class ComponentExplodeOnDeath : public Component {
public:
	virtual string getTypeString() const {
		return "ExplodeOnDeath";
	}
	
	ComponentExplodeOnDeath(UID uid, ScopedEventHandler *blackBoard);
	
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
	void handleEventCharacterHasDied(const EventCharacterHasDied *event);
	void handleEventOrientationUpdate(const EventOrientationUpdate *event);
	void handleEventPositionUpdate(const EventPositionUpdate *event);
	void handleMessagePassWorld(const MessagePassWorld *event);
	
private:
	vec3 lastReportedPosition;
	mat3 lastReportedOrientation;
	World *world;
	
	int baseDamage;
	FileName soundFileName;
	FileName particlesFileName;
};

#endif
