#include "stdafx.h"
#include "Actor.h"
#include "ComponentSwitchReceiver.h"
#include "EventUsesObject.h"

ComponentSwitchReceiver::
ComponentSwitchReceiver(UID uid, ScopedEventHandler *parentScope)
: Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentSwitchReceiver::handleEventSwitchToggled);
}

void ComponentSwitchReceiver::handleEventSwitchToggled( const EventSwitchToggled *event )
{
	const int categoryID = event->categoryID;
	
	ASSERT(categoryID<MAX_SWITCHES && categoryID>=0,
		"Message field \"categoryID\" is invalid: " + itos(categoryID));	
	switchState[categoryID] = !switchState[categoryID];

	if(false) // if(expr_on_switch.eval<bool>())
	{
		for(int i=0; i<MAX_SWITCHES; ++i)
		{
			switchStateOld[i] = switchState[i];
		}

		EventUsesObject m(event->requesterID);
		sendEvent(&m);
	}
}

void ComponentSwitchReceiver::resetMembers()
{
	for(int i=0; i<MAX_SWITCHES; ++i)
	{
		switchState[i] = switchStateOld[i] = false;
	}
}

void ComponentSwitchReceiver::load(const PropertyBag &data)
{
	resetMembers();
	// TODO: Load script that determines when switch conditions are met
}

void ComponentSwitchReceiver::update(float)
{
	/* Do Nothing */
}
