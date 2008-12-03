#include "stdafx.h"
#include "ActionPlaySound.h"
#include "ComponentPlaySoundOnUse.h"

ComponentPlaySoundOnUse::
ComponentPlaySoundOnUse(UID uid, ScopedEventHandler *parentScope)
: Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentPlaySoundOnUse::handleEventUsesObject);
}

void ComponentPlaySoundOnUse::resetMembers()
{
	sound = FileName("");
}

void ComponentPlaySoundOnUse::load(const PropertyBag &data)
{
	resetMembers();
	sound = data.getFileName("sound");
}

void ComponentPlaySoundOnUse::update(float)
{
	/* Do Nothing */
}

void ComponentPlaySoundOnUse::handleEventUsesObject(const EventUsesObject *)
{
	ActionPlaySound m(sound);
	sendGlobalAction(&m);
}
