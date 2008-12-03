#ifndef ACTION_TURN_H
#define ACTION_TURN_H

#include "EventHandler.h"

/** Request that the character turn by some angle off the x axis */
class ActionTurn : public Action
{
public:
	/**
	Construct the message
	@param facingAngle Radians
	*/
	ActionTurn(float _dFacingAngle)
	{
		dFacingAngle = _dFacingAngle;
	}

float dFacingAngle;
};

#endif
