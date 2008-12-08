#ifndef PICKUPITEM_HEALTH_H
#define PICKUPITEM_HEALTH_H

#include "Pickups.h"

class PickupItem_Health : public PickupItem {
public:
	/**
	Constructor
	@param world Game world
	*/
	PickupItem_Health(World *world)
			: PickupItem(world),
			healing(0) {}
			
	/** Performs the effect associated with the pickup of this item */
	virtual void executeEffect(ActorID id);
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
private:
	/** Amount to heal the character that uses this powerup */
	int healing;
};

#endif
