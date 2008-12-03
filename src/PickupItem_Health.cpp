#include "stdafx.h"
#include "PickupItem_Health.h"
#include "EventHealingReceived.h"

void PickupItem_Health::executeEffect(ActorID id)
{
	playSound(getSound());
	EventHealingReceived m(healing);
	sendMessage(id, &m);

	// TODO: Special FX
}

void PickupItem_Health::load( const PropertyBag &data )
{
	PickupItem::load(data);
	healing = data.getInt("healing");
}
