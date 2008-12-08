#ifndef COMPONENT_USE_ON_COLLISION_H
#define COMPONENT_USE_ON_COLLISION_H

#include "PropertyBag.h"
#include "Component.h"
#include "MessagePassWorld.h"
#include "EventCollisionOccurred.h"

/**
Component fires a use message on collision between this actor and any
other actor.
*/
class ComponentUseOnCollision : public Component {
public:
	virtual string getTypeString() const {
		return "UseOnCollision";
	}
	
	ComponentUseOnCollision(UID uid, ScopedEventHandler *blackBoard);
	
	virtual void update(float) {}
	
	virtual void load(const PropertyBag &data);
	
	virtual void resetMembers();
	
private:
	void handleEventCollisionOccurred(const EventCollisionOccurred *event);
	void handleMessagePassWorld(const MessagePassWorld *message);
	
private:
	World *world;
	bool onlyPlayers;
};

#endif
