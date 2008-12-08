#include "stdafx.h"
#include "Actions.h"

float getAngleFromDirection(Direction facing) {
	const float a = float(M_PI / 4.0);
	
	switch (facing) {
	case SouthWest:
		return 0.0f;
	case South:
		return a;
	case SouthEast:
		return a * 2.0f;
	case East:
		return a * 3.0f;
	case NorthEast:
		return a * 4.0f;
	case North:
		return a * 5.0f;
	case NorthWest:
		return a * 6.0f;
	case West:
		return a * 7.0f;
	}
	
	FAIL("Invalid value for facing");
	return 0.0f;
}

CharacterAction getActionFromAngle(float r) {
	float angle = r / (float)(2.0 * M_PI) * 16.0f;
	
	if (angle > 15.0f || angle <= 1.0f) {
		return StepSouthWest;
	} else if (angle > 1.0f && angle <= 3.0f) {
		return StepSouth;
	} else if (angle > 3.0f && angle <= 5.0f) {
		return StepSouthEast;
	} else if (angle > 5.0f && angle <= 7.0f) {
		return StepEast;
	} else if (angle > 7.0f && angle <= 9.0f) {
		return StepNorthEast;
	} else if (angle > 9.0f && angle <= 11.0f) {
		return StepNorth;
	} else if (angle > 11.0f && angle <= 13.0f) {
		return StepNorthWest;
	} else if (angle > 13.0f && angle <= 15.0f) {
		return StepWest;
	} else {
		return InvalidAction;
	}
}

Direction getDirectionFromAngle(float r) {
	float angle = r / (float)(2.0 * M_PI) * 16.0f;
	
	if (angle > 15.0f || angle <= 1.0f) {
		return SouthWest;
	} else if (angle > 1.0f && angle <= 3.0f) {
		return South;
	} else if (angle > 3.0f && angle <= 5.0f) {
		return SouthEast;
	} else if (angle > 5.0f && angle <= 7.0f) {
		return East;
	} else if (angle > 7.0f && angle <= 9.0f) {
		return NorthEast;
	} else if (angle > 9.0f && angle <= 11.0f) {
		return North;
	} else if (angle > 11.0f && angle <= 13.0f) {
		return NorthWest;
	} else if (angle > 13.0f && angle <= 15.0f) {
		return West;
	} else {
		return InvalidDirection;
	}
}
