#ifndef ACTION_DELETE_ACTOR_H
#define ACTION_DELETE_ACTOR_H

#include "EventHandler.h"

/**
Message to request that the specified actor become a zombie and be removed
during the next garbage collection operation
*/
class ActionDeleteActor : public Action {
public:
	ActionDeleteActor(ActorID _id) {
		id = _id;
	}
	
public:
	ActorID id;
};

#endif
