#ifndef ACTION_CHANGE_ANIMATION_H
#define ACTION_CHANGE_ANIMATION_H

#include "EventHandler.h"

/** Message to request that the object's animation be changed */
class ActionChangeAnimation : public Action
{
public:
	ActionChangeAnimation(const string &_animationName)
	{
		animationName = _animationName;
	}

public:
	string animationName;
};

#endif
