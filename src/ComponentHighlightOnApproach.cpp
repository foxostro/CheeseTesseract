#include "stdafx.h"
#include "World.h"
#include "Actor.h"
#include "ComponentHighlightOnApproach.h"

#include "ActionEnableModelHighlight.h"
#include "ActionDisableModelHighlight.h"

ComponentHighlightOnApproach::
ComponentHighlightOnApproach(UID uid, ScopedEventHandler *blackBoard)
: Component(uid, blackBoard)
{
	REGISTER_HANDLER(ComponentHighlightOnApproach::handleEventApproachActor);
	REGISTER_HANDLER(ComponentHighlightOnApproach::handleEventRecedesFromActor);

	resetMembers();
}

void ComponentHighlightOnApproach::handleEventApproachActor(const EventApproachActor *)
{
	ActionEnableModelHighlight m(HighlightOutline, 60000.0f);
	sendAction(&m);
}

void ComponentHighlightOnApproach::handleEventRecedesFromActor(const EventRecedesFromActor *)
{
	ActionDisableModelHighlight m;
	sendAction(&m);
}
