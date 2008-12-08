#ifndef COMPONENT_DEATH_BEHAVIOR_H
#define COMPONENT_DEATH_BEHAVIOR_H

#include "PropertyBag.h"
#include "Component.h"
#include "DeathBehavior.h"

#include "EventCharacterRevived.h"
#include "EventDeathBehaviorUpdate.h"

/** Contains actor settings pertaining to character behavior on death */
class ComponentDeathBehavior : public Component {
public:
	virtual string getTypeString() const {
		return "DeathBehavior";
	}
	
	ComponentDeathBehavior(UID uid, ScopedEventHandler *blackBoard);
	
	/** Resets all object members to defaults */
	virtual void resetMembers();
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float) { /* Do Nothing */ }
	
	/** Gets character death behavior */
	DeathBehavior getDeathBehavior() const {
		return deathBehavior;
	}
	
private:
	void handleEventCharacterRevived(const EventCharacterRevived *message);
	void handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *message);
	
	static DeathBehavior deathBehaviorFromString(const string &_s);
	
	/** Sends death behavior to other components when value is updated */
	void broadcastDeathBehavior() {
		EventDeathBehaviorUpdate m(deathBehavior);
		sendEvent(&m);
	}
	
private:
	DeathBehavior deathBehavior;
};

#endif
