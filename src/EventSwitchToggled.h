#ifndef EVENT_SWITCH_TOGGLED_H
#define EVENT_SWITCH_TOGGLED_H

#include "EventHandler.h"

/** Message to notify that some switch in the map has been toggled */
class EventSwitchToggled : public Event {
public:
	EventSwitchToggled(int _categoryID, ActorID _requesterID) {
		categoryID = _categoryID;
		requesterID = _requesterID;
	}
	
public:
	int categoryID;
	ActorID requesterID;
};

#endif
