#include "stdafx.h"
#include "ComponentDeathBehavior.h"

#include "EventDeathBehaviorUpdate.h"

#include "ActionPhysicsEnable.h"
#include "ActionPhysicsDisable.h"

ComponentDeathBehavior::
ComponentDeathBehavior(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope) {
	REGISTER_HANDLER(ComponentDeathBehavior::handleEventCharacterRevived);
	REGISTER_HANDLER(ComponentDeathBehavior::handleEventDeathBehaviorUpdate);
	resetMembers();
}

void ComponentDeathBehavior::resetMembers() {
	deathBehavior = Corpse;
}

void ComponentDeathBehavior::load(const PropertyBag &data) {
	resetMembers();
	deathBehavior = deathBehaviorFromString(data.getString("deathBehavior"));
	broadcastDeathBehavior(); // broadcast initial value
}

DeathBehavior ComponentDeathBehavior::deathBehaviorFromString(const string &_s) {
	string s = toLowerCase(_s);
	
	if (s == "corpse") {
		return Corpse;
	} else if (s == "disappear") {
		return Disappear;
	} else if (s == "ghost") {
		return Ghost;
	}
	
	FAIL("Invalid death behavior string: " + _s);
	return Corpse;
}

void ComponentDeathBehavior::handleEventCharacterRevived(const EventCharacterRevived *) {
	ActionPhysicsEnable m;
	sendAction(&m);
}

void ComponentDeathBehavior::handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *) {
	if (getDeathBehavior() != Ghost) {
		ActionPhysicsDisable m;
		sendAction(&m);
	}
}
