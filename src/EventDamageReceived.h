#ifndef EVENT_DAMAGE_RECEIVED_H
#define EVENT_DAMAGE_RECEIVED_H

#include "EventHandler.h"

/** Message to notify that the specified character has received damage */
class EventDamageReceived : public Event
{
public:
	EventDamageReceived(int _damage)
	{
		damage = _damage;
	}

public:
	int damage;
};

#endif
