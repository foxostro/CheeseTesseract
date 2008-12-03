#ifndef EVENT_ORIENTATION_UPDATE_H
#define EVENT_ORIENTATION_UPDATE_H

#include "EventHandler.h"

/**
Message to notify that the object's orientation has been set for the frame.
The orientation may not have necessarily changed since the last update.
*/
class EventOrientationUpdate : public Event
{
public:
	EventOrientationUpdate(const mat3 &_orientation)
	{
		orientation = _orientation;
	}

public:
	mat3 orientation;
};

#endif
