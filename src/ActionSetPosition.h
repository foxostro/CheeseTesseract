#ifndef ACTION_SET_POSITION_H
#define ACTION_SET_POSITION_H

#include "EventHandler.h"

/** Request that the object's position be moved to the specified point */
class ActionSetPosition : public Action
{
public:
	ActionSetPosition(const vec3 &_position)
	{
		position = _position;
	}

public:
	vec3 position;
};

#endif
