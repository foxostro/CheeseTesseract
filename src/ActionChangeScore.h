#ifndef ACTION_CHANGE_SCORE_H
#define ACTION_CHANGE_SCORE_H

#include "EventHandler.h"

/** Message to request that the game score be changed */
class ActionChangeScore : public Action {
public:
	ActionChangeScore(int _delta) {
		delta = _delta;
	}
	
public:
	int delta;
};

#endif
