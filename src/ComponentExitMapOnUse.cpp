#include "stdafx.h"
#include "ComponentExitMapOnUse.h"

#include "ActionChangeMap.h"
#include "ActionPlaySound.h"

ComponentExitMapOnUse::
ComponentExitMapOnUse(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope),
		world(0) {
	REGISTER_HANDLER(ComponentExitMapOnUse::handleEventUsesObject);
	REGISTER_HANDLER(ComponentExitMapOnUse::handleMessagePassWorld);
	resetMembers();
}

void ComponentExitMapOnUse::resetMembers() {
	nextMap = FileName("data/maps/nil");
	sfxOnFail = FileName("data/sounds/default.wav");
}

void ComponentExitMapOnUse::load(const PropertyBag &data) {
	resetMembers();
	nextMap = data.getFileName("nextMap");
	sfxOnFail = data.getFileName("sfxOnFailed");
}

void ComponentExitMapOnUse::update(float) {
	/* Do Nothing */
}

void ComponentExitMapOnUse::handleMessagePassWorld( const MessagePassWorld *message ) {
	world = message->world;
}

void ComponentExitMapOnUse::handleEventUsesObject(const EventUsesObject *) {
	ActionChangeMap m(nextMap);
	sendGlobalAction(&m);
	
	/*if(true)
	{
		ActionChangeMap m(nextMap);
		sendGlobalAction(&m);
	}
	else
	{
		ActionPlaySound m(sfxOnFail);
		sendGlobalAction(&m);
	}*/
}
