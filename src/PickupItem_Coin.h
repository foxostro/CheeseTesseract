#ifndef PICKUPITEM_COIN_H
#define PICKUPITEM_COIN_H

#include "Pickups.h"

class PickupItem_Coin : public PickupItem
{
public:
	/**
	Constructor
	@param world Game world
	*/
	PickupItem_Coin(World *world)
	: PickupItem(world)
	{}

	/** Performs the effect associated with the pickup of this item */
	virtual void executeEffect(ActorID id);

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
};

#endif
