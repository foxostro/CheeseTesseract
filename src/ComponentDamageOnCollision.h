#ifndef COMPONENT_DAMAGE_ON_COLLISION_H
#define COMPONENT_DAMAGE_ON_COLLISION_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventCollisionOccurred.h"
#include "MessagePassWorld.h"

/**
Component fires a character damage message to any actors that this actor
collides with.
*/
class ComponentDamageOnCollision : public Component
{
public:
	virtual string getTypeString() const { return "DamageOnCollision"; }

	ComponentDamageOnCollision(UID uid, ScopedEventHandler *blackBoard);

	virtual void update(float) {}

	virtual void load(const PropertyBag &data);

	virtual void resetMembers();

private:
	void handleEventCollisionOccurred(const EventCollisionOccurred *message);
	void handleMessagePassWorld(const MessagePassWorld *message);

private:
	World *world;
	bool onlyPlayers;
	int damage;
};

#endif
