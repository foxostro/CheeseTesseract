#ifndef COMPONENT_HIGHLIGHT_ON_APPROACH_H
#define COMPONENT_HIGHLIGHT_ON_APPROACH_H

#include "PropertyBag.h"
#include "Component.h"

#include "EventApproachActor.h"
#include "EventRecedesFromActor.h"

/** Object can be approached and then used by some actor */
class ComponentHighlightOnApproach : public Component
{
public:
	virtual string getTypeString() const { return "HighlightOnApproach"; }

	ComponentHighlightOnApproach(UID uid, ScopedEventHandler *blackBoard);

	/** Resets all object members to defaults */
	virtual void resetMembers() { /* Do Nothing */ }

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &) { /* Do Nothing */ }

	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float) { /* Does nothing */ }

	/** Draws any visual component represented here */
	virtual void draw() const { /* Does nothing */ }

private:
	void handleEventApproachActor(const EventApproachActor *event);
	void handleEventRecedesFromActor(const EventRecedesFromActor *event);
};

#endif
