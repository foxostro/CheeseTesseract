#ifndef ACTION_LOOKAT_H
#define ACTION_LOOKAT_H

#include "EventHandler.h"

/**
Request that the object be oriented to stand upward in the XY plane and look
toward a point rotated some angle about the Z axis.
*/
class ActionLookAt : public Action {
public:
	ActionLookAt(float _facingAngle) {
		facingAngle = _facingAngle;
	}
	
public:
	float facingAngle;
};

#endif
