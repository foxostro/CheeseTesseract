#ifndef EVENT_CHARACTER_PICKS_UP_ITEM_H
#define EVENT_CHARACTER_PICKS_UP_ITEM_H

#include "EventHandler.h"

/**
Tokens to identify possible pickups that the player may interact with.
All possible pickup items are listed here.
*/
enum PickupType
{
	Pickup_Health, // Health Pickup of any type
	Pickup_Power,  // Weapon power
	Pickup_Coin    // Coin pickup
};

/** 
Message to notify that a character has picked up an item
of the given PickupType.
*/
class EventPicksUpItem : public Event
{
public:
	EventPicksUpItem(ActorID _id, PickupType _ptype)
	{
		id = _id;
		ptype = _ptype;
	}

public:
	ActorID id;
	PickupType ptype;
};

#endif
