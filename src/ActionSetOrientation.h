#ifndef ACTION_SET_ORIENTATION_H
#define ACTION_SET_ORIENTATION_H

#include "EventHandler.h"

/** Request to set the object's orientation */
class ActionSetOrientation : public Action
{
public:
	ActionSetOrientation(const mat3 &_orientation)
	{
		orientation = _orientation;
	}

public:
	mat3 orientation;
};

#endif
