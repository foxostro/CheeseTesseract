#ifndef EVENT_APPROACH_ACTOR_H
#define EVENT_APPROACH_ACTOR_H

#include "EventHandler.h"

/**
Message to declare that a client has closely approached the specified actor
*/
class EventApproachActor : public Event {
public:
	EventApproachActor(ActorID _id) {
		id = _id;
	}
	
public:
	ActorID id;
};

#endif
