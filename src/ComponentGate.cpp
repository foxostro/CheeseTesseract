#include "stdafx.h"
#include "color.h"
#include "World.h"
#include "Actor.h"
#include "ActionSetPosition.h"
#include "ComponentGate.h"

ComponentGate::ComponentGate(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope) {
	REGISTER_HANDLER(ComponentGate::handleEventUsesObject);
	REGISTER_HANDLER(ComponentGate::handleEventDeclareInitialPosition);
}

void ComponentGate::handleEventUsesObject(const EventUsesObject *) {
	switch (state) {
	case STATE_A:
		enterState(STATE_TRANSITION_AB);
		break;
	case STATE_B:
		enterState(STATE_TRANSITION_BA);
		break;
	case STATE_TRANSITION_AB: /* do nothing */
		break;
	case STATE_TRANSITION_BA: /* do nothing */
		break;
	}
}

void ComponentGate::handleEventDeclareInitialPosition( const EventDeclareInitialPosition *event ) {
	initialPosition = event->position;
}

void ComponentGate::resetMembers() {
	positionA = vec3(0.0f, 0.0f, 0.5f);
	positionB = vec3(0.0f, 0.0f, 5.0f);
	initialPosition = vec3(0.0f, 0.0f, 0.5f);
	timeToTransitionAB = 1000.0f;
	timeToTransitionBA = 1000.0f;
	timeInState = 0.0f;
}

void ComponentGate::draw() const {}

void ComponentGate::load(const PropertyBag &data) {
	resetMembers();
	
	state = ("a" == toLowerCase(data.getString("initialState")))
	        ? STATE_A
	        : STATE_B;
	        
	positionA = data.getVec3("positionA");
	positionB = data.getVec3("positionB");
	timeToTransitionAB = data.getFloat("timeToTransitionAB");
	timeToTransitionBA = data.getFloat("timeToTransitionBA");
}

void ComponentGate::update(float milliseconds) {
	timeInState += milliseconds;
	
	switch (state) {
	case STATE_A:
		break;
		
	case STATE_B:
		break;
		
	case STATE_TRANSITION_AB:
		update_transition(milliseconds,
		                  timeInState,
		                  timeToTransitionAB,
		                  STATE_B,
		                  positionA,
		                  positionB);
		break;
		
	case STATE_TRANSITION_BA:
		update_transition(milliseconds,
		                  timeInState,
		                  timeToTransitionBA,
		                  STATE_A,
		                  positionB,
		                  positionA);
		break;
	}
}

void ComponentGate::update_transition(float,
                                      float timeInTransition,
                                      float timeToTransition,
                                      GateState endState,
                                      const vec3 &positionStart,
                                      const vec3 &positionEnd) {
	const float bias = timeInTransition / timeToTransition;
	const vec3 offset = positionStart + (positionEnd - positionStart) * bias;
	const vec3 position = offset + initialPosition;
	
	setPosition(position);
	
	if (timeInTransition > timeToTransition) {
		enterState(endState);
	}
}

void ComponentGate::enterState(GateState newState) {
	timeInState = 0.0f;
	state = newState;
}

void ComponentGate::setPosition(const vec3 &position) {
	ActionSetPosition m(position);
	sendAction(&m);
}
