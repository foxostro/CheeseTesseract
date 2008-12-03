#ifndef ACTION_PERFORM_ACTION_H
#define ACTION_PERFORM_ACTION_H

#include "EventHandler.h"
#include "Actions.h"

/** Message to request that object perform some action in the game world */
class ActionPerformAction : public Action
{
public:
	ActionPerformAction(CharacterAction _action)
	{
		action = _action;
	}

CharacterAction action;
};

#endif
