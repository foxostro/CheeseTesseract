#ifndef COMPONENT_EXPLODE_AFTER_TIMEOUT_H
#define COMPONENT_EXPLODE_AFTER_TIMEOUT_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventOrientationUpdate.h"
#include "EventPositionUpdate.h"
#include "MessagePassWorld.h"

class World;

/**
After a timeout period, explode and destroy actor
(good for bombs and grenades)
*/
class ComponentExplodeAfterTimeout : public Component
{
public:
	virtual string getTypeString() const { return "ExplodeAfterTimeout"; }

	ComponentExplodeAfterTimeout(UID uid, ScopedEventHandler *blackBoard);

	/** Resets all object members to defaults */
	virtual void resetMembers();

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);

	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float milliseconds);

private:
	void handleEventOrientationUpdate(const EventOrientationUpdate *event);
	void handleEventPositionUpdate(const EventPositionUpdate *event);
	void handleMessagePassWorld(const MessagePassWorld *message);

	void explode();

	/** Alert all listeners that the object is to be destroyed */
	void broadcastDeleteActor();

private:
	vec3 lastReportedPosition;
	mat3 lastReportedOrientation;
	World *world;

	float timeleft;
	int baseDamage;
	FileName soundFileName;
	FileName particlesFileName;
};

#endif
