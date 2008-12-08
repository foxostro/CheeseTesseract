#include "stdafx.h"
#include "World.h"
#include "ActionPlaySound.h"
#include "ComponentPhysics.h"
#include "PickupItem_Coin.h"

void PickupItem_Coin::load(const PropertyBag &data) {
	PickupItem::load(data);
}

void PickupItem_Coin::executeEffect(ActorID) {}
