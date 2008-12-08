#ifndef COMPONENT_OBJECT_APPROACHABLE_H
#define COMPONENT_OBJECT_APPROACHABLE_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventPositionUpdate.h"
#include "MessagePassWorld.h"

class World;

/**
Fires off a message when the object is approached closely by some
player character.
*/
class ComponentObjectApproachable : public Component {
public:
	virtual string getTypeString() const {
		return "ObjectApproachable";
	}
	
	/** Constructor */
	ComponentObjectApproachable(UID uid, ScopedEventHandler *blackBoard);
	
	/** Resets object members to defaults */
	virtual void resetMembers();
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float milliseconds);
	
	/** Draws any visual component represented here */
	virtual void draw() const { /* Does nothing */ }
	
private:
	void handleEventPositionUpdate(const EventPositionUpdate *event);
	void handleMessagePassWorld(const MessagePassWorld *message);
	
	/** Player approaches to within the trigger distance */
	void playerApproaches(ActorID id);
	
	/** Player recedes to greater than the release distance */
	void playerRecedes(ActorID id);
	
private:
	vec3 lastReportedPosition;
	World *world;
	float thresholdTrigger;
	float thresholdRelease;
	
	enum State { OBJECT_NEAR, OBJECT_FAR };
	
	map<ActorID, State> playerState;
};

#endif
