#ifndef EVENT_POSITION_UPDATE_H
#define EVENT_POSITION_UPDATE_H

#include "EventHandler.h"

/**
Message to notify that the object's position has been set for the frame.
(The position may not have necessarily changed since the last update.)
*/
class EventPositionUpdate : public Event
{
public:
	EventPositionUpdate(const vec3 &_position)
	{
		position = _position;
	}

public:
	vec3 position;
};

#endif
