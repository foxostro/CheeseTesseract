#ifndef COMPONENT_DESTROY_SELF_ON_COLLISION_H
#define COMPONENT_DESTROY_SELF_ON_COLLISION_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventCollisionOccurred.h"
#include "MessagePassWorld.h"

/**
Component ensures that actor destroys itself when a collision with another
actor occurs. (e.g. Projectile such as a missile)
*/
class ComponentDestroySelfOnCollision : public Component
{
public:
	virtual string getTypeString() const { return "DestroySelfOnCollision"; }

	ComponentDestroySelfOnCollision(UID uid, ScopedEventHandler *blackBoard);

	virtual void update(float);

	virtual void load(const PropertyBag &data);

	virtual void resetMembers();

private:
	void handleEventCollisionOccurred(const EventCollisionOccurred *message);
	void handleMessagePassWorld(const MessagePassWorld *message);

	/** Notify listeners that the self actor is to be deleted */
	void broadcastDeleteSelfActor();

	/** Notifies listeners that the self actor/character has died */
	void broadcastEventCharacterHasDied();

private:
	World *world;
	bool shouldActivate;
	bool onlyPlayers;
};

#endif
