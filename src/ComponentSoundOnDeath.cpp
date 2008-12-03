#include "stdafx.h"
#include "ActionPlaySound.h"
#include "ComponentSoundOnDeath.h"

ComponentSoundOnDeath::
ComponentSoundOnDeath(UID uid, ScopedEventHandler *parentScope)
: Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentSoundOnDeath::handleEventCharacterHasDied);
	resetMembers();
}

void ComponentSoundOnDeath::load(const PropertyBag &data)
{
	resetMembers();
	deathSounds = data.getListBag<FileName>("deathSounds");
}

void ComponentSoundOnDeath::resetMembers()
{
	deathSounds.clear();
}

void ComponentSoundOnDeath::handleEventCharacterHasDied(const EventCharacterHasDied *)
{
	if(!deathSounds.empty())
	{
		FileName deathSound = deathSounds.getRandom();
		ActionPlaySound m(deathSound);
		sendGlobalAction(&m);
	}
}
