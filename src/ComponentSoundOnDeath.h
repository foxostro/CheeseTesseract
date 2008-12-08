#ifndef COMPONENT_SOUND_ON_DEATH_H
#define COMPONENT_SOUND_ON_DEATH_H

#include "PropertyBag.h"
#include "Component.h"
#include "EventCharacterHasDied.h"

/** Component plays a sound when the character dies */
class ComponentSoundOnDeath : public Component {
public:
	virtual string getTypeString() const {
		return "SoundOnDeath";
	}
	
	ComponentSoundOnDeath(UID uid, ScopedEventHandler *blackBoard);
	
	virtual void update(float) {}
	
	virtual void load(const PropertyBag &data);
	
	virtual void resetMembers();
	
private:
	void handleEventCharacterHasDied(const EventCharacterHasDied *event);
	
private:
	/** List of possible sounds when dying */
	ListBag<FileName> deathSounds;
};

#endif
