#ifndef EVENT_RECEDES_FROM_ACTOR_H
#define EVENT_RECEDES_FROM_ACTOR_H

#include "EventHandler.h"

/**
Message to declare that a client has closely receded from the specified actor
*/
class EventRecedesFromActor : public Event
{
public:
	EventRecedesFromActor(ActorID _id)
	{
		id = _id;
	}

public:
	ActorID id;
};

#endif
