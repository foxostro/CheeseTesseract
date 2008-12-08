#include "stdafx.h"
#include "AnimationController.h"

AnimationController::AnimationController() {
	clear();
}

AnimationController::AnimationController(const AnimationController &a)
		: animations(a.animations),
		current(a.current) { /* Do Nothing */ }

void AnimationController::update(float milliseconds) {
	ASSERT(current < animations.size(),
	       "Invalid m_nCurrentAnimation: " + sizet_to_string(current));
	       
	animations[current].update(milliseconds);
}

const AnimationSequence& AnimationController::getAnimation() const {
	ASSERT(current < animations.size(),
	       "Invalid animation handle: " + sizet_to_string(current));
	       
	return animations[current];
}

AnimationSequence& AnimationController::getAnimation() {
	ASSERT(current < animations.size(),
	       "Invalid animation handle: " + sizet_to_string(current));
	       
	return animations[current];
}

const AnimationSequence& AnimationController::getAnimation(size_t handle) const {
	ASSERT(handle < animations.size(),
	       "Invalid animation handle: " + sizet_to_string(handle));
	       
	return animations[handle];
}

size_t AnimationController::addAnimation(AnimationSequence &animation) {
	animations.push_back(animation);
	return animations.size()-1;
}

size_t AnimationController::getAnimationHandle(const string &name) const {
	for (size_t i=0; i<animations.size(); ++i) {
		if (animations[i].getName() == name)
			return i;
	}
	
	return 0;
}

bool AnimationController::requestAnimationChange(size_t handle, float speed) {
	ASSERT(handle < animations.size(),
	       "Invalid handle: " + sizet_to_string(handle));
	       
	ASSERT(current < animations.size(),
	       "Invalid m_nCurrentAnimation: " + sizet_to_string(current));
	       
	AnimationSequence &currentSequence = animations[current];
	AnimationSequence &requestedSequence = animations[handle];
	
	// Fail if the current is animation is super high priority
	if (!currentSequence.isFinished() && currentSequence.isHighPriority())
		return false;
		
	// Fail if the current animation has a higher priority
	if (!currentSequence.isFinished() &&
	    currentSequence.getPriority() > requestedSequence.getPriority())
		return false;
		
	// Fail if the requested animation is already playing
	if (current == handle) {
		// change the speed, though
		currentSequence.setSpeed(speed);
		
		/*
		      If the current animation is already playing, but IS finished, then
		      fall-through to restart the animation
		      */
		if (!currentSequence.isFinished())
			return false;
	}
	
	// Fulfill the request
	current = handle;
	animations[current].setTime(0.0f);
	animations[current].setSpeed(speed);
	
	return true;
}

float AnimationController::calculateCylindricalRadius() {
	ASSERT(current < animations.size(),
	       "Invalid m_nCurrentAnimation: " + sizet_to_string(current));
	       
	return animations[current].calculateCylindricalRadius();
}

float AnimationController::calculateRadius() {
	ASSERT(current < animations.size(),
	       "Invalid m_nCurrentAnimation: " + sizet_to_string(current));
	       
	return animations[current].calculateRadius();
}

float AnimationController::calculateHeight() {
	ASSERT(current < animations.size(),
	       "Invalid m_nCurrentAnimation: " + sizet_to_string(current));
	       
	return animations[getAnimationHandle("idle")].calculateHeight();
}

void AnimationController::clear() {
	current = 0;
	animations.clear();
}

void AnimationController::getGeometryChunks( vector<GeometryChunk> &m ) const {
	ASSERT(current < animations.size(),
	       "Invalid current animation: " + sizet_to_string(current));
	       
	animations[current].getGeometryChunks(m);
}

void AnimationController::uniformScale( float scale ) {
	for (vector<AnimationSequence>::iterator i=animations.begin();
	     i!=animations.end(); ++i) {
		i->uniformScale(scale);
	}
}