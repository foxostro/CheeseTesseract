#ifndef EVENT_HEIGHT_UPDATE_H
#define EVENT_HEIGHT_UPDATE_H

#include "EventHandler.h"

/** Message to notify that the object's height has been updated */
class EventHeightUpdate : public Event
{
public:
	EventHeightUpdate(float _height)
	{
		height = _height;
	}

public:
	float height;
};

#endif
