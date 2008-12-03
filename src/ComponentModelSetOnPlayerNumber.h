#ifndef COMPONENT_MODEL_SET_ON_PLAYER_NUMBER_H
#define COMPONENT_MODEL_SET_ON_PLAYER_NUMBER_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventPlayerNumberSet.h"

/**
Changes the object model when it receives a player number for this object.
Really only applicable to Player Character objects...
*/
class ComponentModelSetOnPlayerNumber : public Component
{
public:
	virtual string getTypeString() const { return "ModelSetOnPlayerNumber"; }

	ComponentModelSetOnPlayerNumber(UID uid, ScopedEventHandler *parentScope);

	/** Does nothing */
	virtual void load(const PropertyBag &) { /* Do Nothing */ }

	/** Does nothing */
	virtual void update(float) { /* Do Nothing */ }

	/** Does nothing */
	virtual void resetMembers() { /* Do Nothing */ }

private:
	/** Sets the player number */
	void handleEventPlayerNumberSet(const EventPlayerNumberSet *event);
};

#endif
