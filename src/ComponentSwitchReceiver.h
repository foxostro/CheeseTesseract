#ifndef COMPONENT_SWITCH_RECEIVER_H
#define COMPONENT_SWITCH_RECEIVER_H

#include "PropertyBag.h"
#include "Component.h"
#include "EventSwitchToggled.h"

/**
Listens for when a switch in the map is toggled and if that switch is of the
appropriate category, the object is treated as if it was directly "used" by
an actor.
*/
class ComponentSwitchReceiver : public Component {
public:
	virtual string getTypeString() const {
		return "SwitchReceiver";
	}
	
	ComponentSwitchReceiver(UID uid, ScopedEventHandler *blackBoard);
	
	/** Resets all object members to defaults */
	virtual void resetMembers();
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float milliseconds);
	
private:
	void handleEventSwitchToggled(const EventSwitchToggled *event);
	
private:
	static const int MAX_SWITCHES=1024;
	bool switchState[MAX_SWITCHES];
	bool switchStateOld[MAX_SWITCHES];
};

#endif
