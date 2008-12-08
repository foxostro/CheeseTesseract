#include "stdafx.h"
#include "World.h"
#include "Actor.h"
#include "ComponentObjectCanBeUsed.h"
#include "EventUsesObject.h"

ComponentObjectCanBeUsed::
ComponentObjectCanBeUsed(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope) {
	REGISTER_HANDLER(ComponentObjectCanBeUsed::handleActionUseObject);
	REGISTER_HANDLER(ComponentObjectCanBeUsed::handleEventApproachActor);
	REGISTER_HANDLER(ComponentObjectCanBeUsed::handleEventRecedesFromActor);
	
	resetMembers();
}

void ComponentObjectCanBeUsed::handleEventApproachActor( const EventApproachActor *event ) {
	playerNear[event->id] = true;
}

void ComponentObjectCanBeUsed::handleEventRecedesFromActor( const EventRecedesFromActor *event ) {
	playerNear[event->id] = false;
}

void ComponentObjectCanBeUsed::resetMembers() {
	playerNear.clear();
}

void ComponentObjectCanBeUsed::load(const PropertyBag &) {
	resetMembers();
}

void ComponentObjectCanBeUsed::handleActionUseObject( const ActionUseObject *action ) {
	const ActorID requesterID = action->requesterID;
	
	if (playerNear[requesterID]) {
		EventUsesObject m(requesterID);
		sendEvent(&m);
	}
}
