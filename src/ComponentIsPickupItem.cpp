#include "stdafx.h"
#include "World.h"
#include "ComponentIsPickupItem.h"
#include "PickupItem_Health.h"
#include "PickupItem_Coin.h"
#include "EventPicksUpItem.h"
#include "ActionDeleteActor.h"

ComponentIsPickupItem::~ComponentIsPickupItem() {
	delete effects;
}

ComponentIsPickupItem::
ComponentIsPickupItem(UID uid, ScopedEventHandler *parentScope)
		: world(0),
		effects(0),
		activated(false),
		Component(uid, parentScope) {
	REGISTER_HANDLER(ComponentIsPickupItem::handleEventUsesObject);
	REGISTER_HANDLER(ComponentIsPickupItem::handleMessagePassWorld);
	
	stringToPType["Health"] = Pickup_Health;
	stringToPType["Power"] = Pickup_Power;
	stringToPType["Coin"] = Pickup_Coin;
	
	resetMembers();
}

void ComponentIsPickupItem::resetMembers() {
	type = Pickup_Coin;
	
	delete effects;
	effects = 0;
	
	activated = false;
}

PickupType ComponentIsPickupItem::getPickupTypeFromName(const string &s) const {
	map<string, PickupType>::const_iterator i = stringToPType.find(s);
	VERIFY(i != stringToPType.end(), "Cannot identify pickup type: " + s);
	return i->second;
}

void ComponentIsPickupItem::load(const PropertyBag &data) {
	resetMembers();
	
	type = getPickupTypeFromName(data.getString("pickupType"));
	
	switch (type) {
	case Pickup_Health:
		effects = new PickupItem_Health(world);
		break;
	case Pickup_Coin:
		effects = new PickupItem_Coin(world);
		break;
	}
	
	effects->load(data.getBag("effects"));
}

void ComponentIsPickupItem::handleMessagePassWorld( const MessagePassWorld *message ) {
	world = message->world;
}

void ComponentIsPickupItem::handleEventUsesObject( const EventUsesObject *event ) {
	ASSERT(effects, "Member was null: effects");
	ASSERT(event, "Parameter was null: effects");
	
	effects->executeEffect(event->requesterID);
	
	{
		EventPicksUpItem m2(event->requesterID,type);
		sendGlobalEvent(&m2);
	}
	
	/*
	we really don't want to be freeing resources within a collision
	callback handler, so flag that this actor should be destroyed at
	the next tick.
	*/
	activated=true;
}

void ComponentIsPickupItem::update(float) {
	if (activated) {
		ActorID actor = getActorID();
		ActionDeleteActor m(actor);
		sendGlobalAction(&m);
	}
}
