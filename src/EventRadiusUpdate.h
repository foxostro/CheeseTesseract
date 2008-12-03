#ifndef EVENT_RADIUS_UPDATE_H
#define EVENT_RADIUS_UPDATE_H

#include "EventHandler.h"

/** Message to notify that the object's radius has been updated */
class EventRadiusUpdate : public Event
{
public:
	EventRadiusUpdate(float _radius)
	{
		radius = _radius;
	}

public:
	float radius;
};

#endif
