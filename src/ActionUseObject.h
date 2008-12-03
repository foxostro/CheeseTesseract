#ifndef ACTION_USE_OBJECT_H
#define ACTION_USE_OBJECT_H

#include "EventHandler.h"

/** Message to request to activate and "use" some object */
class ActionUseObject : public Action
{
public:
	ActionUseObject(ActorID _requesterID)
	{
		requesterID = _requesterID;
	}

	ActorID requesterID;
};

#endif
