#ifndef ACTIONS_H
#define ACTIONS_H

/**
Tokens to identify possible actions.
All possible actions are listed here
*/
enum CharacterAction {
	InvalidAction,
	Stand,
	BeginChargeUp, /** Charge up in anticipation of an attack or ability */
	EndChargeUp,   /** Executes an attack and exits the charging state */
	AttackOnce,
	Suicide,
	StepNorth,
	StepSouth,
	StepEast,
	StepWest,
	StepNorthEast,
	StepNorthWest,
	StepSouthEast,
	StepSouthWest,
	StepForward,
	StepForwardLeft,
	StepLeft,
	StepBackWardLeft,
	StepBackWard,
	StepBackWardRight,
	StepRight,
	StepForwardRight
};

enum Direction {
	InvalidDirection,
	East,
	NorthEast,
	North,
	NorthWest,
	West,
	SouthWest,
	South,
	SouthEast
};

/**
Given the angle to the waypoint, get the action to take to mve their
@param angle Angle we need to go to reach the next waypoint, expressed
in radians [0, 2*pi]
@return action to go in the desired direction
*/
CharacterAction getActionFromAngle(float angle);

/**
Given the angle we are facing, get the direction token
@param angle expressed	in radians [0, 2*pi]
@return facing direction
*/
Direction getDirectionFromAngle(float angle);

/**
Given the facing, get the angle we are facing
@param facing direction we are facing
@return Angle expressed	in radians [0, 2*pi]
*/
float getAngleFromDirection(Direction facing);

#endif
