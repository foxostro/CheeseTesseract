#ifndef EVENT_USES_OBJECT_H
#define EVENT_USES_OBJECT_H

#include "EventHandler.h"

/**
Message to declare how a client has requested to use the object.
Some validation has been done by the message sender, but the object ID has
also been sent to further validate the request.
*/
class EventUsesObject : public Event
{
public:
	EventUsesObject(ActorID _requesterID)
	{
		requesterID = _requesterID;
	}

public:
	ActorID requesterID;
};

#endif
