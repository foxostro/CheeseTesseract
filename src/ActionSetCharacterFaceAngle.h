#ifndef ACTION_SET_CHARACTER_FACE_ANGLE_H
#define ACTION_SET_CHARACTER_FACE_ANGLE_H

#include "EventHandler.h"

/** Request that the character turn to a specific facing off of the X-axis */
class ActionSetCharacterFaceAngle : public Action {
public:
	/**
	Construct the message
	@param facingAngle Radians
	*/
	ActionSetCharacterFaceAngle(float _facingAngle) {
		facingAngle = _facingAngle;
	}
	
	float facingAngle;
};

#endif
