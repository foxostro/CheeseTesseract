#ifndef COMPONENT_PLAY_SOUND_ON_USE_H
#define COMPONENT_PLAY_SOUND_ON_USE_H

#include "PropertyBag.h"
#include "Component.h"
#include "EventUsesObject.h"

class World;

/** When the actor is used (assuming the actor is useable), play a sound */
class ComponentPlaySoundOnUse : public Component {
public:
	virtual string getTypeString() const {
		return "PlaySoundOnUse";
	}
	
	ComponentPlaySoundOnUse(UID uid, ScopedEventHandler *blackBoard);
	
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
	FileName sound;
};

#endif
