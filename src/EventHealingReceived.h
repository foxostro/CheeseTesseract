#ifndef EVENT_HEALING_RECEIVED_H
#define EVENT_HEALING_RECEIVED_H

#include "EventHandler.h"

/** Message to notify that the specified character has received healing */
class EventHealingReceived : public Event {
public:
	EventHealingReceived(int _healing) {
		healing = _healing;
	}
	
public:
	int healing;
};

#endif
