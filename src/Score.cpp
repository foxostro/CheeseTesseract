#include "stdafx.h"
#include "World.h"
#include "ActionChangeScore.h"
#include "Score.h"

Score::Score(int initScore, UID uid, ScopedEventHandler *parentScope)
		: ScopedEventHandlerSubscriber(uid, parentScope) {
	score = initScore;
	multiplier = 1.0;
	multTTL = 0;
	
	REGISTER_HANDLER(Score::handleActionChangeScore);
}

Score::~Score(void) {
}

void Score::update(float milliseconds) {
	multTTL -= (int)milliseconds;
	timeToComplete += milliseconds / 1000.0;
	
	//if TTL of multiplier is <= 0, reset multiplier
	if (multTTL <= 0 && multiplier != 1.0) {
		multiplier = 1.0;
	}
}

int Score::getScore() {
	return score;
}

void Score::setMultiplier(float m, unsigned int t) {
	multiplier = m;
	multTTL = t;
}

float Score::getMultiplier() {
	return multiplier;
}

void Score::handleActionChangeScore(const ActionChangeScore *action) {
	score += (int)ceilf((float)(action->delta) * multiplier);
}
