#ifndef COMPONENT_EXIT_MAP_ON_USE_H
#define COMPONENT_EXIT_MAP_ON_USE_H

#include "PropertyBag.h"
#include "World.h"
#include "Component.h"

#include "EventUsesObject.h"
#include "MessagePassWorld.h"

/**
The trigger can be fired by an actor that is close enough to the trigger
object.  Then, the trigger will cause the game to exit the map and move to
the next map in the map list.
*/
class ComponentExitMapOnUse : public Component
{
public:
	virtual string getTypeString() const { return "ExitMapOnUse"; }

	ComponentExitMapOnUse(UID uid, ScopedEventHandler *blackBoard);

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
	void handleMessagePassWorld(const MessagePassWorld *message);
	void handleEventUsesObject(const EventUsesObject *message);

private:
	/** Exit gate will lead to this map */
	FileName nextMap;

	/** Game world */
	World *world;

	/** Sound to play if the exit gates fails to activate when used */
	FileName sfxOnFail;
};

#endif
