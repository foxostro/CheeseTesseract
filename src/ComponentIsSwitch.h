#ifndef COMPONENT_IS_SWITCH_H
#define COMPONENT_IS_SWITCH_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventUsesObject.h"

/**
When the object is "used" by a client, sends a switch toggle message to all
actors in the game world.
*/
class ComponentIsSwitch : public Component {
public:
	virtual string getTypeString() const {
		return "IsSwitch";
	}
	
	ComponentIsSwitch(UID uid, ScopedEventHandler *blackBoard);
	
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
	void handleEventUsesObject(const EventUsesObject *event);
	
private:
	int categoryID;
};

#endif
