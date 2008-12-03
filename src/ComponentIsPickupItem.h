#ifndef COMPONENT_IS_PICKUP_ITEM_H
#define COMPONENT_IS_PICKUP_ITEM_H

#include "Component.h"
#include "PropertyBag.h"
#include "Pickups.h"
#include "MessagePassWorld.h"
#include "EventUsesObject.h"
#include "EventPicksUpItem.h"

class ComponentIsPickupItem : public Component
{
private:
	/** Type of the pickup in question */
	PickupType type;

	/** Handles power up effects */
	PickupItem *effects;
	
	/** Game world */
	World *world;

	/** string -> PickupType */
	map<string, PickupType> stringToPType;

	bool activated;

public:
	/** Destructor */
	virtual ~ComponentIsPickupItem();

	/** Constructor */
	ComponentIsPickupItem(UID uid, ScopedEventHandler *blackBoard);

	/** Returns type of component in string */
	virtual string getTypeString() const { return "IsPickupItem"; }

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
	void handleMessagePassWorld(const MessagePassWorld *message);
	void handleEventUsesObject(const EventUsesObject *event);

	PickupType getPickupTypeFromName(const string &s) const;
};

#endif
