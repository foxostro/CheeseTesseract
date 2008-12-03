#ifndef EVENT_DECLARE_INITIAL_POSITION_H
#define EVENT_DECLARE_INITIAL_POSITION_H

#include "EventHandler.h"

/**
Sent by the world to declare an object's initial position and velocity
*/
class EventDeclareInitialPosition : public Event
{
public:
	EventDeclareInitialPosition(const vec3 &_position)
	{
		position = _position;
		velocity = vec3(0,0,0);
	}

	EventDeclareInitialPosition(const vec3 &_position, const vec3 _velocity)
	{
		position = _position;
		velocity = _velocity;
	}

public:
	vec3 position;
	vec3 velocity;
};

#endif
