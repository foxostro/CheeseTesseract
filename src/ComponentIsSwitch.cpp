#include "stdafx.h"
#include "color.h"
#include "World.h"
#include "Actor.h"
#include "ComponentIsSwitch.h"
#include "EventSwitchToggled.h"

ComponentIsSwitch::ComponentIsSwitch(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope) {
	REGISTER_HANDLER(ComponentIsSwitch::handleEventUsesObject);
}

void ComponentIsSwitch::handleEventUsesObject( const EventUsesObject *event ) {
	EventSwitchToggled m(categoryID, event->requesterID);
	sendGlobalEvent(&m);
}

void ComponentIsSwitch::resetMembers() {
	categoryID = 0;
}

void ComponentIsSwitch::load(const PropertyBag &data) {
	resetMembers();
	categoryID = data.getInt("categoryID");
}

void ComponentIsSwitch::update(float) {
	/* Do Nothing */
}
